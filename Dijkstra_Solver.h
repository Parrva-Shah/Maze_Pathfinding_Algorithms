#ifndef DIJKSTRA_SOLVER_H
#define DIJKSTRA_SOLVER_H

#include "Solver.h"
#include "Utils.h"
#include <queue>
#include <vector>

class Dijkstra_Solver : public Solver {
public:
    explicit Dijkstra_Solver(const Maze& maze);

    void step() override;

private:
    struct Node {
        int dist;
        std::pair<int,int> pos;
        bool operator>(const Node& other) const {
            return dist > other.dist;
        }
    };

    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> pq;
    std::vector<std::vector<int>> distMap;
    std::vector<std::vector<bool>> visited;
};

#endif