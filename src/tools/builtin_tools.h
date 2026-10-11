#ifndef PILOT_TOOLS_BUILTIN_TOOLS_H
#define PILOT_TOOLS_BUILTIN_TOOLS_H

#include "tools/tool.h"

namespace pilot::tools {

/**
 * @brief Tool to read a file from the workspace.
 */
class FileReadTool : public ITool {
public:
    std::string getName() const override { return "file_read"; }
    std::string getDescription() const override { return "Read contents of a file. Arguments: {\"path\": \"file_path\"}"; }
    nlohmann::json execute(const nlohmann::json& arguments) override;
};

/**
 * @brief Tool to write or overwrite a file in the workspace.
 */
class FileWriteTool : public ITool {
public:
    std::string getName() const override { return "file_write"; }
    std::string getDescription() const override { return "Write content to a file. Arguments: {\"path\": \"file_path\", \"content\": \"...\"}"; }
    nlohmann::json execute(const nlohmann::json& arguments) override;
};

/**
 * @brief Tool to execute shell commands (e.g., compilation, tests).
 */
class ShellExecTool : public ITool {
public:
    std::string getName() const override { return "shell_exec"; }
    std::string getDescription() const override { return "Execute a shell command. Arguments: {\"command\": \"...\"}"; }
    nlohmann::json execute(const nlohmann::json& arguments) override;
};

} // namespace pilot::tools

#endif // PILOT_TOOLS_BUILTIN_TOOLS_H
