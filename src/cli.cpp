#include "cli.h"

Output cli::parse_args(int argc, char* argv[]) {
    Output out;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-h" || arg == "--help") {
            out.show_help = true;
        } else if (arg == "-n" || arg == "--name") {
            if (i + 1 < argc) {
                out.name = argv[++i];
            } else {
                std::cerr << "Error: --name option requires a value.\n";
            }
        }
    }

    return out;
}

void cli::run(const Output& out) {
    if (out.show_help) {
        show_help();
        return;
    }

    if (!out.name.empty()) {
        std::cout << "Hello, " << out.name << "!\n";
    } else {
        std::cout << "Hello, World!\n";
    }
}

void cli::show_help() const {
    std::cout << "Usage: cli-tool [options]\n"
              << "Options:\n"
              << "  -h, --help        HELP!\n"
              << "  -n, --name NAME   return the nme as it is\n";
}
