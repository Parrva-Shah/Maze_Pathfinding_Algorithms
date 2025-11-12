#include "Maze.h"
#include <random>
#include <ctime>
#include <algorithm> 

using namespace std;

Maze::Maze(int rows, int cols) : rows(rows), cols(cols) {
    grid.assign(rows, string(cols, '#'));
    generateSolvableMaze();
}

void Maze::generateSolvableMaze(int openingFactor) {
    // Basic RNG
    mt19937 rng((unsigned)time(nullptr));
    uniform_int_distribution<int> dist(0, 3);

    // Fill with walls
    for (auto& row : grid)
        fill(row.begin(), row.end(), '#');

    start = {1, 1};
    goal = {rows - 2, cols - 2};

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

    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            if (grid[i][j] == '#' && rand() % openingFactor == 0)
                grid[i][j] = ' ';
}

char Maze::at(int r, int c) const {
    if (r < 0 || c < 0 || r >= rows || c >= cols) return '#';
    return grid[r][c];
}

void Maze::setCell(int r, int c, char val) {
    if (r >= 0 && c >= 0 && r < rows && c < cols)
        grid[r][c] = val;
}

pair<int,int> Maze::getStart() const { return start; }
pair<int,int> Maze::getGoal() const { return goal; }
int Maze::getRows() const { return rows; }
int Maze::getCols() const { return cols; }
