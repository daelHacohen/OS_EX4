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


#include "stradegy.hpp"
#include "factory.hpp"
using namespace graph;
using namespace std::chrono;
using namespace std;


#define PORT 8080

string calculate(string name, Graph& g,int start,int end){
    factory f;
    stradegy* action=f.create(name);
    return action->run(g,start,end);
    
    
}


static int server_sock = -1;
static std::mutex m;
static std::condition_variable cv;
static bool leader_present = false;

static void worker_loop(int tid) {
    for (;;) {
        {
            std::unique_lock<std::mutex> lock(m);
            cv.wait(lock, [] { return !leader_present; });
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
            if (errno == EINTR) continue;
            perror("accept");
            continue;
        }

        {
            std::lock_guard<std::mutex> lk(m);
            leader_present = false; 
            cv.notify_one();        
        }

        // ====== hendle the client  ======

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
 
    string Smessege="Euler circle: "+calculate("Euler circle",g,r,v)+"\n"+"max flow: "+calculate("max flow",g,r,v)+"\n"+"count cliques: "+calculate("count cliques",g,r,v)+"\n"+"find Max clique: "+calculate("find Max clique",g,r,v)+"\n"+"MST: "+calculate("MST",g,r,v)+"\n";
    
    const char* message = Smessege.c_str();
            if (send(client_fd, message, strlen(message), 0) == -1) {
                cerr << "Sending failed!" << endl;
                return;
            }
       

       cout << "The message has been sent" << endl;
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

     // הפעלת Thread-Pool
    const unsigned hw = std::thread::hardware_concurrency();
    const int N = hw ? int(hw) : 4;        
    std::vector<std::thread> pool;
    pool.reserve(N);
    for (int i = 0; i < N; ++i) pool.emplace_back(worker_loop, i);

    // מנהיג ראשון
    {
        std::lock_guard<std::mutex> lk(m);
        leader_present = false;
        cv.notify_one();
    }

    for (auto& t : pool) t.join();
    ::close(server_sock);
    
        

    return 0;
}
