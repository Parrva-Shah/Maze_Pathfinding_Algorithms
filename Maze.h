#ifndef MAZE_H
#define MAZE_H

#include <vector>
#include <string>
#include <utility> // For std::pair

class Maze {
public:
    // Construct a maze with given dimensions and optional seed
    Maze(int rows = 21, int cols = 41, unsigned seed = 0);

    // Direct access to the grid (each string = one row)
    // This is public, so solvers can access it (as per your original design)
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

    // Private helper function to do the actual generation
    // The parameter is now wallDensity (e.g., 25 for 25%)
    void generateSolvableMaze(int wallDensity);
};

#endif  // MAZE_H