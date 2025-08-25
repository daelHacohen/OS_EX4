#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <string>
#include "Graph.hpp" 


// ====== Messages ======
struct Request {
    int client_fd;
    uint64_t job_id;
    int V, R; 
    std::shared_ptr<graph::Graph> g; 
    std::string answer;                   
};
using ReqPtr = std::shared_ptr<Request>;

// ====== Thread-safe queue with close() ======
template<class T>
class TSQueue {
    std::queue<T> q; std::mutex m; std::condition_variable cv; bool closed=false;
public:
    void push(T v){  std::lock_guard<std::mutex> lk(m); if(closed) return; q.push(std::move(v));  cv.notify_one(); }
    bool pop(T& out){
        std::unique_lock<std::mutex> lk(m);
        cv.wait(lk,[&]{ return closed || !q.empty(); });
        if(q.empty()) return false; out = std::move(q.front()); q.pop(); return true;
    }
    void close(){  std::lock_guard<std::mutex> lk(m); closed=true;  cv.notify_all(); }
};
