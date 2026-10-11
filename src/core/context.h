#ifndef PILOT_CORE_CONTEXT_H
#define PILOT_CORE_CONTEXT_H

#include <string>
#include <unordered_map>
#include <mutex>
#include <nlohmann/json.hpp>

namespace pilot::core {

/**
 * @brief Shared execution context and key-value store for agents.
 */
class Context {
public:
    Context() = default;
    ~Context() = default;

    void set(const std::string& key, const nlohmann::json& value);
    nlohmann::json get(const std::string& key) const;
    bool contains(const std::string& key) const;
    void remove(const std::string& key);

    void setWorkspacePath(const std::string& path);
    std::string getWorkspacePath() const;

    void clear();

    nlohmann::json serialize() const;
    void deserialize(const nlohmann::json& j);

private:
    mutable std::mutex mutex_;
    std::unordered_map<std::string, nlohmann::json> store_;
    std::string workspace_path_ = ".";
};

} // namespace pilot::core

#endif // PILOT_CORE_CONTEXT_H
