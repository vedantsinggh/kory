#include "cli.h"
#include "backup_engine.h"
#include <iostream>

BackupEngine engine;

int cmd_init(const State& state) {
    return engine.init();
}

int cmd_scan(const State& state) {
    State s;
    engine.scan(".", s);
    for (const auto& f : s.files) {
        std::cout << f << std::endl;
    }
    return 0;
}

int cmd_help(const State&) {
    std::cout << "Kor Backup Tool - Available Commands:\n";
    std::cout << "  init                Initialize backup index file.\n";
    std::cout << "  scan                Recursively list files.\n";
    std::cout << "  help                Show this help message.\n";
    return 0;
}

int main(int argc, char* argv[]) {
    cli tool;
    State state;

    tool.add_command("init", cmd_init);
    tool.add_command("scan", cmd_scan);
    tool.add_command("help", cmd_help);

    tool.run(state, argc, argv);
    return 0;
}
