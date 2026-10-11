#ifndef PILOT_MCP_MCP_CLIENT_H
#define PILOT_MCP_MCP_CLIENT_H

#include <string>
#include <vector>
#include <memory>
#include <nlohmann/json.hpp>

namespace pilot::mcp {

struct MCPToolDefinition {
    std::string name;
    std::string description;
    nlohmann::json input_schema;
};

class MCPClient {
public:
    MCPClient() = default;
    ~MCPClient();

    bool connect(const std::string& server_command);
    void disconnect();

    std::vector<MCPToolDefinition> listTools();
    nlohmann::json callTool(const std::string& tool_name, const nlohmann::json& arguments);

    bool isConnected() const { return connected_; }

private:
    nlohmann::json sendRequest(const std::string& method, const nlohmann::json& params = nullptr);

    bool connected_ = false;
    std::string server_command_;
    int request_id_ = 0;
};

} // namespace pilot::mcp

#endif // PILOT_MCP_MCP_CLIENT_H
