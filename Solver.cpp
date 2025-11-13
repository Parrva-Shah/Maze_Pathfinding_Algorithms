#include "Solver.h"

Solver::Solver(const Maze& maze, char marker) 
    : symbol(marker),
      currentState(State::SEARCHING),
      //finished(false),
      found(false)
{
    // Deep copy the base maze grid
    grid = maze.grid;

    // IMPORTANT: copy start & goal
    start = maze.getStart();
    goal  = maze.getGoal();

    // Initialize parent map for all solvers
    int R = maze.getRows();
    int C = maze.getCols();
    parent.assign(R, std::vector<std::pair<int,int>>(C, {-1,-1}));
}