#include "DFS_Solver.h"
#include <iostream>

DFS_Solver::DFS_Solver(const Maze& maze)
    : Solver(maze, 'D')
{
    int R = maze.getRows();
    int C = maze.getCols();
    visited.assign(R, std::vector<bool>(C, false));
    // 'parent' is initialized in base Solver
    
    stk.push(start);
}

void DFS_Solver::step() {
    
    if (currentState == State::TRACING_PATH) {
        if (tracePos == start) {
            currentState = State::DONE;
            return;
        }
        // Use 'X' for the final path
        if (grid[tracePos.first][tracePos.second] != 'E') {
            grid[tracePos.first][tracePos.second] = 'X';
        }
        tracePos = parent[tracePos.first][tracePos.second];
        return;
    }

    if (currentState != State::SEARCHING) return;

    // --- THIS LOOP IS THE MAIN FIX ---
    // Loop until we find a new node to process or the stack is empty
    while (!stk.empty()) {
        auto [r, c] = stk.top();
        stk.pop();

        // If already visited, pop and continue the loop (do not return!)
        if (visited[r][c]) {
            continue; 
        }

        // --- Found a new node to process ---
        visited[r][c] = true;
        if (grid[r][c] == ' ') grid[r][c] = symbol;

        // If we've reached the goal cell, switch to TRACING
        if (std::make_pair(r,c) == goal) {
            found = true;
            currentState = State::TRACING_PATH;
            tracePos = goal; // Start tracing from the goal
            return; // Exit step
        }

        // Push unvisited neighbors
        for (int i = 3; i >= 0; --i) {
            int nr = r + directions[i].first;
            int nc = c + directions[i].second;
            if (!isInside(grid, nr, nc)) continue;
            if (grid[nr][nc] == '#') continue;
            if (visited[nr][nc]) continue;
            
            parent[nr][nc] = {r, c};
            stk.push({nr, nc});
        }

        // Return *after* processing one valid node
        return;
    }
    
    // If we are here, the stack became empty
    if (currentState == State::SEARCHING) {
        currentState = State::DONE;
        found = false;
    }
}