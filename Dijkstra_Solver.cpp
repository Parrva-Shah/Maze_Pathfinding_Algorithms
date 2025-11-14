#include "Dijkstra_Solver.h"
#include <limits>

Dijkstra_Solver::Dijkstra_Solver(const Maze& maze)
    : Solver(maze, 'K')
{
    int R = maze.getRows();
    int C = maze.getCols();
    // distMap is a 2D vector<int> storing the currently known shortest distance
    distMap.assign(R, std::vector<int>(C, std::numeric_limits<int>::max()));
    visited.assign(R, std::vector<bool>(C, false)); 

    distMap[start.first][start.second] = 0;
    pq.push({0, start});//priority queue (min-heap)
    
    // Start the algorithm's timer
    m_clock.restart();
}

void Dijkstra_Solver::step() {
    if (currentState == State::TRACING_PATH) {
        
        // Count this node as part of the final path
        m_pathLength++;
        
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
        
        // Stop the clock if the search fails
        m_timeTaken = m_clock.getElapsedTime();
        
        return;
    }

    // Loop until we find an unvisited node
    while (!pq.empty()) {
        Node cur = pq.top();
        pq.pop();

        int r = cur.pos.first;
        int c = cur.pos.second;

        if (visited[r][c]) continue; // Already processed
        
        // We are officially exploring this node
        m_nodesExplored++;
        
        visited[r][c] = true;

        if (grid[r][c] == ' ')
            grid[r][c] = symbol;

        if (r == goal.first && c == goal.second) {
            found = true;
            currentState = State::TRACING_PATH;
            tracePos = goal;
            
            // Stop the clock on success
            m_timeTaken = m_clock.getElapsedTime();
            
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
            }
        }
        // We processed one valid, unvisited node. Exit step for visualization.
        return;
    }

    // If loop finishes, pq was emptied
    currentState = State::DONE;
    found = false;
    
    // Make sure clock is stopped even if it fails in a weird way
    if (m_timeTaken == sf::Time::Zero) {
        m_timeTaken = m_clock.getElapsedTime();
    }
}