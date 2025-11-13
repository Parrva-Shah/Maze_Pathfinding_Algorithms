#ifndef BFS_SOLVER_H
#define BFS_SOLVER_H

#include "Solver.h"
#include "Utils.h"
#include <queue>
#include <vector>
#include <utility>

class BFS_Solver : public Solver {
public:
    explicit BFS_Solver(const Maze& maze);

    void step() override;
    // bool isFinished() const override; <-- THIS LINE IS REMOVED

private:
    std::queue<std::pair<int,int>> q;
    std::vector<std::vector<bool>> visited;
};

#endif  // BFS_SOLVER_H