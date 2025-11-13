#ifndef SOLVER_H
#define SOLVER_H

#include <vector>
#include <string>
#include <utility>
#include "Maze.h" // Needs to know about the Maze

class Solver {
public:
    // This enum is used by all your child solvers
    enum class State {
        SEARCHING,
        TRACING_PATH,
        DONE
    };

    // Constructor (matches your Solver.cpp)
    Solver(const Maze& maze, char marker);
    
    // Virtual destructor (good practice for base classes)
    virtual ~Solver() = default;

    // --- Public Interface used by main.cpp ---

    // Pure virtual function: forces child classes (BFS, DFS...)
    // to implement their own step() logic.
    virtual void step() = 0;

    // Checks if the solver is done (this is the FIX)
    bool isFinished() const {
        // It's finished when its state is DONE
        return currentState == State::DONE; 
    }

    // Accessors used by main.cpp
    bool wasPathFound() const { return found; }
    std::vector<std::string> getGrid() const { return grid; }


protected:
    // --- Members shared by all solvers ---

    char symbol;         // The character to draw (B, D, K, A, etc.)
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