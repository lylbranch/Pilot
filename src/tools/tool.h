#ifndef PILOT_TOOLS_TOOL_H
#define PILOT_TOOLS_TOOL_H

#include <string>
#include <memory>
#include <functional>
#include <nlohmann/json.hpp>

namespace pilot::tools {

/**
 * @brief Abstract base interface for all Pilot agent tools.
 * Developers can implement new tools (e.g., FileWriteTool, ShellExecTool) by inheriting from this class.
 */
class ITool {
public:
    virtual ~ITool() = default;

    virtual std::string getName() const = 0;
    virtual std::string getDescription() const = 0;
    virtual nlohmann::json execute(const nlohmann::json& arguments) = 0;
};

/**
 * @brief Registry for managing and dispatching tools available to agents.
 */
class ToolRegistry {
public:
    static ToolRegistry& getInstance() {
        static ToolRegistry instance;
        return instance;
    }

    void registerTool(std::shared_ptr<ITool> tool) {
        tools_[tool->getName()] = tool;
    }

    std::shared_ptr<ITool> getTool(const std::string& name) const {
        auto it = tools_.find(name);
        if (it != tools_.end()) {
            return it->second;
        }
        return nullptr;
    }

    std::vector<std::string> getAvailableTools() const {
        std::vector<std::string> list;
        for (const auto& pair : tools_) {
            list.push_back(pair.first);
        }
        return list;
    }

private:
    ToolRegistry() = default;
    ~ToolRegistry() = default;
    std::unordered_map<std::string, std::shared_ptr<ITool>> tools_;
};

} // namespace pilot::tools

#endif // PILOT_TOOLS_TOOL_H
