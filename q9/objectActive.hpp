class MaxCliqueStage; class MstStage; class MaxFlowStage;
class EulerStage {
public:
    void start();
    void post(ReqPtr r);
    void stop();
    void set_next(MaxFlowStage* n);
private:
    void run();           
    MaxFlowStage* next;
    TSQueue<ReqPtr> q;
    std::thread th;
    std::atomic<bool> started{false};
};

class MaxFlowStage {
public:
    void start();
    void post(ReqPtr r);
    void stop();
    void set_next(MaxCliqueStage* n);
private:
    void run();           
    MaxCliqueStage* next;  
    TSQueue<ReqPtr> q;
    std::thread th;
    std::atomic<bool> started{false};
};

class MaxCliqueStage {
public:
    void start();
    void post(ReqPtr r);
    void stop();
    void set_next(MstStage* n);
private:
    void run();           
    MstStage* next;
    TSQueue<ReqPtr> q;
    std::thread th;
    std::atomic<bool> started{false};
};

class MstStage {
public:
    void start();
    void post(ReqPtr r);
    void stop();
private:
    void run();           
    TSQueue<ReqPtr> q;
    std::thread th;
    std::atomic<bool> started{false};
};
