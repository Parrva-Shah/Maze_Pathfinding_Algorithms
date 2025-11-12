#pragma once
#include "Maze.h"

class Solver {
public:
    using Grid = Maze::Grid;

    Solver(const Grid& g, std::pair<int,int> s, std::pair<int,int> e, char markChar)
        : grid(g), start(s), goal(e), markCh(markChar), done(false), found(false)
    {}

    virtual ~Solver() = default;

    // perform one step of the algorithm (one expansion)
    virtual void step() = 0;

    // return whether solver finished
    bool isFinished() const { return done; }

    // whether solver found goal
    bool hasFound() const { return found; }

    // get current grid copy for printing
    Grid getGrid() const { return grid; }

    char getMark() const { return markCh; }

protected:
    Grid grid;
    std::pair<int,int> start;
    std::pair<int,int> goal;
    char markCh;
    bool done;
    bool found;
};
