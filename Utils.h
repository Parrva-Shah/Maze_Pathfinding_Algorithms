#pragma once
#include <vector>
#include <string>
#include <array>

extern const std::array<std::pair<int, int>, 4> directions;

bool isInside(const std::vector<std::string>& grid, int r, int c);
void clearConsole();
void sleep_ms(int ms);
void printSideBySide(
    const std::vector<std::vector<std::string>>& mazes,
    const std::vector<std::string>& labels);
