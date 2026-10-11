#ifndef PILOT_CORE_ORCHESTRATOR_H
#define PILOT_CORE_ORCHESTRATOR_H

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include "core/context.h"
#include "core/task.h"
#include "core/agent.h"
#include "core/message_bus.h"

namespace pilot::core {

class Orchestrator {
public:
    Orchestrator();
    ~Orchestrator() = default;

    void registerAgent(std::shared_ptr<Agent> agent);
    void addTask(const Task& task);
    
    void runWorkflow(const std::string& user_prompt);

    Context& getContext() { return context_; }
    MessageBus& getMessageBus() { return message_bus_; }

private:
    Context context_;
    MessageBus message_bus_;
    std::unordered_map<std::string, std::shared_ptr<Agent>> agents_;
    std::vector<Task> tasks_;
};

} // namespace pilot::core

#endif // PILOT_CORE_ORCHESTRATOR_H
