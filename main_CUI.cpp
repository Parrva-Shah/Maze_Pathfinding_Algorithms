// main.cpp — 6-solver visualizer
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <climits>

#include "Maze.h"
#include "Utils.h"
#include "Solver.h"
#include "BFS_Solver.h"
#include "DFS_Solver.h"
#include "Dijkstra_Solver.h"
#include "AStar_Solver.h"
#include "GreedyBestFirst_Solver.h" // <-- ADDED

int main() {
    // --- generate a single maze and make independent copies for each solver ---
    const int R = 21, C = 41;
    Maze baseMaze(R, C); // This uses the new random-wall-spray maze

    // create independent maze copies (deep copies)
    Maze mazeB = baseMaze;
    Maze mazeD = baseMaze;
    Maze mazeK = baseMaze;
    Maze mazeA = baseMaze;
    Maze mazeG = baseMaze; // <-- ADDED

    // create solver objects operating on their own Maze copies
    BFS_Solver               bfs(mazeB);
    DFS_Solver               dfs(mazeD);
    Dijkstra_Solver          dij(mazeK);
    AStar_Solver             ast(mazeA);
    GreedyBestFirst_Solver   gbs(mazeG); // <-- ADDED

    // keep pointers for uniform iteration
    std::vector<Solver*> solvers = { &bfs, &dfs, &dij, &ast, &gbs };
    
    std::vector<std::string> titles = { 
        "BFS (B)", "DFS (D)", "Dijkstra (K)", 
        "A* (A)", "Greedy (G)"
    };

    // to record which frame each solver finished on
    std::vector<int> finishFrame(solvers.size(), -1);
    std::vector<bool> solverFoundPath(solvers.size(), false);

    int frame = 0;
    bool allFinished = false;

    while (!allFinished) {
        frame++;
        allFinished = true;

        // Step every solver exactly once (or do nothing if finished)
        for (size_t i = 0; i < solvers.size(); ++i) {
            Solver* s = solvers[i];
            if (!s->isFinished()) {
                s->step(); // Just call step, it returns void
                
                if (s->isFinished() && finishFrame[i] == -1) {
                    finishFrame[i] = frame;
                    solverFoundPath[i] = s->wasPathFound();
                }
            }
            if (!s->isFinished()) allFinished = false;
        }

        // Collect current grids for display
        std::vector<std::vector<std::string>> frames;
        frames.reserve(solvers.size());
        for (size_t i = 0; i < solvers.size(); ++i) {
            auto grid = solvers[i]->getGrid(); // deep copy
            
            auto baseStart = baseMaze.getStart();
            auto baseGoal = baseMaze.getGoal();
            grid[baseStart.first][baseStart.second] = 'S';
            grid[baseGoal.first][baseGoal.second] = 'E';

            frames.push_back(std::move(grid));
        }

        // Print the side-by-side view (now 2x3)
        clearConsole();
        std::cout << "Frame: " << frame << "\n\n";
        printSideBySide(frames, titles); // This now handles the 2x3 layout

        // small delay so you can watch it (tune to taste)
        sleep_ms(20);
    }

    // Final summary
    std::cout << "\n\n--- Results ---\n";
    for (size_t i = 0; i < solvers.size(); ++i) {
        if (!solverFoundPath[i]) std::cout << titles[i] << " : NOT FOUND\n";
        else std::cout << titles[i] << " : Finished in " << finishFrame[i] << " frames\n";
    }

    // find fastest (smallest non-negative finish frame *that found a path*)
    int fastest = -1;
    int best = INT_MAX;
    for (size_t i = 0; i < finishFrame.size(); ++i) {
        if (solverFoundPath[i] && finishFrame[i] < best) {
            best = finishFrame[i];
            fastest = (int)i;
        }
    }
    if (fastest == -1) std::cout << "\nNo solver found a path.\n";
    else std::cout << "\nFastest Algorithm: " << titles[fastest] << " 🎉\n";

    return 0;
}