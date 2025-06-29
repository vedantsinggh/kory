#pragma once
#include <string>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <cassert>
#include <filesystem>
#include <vector>

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
        int scan(const std::string& source, State& state);
        int log(const LogType& type, const std::string& statement);
};
