#include "pomodoro_cpp_linux.h"

void pomodoro_timer::setTimeout(){
    canceled = false;
    std::thread t2d([=]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(interval));
        if(canceled) return;
        cbFunctor();
    });
    t2d.detach();
}
//
pomodoro_timer::pomodoro_timer(unsigned int miliseconds, std::function<void()> callback){
    canceled = false;
    interval = miliseconds;
    cbFunctor = callback;
    setTimeout();
}
//
void pomodoro_timer::cancel(){
    canceled = true;
}
//
pomodoro_main_loop_class::pomodoro_main_loop_class(){
    std::cout<<"Here goes..."<<std::endl;
    my_state = 0;
    my_cb = []() {
        pomodoro_main_loop_class::getAppHandle()->nextState();
    };

}
//
pomodoro_main_loop_class::~pomodoro_main_loop_class(){

}
//
pomodoro_main_loop_class* pomodoro_main_loop_class::getAppHandle(){
    if(self == nullptr) self = new pomodoro_main_loop_class();
    return self;
}
//
bool pomodoro_main_loop_class::destruct(){
    if(self == nullptr) return false;
    delete self;
    return true;
}
//
void pomodoro_main_loop_class::nextState(){

}
/*
class pomodoro_main_loop_class{
        enum my_states {WORKING, ON_SBREAK, ON_LBREAK};
        pomodoro_main_loop_class();
        ~pomodoro_main_loop_class();
        unsigned int my_state;
    public:
        static pomodoro_main_loop_class* getAppHandle();
        static bool destruct();
        std::function<void()> my_cb;
};*/