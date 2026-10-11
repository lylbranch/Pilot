#ifndef PILOT_LLM_CLIENT_H
#define PILOT_LLM_CLIENT_H

#include <string>
#include <vector>
#include <memory>
#include <nlohmann/json.hpp>

namespace pilot::llm {

struct Message {
    std::string role; // "system", "user", "assistant"
    std::string content;
};

struct LLMOptions {
    std::string model = "gpt-4o";
    double temperature = 0.2;
    int max_tokens = 2048;
};

class ILLMClient {
public:
    virtual ~ILLMClient() = default;
    virtual std::string chat(const std::vector<Message>& messages, const LLMOptions& options) = 0;
};

class LLMClientFactory {
public:
    static std::shared_ptr<ILLMClient> createClient(const std::string& provider, const std::string& base_url, const std::string& api_key);
};

} // namespace pilot::llm

#endif // PILOT_LLM_CLIENT_H
