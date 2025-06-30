#include "cli.h"

void cli::add_arguments(const std::string name, int (*func)(const State& state)){
   const Command cmd = {
       name,
       func
   };

   commands.push_back(cmd);
}

void cli::run(const State& state, int argc, char* argv[]){
    for(int i = 1; i < argc; i++){
        std::string arg = argv[i];

        for(const Command cmd : commands){
            if(arg == cmd.name){
                cmd.run(state);
                break;
            }
        }
    }
}
