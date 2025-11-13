#include "Maze.h"
#include <random>    // <-- Required
#include <algorithm> // <-- For std.max, std.fill
#include <ctime>     // Not really used, but fine

using namespace std;

Maze::Maze(int rows, int cols, unsigned seed_)
    : rows(rows), cols(cols)
{
    // Ensure minimum usable dimensions
    rows = max(rows, 5);
    cols = max(cols, 5);

    grid.assign(rows, string(cols, '#'));

    // Use a high-quality, non-deterministic seed if none is provided
    if (seed_ != 0) {
        seed = seed_;
    } else {
        std::random_device rd; // Get a true random seed
        seed = rd();
    }

    // Generate the maze using the new, simpler logic
    generateSolvableMaze(25);  // Pass wall density (e.g., 25%)
}

// NOTE: The function name is the same, but the logic is totally different.
// The 'openingFactor' is now 'wallDensity'
void Maze::generateSolvableMaze(int wallDensity) {
    
    // 1. Create the random number generator
    mt19937 rng(seed);
    uniform_int_distribution<int> rowDist(1, rows - 2);
    uniform_int_distribution<int> colDist(1, cols - 2);
    uniform_int_distribution<int> percent(0, 99);

    // 2. Pick random start (S) and end (E) points *first*
    start = {rowDist(rng), colDist(rng)};
    do {
        goal = {rowDist(rng), colDist(rng)};
    } while (start == goal); // Ensure they aren't the same spot

    // 3. Fill the entire grid with walls
    for (auto& row : grid)
        fill(row.begin(), row.end(), '#');

    // 4. Carve out the *entire* inside area
    for (int r = 1; r < rows - 1; ++r) {
        for (int c = 1; c < cols - 1; ++c) {
            grid[r][c] = ' ';
        }
    }

    // 5. "Spray" random walls inside
    for (int r = 1; r < rows - 1; ++r) {
        for (int c = 1; c < cols - 1; ++c) {
            // Don't place a wall on S or E
            if ((r == start.first && c == start.second) ||
                (r == goal.first  && c == goal.second))
                continue;

            if (percent(rng) < wallDensity) {
                grid[r][c] = '#';
            }
        }
    }

    // 6. Finally, place Start and End
    grid[start.first][start.second] = 'S';
    grid[goal.first][goal.second]   = 'E';
}