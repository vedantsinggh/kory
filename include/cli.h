#pragma once

#include <string>
#include <iostream>
#include <unordered_map>
#include "backup_engine.h"

struct Command {
    std::string name;
    int (*run)(const State& state);
};

class cli {
    public:
        std::unordered_map<std::string, Command> command_map;
        void add_arguments(const std::string name, int (*func)(const State& state));
        void run(const State& state, int argc, char* argv[]);
};
