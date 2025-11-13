#include "Utils.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib> // <-- The critical include for system()

using namespace std;

// Define movement directions (Up, Right, Down, Left)
// This order (0=Up, 1=Right, 2=Down, 3=Left) is
// required by WallFollower_Solver.
const array<pair<int,int>,4> directions {{
    {-1, 0},  // 0: Up
    {0, 1},   // 1: Right
    {1, 0},   // 2: Down
    {0, -1}   // 3: Left
}};

bool isInside(const vector<string>& g, int r, int c) {
    return r >= 0 && c >= 0 &&
           r < (int)g.size() &&
           c < (int)g[0].size();
}

void clearConsole() {
    // --- THIS IS THE CROSS-PLATFORM FIX ---
    #ifdef _WIN32
        system("cls");
    #else
        // Assumes UNIX-like system (Linux, macOS)
        system("clear");
    #endif
    // --- END FIX ---
}

void sleep_ms(int ms) {
    this_thread::sleep_for(chrono::milliseconds(ms));
}

// --- 2x3 LAYOUT PRINT FUNCTION ---
void printSideBySide(const vector<vector<string>>& grids,
                     const vector<string>& titles)
{
    size_t N = grids.size();
    if (N == 0) return;

    size_t R = grids[0].size();
    // Calculate width of one maze + padding
    size_t C_WIDTH = (grids[0].empty() ? 0 : grids[0][0].size()) + 3;
    size_t TITLE_WIDTH = C_WIDTH; 

    // --- Print Top Row (Solvers 0, 1, 2) ---
    if (N > 0) {
        // Print top 3 titles
        for (size_t i = 0; i < 3 && i < N; i++) {
            cout << titles[i];
            // Add padding
            if (titles[i].size() < TITLE_WIDTH) {
                cout << string(TITLE_WIDTH - titles[i].size(), ' ');
            }
        }
        cout << "\n\n";

        // Print top 3 grids, row by row
        for (size_t r = 0; r < R; r++) {
            for (size_t i = 0; i < 3 && i < N; i++) {
                if (r < grids[i].size()) {
                    cout << grids[i][r] << "   ";
                }
            }
            cout << "\n";
        }
    }

    cout << "\n\n"; // Spacer between rows

    // --- Print Bottom Row (Solvers 3, 4, 5) ---
    if (N > 3) {
        // Print bottom 3 titles
        for (size_t i = 3; i < 6 && i < N; i++) {
            cout << titles[i];
            if (titles[i].size() < TITLE_WIDTH) {
                cout << string(TITLE_WIDTH - titles[i].size(), ' ');
            }
        }
        cout << "\n\n";

        // Print bottom 3 grids, row by row
        for (size_t r = 0; r < R; r++) {
            for (size_t i = 3; i < 6 && i < N; i++) {
                if (r < grids[i].size()) {
                    cout << grids[i][r] << "   ";
                }
            }
            cout << "\n";
        }
    }
}