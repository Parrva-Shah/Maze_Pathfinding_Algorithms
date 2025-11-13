#ifndef ASTAR_SOLVER_H
#define ASTAR_SOLVER_H

#include "Solver.h"
#include "Utils.h"
#include <queue>
#include <vector>
#include <utility>

class AStar_Solver : public Solver {
public:
    explicit AStar_Solver(const Maze& maze);

    void step() override;

private:
    using Node = std::pair<int, int>;

    struct NodeData {
        int f; Node pos;
        bool operator>(const NodeData& other) const { return f > other.f; }
    };

    std::priority_queue<NodeData, std::vector<NodeData>, std::greater<NodeData>> openSet;
    std::vector<std::vector<int>> gScore;
    std::vector<std::vector<bool>> visited;

    int heuristic(int r, int c) const;
};

#endif  // ASTAR_SOLVER_H