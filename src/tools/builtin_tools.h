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
 * @brief Tool to execute shell commands.
 */
class ShellExecTool : public ITool {
public:
    std::string getName() const override { return "shell_exec"; }
    std::string getDescription() const override { return "Execute a shell command. Arguments: {\"command\": \"...\"}"; }
    nlohmann::json execute(const nlohmann::json& arguments) override;
};

/**
 * @brief Network module tool for HTTP requests (GET/POST).
 */
class NetworkTool : public ITool {
public:
    std::string getName() const override { return "network_request"; }
    std::string getDescription() const override { return "Perform HTTP requests. Arguments: {\"url\": \"...\", \"method\": \"GET/POST\", \"data\": \"...\"}"; }
    nlohmann::json execute(const nlohmann::json& arguments) override;
};

/**
 * @brief Compiler module tool (C/C++ g++/gcc/make).
 */
class CompilerTool : public ITool {
public:
    std::string getName() const override { return "compiler"; }
    std::string getDescription() const override { return "Compile C/C++ source code. Arguments: {\"source\": \"main.cpp\", \"output\": \"app\", \"flags\": \"-std=c++17 -O3\"}"; }
    nlohmann::json execute(const nlohmann::json& arguments) override;
};

/**
 * @brief Interpreter module tool (Python, Node, Bash).
 */
class InterpreterTool : public ITool {
public:
    std::string getName() const override { return "interpreter"; }
    std::string getDescription() const override { return "Run scripts using language interpreters. Arguments: {\"language\": \"python3/node/bash\", \"script_path\": \"script.py\" or \"code\": \"...\"}"; }
    nlohmann::json execute(const nlohmann::json& arguments) override;
};

} // namespace pilot::tools

#endif // PILOT_TOOLS_BUILTIN_TOOLS_H
