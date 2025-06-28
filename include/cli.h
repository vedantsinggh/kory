#pragma once

#include <string>
#include <iostream>

struct Output {
    bool show_help = false;
    std::string name;
};

class cli {
    public:
        Output parse_args(int argc, char* argv[]);
        void run(const Output& out);
    private:
        void show_help() const;
};
