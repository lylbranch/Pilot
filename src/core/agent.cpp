#include "core/agent.h"
#include <iostream>

namespace pilot::core {

Agent::Agent(std::string name, std::string system_prompt, std::shared_ptr<llm::ILLMClient> llm_client)
    : name_(std::move(name)), system_prompt_(std::move(system_prompt)), llm_client_(std::move(llm_client)) {}

std::string Agent::execute(Task& task, Context& context) {
    std::cout << "[Agent: " << name_ << "] Executing task: " << task.title << "\n";

    std::vector<llm::Message> messages;
    messages.push_back({"system", system_prompt_});
    
    std::string user_input = "Task Title: " + task.title + "\nDescription: " + task.description;
    if (context.contains("user_prompt")) {
        user_input += "\nOverall User Prompt: " + context.get("user_prompt").get<std::string>();
    }
    messages.push_back({"user", user_input});

    llm::LLMOptions options;
    options.model = "gpt-4o";

    std::string response = llm_client_->chat(messages, options);
    return response;
}

} // namespace pilot::core
