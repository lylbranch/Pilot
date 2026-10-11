#include "llm/llm_client.h"
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <memory>
#include <array>
#include <sstream>

namespace pilot::llm {

class GenericLLMClient : public ILLMClient {
public:
    GenericLLMClient(std::string provider, std::string base_url, std::string api_key)
        : provider_(std::move(provider)), base_url_(std::move(base_url)), api_key_(std::move(api_key)) {}

    std::string chat(const std::vector<Message>& messages, const LLMOptions& options) override {
        std::cout << "[LLMClient] Calling API via Provider: " << provider_ << " (Model: " << options.model << ")...\n";

        nlohmann::json envelope;
        envelope["base_url"] = base_url_;
        envelope["api_key"] = api_key_;

        nlohmann::json payload;
        payload["model"] = "ag/gemini-3-flash";
        nlohmann::json j_messages = nlohmann::json::array();
        for (const auto& msg : messages) {
            j_messages.push_back({{"role", msg.role}, {"content", msg.content}});
        }
        payload["messages"] = j_messages;
        payload["max_tokens"] = options.max_tokens;
        payload["temperature"] = options.temperature;
        envelope["payload"] = payload;

        std::string envelope_str = envelope.dump();
        return callBridgeViaTempFile(envelope_str);
    }

private:
    std::string callBridgeViaTempFile(const std::string& json_data) {
        std::string tmp_path = "/tmp/pilot_llm_request.json";
        FILE* f = fopen(tmp_path.c_str(), "w");
        if (!f) return "[LLM Error] Failed to write temp request file.";
        fwrite(json_data.data(), 1, json_data.size(), f);
        fclose(f);

        std::string cmd = "python3 scripts/llm_bridge.py < " + tmp_path;
        std::array<char, 256> buffer;
        std::string result;

        FILE* pipe = popen(cmd.c_str(), "r");
        if (!pipe) {
            return "[LLM Error] Failed to execute LLM bridge.";
        }
        while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
            result += buffer.data();
        }
        pclose(pipe);

        if (!result.empty() && result.back() == '\n') {
            result.pop_back();
        }

        if (result.empty() || result.rfind("ERROR:", 0) == 0) {
            return "[LLM Error] " + result;
        }

        return result;
    }

    std::string provider_;
    std::string base_url_;
    std::string api_key_;
};

std::shared_ptr<ILLMClient> LLMClientFactory::createClient(const std::string& provider, const std::string& base_url, const std::string& api_key) {
    return std::make_shared<GenericLLMClient>(provider, base_url, api_key);
}

} // namespace pilot::llm
