#ifndef POMODORO_H
#define POMODORO_H
#include <thread>
#include <chrono>
#include <functional>
#include <iostream>


class pomodoro_timer{
        bool canceled;
        unsigned int interval;
        std::function<void()> cbFunctor;
        void setTimeout();
    public:
        pomodoro_timer(unsigned int miliseconds, std::function<void()> callback);
        void cancel();
};

class pomodoro_main_loop_class{
        enum my_states {
            WORKING, 
            ON_SBREAK, 
            ON_LBREAK
        };
        my_states state_array[8] = {
            my_states::WORKING,
            my_states::ON_SBREAK,
            my_states::WORKING,
            my_states::ON_SBREAK,
            my_states::WORKING,
            my_states::ON_SBREAK,
            my_states::WORKING,
            my_states::ON_LBREAK
        };
        pomodoro_main_loop_class();
        ~pomodoro_main_loop_class();
        unsigned int my_state;
        static pomodoro_main_loop_class* self;
    public:
        static pomodoro_main_loop_class* getAppHandle();
        static bool destruct();
        std::function<void()> my_cb;
        void nextState();
};
pomodoro_main_loop_class* pomodoro_main_loop_class::self = nullptr;

#endif //POMODORO_H