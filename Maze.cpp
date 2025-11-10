#include "Maze.h"
#include <iostream>
#include <queue>
#include <random>
#include <ctime>

Maze::Maze(int rows, int cols) : rows(rows), cols(cols) {
    grid.assign(rows, std::string(cols, '#'));
    generateSolvableMaze();
}

void Maze::generateSolvableMaze() {
    // Basic RNG
    std::mt19937 rng((unsigned)time(nullptr));
    std::uniform_int_distribution<int> dist(0, 3);

    // Fill with walls
    for (auto& row : grid)
        std::fill(row.begin(), row.end(), '#');

    start = {1, 1};
    goal = {rows - 2, cols - 2};

    // Step 1: Carve a path from start to goal
    int r = start.first, c = start.second;
    grid[r][c] = 'S';
    while (r != goal.first || c != goal.second) {
        grid[r][c] = ' ';
        int dir = dist(rng);
        int nr = r, nc = c;

        if (dir == 0 && r + 1 < rows - 1) nr++;
        else if (dir == 1 && c + 1 < cols - 1) nc++;
        else if (dir == 2 && r - 1 > 0) nr--;
        else if (dir == 3 && c - 1 > 0) nc--;
        else continue;

        r = nr; c = nc;
    }
    grid[goal.first][goal.second] = 'E';

    // Step 2: Random openings elsewhere
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            if (grid[i][j] == '#' && rand() % 3 == 0)
                grid[i][j] = ' ';
}

void Maze::print() const {
    for (const auto& row : grid)
        std::cout << row << "\n";
}

char Maze::at(int r, int c) const {
    if (r < 0 || c < 0 || r >= rows || c >= cols) return '#';
    return grid[r][c];
}

void Maze::setCell(int r, int c, char val) {
    if (r >= 0 && c >= 0 && r < rows && c < cols)
        grid[r][c] = val;
}

std::pair<int,int> Maze::getStart() const { return start; }
std::pair<int,int> Maze::getGoal() const { return goal; }
int Maze::getRows() const { return rows; }
int Maze::getCols() const { return cols; }
