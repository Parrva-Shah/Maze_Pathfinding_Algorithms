#include "Maze.h"
#include <random>    
#include <algorithm> 
#include <ctime>     

using namespace std;

Maze::Maze(int rows, int cols, unsigned seed_)
    : rows(rows), cols(cols)
{
    // Ensure minimum usable dimensions
    rows = max(rows, 5);
    cols = max(cols, 5);

    grid.assign(rows, string(cols, '#'));

    if (seed_ != 0) {
        seed = seed_;
    } else {
        std::random_device rd; // Get a true random seed
        seed = rd();
    }

    generateSolvableMaze(25);  
}


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

    // 5. Add random walls inside
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