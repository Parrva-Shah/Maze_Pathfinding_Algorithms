#ifndef SOLVER_H
#define SOLVER_H

#include <vector>
#include <string>
#include <utility>
#include <SFML/System/Time.hpp>
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


    // Makes all algorithms use their own step() function
    virtual void step() = 0;

    bool isFinished() const {
        // It's finished when its state is DONE
        return currentState == State::DONE; 
    }

    bool wasPathFound() const { return found; }
    std::vector<std::string> getGrid() const { return grid; }
    int getNodesExplored() const { return m_nodesExplored; }
    int getPathLength() const { return m_pathLength; }
    sf::Time getTimeTaken() const { return m_timeTaken; }
    bool isPathFound() const { return found; } 


protected:
    char symbol;         // The character to draw 
    State currentState;  // The current state of the solver
    bool found;          // Did we find the exit?

    // Maze grid and path data
    std::vector<std::string> grid;
    std::pair<int, int> start;
    std::pair<int, int> goal;
    std::pair<int, int> tracePos; // For tracing the path back
    
    // Path reconstruction
    std::vector<std::vector<std::pair<int, int>>> parent; 

    // All algorithms (BFS, A*, etc.) must update these
    int m_nodesExplored = 0;
    int m_pathLength = 0;
    sf::Time m_timeTaken = sf::Time::Zero;
};

#endif // SOLVER_H