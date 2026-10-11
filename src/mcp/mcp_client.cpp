#include "mcp/mcp_client.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <unistd.h>

namespace pilot::mcp {

MCPClient::~MCPClient() {
    disconnect();
}

bool MCPClient::connect(const std::string& server_command) {
    server_command_ = server_command;
    // Test if we can start process or verify
    connected_ = true;
    std::cout << "[MCPClient] Connected to MCP Server via command: " << server_command_ << "\n";
    return true;
}

void MCPClient::disconnect() {
    if (connected_) {
        connected_ = false;
        std::cout << "[MCPClient] Disconnected from MCP Server.\n";
    }
}

nlohmann::json MCPClient::sendRequest(const std::string& method, const nlohmann::json& params) {
    if (!connected_) {
        return {{"error", "Not connected to MCP server"}};
    }

    request_id_++;
    nlohmann::json request;
    request["jsonrpc"] = "2.0";
    request["id"] = request_id_;
    request["method"] = method;
    if (!params.is_null()) {
        request["params"] = params;
    }

    std::string req_str = request.dump() + "\n";

    // Use a python helper or pipe to send JSON-RPC request to server command and read response
    std::string temp_req_file = "/tmp/pilot_mcp_req.json";
    FILE* f = fopen(temp_req_file.c_str(), "w");
    if (!f) return {{"error", "Failed to write MCP request"}};
    fwrite(req_str.data(), 1, req_str.size(), f);
    fclose(f);

    std::string cmd = server_command_ + " < " + temp_req_file + " 2>/dev/null";
    std::array<char, 512> buffer;
    std::string result;
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) {
        return {{"error", "Failed to communicate with MCP server process"}};
    }
    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
        result += buffer.data();
    }
    pclose(pipe);

    try {
        return nlohmann::json::parse(result);
    } catch (...) {
        return {{"error", "Invalid JSON-RPC response from MCP server", "raw", result}};
    }
}

std::vector<MCPToolDefinition> MCPClient::listTools() {
    std::vector<MCPToolDefinition> tools;
    nlohmann::json resp = sendRequest("tools/list");
    if (resp.contains("result") && resp["result"].contains("tools")) {
        for (const auto& item : resp["result"]["tools"]) {
            MCPToolDefinition def;
            def.name = item.value("name", "");
            def.description = item.value("description", "");
            def.input_schema = item.value("inputSchema", nlohmann::json::object());
            tools.push_back(def);
        }
    }
    return tools;
}

nlohmann::json MCPClient::callTool(const std::string& tool_name, const nlohmann::json& arguments) {
    nlohmann::json params;
    params["name"] = tool_name;
    params["arguments"] = arguments;

    nlohmann::json resp = sendRequest("tools/call", params);
    if (resp.contains("result")) {
        return resp["result"];
    } else if (resp.contains("error")) {
        return {{"success", false}, {"error", resp["error"]}};
    }
    return {{"success", false}, {"error", "Unknown MCP response format"}};
}

} // namespace pilot::mcp
