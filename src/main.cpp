#include "cli.h"
#include "backup_engine.h"
#include <iostream>

int show_help(const State& state){
    std::cout << "this is help" << std::endl;
    return 0;
}

int main(int argc, char* argv[]){
    cli tool;
    State state;

    tool.add_arguments("help", *show_help);
    tool.run(state, argc, argv);

    return 0;
}
