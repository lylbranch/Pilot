cmake_minimum_required(VERSION 3.14)
project(agent-code-Pilot CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler warnings
if(MSVC)
    add_compile_options(/W4 /wx)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/src)

# Fetch nlohmann_json for modern JSON handling
include(FetchContent)
FetchContent_Declare(
    nlohmann_json
    URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz
)
FetchContent_MakeAvailable(nlohmann_json)

# Source files
set(CORE_SOURCES
    src/core/context.cpp
    src/core/task.cpp
    src/core/agent.cpp
    src/core/message_bus.cpp
    src/core/orchestrator.cpp
)

set(LLM_SOURCES
    src/llm/llm_client.cpp
)

set(TOOLS_SOURCES
    src/tools/builtin_tools.cpp
)

set(CLI_SOURCES
    src/cli/cli.cpp
)

set(MAIN_SOURCES
    src/main.cpp
)

add_executable(pilot
    ${CORE_SOURCES}
    ${LLM_SOURCES}
    ${TOOLS_SOURCES}
    ${CLI_SOURCES}
    ${MAIN_SOURCES}
)

target_link_libraries(pilot PRIVATE nlohmann_json::nlohmann_json pthread)

# Enable testing if needed later
enable_testing()
