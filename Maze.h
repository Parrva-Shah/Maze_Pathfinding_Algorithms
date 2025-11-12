#ifndef MAZE_H
#define MAZE_H

#include <vector>
#include <string>
#include <utility> 

class Maze {
public:
    using Grid = std::vector<std::string>;

    Maze(int rows, int cols);
    
    void generateSolvableMaze(int openingFactor = 3);
    
    char at(int r, int c) const;
    void setCell(int r, int c, char val);
    std::pair<int,int> getStart() const;
    std::pair<int,int> getGoal() const;
    int getRows() const;
    int getCols() const;
    
    std::pair<int,int> getEnd() const { return goal; }
    Grid copyGrid() const { return grid; }

    Grid grid;

private:
    int rows, cols;
    std::pair<int,int> start, goal;
};

#endif