#ifndef DFS_SOLVER_H
#define DFS_SOLVER_H

#include "Solver.h"
#include "Utils.h"
#include <stack>
#include <vector>
#include <utility>

using namespace std;

class DFS_Solver : public Solver {
public:
    DFS_Solver(const Maze& maze);
    void step() override;     // perform exactly 1 DFS action

private:
    stack<pair<int,int>> stk;      // DFS stack
    vector<vector<bool>> visited;
};

#endif // DFS_SOLVER_H