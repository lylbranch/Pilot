#include "tools/builtin_tools.h"
#include <fstream>
#include <sstream>
#include <cstdio>
#include <memory>
#include <array>

namespace pilot::tools {

nlohmann::json FileReadTool::execute(const nlohmann::json& arguments) {
    if (!arguments.contains("path")) {
        return {{"success", false}, {"error", "Missing 'path' argument"}};
    }
    std::string path = arguments["path"].get<std::string>();
    std::ifstream file(path);
    if (!file.is_open()) {
        return {{"success", false}, {"error", "Could not open file: " + path}};
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return {{"success", true}, {"content", buffer.str()}};
}

nlohmann::json FileWriteTool::execute(const nlohmann::json& arguments) {
    if (!arguments.contains("path") || !arguments.contains("content")) {
        return {{"success", false}, {"error", "Missing 'path' or 'content' argument"}};
    }
    std::string path = arguments["path"].get<std::string>();
    std::string content = arguments["content"].get<std::string>();

    std::ofstream file(path);
    if (!file.is_open()) {
        return {{"success", false}, {"error", "Could not write file: " + path}};
    }
    file << content;
    file.close();
    return {{"success", true}, {"message", "Successfully wrote to " + path}};
}

nlohmann::json ShellExecTool::execute(const nlohmann::json& arguments) {
    if (!arguments.contains("command")) {
        return {{"success", false}, {"error", "Missing 'command' argument"}};
    }
    std::string cmd = arguments["command"].get<std::string>();
    cmd += " 2>&1"; // redirect stderr to stdout

    std::array<char, 128> buffer;
    std::string result;
    
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) {
        return {{"success", false}, {"error", "Failed to execute shell command."}};
    }
    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
        result += buffer.data();
    }
    pclose(pipe);

    return {{"success", true}, {"output", result}};
}

} // namespace pilot::tools
