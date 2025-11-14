# C++ Maze Pathfinding Visualizer

This is a C++ application that visualizes and compares five different pathfinding algorithms in real-time using the SFML library. The program generates a random maze, runs each algorithm to find a path from the start ('S') to the goal ('E'), and then displays a final screen comparing the performance of each algorithm.

This repository is pre-configured for **Linux** development with Visual Studio Code, including a build task and IntelliSense settings and we the reproducibility of the repository is dependent on it. We have included seperate instructions to run this on windows as well but then the `.json` files go to waste.

Made by : Parrva Chirag Shah (B24CS1053)
          Ishita Tyagi (B24CS1083)
(This is a new repository created on 10th November when our third group member Veer Shah ditched the group, we have talked with Prajjwal via mail and he told us to proceed as a group of two)
##  Features

* **Real-Time Visualization**: Watch each algorithm explore the maze step-by-step.
* **Algorithm Comparison**: See how different algorithms perform on the same maze.
* **Statistics Screen**: After all visualizations, a final screen compares all algorithms on:
    * Time Taken (ms)
    * Nodes Explored
    * Path Length
* **Random Maze Generation**: A new, randomized maze is generated every time you run the program.
* **VS Code Ready**: Includes:
    * `.vscode/tasks.json` for one-press building (`Ctrl+Shift+B`) on Linux.
    * `.vscode/c_cpp_properties.json` for Linux-based IntelliSense.
* **Object-Oriented Design**: Uses an abstract `Solver` base class with concrete implementations for each algorithm, making the code clean and easy to extend.

---

##  Algorithms Implemented

The visualizer includes five classic pathfinding algorithms:

1.  **Breadth-First Search (BFS)**: A simple search that explores all neighbors at the present depth before moving on. Guaranteed to find the shortest path in an unweighted grid. Uses a `std::queue`.
2.  **Depth-First Search (DFS)**: Explores as far as possible down one branch before backtracking. Very fast but not guaranteed to find the shortest path. Uses a `std::stack`.
3.  **Dijkstra's Algorithm**: Finds the shortest path in a weighted graph (in this unweighted grid, it behaves similarly to BFS but is built to handle costs). Uses a `std::priority_queue`.
4.  **A\* (A-Star) Search**: An informed search algorithm that uses a heuristic (Manhattan distance) to guide its search. It is efficient and guaranteed to find the shortest path. Uses a `std::priority_queue`.
5.  **Greedy Best-First Search**: An informed search that only follows the heuristic. It's very fast but "greedy," so it may get stuck in loops or fail to find the shortest path. Uses a `std::priority_queue`.

---

##  Tech Stack

* **C++17**: The core application logic.
* **SFML (Simple and Fast Multimedia Library)**: Used for all graphics, windowing, and timing.
* **Key Data Structures**:
    * `std::vector` (for grids, visited maps, etc.)
    * `std::map` (to store final results)
    * `std::unique_ptr` (to manage the active solver)
    * `std::queue`, `std::stack`, `std::priority_queue`

---

##  Getting Started: How to Compile & Run

The build process is different for Linux and Windows.

---

###  On Linux (Recommended)

This repository is set up for Linux. You can use the VS Code task or compile manually.

#### Method 1: Using VS Code (Easiest)

1.  **Install Prerequisites**:
    ```sh
    sudo apt install build-essential libsfml-dev git
    ```
2.  **Clone & Open**:
    ```sh
    git clone [https://github.com/Parrva-Shah/Maze_Pathfinding_Algorithms.git](https://github.com/Parrva-Shah/Maze_Pathfinding_Algorithms.git)
    cd Maze_Pathfinding_Algorithms
    code .
    ```
3.  **Build**:
    * Once VS Code opens, it will detect the `.vscode` files.
    * Press **`Ctrl+Shift+B`**. This will use the included `tasks.json` to build the executable, `maze_visualizer`.
4.  **Run**:
    * Run the program from the built-in terminal:
    ```sh
    ./maze_visualizer
    ```

#### Method 2: Manual Compilation

1.  **Install Prerequisites**:
    ```sh
    sudo apt install build-essential libsfml-dev git
    ```
2.  **Clone Repository**:
    ```sh
    git clone [https://github.com/Parrva-Shah/Maze_Pathfinding_Algorithms.git](https://github.com/Parrva-Shah/Maze_Pathfinding_Algorithms.git)
    cd Maze_Pathfinding_Algorithms
    ```
