#include "pomodoro_cpp_linux.h"

pomodoro_main_loop_class* pomodoro_main_loop_class::self = nullptr;

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
pomodoro_timer::~pomodoro_timer(){
    cancel();
}
//
pomodoro_main_loop_class::pomodoro_main_loop_class(pomodoro_parameters params, pomodoro_actions actions, std::vector<my_states> stateVec){
    my_state = -1;
    states = stateVec;
    my_params = params;
    my_actions = actions; 
    my_cb = []() {
        
        pomodoro_main_loop_class::getAppHandle()->nextState();
    };
    nextState();
}
//
pomodoro_main_loop_class::~pomodoro_main_loop_class(){

}
//
pomodoro_main_loop_class* pomodoro_main_loop_class::getAppHandle(pomodoro_parameters params, pomodoro_actions actions, std::vector<my_states> stateVec){
    if(self == nullptr) self = new pomodoro_main_loop_class(params, actions, stateVec);
    return self;
}
//
pomodoro_main_loop_class* pomodoro_main_loop_class::getAppHandle(){
    if (self == nullptr) throw "WTF";
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
    if(current_timer != nullptr){
        delete current_timer;
        current_timer = nullptr;
    }
    my_state++;
    my_state = my_state%(states.size());
    switch(states[my_state]){
        case my_states::WORKING:
            current_timer = new pomodoro_timer(my_params.work_time, my_cb);
             my_actions.working->act(my_params.work_time);
        break;
        case my_states::ON_SBREAK:
            current_timer = new pomodoro_timer(my_params.short_break, my_cb);
            my_actions.s_break->act(my_params.short_break);
        break;
        case my_states::ON_LBREAK:
            current_timer = new pomodoro_timer(my_params.long_break, my_cb);
            my_actions.l_break->act(my_params.long_break);
        break;
        case my_states::END:
            my_actions.end->act(0);
            my_state = -1;
            if(my_params.my_type == pomodoro_parameters::INFINITE) nextState();
        break;
        default:
            std::cout<<my_state<<std::endl;
            throw "WTF2";
    }
        

}
//
#ifdef STANDALONE_TEST
std::vector<pomodoro_main_loop_class::my_states> statelist = {
            pomodoro_main_loop_class::my_states::WORKING,
            pomodoro_main_loop_class::my_states::ON_SBREAK,
            pomodoro_main_loop_class::my_states::WORKING,
            pomodoro_main_loop_class::my_states::ON_SBREAK,
            pomodoro_main_loop_class::my_states::WORKING,
            pomodoro_main_loop_class::my_states::ON_SBREAK,
            pomodoro_main_loop_class::my_states::WORKING,
            pomodoro_main_loop_class::my_states::ON_LBREAK,
            pomodoro_main_loop_class::my_states::END
};
struct w_act : public action{
    w_act() = default;
    void act(int duration){
        std::cout<<"Dummy work action."<<std::endl;
    }
};
struct s_act : public action{
    s_act() = default;
    void act(int duration){
        std::cout<<"Dummy short action."<<std::endl;
    }
};
struct l_act : public action{
    l_act() = default;
    void act(int duration){
        std::cout<<"Dummy long action."<<std::endl;
    }
};
struct e_act : public action{
    e_act() = default;
    void act(int duration){
        std::cout<<"End"<<std::endl;
    }
};
int main(){
    pomodoro_main_loop_class* a = pomodoro_main_loop_class::getAppHandle({pomodoro_parameters::INFINITE, 1000, 1000, 1000},{new w_act(), new s_act(), new l_act(), new e_act()}, statelist);
    while(true){
        std::this_thread::sleep_for(std::chrono::milliseconds(1000000));
    }
}
#endif