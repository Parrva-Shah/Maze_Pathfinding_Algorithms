#include "BFS_Solver.h"
#include <iostream>

BFS_Solver::BFS_Solver(const Maze& maze)
    : Solver(maze, 'B')
{
    int rows = maze.getRows();
    int cols = maze.getCols();
    visited.assign(rows, std::vector<bool>(cols, false));//2D vector<bool> functioning as a boolean matrix.
    // parent is in base
    
    q.push(start);//BFS requires a queue for level-by-level exploration
    visited[start.first][start.second] = true;// start and goal are stored as pair<int,int> representing grid coordinates.
}

void BFS_Solver::step() {
    
    if (currentState == State::TRACING_PATH) {
        if (tracePos == start) {
            currentState = State::DONE;// currentState is an enum that tracks the solver's phase
            return;
        }
        if (grid[tracePos.first][tracePos.second] != 'E') {
            grid[tracePos.first][tracePos.second] = 'X';
        }
        tracePos = parent[tracePos.first][tracePos.second];
        return;
    }

    if (currentState != State::SEARCHING) return;

    // No more nodes to explore means path not found
    if (q.empty()) {
        currentState = State::DONE;
        found = false;
        return;
    }
    // Process next cell in the BFS queue
    auto [r, c] = q.front();
    q.pop();

    // --- VISUALIZATION FIX ---
    // Color the cell when we *process* it, not when we add it
    if (grid[r][c] == ' ') {
        grid[r][c] = symbol;
    }
    // If we popped the goal, switch to tracing
    // (More efficient to check when adding, but this is fine)
    if (r == goal.first && c == goal.second) {
         found = true;
         currentState = State::TRACING_PATH;
         tracePos = goal;
         return;
    }
    // Explore all four directions
    for (auto [dr, dc] : directions) {
        int nr = r + dr, nc = c + dc;

        if (!isInside(grid, nr, nc)) continue;
        char cell = grid[nr][nc];
        if (cell == '#' || visited[nr][nc]) continue;

        visited[nr][nc] = true;
        parent[nr][nc] = {r, c};

        // We only push to queue here. We *don't* color.
        q.push({nr, nc});
    }
}

