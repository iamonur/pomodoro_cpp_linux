#include "pomodoro_cpp_linux.h"
#include <iostream>
#include <getopt.h>
#include <string>
/*
*
*
*
*
*
*/

struct action_cli : public action {
    std::string my_item_type;
    action_cli(std::string typ){my_item_type = typ;}
};

struct action_nothing : public action_cli{
    void act(int duration){
        if(my_item_type == "end"){
            std::cout<<"Well done! Your pomodoro period has been ended."<<std::endl;
        }
        else std::cout<<"Time to "<<my_item_type<<". Duration of it is: "<<duration<<" minutes."<<std::endl;
    }
};

struct action_timer : public action_cli {
};

struct action_stopwatch : public action_cli {
};

struct action_clock : public action_cli {    
};



class cli_app{
        std::vector<pomodoro_main_loop_class::my_states> stateList;
    public:
        cli_app(cli_config config){
            std::string pattern = config.pattern;
            for(unsigned int i = 0; i < pattern.size(); i++)
                switch (pattern[i])
                {
                case 'W':
                    stateList.push_back(pomodoro_main_loop_class::my_states::WORKING);
                    break;
                case 'S':
                    stateList.push_back(pomodoro_main_loop_class::my_states::ON_SBREAK);
                    break;
                case 'L':
                    stateList.push_back(pomodoro_main_loop_class::my_states::ON_LBREAK);                
                    break;
                default:
                    display_help();
                    abort();
                    break;
                }
            stateList.push_back(pomodoro_main_loop_class::my_states::END);

            pomodoro_parameters params;
            params.work_time = config.workTime;
            params.short_break = config.sbreakTime;
            params.long_break = config.lbreakTime;
            if(config.isInfinite) params.my_type = pomodoro_parameters::pomodoro_types::INFINITE;
            else params.my_type = pomodoro_parameters::pomodoro_types::ONCE;
        }
};


struct cli_config{
    std::string pattern = "WSWSWSWLE";
    unsigned int workTime = 25;
    unsigned int sbreakTime = 5;
    unsigned int lbreakTime = 25;
    int isInfinite = 0;
    int isBellOn = 0;
    int isConfirmationEnabled = 0;
    enum clockType{NOTHING, TIMER, STOPWATCH, CLOCK};
    clockType cType = NOTHING;
};

void display_help(){
    std::cout<<"Help for Pomodoro CLI app:\
    Usage:\
        pomodoro-cli {optionals}\
    Params to pass:\
        -p {}: Pattern of pomodoro schedule. Use S for short breaks, W for working, L for long breaks.\
            default value: WSWSWSWL\
        -w {}: Length of working period.\
            default value: 25\
        -s {}: Length of a short break.\
            default value: 5\
        -l {}: Length of a long break.\
            default value: 25\
        -i: Do not pass -i for running your pattern ONCE, pass -i for to run it in an infinite loop.\
            default value: -o\
        -h: Displays this help\
        -n : Enables a bell sound to be heard between switches.\
        -a: Waits for you to press RETURN to switch between tasks\
            default value: Don't wait for me to confirm.\
        -c {}: Displays a clock or timer in your terminal.\
            0: Don't show anything.\
            1: Show a timer.\
            2: Show a stopwatch.\
            3: Show the actual clock instead.\
            default value: 0\
    "<<std::endl;
}

int main(int argc, char** argv) {
    cli_config cli_params;
    int c;
    while( (c =getopt(argc,argv, "p:w:s:l:ihnac:")) != -1){
        switch(c){
            case 'p':
                cli_params.pattern = std::string(optarg);
            break;
            case 'w':
                cli_params.workTime = atoi(optarg);
            break;
            case 's':
                cli_params.sbreakTime = atoi(optarg);
            break;
            case 'l':
                cli_params.lbreakTime = atoi(optarg);
            break;
            case 'i':
                cli_params.isInfinite = true;
            break;
            case 'h':
                display_help();
                abort();
            break;
            case 'n':
                cli_params.isBellOn = true;
            break;
            case 'a':
                cli_params.isConfirmationEnabled = true;
            break;
            case 'c':
                switch (atoi(optarg))
                {
                case 0:
                    cli_params.cType = cli_params.NOTHING;
                break;
                case 1:
                    cli_params.cType = cli_params.TIMER;
                break;
                case 2:
                    cli_params.cType = cli_params.STOPWATCH;
                break;
                case 3:
                    cli_params.cType = cli_params.CLOCK;
                break;                
                default:
                    display_help();
                    abort();
                break;
                }
            break;
            default:
                display_help();
                abort();
        }
    }
    
    
}