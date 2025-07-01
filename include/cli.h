#pragma once

#include <string>
#include <iostream>
#include <unordered_map>
#include <vector>
#include "backup_engine.h"

struct SubCommand {
    std::string name;
    int (*run)(const State& state);
};

struct Command {
    std::string name;
    int (*run)(const State& state);
    std::vector<SubCommand> subcommands;
};

class cli {
    public:
        std::unordered_map<std::string, Command> command_map;
        void add_command(const std::string name, int (*func)(const State& state));
        void add_subcommand(const std::string command_name, const std::string subcommand_name, int (*func)(const State& state));
        void run(const State& state, int argc, char* argv[]);
};