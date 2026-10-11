#ifndef PILOT_CORE_AGENT_H
#define PILOT_CORE_AGENT_H

#include <string>
#include <memory>
#include "core/context.h"
#include "core/task.h"
#include "llm/llm_client.h"

namespace pilot::core {

class Agent {
public:
    Agent(std::string name, std::string system_prompt, std::shared_ptr<llm::ILLMClient> llm_client);
    virtual ~Agent() = default;

    const std::string& getName() const { return name_; }
    const std::string& getSystemPrompt() const { return system_prompt_; }

    virtual std::string execute(Task& task, Context& context);

protected:
    std::string name_;
    std::string system_prompt_;
    std::shared_ptr<llm::ILLMClient> llm_client_;
};

} // namespace pilot::core

#endif // PILOT_CORE_AGENT_H
