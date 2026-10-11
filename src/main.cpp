#include <iostream>
#include <memory>
#include <cstdlib>
#include "core/orchestrator.h"
#include "core/agent.h"
#include "llm/llm_client.h"
#include "cli/cli.h"

int main(int argc, char* argv[]) {
    // Initialize LLM client (using proxy API key & base URL)
    std::string api_key = "sk-f3a42e71a134e259-lqeopw-0632a5f5";
    if (const char* env_key = std::getenv("PROXY_API_KEY")) {
        api_key = env_key;
    }
    std::string base_url = "http://189.24.105.44:20128/v1";

    auto llm_client = pilot::llm::LLMClientFactory::createClient("proxy", base_url, api_key);

    // Create Orchestrator
    auto orchestrator = std::make_shared<pilot::core::Orchestrator>();

    // Register Agents
    auto planner = std::make_shared<pilot::core::Agent>(
        "planner",
        "You are an expert software architect responsible for breaking down user requirements into concrete development tasks.",
        llm_client
    );

    auto coder = std::make_shared<pilot::core::Agent>(
        "coder",
        "You are an expert C++ developer responsible for writing clean, high-performance, and robust code.",
        llm_client
    );

    orchestrator->registerAgent(planner);
    orchestrator->registerAgent(coder);

    // If an argument is provided on command line, run it once as a quick task; otherwise start interactive CLI
    if (argc > 1) {
        std::string prompt = argv[1];
        std::cout << "[Pilot] Running single task from CLI argument: \"" << prompt << "\"\n";
        
        pilot::core::Task task1;
        task1.id = "t1";
        task1.title = "Architecture Planning";
        task1.description = "Design modular architecture and define component interfaces for: " + prompt;
        task1.assigned_agent = "planner";
        orchestrator->addTask(task1);

        pilot::core::Task task2;
        task2.id = "t2";
        task2.title = "Code Implementation";
        task2.description = "Implement the core C++ modules based on the architecture plan.";
        task2.assigned_agent = "coder";
        task2.dependencies = {"t1"};
        orchestrator->addTask(task2);

        orchestrator->runWorkflow(prompt);
        return 0;
    }

    // Start Interactive REPL CLI
    pilot::cli::CLI cli(orchestrator);
    cli.run();

    return 0;
}
