#include "Solver.h"
using namespace std;

Solver::Solver(const Maze& maze, char marker) 
    : symbol(marker),
      currentState(State::SEARCHING),
      found(false)
{
    // Deep copy the base maze grid
    grid = maze.grid;

    start = maze.getStart();
    goal  = maze.getGoal();

    // Initialize parent map for all solvers
    int R = maze.getRows();
    int C = maze.getCols();
    parent.assign(R, vector<pair<int,int>>(C, {-1,-1}));
}