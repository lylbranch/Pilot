#include "core/message_bus.h"

namespace pilot::core {

void MessageBus::subscribe(const std::string& topic, MessageCallback callback) {
    std::lock_guard<std::mutex> lock(mutex_);
    subscribers_[topic].push_back(callback);
}

void MessageBus::publish(const MessageBusMessage& message) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = subscribers_.find(message.topic);
    if (it != subscribers_.end()) {
        for (const auto& cb : it->second) {
            cb(message);
        }
    }
    // Also trigger wildcard subscribers if any
    auto wildcard_it = subscribers_.find("*");
    if (wildcard_it != subscribers_.end()) {
        for (const auto& cb : wildcard_it->second) {
            cb(message);
        }
    }
}

} // namespace pilot::core
