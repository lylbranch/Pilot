#include "core/task.h"

namespace pilot::core {

nlohmann::json Task::serialize() const {
    nlohmann::json j;
    j["id"] = id;
    j["title"] = title;
    j["description"] = description;
    j["assigned_agent"] = assigned_agent;
    j["status"] = taskStatusToString(status);
    j["result"] = result;
    j["dependencies"] = dependencies;
    return j;
}

Task Task::deserialize(const nlohmann::json& j) {
    Task task;
    if (j.contains("id")) task.id = j["id"].get<std::string>();
    if (j.contains("title")) task.title = j["title"].get<std::string>();
    if (j.contains("description")) task.description = j["description"].get<std::string>();
    if (j.contains("assigned_agent")) task.assigned_agent = j["assigned_agent"].get<std::string>();
    if (j.contains("status")) {
        std::string status_str = j["status"].get<std::string>();
        if (status_str == "PENDING") task.status = TaskStatus::PENDING;
        else if (status_str == "IN_PROGRESS") task.status = TaskStatus::IN_PROGRESS;
        else if (status_str == "COMPLETED") task.status = TaskStatus::COMPLETED;
        else if (status_str == "FAILED") task.status = TaskStatus::FAILED;
    }
    if (j.contains("result")) task.result = j["result"].get<std::string>();
    if (j.contains("dependencies")) task.dependencies = j["dependencies"].get<std::vector<std::string>>();
    return task;
}

} // namespace pilot::core
