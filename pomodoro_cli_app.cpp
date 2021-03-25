#include "pomodoro_cpp_linux.h"
#include <iostream>
/*
*
*
*
*
*
*/

class cli_app{
    public:

};

struct cli_config{

};

void display_help(){
    std::cout<<"Help for Pomodoro CLI app:\
    Usage:\
        pomodoro-cli {optionals}\
    Params to pass:\
        -pattern {}: Pattern of pomodoro schedule. Use S for short breaks, W for working, L for long breaks.\
            default value: WSWSWSWL\
        -w {}: Length of working period.\
            default value: 25\
        -s {}: Length of a short break.\
            default value: 5\
        -l {}: Length of a long break.\
            default value: 25\
        -o OR -i: -o for running your pattern ONCE, -i for to run it in an infinite loop. These two are mutually exclusive.\
            default value: -o\
        -h: Displays this help\
        -default: Displays the default values.\
        -f {}: Gets configuration from a JSON file you provide.\
        -s {}: Saves your configuration to a JSON file you want before starting.\
        -n {}: Notification type for switches between works and breaks. This can be:\
            0: Only on standard output.\
            1: A bell sound provided from a UNIX terminal.\
            2: A GTK notification.\
            default value: 0\
        -a {}: Waits for you to press RETURN to switch between tasks\
            0: Don't wait for me to confirm.\
            1: Wait for me to confirm.\
            default value: 0\
        -c {}: Displays a clock or timer in your terminal.\
            0: Don't show anything.\
            1: Show a timer.\
            2: Show a stopwatch.\
            3: Show the actual clock instead.\
    "<<std::endl;
}

int main(int argc, char** argv) {
    
}