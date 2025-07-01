#include "cli.h"
#include "backup_engine.h"
#include <iostream>

int cumand(const State& state){
    std::cout << "this is cumand" << std::endl;
    return 0;
}

int subcumand(const State& state){
    std::cout << "this is subcumand" << std::endl;
    return 0;
}

int init(const State& state){
    engine.init();
    return 0;
}

int main(int argc, char* argv[]){
    cli tool;
    State state;

    tool.add_command("cumand", *cumand);
    tool.add_subcommand("cumand","subcumand",*subcumand);
    tool.run(state, argc, argv);

    return 0;
}
