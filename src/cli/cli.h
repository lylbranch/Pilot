#ifndef PILOT_CLI_CLI_H
#define PILOT_CLI_CLI_H

#include <string>
#include <memory>
#include "core/orchestrator.h"

namespace pilot::cli {

class CLI {
public:
    explicit CLI(std::shared_ptr<core::Orchestrator> orchestrator);
    ~CLI() = default;

    void run();

private:
    void printBanner() const;
    void printHelp() const;
    void handleCommand(const std::string& input);

    std::shared_ptr<core::Orchestrator> orchestrator_;
    bool running_ = true;
};

} // namespace pilot::cli

#endif // PILOT_CLI_CLI_H
