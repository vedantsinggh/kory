#include "backup_engine.h"

int BackupEngine::init(){
    if (!std::filesystem::exists("./kor.bak")){
        log(INFO, "Intializing Kor directory for backup!");
        std::ofstream file;
        try {
            file.open("./kor.bak");
            log(INFO, "Created kor.bak");
        }catch (...) {
            log(HALT, "Unable to create backup config file!");
        }
        return 0;
    }else {
        log(INFO, "Reinitlised kor backup folder!");
        return 0;
    }
}

int BackupEngine::scan(const std::string& source, State& state){
    try{
        for (const auto& entry : std::filesystem::directory_iterator(source)){
            if(std::filesystem::is_directory(entry)){
                scan(entry.path(), state);
            }else{
                state.files.push_back(entry.path());
            }
        }
    } catch (const std::filesystem::filesystem_error& e){
        log(WARN, "Access denied or error: " + std::string(e.what()));
    }
    return 0;
}

int BackupEngine::log(const LogType& type, const std::string& statement){
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
