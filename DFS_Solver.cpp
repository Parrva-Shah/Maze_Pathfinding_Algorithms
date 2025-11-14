#include "DFS_Solver.h"
#include <iostream>

DFS_Solver::DFS_Solver(const Maze& maze)
    : Solver(maze, 'D')
{
    int R = maze.getRows();
    int C = maze.getCols();
    visited.assign(R, std::vector<bool>(C, false));
    // 'parent' is initialized in base Solver
    
    stk.push(start);//stk is a stack<pair<int,int>> used to implement DFS iteratively.
    
    // Start the algorithm's timer
    m_clock.restart();
}

void DFS_Solver::step() {
    
    if (currentState == State::TRACING_PATH) {
        
        // Count this node as part of the final path
        m_pathLength++;
        
        if (tracePos == start) {
            currentState = State::DONE;
            return;
        }
        // Use 'X' for the final path
        if (grid[tracePos.first][tracePos.second] != 'E') {// grid is a 2D vector<char> representing the maze layout
            grid[tracePos.first][tracePos.second] = 'X';
        }
        tracePos = parent[tracePos.first][tracePos.second];
        return;
    }

    if (currentState != State::SEARCHING) return;

    // Loop until we find a new node to process or the stack is empty
    while (!stk.empty()) {
        auto [r, c] = stk.top();
        stk.pop();

        // If already visited, pop and continue the loop (do not return!)
        if (visited[r][c]) {
            continue; 
        }
        
        // This is the first time we are officially processing this node
        m_nodesExplored++;
        
        visited[r][c] = true;
        if (grid[r][c] == ' ') grid[r][c] = symbol;

        // If we've reached the goal cell, switch to TRACING
        if (std::make_pair(r,c) == goal) {
            found = true;
            currentState = State::TRACING_PATH;
            tracePos = goal; // Start tracing from the goal
            
            // Stop the clock on success
            m_timeTaken = m_clock.getElapsedTime();
            
            return; // Exit step
        }

        // Push unvisited neighbors
        for (int i = 3; i >= 0; --i) { // Iterating backwards to explore in a consistent order
            int nr = r + directions[i].first;
            int nc = c + directions[i].second;
            if (!isInside(grid, nr, nc)) continue;
            if (grid[nr][nc] == '#') continue;
            if (visited[nr][nc]) continue;
            
            parent[nr][nc] = {r, c};
            stk.push({nr, nc});
        }

        // We processed one valid, unvisited node. Exit step for visualization.
        return;
    }
    
    // This part is reached only if the while loop exits (stack is empty)
    if (currentState == State::SEARCHING) {
        currentState = State::DONE;
        found = false;
        
        // Stop the clock if the search fails
        m_timeTaken = m_clock.getElapsedTime();
    }
}