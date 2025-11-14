#ifndef MAZE_H
#define MAZE_H

#include <vector>
#include <string>
#include <utility> 

class Maze {
public:
    // Construct a maze with given dimensions and optional seed
    Maze(int rows = 21, int cols = 41, unsigned seed = 0);

    std::vector<std::string> grid;

    // Accessors
    std::pair<int, int> getStart() const { return start; }
    std::pair<int, int> getGoal()  const { return goal;  }
    int getRows() const { return rows; }
    int getCols() const { return cols; }

private:
    int rows;
    int cols;
    unsigned seed;
    std::pair<int, int> start;
    std::pair<int, int> goal;

    void generateSolvableMaze(int wallDensity);
};

#endif  // MAZE_H