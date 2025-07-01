#include "cli.h"

void cli::add_command(const std::string name, int (*func)(const State& state)){
    const Command cmd = {
        name,
        func,
        {}
    };
    command_map[name] = cmd;
}

void cli::add_subcommand(const std::string command_name, const std::string subcommand_name, int (*func)(const State& state)){
    SubCommand subcmd = {
        subcommand_name,
        func
    };
    
    if(command_map.find(command_name) != command_map.end()){
        command_map[command_name].subcommands.push_back(subcmd);
    } else{
        std::cout << "Error:Command '" << command_name << "' not found when adding subcommand '" 
                  << subcommand_name << "'" << std::endl;
    }
}

void cli::run(const State& state, int argc, char* argv[]){
    if(argc == 1){
        command_map["help"].run(state);
        return;
    }

    std::string command_name = argv[1];
    
    if(command_map.find(command_name) == command_map.end()){
        command_map["help"].run(state);
        return;
    }

    if(argc == 2){
        command_map[command_name].run(state);
        return;
    }

    std::string subcommand_name = argv[2];
    auto& command = command_map[command_name];

    if (argc > 2){
        for (const auto& subcmd : command.subcommands) {
            if (subcmd.name == subcommand_name) {
                subcmd.run(state);
                return;
            }
        }
        command_map["help"].run(state);   // here we have to decide what to do if there is no subcommand present
        return;
    }
}