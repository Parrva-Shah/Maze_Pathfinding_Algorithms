#include "Dijkstra_Solver.h"
#include <limits>

Dijkstra_Solver::Dijkstra_Solver(const Maze& maze)
    : Solver(maze, 'K')
{
    int R = maze.getRows();
    int C = maze.getCols();

    distMap.assign(R, std::vector<int>(C, std::numeric_limits<int>::max()));
    visited.assign(R, std::vector<bool>(C, false)); 
    // parent in base

    distMap[start.first][start.second] = 0;
    pq.push({0, start});
}

void Dijkstra_Solver::step() {
    if (currentState == State::TRACING_PATH) {
        if (tracePos == start) {
            currentState = State::DONE;
            return;
        }
        if (grid[tracePos.first][tracePos.second] != 'E') {
            grid[tracePos.first][tracePos.second] = 'X';
        }
        tracePos = parent[tracePos.first][tracePos.second];
        return;
    }

    if (currentState != State::SEARCHING) return;
    
    if (pq.empty()) {
        currentState = State::DONE;
        found = false;
        return;
    }

    // Loop until we find an unvisited node
    while (!pq.empty()) {
        Node cur = pq.top();
        pq.pop();

        int r = cur.pos.first;
        int c = cur.pos.second;

        if (visited[r][c]) continue; // Already processed
        visited[r][c] = true;

        // Color when processing
        if (grid[r][c] == ' ')
            grid[r][c] = symbol;

        if (r == goal.first && c == goal.second) {
            found = true;
            currentState = State::TRACING_PATH;
            tracePos = goal;
            return; // Exit step
        }

        for (auto [dr, dc] : directions) {
            int nr = r + dr, nc = c + dc;
            if (!isInside(grid, nr, nc)) continue;
            if (grid[nr][nc] == '#') continue;

            int newCost = distMap[r][c] + 1;
            if (newCost < distMap[nr][nc]) {
                distMap[nr][nc] = newCost;
                parent[nr][nc] = {r, c};
                pq.push({newCost, {nr, nc}});
                // Don't color here, color when popped
            }
        }
        // Return after processing one valid node
        return;
    }

    // If loop finishes, pq was emptied
    currentState = State::DONE;
    found = false;
}