3.  **Compile**:
    * The `tasks.json` uses a `*.cpp` glob, which is very clean. We can use that here too.
    ```sh
    g++ -g *.cpp -o maze_visualizer -lsfml-graphics -lsfml-window -lsfml-system
    ```
4.  **Run**:
    ```sh
    ./maze_visualizer
    ```

---

###  On Windows (Manual Method)

The `tasks.json` in this repository is **Linux-specific** and will not work on Windows. You must compile manually.

#### 1. Prerequisites

1.  **Git**: To clone the repository.
2.  **C++ Compiler**: [Download MinGW-w64](https://www.mingw-w64.org/).
    * During installation, make sure to add it to your system's `PATH`.
3.  **SFML Binaries**: [Download SFML](https://www.sfml-dev.org/download.php).
    * Make sure you get the **MinGW** version (e.g., `GCC 13.1.0 MinGW (SEH) - 64-bit`).
    * Extract the downloaded `.zip` file to a simple-to-remember location, for example: `C:\sfml`

#### 2. Compilation Steps

1.  **Clone the Repository**:
    ```sh
    git clone [https://github.com/Parrva-Shah/Maze_Pathfinding_Algorithms.git](https://github.com/Parrva-Shah/Maze_Pathfinding_Algorithms.git)
    cd Maze_Pathfinding_Algorithms
    ```

2.  **Compile the Code**:
    * Open a terminal (Command Prompt or PowerShell) in the project directory.
    * Run the following command, which is the Windows equivalent of your `tasks.json`.
    > **Note**: Replace `C:\sfml` if you extracted SFML to a different location.

    ```sh
    g++ -g *.cpp -o maze_visualizer.exe -I C:\sfml\include -L C:\sfml\lib -lsfml-graphics -lsfml-window -lsfml-system
    ```
    * `-o maze_visualizer.exe`: Names the final executable.
    * `-I C:\sfml\include`: Tells the compiler where to find the SFML header files (`.hpp`).
    * `-L C:\sfml\lib`: Tells the linker where to find the SFML library files (`.a`).
    * `-lsfml-graphics ...`: Links the required SFML modules.

3.  **Run the Program (Critical Step)**:
    * Your program (`maze_visualizer.exe`) will not run until you provide the SFML `.dll` files.
    * **Copy DLLs**: Go to your SFML folder (e.g., `C:\sfml\bin`) and copy all the `.dll` files (like `sfml-graphics-2.dll`, `sfml-window-2.dll`, etc.).
    * **Paste DLLs**: Paste them into your project folder, right next to `maze_visualizer.exe`.
    * **Check Font**: Ensure the `arial.ttf` file is in the *same directory*.
    * Now you can run the program from your terminal:
    ```sh
    ./maze_visualizer.exe
    ```

---

##  How to Use

The controls are simple:

* **Press [Spacebar]**: This advances the program to the next state.
    1.  Starts on the "Base Maze" screen.
    2.  Press **Space**: Runs the BFS visualization.
    3.  Press **Space**: Runs the DFS visualization.
    4.  Press **Space**: Runs the A* visualization.
    5.  Press **Space**: Runs the Dijkstra visualization.
    6.  Press **Space**: Runs the Greedy Best-First visualization.
    7.  Press **Space**: Shows the final "Results" screen.
    8.  Press **Space**: Restarts the entire process with a new maze.

---

##  Code Structure

* **`main_gui.cpp`**: The main application entry point. Handles the SFML window, state management (starting, running, paused, results), and drawing the grid.
* **`Maze.h` / `Maze.cpp`**: Contains the `Maze` class, which is responsible for generating and storing the random grid.
* **`Solver.h` / `Solver.cpp`**: Defines the `Solver` abstract base class. This class provides the common interface (`step()`, `isFinished()`, etc.) that all algorithm implementations must follow.
* **`*Solver.h` / `*Solver.cpp`**: The concrete implementations for each algorithm (e.g., `BFS_Solver`, `AStar_Solver`), which inherit from `Solver`.
* **`Utils.h` / `Utils.cpp`**: Helper functions for clearing the console and printing grids side-by-side (for a console-based version).
* **`arial.ttf`**: The font file used for rendering text in the GUI.
* **`.vscode/*.json`**: VS Code configuration files for building the project on Linux.
