#include"Algorithms.hpp"
#include"Graph.hpp"
#include"factory.hpp"
#include"UnionFind.hpp"
#include"stradegy.hpp"
#include"TSQueue.hpp"
#include<thread>
#include <atomic>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include"objectActive.hpp"
using namespace std;


class MaxFlowStage; class MaxCliqueStage; class MstStage; class EulerStage; 
factory f;

string calculate(string name, Graph& g,int start,int end){
    
  try {
    factory f;
    std::unique_ptr<stradegy> action=f.create(name);
    // stradegy* action=f.create(name);
    string ans = action->run(g,start,end);
    // delete action;
    return ans;
      } catch (const std::exception& ex) {
        return std::string("error: ") + ex.what();
    }
}



// ---- Stage 1: Euler ----


    void EulerStage::run(){
        ReqPtr r;
        while(q.pop(r)){
            
            // bool ok = Algorithms::Euler_circle(*r->g, r->R);
            r->answer += "Euler circle: " + calculate("Euler circle",*r->g,r->R,r->V) + "\n";
            if (next) next->post(std::move(r));
        } 
    }

    void EulerStage::set_next(MaxFlowStage* n){ next=n; }
    void EulerStage::start(){ if(started.exchange(true)) return; th=std::thread([this]{run();}); }
    void EulerStage::post(ReqPtr r){ q.push(std::move(r)); }
    void EulerStage::stop(){ q.close(); if(th.joinable()) th.join(); }


// ---- Stage 2: Max Flow ----

    
   void MaxFlowStage::run(){
        ReqPtr r;
        while(q.pop(r)){
            r->answer += "Max Flow: " + calculate("max flow",*r->g,r->R,r->V) + "\n";
            if (next) next->post(std::move(r));
        } 
    }

    void MaxFlowStage::set_next(MaxCliqueStage* n){ next=n; }
    void MaxFlowStage::start(){ if(started.exchange(true)) return; th=std::thread([this]{run();}); }
    void MaxFlowStage::post(ReqPtr r){ q.push(std::move(r)); }
    void MaxFlowStage::stop(){ q.close(); if(th.joinable()) th.join(); }

// ---- Stage 3: Max Clique ----

   
  void MaxCliqueStage::run(){
        ReqPtr r;
        while(q.pop(r)){
            r->answer += "Max Clique: " + calculate("Max Clique",*r->g,r->R,r->V) + "\n";
            if (next) next->post(std::move(r));
        } 
    }

    void MaxCliqueStage::set_next(MstStage* n){ next=n; }
    void MaxCliqueStage::start(){ if(started.exchange(true)) return; th=std::thread([this]{run();}); }
    void MaxCliqueStage::post(ReqPtr r){ q.push(std::move(r)); }
    void MaxCliqueStage::stop(){ q.close(); if(th.joinable()) th.join(); }


// ---- Stage 4: MST (שלב אחרון — שולח ללקוח) ----

   
    void MstStage::run(){
        ReqPtr r;
        while(q.pop(r)){
            r->answer += "MST: " + calculate("MST",*r->g,r->R,r->V) + "\n";
            if (!r->answer.empty() && r->answer.back()!='\n') r->answer.push_back('\n');
            // send_all_and_close(r->client_fd, r->answer);
              if (send(r->client_fd, r->answer.c_str(), r->answer.size(), 0) == -1) {
                cerr << "Sending failed!" << endl;
                return;
            }
            ::close(r->client_fd);
           
            
        }
    }

    void MstStage::start(){ if(started.exchange(true)) return; th=std::thread([this]{run();}); }
    void MstStage::post(ReqPtr r){ q.push(std::move(r)); }
    void MstStage::stop(){ q.close(); if(th.joinable()) th.join(); }

