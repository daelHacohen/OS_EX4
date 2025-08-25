#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>

using namespace std;

int main() {
    int sock = 0;
    sockaddr_in serv_addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket failed");
        return 1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080); 

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address");
        return 1;
    }

    if (connect(sock, (sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        return 1;
    }
    
        string v;
        string e;
        string r;
        cout << " let's make a new graph "<< endl;
        cout << " please enter number of vertex: "<< endl;
        cin >> v;
        cout << " please enter number of edges: "<< endl;
        cin >> e;
        cout << " please enter the number of the root vertex: : "<< endl;
        cin >> r;
        
       

    
        string commend;
        commend = v +","+ e + "," + r;

        send(sock, commend.c_str(), commend.size(), 0);

        // int ans1;
        // while(1){

        
        // cout<<"MENUE:"<<endl;
        // cout << "1. caculate Euler circle: "<< endl;
        // cout << "2. caculate max flow: "<< endl;
        // cout << "3. caculate count cliques: "<< endl;
        // cout << "4. find Max clique: "<< endl;
        // cout << "5. minimum spanning tree: "<< endl;
        // cin >> ans1;
        
        // string ans;

        // if(ans1==1){ans="Euler circle";}
        // if(ans1==2){ans="max flow";}
        // if(ans1==3){ans="count cliques";}
        // if(ans1==4){ans="find Max clique";}
        // if(ans1==5){ans="MST";}
        // send(sock, ans.c_str(), ans.size(), 0);

        char buffer[1024];
        int recv_len;
        if((recv_len = recv(sock, buffer, sizeof(buffer), 0)) == -1) {
        cerr << "you cant recev masseges from the server" << endl;
        return -1;
    }

    buffer[recv_len] = '\0'; 
    cout << "the massege from the server:\n " << buffer << endl;
}
// }
