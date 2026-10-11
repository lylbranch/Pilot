#include "core/context.h"

namespace pilot::core {

void Context::set(const std::string& key, const nlohmann::json& value) {
    std::lock_guard<std::mutex> lock(mutex_);
    store_[key] = value;
}

nlohmann::json Context::get(const std::string& key) const {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = store_.find(key);
    if (it != store_.end()) {
        return it->second;
    }
    return nullptr;
}

bool Context::contains(const std::string& key) const {
    std::lock_guard<std::mutex> lock(mutex_);
    return store_.find(key) != store_.end();
}

void Context::remove(const std::string& key) {
    std::lock_guard<std::mutex> lock(mutex_);
    store_.erase(key);
}

void Context::setWorkspacePath(const std::string& path) {
    std::lock_guard<std::mutex> lock(mutex_);
    workspace_path_ = path;
}

std::string Context::getWorkspacePath() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return workspace_path_;
}

void Context::clear() {
    std::lock_guard<std::mutex> lock(mutex_);
    store_.clear();
    workspace_path_ = ".";
}

nlohmann::json Context::serialize() const {
    std::lock_guard<std::mutex> lock(mutex_);
    nlohmann::json j;
    j["workspace_path"] = workspace_path_;
    j["store"] = store_;
    return j;
}

void Context::deserialize(const nlohmann::json& j) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (j.contains("workspace_path")) {
        workspace_path_ = j["workspace_path"].get<std::string>();
    }
    if (j.contains("store")) {
        store_ = j["store"].get<std::unordered_map<std::string, nlohmann::json>>();
    }
}

} // namespace pilot::core
