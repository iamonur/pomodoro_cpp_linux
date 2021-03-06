#ifndef POMODORO_H
#define POMODORO_H
#include <thread>
#include <chrono>
#include <functional>
#include <iostream>
#include <vector>
struct action{
    virtual void act() = 0;
};

struct pomodoro_parameters{
    enum pomodoro_types {
        INFINITE,
        ONCE
    };
    pomodoro_types my_type;
    unsigned int work_time;
    unsigned int short_break;
    unsigned int long_break;
};

struct pomodoro_actions{
    action* working;
    action* s_break;
    action* l_break;
    action* end;
};

class pomodoro_timer{
        bool canceled;
        unsigned int interval;
        std::function<void()> cbFunctor;
        void setTimeout();
    public:
        pomodoro_timer(unsigned int miliseconds = 0, std::function<void()> callback = {});
        ~pomodoro_timer();
        void cancel();
};

class pomodoro_main_loop_class{
    public:
        enum my_states {
            WORKING, 
            ON_SBREAK, 
            ON_LBREAK,
            END
        };
    private:    
        std::vector<my_states> states;
        pomodoro_main_loop_class(pomodoro_parameters params, pomodoro_actions actions, std::vector<my_states> stateVec);
        ~pomodoro_main_loop_class();
        unsigned int my_state;
        static pomodoro_main_loop_class* self;
        pomodoro_parameters my_params;
        pomodoro_actions my_actions;
        pomodoro_timer* current_timer = nullptr;
    public:
        static pomodoro_main_loop_class* getAppHandle(pomodoro_parameters params, pomodoro_actions actions, std::vector<my_states> stateVec);
        static pomodoro_main_loop_class* getAppHandle();
        static bool destruct();
        std::function<void()> my_cb;
        void nextState();
};

pomodoro_main_loop_class* pomodoro_main_loop_class::self = nullptr;

#endif //POMODORO_H