#ifndef MAZE_H
#define MAZE_H

#include <vector>
#include <string>
#include <utility>

class Maze {
public:
    Maze(int rows, int cols);
    void generateSolvableMaze();
    void print() const;
    char at(int r, int c) const;
    void setCell(int r, int c, char val);
    std::pair<int,int> getStart() const;
    std::pair<int,int> getGoal() const;
    int getRows() const;
    int getCols() const;

    std::vector<std::string> grid;

private:
    int rows, cols;
    std::pair<int,int> start, goal;
};

#endif
