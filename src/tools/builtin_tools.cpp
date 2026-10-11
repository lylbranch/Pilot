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
    cmd += " 2>&1";

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

nlohmann::json NetworkTool::execute(const nlohmann::json& arguments) {
    if (!arguments.contains("url")) {
        return {{"success", false}, {"error", "Missing 'url' argument"}};
    }
    std::string url = arguments["url"].get<std::string>();
    std::string method = arguments.value("method", "GET");
    std::string data = arguments.value("data", "");

    // Use Python curl/urllib via popen for robust HTTP handling
    std::string py_script = 
        "import urllib.request\n"
        "import urllib.parse\n"
        "url = '" + url + "'\n"
        "method = '" + method + "'\n"
        "data = " + (data.empty() ? "None" : "'" + data + "'.encode('utf-8')") + "\n"
        "try:\n"
        "    req = urllib.request.Request(url, data=data, method=method)\n"
        "    with urllib.request.urlopen(req, timeout=30) as resp:\n"
        "        print(resp.read().decode('utf-8'))\n"
        "except Exception as e:\n"
        "    print('ERROR:', str(e))\n";

    std::string tmp_file = "/tmp/pilot_net_req.py";
    std::ofstream f(tmp_file);
    f << py_script;
    f.close();

    std::string cmd = "python3 " + tmp_file + " 2>&1";
    std::array<char, 128> buffer;
    std::string result;
    FILE* pipe = popen(cmd.c_str(), "r");
    if (pipe) {
        while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
            result += buffer.data();
        }
        pclose(pipe);
    }

    if (result.rfind("ERROR:", 0) == 0) {
        return {{"success", false}, {"error", result}};
    }
    return {{"success", true}, {"response", result}};
}

nlohmann::json CompilerTool::execute(const nlohmann::json& arguments) {
    if (!arguments.contains("source")) {
        return {{"success", false}, {"error", "Missing 'source' source file argument"}};
    }
    std::string source = arguments["source"].get<std::string>();
    std::string output = arguments.value("output", "a.out");
    std::string flags = arguments.value("flags", "-std=c++17 -O3 -Wall");

    std::string cmd = "g++ " + flags + " " + source + " -o " + output + " 2>&1";

    std::array<char, 128> buffer;
    std::string result;
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) {
        return {{"success", false}, {"error", "Failed to invoke g++ compiler."}};
    }
    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
        result += buffer.data();
    }
    int exit_code = pclose(pipe);

    bool success = (exit_code == 0);
    return {
        {"success", success},
        {"output", result},
        {"exit_code", exit_code}
    };
}

nlohmann::json InterpreterTool::execute(const nlohmann::json& arguments) {
    std::string lang = arguments.value("language", "python3");
    std::string code = arguments.value("code", "");
    std::string script_path = arguments.value("script_path", "");

    std::string cmd;
    if (!script_path.empty()) {
        cmd = lang + " " + script_path + " 2>&1";
    } else if (!code.empty()) {
        if (lang == "python3") {
            cmd = "python3 -c " + std::string("\"") + code + std::string("\"") + " 2>&1";
        } else if (lang == "node") {
            cmd = "node -e " + std::string("\"") + code + std::string("\"") + " 2>&1";
        } else {
            cmd = "bash -c " + std::string("\"") + code + std::string("\"") + " 2>&1";
        }
    } else {
        return {{"success", false}, {"error", "Neither 'code' nor 'script_path' provided."}};
    }

    std::array<char, 128> buffer;
    std::string result;
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) {
        return {{"success", false}, {"error", "Failed to execute interpreter."}};
    }
    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
        result += buffer.data();
    }
    int exit_code = pclose(pipe);

    return {
        {"success", (exit_code == 0)},
        {"output", result},
        {"exit_code", exit_code}
    };
}

} // namespace pilot::tools
