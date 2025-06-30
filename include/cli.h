#pragma once

#include <string>
#include <iostream>
#include <vector>
#include "backup_engine.h"

struct Command {
    std::string name;
    int (*run)(const State& state);
};

class cli {
    public:
        std::vector<Command> commands;
        void add_arguments(const std::string name, int (*func)(const State& state));
        void run(const State& state, int argc, char* argv[]);
};
