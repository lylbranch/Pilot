#!/usr/bin/env bash
set -e

if [ ! -f "build/pilot" ]; then
    echo "Binary not found. Running setup.sh first..."
    ./scripts/setup.sh
fi

PROMPT="${1:-Develop a high-performance HTTP server in C++17.}"

echo "Running Pilot with prompt: \"$PROMPT\""
./build/pilot "$PROMPT"
