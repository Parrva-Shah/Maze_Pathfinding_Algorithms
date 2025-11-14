// main.cpp — Sequential SFML Visualizer
#include <iostream>
#include <vector>
#include <string>
#include <memory>

// SFML Includes
#include <SFML/Graphics.hpp> 

// Project Includes
#include "Maze.h"
#include "Utils.h" // Assuming this has sleep_ms, clearConsole, etc.
#include "Solver.h"
#include "BFS_Solver.h"
#include "DFS_Solver.h"
#include "Dijkstra_Solver.h"
#include "AStar_Solver.h"
#include "GreedyBestFirst_Solver.h"

// --- Visualization Constants ---
const float CELL_SIZE = 20.0f;  // Size of each maze cell in pixels
const float PADDING = 40.0f;    // Padding around the maze
const unsigned int FONT_SIZE = 24;
const float TITLE_HEIGHT = 40.0f; // Space for the title

// --- Simulation Timing ---
const sf::Time TIME_PER_STEP = sf::milliseconds(5); // How fast the algo runs
const sf::Time PAUSE_ON_FINISH = sf::seconds(2.5f); // How long to show the result

// --- State Definition ---
enum class VizState {
    Starting, // Showing the base maze
    Running,  // Algorithm is solving
    Paused    // Showing the final path
};


/**
 * @brief Gets the SFML color for a specific cell type.
 * @param cellType The character from the grid ('#', ' ', 'S', 'E', '.', '@')
 * @param traversalColor The unique color for this algorithm's "visited" ('.') cells
 * @return The SFML color to draw.
 */
// sf::Color getCellColor(char cellType, sf::Color traversalColor) {
//     switch (cellType) {
//         case '#': return sf::Color(50, 50, 50);    // Wall
//         case ' ': return sf::Color::White;          // Empty
//         case 'S': return sf::Color::Green;          // Start
//         case 'E': return sf::Color(255, 0, 0, 100); // End (semi-transparent)
        
//         // This is the magic:
//         case '.': return traversalColor;            // Visited (use algo-specific color)
//         case '@': return sf::Color::Red;            // Path (ALWAYS red, as requested)
        
//         default:  return sf::Color::Black;
//     }
// }
/**
 * @brief Gets the SFML color for a specific cell type.
 * @param cellType The character from the grid ('#', ' ', 'S', 'E', 'B', 'D', 'A', 'K', 'G', 'X')
 * @param traversalColor The unique color for this algorithm's "visited" cells
 * @return The SFML color to draw.
 */
sf::Color getCellColor(char cellType, sf::Color traversalColor) {
    switch (cellType) {
        case '#': return sf::Color(50, 50, 50);    // Wall
        case ' ': return sf::Color::White;          // Empty
        case 'S': return sf::Color::Green;          // Start
        case 'E': return sf::Color::Yellow; // End (semi-transparent)
        
        // --- THIS IS THE FIX ---
        // Your final path character
        case 'X': return sf::Color::Red;            // Path (ALWAYS red)

        // --- THE SECOND FIX ---
        // Any other character ('B', 'D', 'A', 'K', 'G')
        // will fall through to the default case and be colored
        // with the algorithm-specific traversalColor.
        default:  return traversalColor;
    }
}

/**
 * @brief Draws a single maze (either base or from a solver) to the window
 */
void drawMaze(sf::RenderWindow& window,
              const std::vector<std::string>& grid,
              sf::Font& font,
              const std::string& title,
              sf::Color traversalColor) // The color to use for '.'
{
    // --- 1. Draw Title ---
    sf::Text titleText(title, font, FONT_SIZE);
    titleText.setPosition(PADDING, PADDING / 2.0f);
    titleText.setFillColor(sf::Color::White);
    window.draw(titleText);

    // --- 2. Draw Maze Grid ---
    sf::RectangleShape cellRect(sf::Vector2f(CELL_SIZE, CELL_SIZE));
    float gridBaseY = PADDING + TITLE_HEIGHT;
    
    for (size_t r = 0; r < grid.size(); ++r) {
        for (size_t c = 0; c < grid[r].size(); ++c) {
            cellRect.setPosition(PADDING + c * CELL_SIZE, gridBaseY + r * CELL_SIZE);
            
            // Get the char from the grid and find its color
            char cellType = grid[r][c];
            cellRect.setFillColor(getCellColor(cellType, traversalColor));
            
            window.draw(cellRect);
        }
    }
}


// --- Helper function to create a solver by its index ---
std::unique_ptr<Solver> createSolver(int index, Maze& maze) {
    switch (index) {
        case 0: return std::make_unique<BFS_Solver>(maze);
        case 1: return std::make_unique<DFS_Solver>(maze);
        case 2: return std::make_unique<AStar_Solver>(maze);
        case 3: return std::make_unique<Dijkstra_Solver>(maze);
        case 4: return std::make_unique<GreedyBestFirst_Solver>(maze);
        default: return nullptr;
    }
}


