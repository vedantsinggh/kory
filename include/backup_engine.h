#pragma once
#include <string>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <cassert>

enum LogType {
    INFO,
    WARN,
    HALT
};

class BackupEngine {
    public:
        int scan(const std::string& source);
        int log(const LogType& type, const std::string& statement);
};
