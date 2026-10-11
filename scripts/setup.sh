#!/usr/bin/env bash
set -e

echo "========================================"
echo "    Pilot Setup & Build Script          "
echo "========================================"

# Create build directory
mkdir -p build
cd build

echo "[1/2] Configuring CMake..."
cmake -DCMAKE_BUILD_TYPE=Release ..

echo "[2/2] Building Pilot..."
make -j$(nproc)

echo "========================================"
echo " Setup complete! Binary located at build/pilot"
echo "========================================"
