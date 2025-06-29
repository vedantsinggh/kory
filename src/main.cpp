#include "cli.h"
#include "backup_engine.h"
#include <iostream>

int main(int argc, char* argv[]){
    (void) argc;
    BackupEngine engine;
    State state;

    engine.scan(argv[1], state);

    for(auto& file : state.files){
        std::cout << file << std::endl;
    }

    return 0;
}
