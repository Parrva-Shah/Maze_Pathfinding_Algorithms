#include "AStar_Solver.h"
#include <limits>
#include <iostream>

using namespace std;

AStar_Solver::AStar_Solver(const Maze& maze)
    : Solver(maze, 'A')
{
    int rows = maze.getRows();
    int cols = maze.getCols();

    // Initialize scores and visited grid
    gScore.assign(rows, vector<vector<int>::value_type>(cols, numeric_limits<int>::max()));
    visited.assign(rows, vector<bool>(cols, false));

    gScore[start.first][start.second] = 0;

    // Push starting node with its f-score
    int fStart = heuristic(start.first, start.second);
    openSet.push({fStart, start});
}

int AStar_Solver::heuristic(int r, int c) const {
    // Manhattan distance
    return abs(goal.first - r) + abs(goal.second - c);
}

void AStar_Solver::step() {

    // If path found earlier, now backtracking the parent pointers
    if (currentState == State::TRACING_PATH) {
        if (tracePos == start) {
            currentState = State::DONE;
            return;
        }
        if (grid[tracePos.first][tracePos.second] != 'E') {
            grid[tracePos.first][tracePos.second] = 'X'; // Mark solution path
        }
        tracePos = parent[tracePos.first][tracePos.second];
        return;
    }

    if (currentState != State::SEARCHING) return;

    // No more nodes to explore 
    if (openSet.empty()) {
        currentState = State::DONE;
        found = false;
        return;
    }

    while (!openSet.empty()) {
        auto current = openSet.top();
        openSet.pop();

        auto [r, c] = current.pos;

        if (visited[r][c]) continue;
        visited[r][c] = true;

        // Mark this cell as explored
        if (grid[r][c] == ' ')
            grid[r][c] = symbol;

        // Goal reached → switch to tracing mode
        if (r == goal.first && c == goal.second) {
            found = true;
            currentState = State::TRACING_PATH;
            tracePos = goal;
            return;
        }

        // Explore all 4 neighbours
        for (auto [dr, dc] : directions) {
            int nr = r + dr, nc = c + dc;
            if (!isInside(grid, nr, nc)) continue;
            if (grid[nr][nc] == '#') continue;

            int tentativeG = gScore[r][c] + 1;

            // Found a better path to neighbour
            if (tentativeG < gScore[nr][nc]) {
                gScore[nr][nc] = tentativeG;
                parent[nr][nc] = {r, c};
                int f = tentativeG + heuristic(nr, nc);
                openSet.push({f, {nr, nc}});
            }
        }

        // Process only one node per step() call (good for visualization)
        return;
    }

    currentState = State::DONE;
    found = false;
}
