#include "cli.h"
#include "backup_engine.h"
#include <iostream>

int show_help(const State& state){
    std::cout << "this is help" << std::endl;
    return 0;
}

int main(int argc, char* argv[]){
    (void) argc;
    BackupEngine engine;

    cli tool;
    tool.add_arguments("help", *show_help);
    
    State state;
    tool.run(state, argc, argv);

    return 0;
}
