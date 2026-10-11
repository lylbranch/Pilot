#include "core/orchestrator.h"
#include <iostream>

namespace pilot::core {

Orchestrator::Orchestrator() {
    message_bus_.subscribe("*", [](const MessageBusMessage& msg) {
        std::cout << "[MessageBus][" << msg.topic << "] " << msg.sender << " -> " 
                  << (msg.receiver.empty() ? "BROADCAST" : msg.receiver) << "\n";
    });
}

void Orchestrator::registerAgent(std::shared_ptr<Agent> agent) {
    agents_[agent->getName()] = agent;
    std::cout << "[Orchestrator] Registered agent: " << agent->getName() << "\n";
}

void Orchestrator::addTask(const Task& task) {
    tasks_.push_back(task);
}

void Orchestrator::runWorkflow(const std::string& user_prompt) {
    std::cout << "[Orchestrator] Starting workflow for prompt: \"" << user_prompt << "\"\n";
    context_.set("user_prompt", user_prompt);

    // If no tasks defined yet, create a default planning task
    if (tasks_.empty()) {
        Task default_task;
        default_task.id = "task_1";
        default_task.title = "Analyze and Implement Request";
        default_task.description = user_prompt;
        default_task.assigned_agent = "planner";
        tasks_.push_back(default_task);
    }

    for (auto& task : tasks_) {
        std::cout << "\n----------------------------------------\n";
        std::cout << "[Orchestrator] Executing Task [" << task.id << "]: " << task.title << "\n";
        std::cout << "[Orchestrator] Assigned Agent: " << task.assigned_agent << "\n";

        auto it = agents_.find(task.assigned_agent);
        if (it != agents_.end()) {
            task.status = TaskStatus::IN_PROGRESS;
            message_bus_.publish({
                "orchestrator", task.assigned_agent, "task_start", task.serialize()
            });

            std::string result = it->second->execute(task, context_);
            task.result = result;
            task.status = TaskStatus::COMPLETED;

            std::cout << "[Orchestrator] Task Result Output:\n" << result << "\n";

            message_bus_.publish({
                task.assigned_agent, "orchestrator", "task_complete", task.serialize()
            });
            std::cout << "[Orchestrator] Task [" << task.id << "] Completed Successfully.\n";
        } else {
            std::cerr << "[Orchestrator] Error: Agent not found: " << task.assigned_agent << "\n";
            task.status = TaskStatus::FAILED;
        }
    }
    std::cout << "\n[Orchestrator] Workflow finished.\n";
}

} // namespace pilot::core
