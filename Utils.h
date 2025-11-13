#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>
#include <utility>
#include <array>

extern const std::array<std::pair<int,int>,4> directions;

bool isInside(const std::vector<std::string>& g, int r, int c);
void clearConsole();
void sleep_ms(int ms);
void printSideBySide(const std::vector<std::vector<std::string>>& grids,
                     const std::vector<std::string>& titles);

#endif
