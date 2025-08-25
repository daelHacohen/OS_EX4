#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <cstring>
#include <fcntl.h>
#include <list>
#include <thread>
#include <string>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <mutex>
#include "Graph.hpp"
#include "Algorithms.hpp"
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <bits/stdc++.h>
#include "TSQueue.hpp"
#include "objectActive.hpp"

#include "stradegy.hpp"
#include "factory.hpp"
#include <atomic>  


using namespace graph;
using namespace std::chrono;
using namespace std;


#define PORT 8080




static std::atomic<int> accepted_count{0};
static std::atomic<bool> shutting_down{false};


static EulerStage      ST_euler;
static MaxFlowStage    ST_maxflow;
static MaxCliqueStage  ST_maxclique;
static MstStage        ST_mst;



static void start_pipeline(){
    ST_euler.set_next(&ST_maxflow);
    ST_maxflow.set_next(&ST_maxclique);
    ST_maxclique.set_next(&ST_mst);

    ST_mst.start();
    ST_maxclique.start();
    ST_maxflow.start();
    ST_euler.start();
}
static void stop_pipeline(){ 

    ST_euler.stop();
    ST_maxflow.stop();
    ST_maxclique.stop();
    ST_mst.stop();
}





static int server_sock = -1;
static std::mutex m;
static std::condition_variable cv;
static bool leader_present = false;
static bool stop = false;

static void worker_loop(int tid) {
    while(!stop) {
       
        {
            std::unique_lock<std::mutex> lock(m);
            cv.wait(lock, [] { return shutting_down.load() || !leader_present; });
            if (shutting_down.load()) 
            {
            return; 
            }
            leader_present = true; 
        }

      
        sockaddr_in cli{}; socklen_t clilen = sizeof(cli);
        int client_fd = ::accept(server_sock, (sockaddr*)&cli, &clilen);
        if (client_fd < 0) {
           
            {
                std::lock_guard<std::mutex> lk(m);
                leader_present = false;
                cv.notify_one(); 
            }
            if (shutting_down.load()) return;
            if (errno == EINTR) continue;
            perror("accept");
            continue;
        }
        int now = ++accepted_count;
        bool i_am_the_one_to_shutdown = (now >= 1) && !shutting_down.exchange(true); // <-- 1 במקום 2
        if (i_am_the_one_to_shutdown) {
        ::shutdown(server_sock, SHUT_RDWR);
        ::close(server_sock);

      
        
            std::lock_guard<std::mutex> lk(m);
            leader_present = false; 
            cv.notify_all();      
        }

        // ====== hendle the client======

    cout<<"the current client_fd is: "<< client_fd <<endl;
    char buffer[1024];
    int recv_len;
    if ((recv_len = recv(client_fd, buffer, sizeof(buffer), 0)) == -1) {
        cerr << "Failed to receive message from client!" << endl;
        return;
    }

    buffer[recv_len] = '\0';  // terminate string with NULL
    cout << "Message received from client: " << buffer << endl;

    std::string input(buffer, recv_len);
    int v, e, r;
    sscanf(input.c_str(), "%d,%d,%d", &v, &e, &r);

    std::cout << "v=" << v << ", e=" << e << ", r=" << r << std::endl;

    graph::Graph g(v);
    g = g.generateRandomGraph(v,e);
    g.print_graph();

    auto g_rw = std::make_shared<graph::Graph>(std::move(g));
    

    auto req = std::make_shared<Request>();
    req->client_fd = client_fd;
    req->job_id = std::chrono::steady_clock::now().time_since_epoch().count();
    req->V = v;
    req->R = r;
    req->g = g_rw;
    req->answer = "RESULTS:\n";

    //pipeline
    ST_euler.post(std::move(req));
    ::close(server_sock);
    
   
 }
}

int main() {
    int client_sock;
    struct sockaddr_in server, client;
    socklen_t client_len = sizeof(client);

    // create server socket
    if ((server_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        cerr << "Failed to create server socket!" << endl;
        return -1;
    }
    int optval = 1;
    if (setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1) {
        cerr << "Failed to set SO_REUSEADDR option!" << endl;
        close(server_sock);
        return -1;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);  // port to listen on
    server.sin_addr.s_addr = INADDR_ANY;  // listen on any IP

    // bind socket to address and port
    if (bind(server_sock, (struct sockaddr*)&server, sizeof(server)) == -1) {
        cerr << "Failed to bind socket to address and port!" << endl;
        return -1;
    }

    // listen for connections
    if (listen(server_sock, 5) == -1) {
        cerr << "Failed to listen for connections!" << endl;
        return -1;
    }

    cout << "Server listening on port " << PORT << "..." << endl;

//----------------------------LF----------------------
    start_pipeline();

     //Thread-Pool
    const unsigned hw = std::thread::hardware_concurrency();
    const int N = hw ? int(hw) : 4;         
    std::vector<std::thread> pool;
    pool.reserve(N);
    for (int i = 0; i < N; ++i) pool.emplace_back(worker_loop, i);

    // first lider
    {
        std::lock_guard<std::mutex> lk(m);
        leader_present = false;
        cv.notify_one();
    }

    for (auto& t : pool) t.join();
    stop_pipeline();
    ::close(server_sock);
    

        

    return 0;
}
