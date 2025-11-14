#ifndef SOLVER_H
#define SOLVER_H

#include <vector>
#include <string>
#include <utility>
#include "Maze.h" 

class Solver {
public:
    // This enum is used for all solvers to track their state
    enum class State {
        SEARCHING,
        TRACING_PATH,
        DONE
    };

    Solver(const Maze& maze, char marker);
    virtual ~Solver() = default;

    // Pure virtual function: forces child classes (BFS, DFS...)
    // to implement their own step() logic.
    virtual void step() = 0;

    bool isFinished() const {
        // It's finished when its state is DONE
        return currentState == State::DONE; 
    }

    bool wasPathFound() const { return found; }
    std::vector<std::string> getGrid() const { return grid; }


protected:
    // --- Members shared by all solvers ---

    char symbol;         // The character to draw 
    State currentState;  // The current state of the solver
    bool found;          // Did we find the exit?

    // Maze grid and path data
    std::vector<std::string> grid;
    std::pair<int, int> start;
    std::pair<int, int> goal;
    std::pair<int, int> tracePos; // For tracing the path back
    
    // Parent map for path reconstruction
    std::vector<std::vector<std::pair<int, int>>> parent; 
};

#endif // SOLVER_H