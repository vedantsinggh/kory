#include "backup_engine.h"

int BackupEngine::scan(const std::string& source){
    (void)source;
    return 0;
}

int BackupEngine::log(const LogType& type, const std::string& statement){
    (void) statement;
    auto time_now = std::chrono::system_clock::now();
    auto to_time_t = std::chrono::system_clock::to_time_t(time_now);
    
    const std::string level_str = type == INFO ? "INFO" : type == WARN ? "WARN" : "HALT";
    std::cout << "[" << std::put_time(std::localtime(&to_time_t), "%Y-%m-%d %H:%M:%S")
              << "] [" << level_str << "] " << statement << std::endl;

    if (type == HALT){
        exit(1);
    }

    return 0;
}
