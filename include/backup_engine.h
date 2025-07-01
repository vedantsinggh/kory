#pragma once
#include <string>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <filesystem>
#include <vector>
#include <fstream>
#include "sha256.h"

enum LogType {
    INFO,
    WARN,
    HALT
};

enum Status {
    RAW,
    SCANNED,
    COMMITTED,
    COMPLETED
};

struct State {
    Status status;
    std::vector<std::string> files;
};

class BackupEngine {
    public:
        int init();
        int scan(const std::string& source, State& state);
        int log(const LogType& type, const std::string& statement);
    private:
        std::string config_file;
};
