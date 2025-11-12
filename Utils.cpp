#include "Utils.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <string>
#include <array>

using namespace std;

// Define movement directions (up, right, down, left)
const array<pair<int, int>, 4> directions = {{
    {-1, 0}, {0, 1}, {1, 0}, {0, -1}
}};

bool isInside(const vector<string>& grid, int r, int c) {
    return r >= 0 && r < (int)grid.size() && c >= 0 && c < (int)grid[0].size();
}

void clearConsole() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void sleep_ms(int ms) {
    std::this_thread::sleep_for(chrono::milliseconds(ms));
}

void printSideBySide(
    const vector<vector<string>>& mazes,
    const vector<string>& labels)
{
    if (mazes.empty()) return;

    int rows = mazes[0].size();
    for (int r = 0; r < rows; ++r) {
        for (size_t m = 0; m < mazes.size(); ++m) {
            cout << mazes[m][r] << "   ";
        }
        cout << "\n";
    }

    for (auto& lbl : labels) {
        cout << lbl << "   ";
    }
    cout << "\n";
}