int main() {
    // --- 1. Base Maze Setup ---
    const int R = 31, C = 51; // A good size for one maze
    Maze baseMaze(R, C);
    Maze mazeCopy = baseMaze; // This will be reset for each algo

    // --- 2. Algorithm Definitions ---
    std::vector<std::string> titles = {
        "1. Breadth-First Search (BFS)",
        "2. Depth-First Search (DFS)",
        "3. A* Search",
        "4. Dijkstra's Algorithm",
        "5. Greedy Best-First Search"
    };
    std::vector<sf::Color> traversalColors = {
        sf::Color(0, 150, 255),  // BFS (Blue)
        sf::Color(0, 200, 100),  // DFS (Green)
        sf::Color(200, 0, 200),  // A* (Purple)
        sf::Color(255, 150, 0),  // Dijkstra (Orange)
        sf::Color(0, 200, 200)   // Greedy (Cyan)
    };
    int currentAlgoIndex = 0;

    // --- 3. SFML Window Setup ---
    const float mazeWidth = baseMaze.getCols() * CELL_SIZE;
    const float mazeHeight = baseMaze.getRows() * CELL_SIZE;
    unsigned int windowWidth = (unsigned int)(mazeWidth + PADDING * 2);
    unsigned int windowHeight = (unsigned int)(mazeHeight + PADDING * 2 + TITLE_HEIGHT);

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Maze Solver Visualizer");

    // --- 4. Font Setup ---
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Error: Could not load 'arial.ttf'.\n";
        return -1;
    }
    
    // Instructions Text
    sf::Text instructionText("Press [Space] to start next algorithm", font, 16);
    instructionText.setFillColor(sf::Color(255, 255, 255, 150)); // semi-transparent
    instructionText.setPosition(PADDING, windowHeight - PADDING / 1.5f);


    // --- 5. State & Clock Setup ---
    VizState state = VizState::Starting;
    sf::Clock stepClock;
    std::unique_ptr<Solver> currentSolver = nullptr;

    // Get the base grid once for the start screen
    auto baseGrid = baseMaze.grid;
    auto baseStart = baseMaze.getStart();
    auto baseGoal = baseMaze.getGoal();
    baseGrid[baseStart.first][baseStart.second] = 'S';
    baseGrid[baseGoal.first][baseGoal.second] = 'E';


    // --- 6. Main SFML Game Loop ---
    while (window.isOpen()) {
        
        // --- Event Handling ---
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            // Handle starting/progressing
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                if (state == VizState::Starting) {
                    // --- Start the first algorithm ---
                    mazeCopy = baseMaze; // Refresh the maze
                    currentSolver = createSolver(currentAlgoIndex, mazeCopy);
                    state = VizState::Running;
                    stepClock.restart();
                } 
                else if (state == VizState::Paused) {
                    // --- Move to the next algorithm ---
                    currentAlgoIndex++;
                    if (currentAlgoIndex >= titles.size()) {
                        currentAlgoIndex = 0; // Loop back
                    }
                    mazeCopy = baseMaze; // Refresh the maze
                    currentSolver = createSolver(currentAlgoIndex, mazeCopy);
                    state = VizState::Running;
                    stepClock.restart();
                }
            }
        }

        // --- Update Logic ---
        if (state == VizState::Running && currentSolver && !currentSolver->isFinished()) {
            if (stepClock.getElapsedTime() > TIME_PER_STEP) {
                stepClock.restart();
                currentSolver->step();
                
                // When it finishes, change state
                if (currentSolver->isFinished()) {
                    state = VizState::Paused;
                    stepClock.restart();
                }
            }
        }
        // Check if the pause time has elapsed (this is an automatic transition)
        /*
        // --- This code block makes it transition automatically ---
        // --- Remove the 'else if (state == VizState::Paused)' in event handling if you use this ---
        if (state == VizState::Paused && stepClock.getElapsedTime() > PAUSE_ON_FINISH) {
            currentAlgoIndex++;
            if (currentAlgoIndex >= titles.size()) {
                currentAlgoIndex = 0; // Loop back
            }
            mazeCopy = baseMaze; // Refresh the maze
            currentSolver = createSolver(currentAlgoIndex, mazeCopy);
            state = VizState::Running;
            stepClock.restart();
        }
        */


        // --- Drawing ---
        window.clear(sf::Color(20, 20, 20)); // Dark background

        if (state == VizState::Starting) {
            // Draw the base maze
            drawMaze(window, baseGrid, font, "Base Maze (Press Space)", sf::Color::Transparent);
        } 
        else if (currentSolver) {
            // Get the solver's current grid
            auto grid = currentSolver->getGrid();
            // Add Start/End back in (since solvers remove them)
            grid[baseStart.first][baseStart.second] = 'S';
            grid[baseGoal.first][baseGoal.second] = 'E';

            // Draw the solver's grid
            drawMaze(window, grid, font, titles[currentAlgoIndex], traversalColors[currentAlgoIndex]);
            
            // If paused, show the "Press Space" text
            if (state == VizState::Paused) {
                window.draw(instructionText);
            }
        }

        window.display();
    }

    return 0;
}