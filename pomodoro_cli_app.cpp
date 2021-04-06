#include "common/pomodoro_cpp_linux.h"
#include <iostream>
#include <iomanip>
#include <getopt.h>
#include <string>

//Thanks to: https://stackoverflow.com/questions/15777073/how-do-you-print-a-c11-time-point
template<typename Clock, typename Duration> std::ostream &operator<<(std::ostream &stream, const std::chrono::time_point<Clock, Duration> &time_point) {
  const time_t time = Clock::to_time_t(time_point);
#if __GNUC__ > 4 || \
    ((__GNUC__ == 4) && __GNUC_MINOR__ > 8 && __GNUC_REVISION__ > 1)
  // Maybe the put_time will be implemented later?
  struct tm tm;
  localtime_r(&time, &tm);
  return stream << std::put_time(&tm, "%c"); // Print standard date&time
#else
  char buffer[26];
  ctime_r(&time, buffer);
  buffer[24] = '\0';  // Removes the newline that is added
  return stream << buffer;
#endif
}

struct action_cli : public action {
    std::string my_item_type;
    action_cli(std::string typ){my_item_type = typ;}
};

struct action_nothing : public action_cli{
    using action_cli::action_cli;
    void act(int duration){
        if(my_item_type == "end"){
            std::cout<<"Well done! Your pomodoro period has been ended."<<std::endl;
        }
        else std::cout<<"Time to "<<my_item_type<<". Duration of it is: "<<duration<<" minutes."<<std::endl;
    }
};

struct action_timer : public action_cli {
    using action_cli::action_cli;
    std::thread timer_thread;
    void act(int duration){
        timer_thread = std::thread([=](){
            int dur = duration*60;
            while(dur){
                system("clear");
                std::cout<<"Time to finish: "<<my_item_type<<". Target: "<<duration<<":00"<<std::endl;
                std::cout<<std::setfill(' ')<<std::setw(20)<<"Minutes: "<<std::setw(2)<<dur/60<<std::setw(20)<<"Seconds: "<<std::setw(2)<<dur%60<<std::endl;
                dur--;
                std::this_thread::sleep_for(std::chrono::seconds(1)); //TODO: There will be losses here, try to build a more accurate timer.
            }

        });
        timer_thread.detach();
    }
};

struct action_stopwatch : public action_cli {
    using action_cli::action_cli;
    std::thread stopwatch_thread;
    void act(int duration){
        stopwatch_thread = std::thread([=](){
            int dur = 0;
            int foo = duration*60;
            while(dur != foo){
                system("clear");
                std::cout<<"Time to finish: "<<my_item_type<<". Target: "<<duration<<":00"<<std::endl;
                std::cout<<std::setfill(' ')<<std::setw(20)<<"Minutes: "<<dur/60<<std::setw(20)<<"Seconds: "<<std::setw(2)<<dur%60<<std::endl;
                dur++;
                std::this_thread::sleep_for(std::chrono::seconds(1)); //TODO: There will be losses here, try to build a more accurate stopwatch.
            }
        });
        stopwatch_thread.detach();
    }
};

struct action_clock : public action_cli {
    using action_cli::action_cli;
    std::thread clock_thread;
    void act(int duration){
        clock_thread = std::thread([=](){
            std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
            std::chrono::time_point<std::chrono::system_clock> end   = start + std::chrono::minutes(duration);
            while(std::chrono::system_clock::now() < end){
                system("clear");
                std::cout<<"It's time to: "<<my_item_type<<". It will be finished at: "<<end<<std::endl;
                std::cout<<"Now it is: "<<std::chrono::system_clock::now()<<std::endl;
            }
        });
        clock_thread.detach();
    }
};

void display_help(){
    std::cout<<"Help for Pomodoro CLI app:\n\
    Usage:\n\
        pomodoro-cli {optionals}\n\
    Params to pass:\n\
        -p {}: Pattern of pomodoro schedule. Use S for short breaks, W for working, L for long breaks.\n\
            default value: WSWSWSWL\n\
        -w {}: Length of working period.\n\
            default value: 25\n\
        -s {}: Length of a short break.\n\
            default value: 5\n\
        -l {}: Length of a long break.\n\
            default value: 25\n\
        -i: Do not pass -i for running your pattern ONCE, pass -i for to run it in an infinite loop.\n\
            default value: -o\n\
        -h: Displays this help\n\
        -n : Enables a bell sound to be heard between switches.\n\
        -a: Waits for you to press RETURN to switch between tasks\n\
            default value: Don't wait for me to confirm.\n\
        -c {}: Displays a clock or timer in your terminal.\n\
            0: Don't show anything.\n\
            1: Show a timer.\n\
            2: Show a stopwatch.\n\
            3: Show the actual clock instead.\n\
            default value: 0\n\
    "<<std::endl;
}

struct cli_config{
    std::string pattern = "WSWSWSWL";
    unsigned int workTime = 25;
    unsigned int sbreakTime = 5;
    unsigned int lbreakTime = 25;
    int isInfinite = 0;
    int isBellOn = 0;
    int isConfirmationEnabled = 0;
    enum clockType{NOTHING, TIMER, STOPWATCH, CLOCK};
    clockType cType = NOTHING;
};

class cli_app{
        std::vector<pomodoro_main_loop_class::my_states> stateList;
        pomodoro_main_loop_class* pomodoro;
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
                    std::cout<<pattern[i]<<std::endl;
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

            pomodoro_actions actions;
            actions.end = new action_nothing(std::string("end"));
            switch(config.cType){
                case cli_config::clockType::NOTHING:
                    actions.working = new action_nothing(std::string("work"));
                    actions.s_break = new action_nothing(std::string("short break"));
                    actions.l_break = new action_nothing(std::string("long break"));
                break;
                case cli_config::clockType::STOPWATCH:
                    actions.working = new action_stopwatch(std::string("work"));
                    actions.s_break = new action_stopwatch(std::string("short break"));
                    actions.l_break = new action_stopwatch(std::string("long break"));
                break;
                case cli_config::clockType::TIMER:
                    actions.working = new action_timer(std::string("work"));
                    actions.s_break = new action_timer(std::string("short break"));
                    actions.l_break = new action_timer(std::string("long break"));
                break;
                case cli_config::clockType::CLOCK:
                    actions.working = new action_clock(std::string("work"));
                    actions.s_break = new action_clock(std::string("short break"));
                    actions.l_break = new action_clock(std::string("long break"));
                break;
                default:
                    display_help();
                    abort();
            }
            pomodoro = pomodoro_main_loop_class::getAppHandle(params, actions, stateList);
        }
        static cli_app* self;
    public:
        static cli_app* getCLIApp(){
            if(self != nullptr) return self;
            throw new std::exception();//TODO: custom exception.
        }
        static cli_app* getCLIApp(cli_config conf){
            if(self == nullptr) self = new cli_app(conf);
            return self;
        }
};
cli_app* cli_app::self = nullptr;



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
    
    cli_app* app = cli_app::getCLIApp(cli_params);
    while(true){
        std::this_thread::sleep_for(std::chrono::seconds(10000));
    }
}