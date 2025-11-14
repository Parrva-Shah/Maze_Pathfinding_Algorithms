#ifndef BFS_SOLVER_H
#define BFS_SOLVER_H

#include "Solver.h"
#include "Utils.h"
#include <queue>
#include <vector>
#include <utility>
#include <SFML/System/Clock.hpp> 

class BFS_Solver : public Solver {
public:
    explicit BFS_Solver(const Maze& maze);

    void step() override;

private:
    std::queue<std::pair<int,int>> q;
    std::vector<std::vector<bool>> visited;

    // Clock for timing the algorithm
    sf::Clock m_clock;
};

#endif  // BFS_SOLVER_H