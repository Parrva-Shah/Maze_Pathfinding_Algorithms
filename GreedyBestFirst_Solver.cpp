#include "GreedyBestFirst_Solver.h"
#include <limits>
#include <iostream>

GreedyBestFirst_Solver::GreedyBestFirst_Solver(const Maze& maze)
    : Solver(maze, 'G') // Use 'G' as the symbol
{
    int rows = maze.getRows();
    int cols = maze.getCols();

    visited.assign(rows, std::vector<bool>(cols, false));
    // parent is in base class
    
    int hStart = heuristic(start.first, start.second);
    openSet.push({hStart, start});// openSet is a std::priority_queue<Node> (min-heap by heuristic).
}

int GreedyBestFirst_Solver::heuristic(int r, int c) const {
    // Manhattan distance heuristic
    return std::abs(goal.first - r) + std::abs(goal.second - c);
}
// heuristic(r,c) computes the Manhattan distance from (r,c) to the goal.
void GreedyBestFirst_Solver::step() {
    // Path Tracing
    if (currentState == State::TRACING_PATH) {
        if (tracePos == start) {
            currentState = State::DONE;
            return;
        }
        if (grid[tracePos.first][tracePos.second] != 'E') {
            grid[tracePos.first][tracePos.second] = 'X'; // 'X' for final path
        }
        tracePos = parent[tracePos.first][tracePos.second];
        return;
    }

    if (currentState != State::SEARCHING) return;
    
    //  Searching
    if (openSet.empty()) {
        currentState = State::DONE;
        found = false;
        return;
    }

    while (!openSet.empty()) {
        auto current = openSet.top();
        openSet.pop();

        auto [r, c] = current.pos;

        if (visited[r][c]) continue; // Already processed
        visited[r][c] = true;

        // Color when processing
        if (grid[r][c] == ' ')
            grid[r][c] = symbol;

        // Found the goal
        if (r == goal.first && c == goal.second) {
            found = true;
            currentState = State::TRACING_PATH;
            tracePos = goal;
            return; // Exit step
        }

        // Explore neighbors
        for (auto [dr, dc] : directions) {
            int nr = r + dr, nc = c + dc;
            if (!isInside(grid, nr, nc)) continue;
            char cell = grid[nr][nc];
            if (cell == '#') continue;

            if (!visited[nr][nc]) { // Only check if visited
                parent[nr][nc] = {r, c};
                int h = heuristic(nr, nc);
                openSet.push({h, {nr, nc}});
                // Don't color here, color when popped
            }
        }
        // Return after processing one valid node
        return;
    }
    
    // If loop finishes, openSet was emptied
    currentState = State::DONE;
    found = false;
}