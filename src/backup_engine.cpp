#include "backup_engine.h"
#include "json_parser.h"
#include "sha256.h"
#include <fstream>
#include <sstream>
#include <filesystem>
#include <iomanip>
#include <ctime>

using namespace std;

namespace fs = std::filesystem;

static const string KOR_INDEX = "./kor.bak";

std::shared_ptr<JsonValue> indexRoot = std::make_shared<JsonValue>(JsonObject());

int BackupEngine::init() {
    if (!fs::exists(KOR_INDEX)) {
        log(INFO, "Initializing Kor backup index.");
        ofstream file(KOR_INDEX);
        file << "{\"backups\": []}";
        file.close();
    } else {
        log(INFO, "Kor backup already initialized.");
    }
    load_index();
    return 0;
}

int BackupEngine::scan(const string& source, State& state) {
    try {
        for (const auto& entry : fs::recursive_directory_iterator(source)) {
            if (fs::is_regular_file(entry)) {
                state.files.push_back(entry.path());
            }
        }
    } catch (const fs::filesystem_error& e) {
        log(WARN, "Scan error: " + string(e.what()));
    }
    return 0;
}

string BackupEngine::compute_sha256(const fs::path& filepath) {
    ifstream file(filepath, ios::binary);
    stringstream buffer;
    buffer << file.rdbuf();
    return sha256(buffer.str());
}

bool BackupEngine::is_duplicate(const string& hash) {
    const auto& arr = indexRoot->operator[]("backups")->as_array();
    for (const auto& item : arr) {
        if (item->as_object().at("sha256")->as_string() == hash) {
            return true;
        }
    }
    return false;
}

void BackupEngine::save_to_index(const string& src, const string& dest, const string& hash) {
    auto& backups = indexRoot->operator[]("backups")->as_array();
    JsonObject record;
    record["source"] = make_shared<JsonValue>(src);
    record["dest"] = make_shared<JsonValue>(dest);
    record["sha256"] = make_shared<JsonValue>(hash);

    time_t now = time(0);
    stringstream ss;
    ss << put_time(gmtime(&now), "%FT%TZ");
    record["timestamp"] = make_shared<JsonValue>(ss.str());

    backups.push_back(make_shared<JsonValue>(record));
    write_index();
}

void BackupEngine::load_index() {
    ifstream file(KOR_INDEX);
    stringstream ss;
    ss << file.rdbuf();
    indexRoot = JsonValue::parse(ss.str());
    file.close();
}

void BackupEngine::write_index() {
    ofstream file(KOR_INDEX);
    file << "{\"backups\": [";
    const auto& backups = indexRoot->operator[]("backups")->as_array();
    for (size_t i = 0; i < backups.size(); ++i) {
        const auto& obj = backups[i]->as_object();
        file << "{\"source\": \"" << obj.at("source")->as_string() << "\",";
        file << "\"dest\": \"" << obj.at("dest")->as_string() << "\",";
        file << "\"sha256\": \"" << obj.at("sha256")->as_string() << "\",";
        file << "\"timestamp\": \"" << obj.at("timestamp")->as_string() << "\"}";
        if (i + 1 != backups.size()) file << ",";
    }
    file << "]}";
    file.close();
}

int BackupEngine::commit(const string& source, const string& dest, State& state) {
    scan(source, state);
    for (const auto& path : state.files) {
        string hash = compute_sha256(path);
        if (is_duplicate(hash)) {
            log(INFO, "Duplicate skipped: " + path);
            continue;
        }

        fs::path rel = fs::relative(path, source);
        fs::path target = fs::path(dest) / rel;
        fs::create_directories(target.parent_path());

        try {
            fs::copy_file(path, target, fs::copy_options::overwrite_existing);
            log(INFO, "Backed up: " + path);
            save_to_index(path, target.string(), hash);
        } catch (const fs::filesystem_error& e) {
            log(WARN, "Failed to backup: " + path + ", reason: " + string(e.what()));
        }
    }
    return 0;
}

int BackupEngine::log(const LogType& type, const string& statement) {
    auto time_now = chrono::system_clock::now();
    auto to_time_t = chrono::system_clock::to_time_t(time_now);
    const string level = (type == INFO ? "INFO" : type == WARN ? "WARN" : "HALT");
    cout << "[" << put_time(localtime(&to_time_t), "%Y-%m-%d %H:%M:%S") << "] [" << level << "] " << statement << endl;
    if (type == HALT) exit(1);
    return 0;
}
