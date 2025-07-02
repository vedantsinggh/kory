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
        int commit(const std::string& source, const std::string& dest, State& state);
        std::string compute_sha256(const std::filesystem::path& filepath);
        bool is_duplicate(const std::string& hash);
        void save_to_index(const std::string& src, const std::string& dest, const std::string& hash);
        void load_index();
        void write_index();
    private:
        std::string config_file;
};
