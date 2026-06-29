#!/bin/bash
set -e

echo "=== Maze Pathfinding Visualizer ==="

# Check and install g++
if ! command -v g++ &>/dev/null; then
    echo "[*] g++ not found. Installing build-essential..."
    sudo apt-get update -qq
    sudo apt-get install -y build-essential
fi

# Check and install SFML
if ! dpkg -s libsfml-dev &>/dev/null 2>&1; then
    echo "[*] SFML not found. Installing libsfml-dev..."
    sudo apt-get update -qq
    sudo apt-get install -y libsfml-dev
fi

echo "[*] Building..."
g++ -O2 -std=c++17 *.cpp -o maze_visualizer -lsfml-graphics -lsfml-window -lsfml-system

echo "[*] Launching..."
./maze_visualizer
