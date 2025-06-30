#include "cli.h"

void cli::add_arguments(const std::string name, int (*func)(const State& state)){
   const Command cmd = {
       name,
       func
   };

   command_map[name] = cmd;
}

void cli::run(const State& state, int argc, char* argv[]){
    for(int i = 1; i < argc; i++){
        std::string arg = argv[i];
        
        if(command_map.find(arg) != command_map.end()){
            command_map[arg].run(state);
            break;
        }else {
            command_map["help"].run(state);
            break;
        }
    }
    if (argc == 1) command_map["help"].run(state);
}
