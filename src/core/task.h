#ifndef PILOT_CORE_TASK_H
#define PILOT_CORE_TASK_H

#include <string>
#include <vector>
#include <nlohmann/json.hpp>

namespace pilot::core {

enum class TaskStatus {
    PENDING,
    IN_PROGRESS,
    COMPLETED,
    FAILED
};

inline std::string taskStatusToString(TaskStatus status) {
    switch (status) {
        case TaskStatus::PENDING: return "PENDING";
        case TaskStatus::IN_PROGRESS: return "IN_PROGRESS";
        case TaskStatus::COMPLETED: return "COMPLETED";
        case TaskStatus::FAILED: return "FAILED";
    }
    return "UNKNOWN";
}

struct Task {
    std::string id;
    std::string title;
    std::string description;
    std::string assigned_agent;
    TaskStatus status = TaskStatus::PENDING;
    std::string result;
    std::vector<std::string> dependencies;

    nlohmann::json serialize() const;
    static Task deserialize(const nlohmann::json& j);
};

} // namespace pilot::core

#endif // PILOT_CORE_TASK_H
