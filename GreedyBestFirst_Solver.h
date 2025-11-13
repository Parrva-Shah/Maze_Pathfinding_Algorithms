#ifndef GREEDY_BEST_FIRST_SOLVER_H
#define GREEDY_BEST_FIRST_SOLVER_H

#include "Solver.h"
#include "Utils.h"
#include <queue>
#include <vector>
#include <utility>
#include <cmath> 

// Greedy Best-First Search (GBFS)
// Only follows the heuristic (h-score), ignores path cost (g-score)
// Very fast, but not guaranteed to find the shortest path.
class GreedyBestFirst_Solver : public Solver {
public:
    explicit GreedyBestFirst_Solver(const Maze& maze);

    void step() override;

private:
    using Node = std::pair<int, int>; // Grid coordinate

    struct NodeData {
        int h; // Heuristic score
        Node pos;
        bool operator>(const NodeData& other) const { return h > other.h; }
    };

    std::priority_queue<NodeData, std::vector<NodeData>, std::greater<NodeData>> openSet;
    std::vector<std::vector<bool>> visited; // No gScore needed, just visited

    int heuristic(int r, int c) const;
};

#endif // GREEDY_BEST_FIRST_SOLVER_H