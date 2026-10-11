#include "cli/cli.h"
#include <iostream>
#include <string>
#include <algorithm>

namespace pilot::cli {

CLI::CLI(std::shared_ptr<core::Orchestrator> orchestrator)
    : orchestrator_(std::move(orchestrator)) {}

void CLI::printBanner() const {
    std::cout << "==================================================\n";
    std::cout << "         Pilot Multi-Agent System CLI             \n";
    std::cout << "==================================================\n";
    std::cout << "Type your development task or use commands:\n";
    std::cout << "  /help    - Show available commands\n";
    std::cout << "  /status  - Show system and workspace status\n";
    std::cout << "  /clear   - Clear current context/memory\n";
    std::cout << "  /exit    - Exit Pilot CLI\n";
    std::cout << "==================================================\n\n";
}

void CLI::printHelp() const {
    std::cout << "\n--- Pilot CLI Help ---\n";
    std::cout << "  [Any Prompt] : Executes a multi-agent workflow (Planner -> Coder)\n";
    std::cout << "  /help        : Show this help message\n";
    std::cout << "  /status      : Show workspace path and context summary\n";
    std::cout << "  /clear       : Reset context memory\n";
    std::cout << "  /exit        : Quit Pilot\n";
    std::cout << "----------------------\n\n";
}

void CLI::run() {
    printBanner();

    std::string line;
    while (running_) {
        std::cout << "\033[1;36mPilot > \033[0m";
        if (!std::getline(std::cin, line)) {
            break;
        }

        // Trim leading and trailing spaces
        line.erase(line.begin(), std::find_if(line.begin(), line.end(), [](unsigned char ch) {
            return !std::isspace(ch);
        }));
        line.erase(std::find_if(line.rbegin(), line.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
        }).base(), line.end());

        if (line.empty()) {
            continue;
        }

        if (line[0] == '/') {
            handleCommand(line);
        } else {
            if (line == "exit" || line == "quit") {
                running_ = false;
                break;
            }
            std::cout << "\n[Pilot] Running multi-agent workflow...\n";
            orchestrator_->runWorkflow(line);
            std::cout << "\n";
        }
    }

    std::cout << "\nGoodbye from Pilot!\n";
}

void CLI::handleCommand(const std::string& input) {
    if (input == "/exit" || input == "/quit") {
        running_ = false;
    } else if (input == "/help") {
        printHelp();
    } else if (input == "/status") {
        std::cout << "\n--- Pilot Status ---\n";
        std::cout << "Workspace Path: " << orchestrator_->getContext().getWorkspacePath() << "\n";
        std::cout << "--------------------\n\n";
    } else if (input == "/clear") {
        orchestrator_->getContext().clear();
        std::cout << "[Pilot] Context memory cleared.\n\n";
    } else {
        std::cout << "[Pilot] Unknown command: " << input << ". Type /help for assistance.\n\n";
    }
}

} // namespace pilot::cli
