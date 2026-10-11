#ifndef PILOT_CORE_MESSAGE_BUS_H
#define PILOT_CORE_MESSAGE_BUS_H

#include <string>
#include <functional>
#include <unordered_map>
#include <vector>
#include <mutex>
#include <nlohmann/json.hpp>

namespace pilot::core {

struct MessageBusMessage {
    std::string sender;
    std::string receiver; // empty for broadcast
    std::string topic;
    nlohmann::json payload;
};

using MessageCallback = std::function<void(const MessageBusMessage&)>;

class MessageBus {
public:
    MessageBus() = default;
    ~MessageBus() = default;

    void subscribe(const std::string& topic, MessageCallback callback);
    void publish(const MessageBusMessage& message);

private:
    mutable std::mutex mutex_;
    std::unordered_map<std::string, std::vector<MessageCallback>> subscribers_;
};

} // namespace pilot::core

#endif // PILOT_CORE_MESSAGE_BUS_H
