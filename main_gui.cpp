
#include <iostream>
#include <vector>       // Data Structure: Using std::vector to store ordered lists (e.g., titles, colors)
#include <string>
#include <memory>       //  Data Structure: Using std::unique_ptr for smart pointers (manages solver memory)
#include <map>          // Data Structure: Using std::map to store results (key=algo name, value=stats)
#include <limits>   
#include <SFML/Graphics.hpp> 
#include "Maze.h"
#include "Utils.h" 
#include "Solver.h"
#include "BFS_Solver.h"
#include "DFS_Solver.h"
#include "Dijkstra_Solver.h"
#include "AStar_Solver.h"
#include "GreedyBestFirst_Solver.h"

// For Visualisation Window 
const float CELL_SIZE = 20.0f;  
const float PADDING = 40.0f;    
const unsigned int FONT_SIZE = 24;
const float TITLE_HEIGHT = 40.0f; 

// To simulate the speed of visualisation
const sf::Time TIME_PER_STEP = sf::milliseconds(5); 
const sf::Time PAUSE_ON_FINISH = sf::seconds(2.5f); 

// Storing state values
enum class VizState {
    Starting,       // Base maze
    Running,        // Algorithm solving
    Paused,         // Final path
    ShowingResults  // Final results screen
};

// Simple struct to hold results. 
struct AlgoStats {
    int nodesExplored = 0;
    int pathLength = 0;
    float timeTakenMs = 0.0f;
    bool pathFound = false;
};



 //@brief Gets the SFML color for a specific cell type.
 
sf::Color getCellColor(char cellType, sf::Color traversalColor) {
    switch (cellType) {
        case '#': return sf::Color(50, 50, 50);    // Wall
        case ' ': return sf::Color::White;          // Empty
        case 'S': return sf::Color::Green;          // Start
        case 'E': return sf::Color::Yellow;         // End 
        case 'X': return sf::Color::Red;            // Final Path
        default:  return traversalColor;            // Algorithm's visited color
    }
}


 // @brief Draws a single maze (either base or from a solver) to the window
void drawMaze(sf::RenderWindow& window,
              const std::vector<std::string>& grid, // Data Structure: Using std::vector<std::string> as the 2D grid
              sf::Font& font,
              const std::string& title,
              sf::Color traversalColor) 
{
    sf::Text titleText(title, font, FONT_SIZE);
    titleText.setPosition(PADDING, PADDING / 2.0f);
    titleText.setFillColor(sf::Color::White);
    window.draw(titleText);

    // Draw maze
    sf::RectangleShape cellRect(sf::Vector2f(CELL_SIZE, CELL_SIZE));
    float gridBaseY = PADDING + TITLE_HEIGHT;
    
    for (size_t r = 0; r < grid.size(); ++r) {
        for (size_t c = 0; c < grid[r].size(); ++c) {
            cellRect.setPosition(PADDING + c * CELL_SIZE, gridBaseY + r * CELL_SIZE);
            
            char cellType = grid[r][c];
            cellRect.setFillColor(getCellColor(cellType, traversalColor));
            
            window.draw(cellRect);
        }
    }
}



 // @brief Helper function to create a solver by its index
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
    // Base maze
    const int R = 31, C = 51; 
    Maze baseMaze(R, C);
    Maze mazeCopy = baseMaze; 

    // All Algorithms implemented
    // Data Structure: std::vector for ordered lists of titles and colors 
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

    // Data Structure: std::map to store stats, keyed by algorithm title (string) 
    std::map<std::string, AlgoStats> results;
    int shortestPath = std::numeric_limits<int>::max();

    // SFML Window Setup
    const float mazeWidth = baseMaze.getCols() * CELL_SIZE;
    const float mazeHeight = baseMaze.getRows() * CELL_SIZE;
    unsigned int windowWidth = (unsigned int)(mazeWidth + PADDING * 2);
    unsigned int windowHeight = (unsigned int)(mazeHeight + PADDING * 2 + TITLE_HEIGHT);

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Maze Solver Visualizer");

    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Error: Could not load 'arial.ttf'.\n";
        return -1;
    }
    
    sf::Text instructionText("Press [Space] to start next algorithm", font, 16);
    instructionText.setFillColor(sf::Color(255, 255, 255, 150)); 
    instructionText.setPosition(PADDING, windowHeight - PADDING / 1.5f);

    VizState state = VizState::Starting;
    sf::Clock stepClock;
    //  Data Structure: std::unique_ptr to hold the currently active solver
    std::unique_ptr<Solver> currentSolver = nullptr;

    // Get the base grid once for the start screen
    auto baseGrid = baseMaze.grid; 
    auto baseStart = baseMaze.getStart();
    auto baseGoal = baseMaze.getGoal();
    baseGrid[baseStart.first][baseStart.second] = 'S';
    baseGrid[baseGoal.first][baseGoal.second] = 'E';


    // Main loop
    while (window.isOpen()) {
        
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                
                if (state == VizState::Starting) {
                    // Start the first algorithm 
                    mazeCopy = baseMaze; // Refresh the maze
                    currentSolver = createSolver(currentAlgoIndex, mazeCopy);
                    state = VizState::Running;
                    stepClock.restart();
                } 
                else if (state == VizState::Paused) {
                    // Move to next screen
                    currentAlgoIndex++;
                    if (currentAlgoIndex >= titles.size()) {
                        state = VizState::ShowingResults;
                        currentSolver = nullptr; // Clear the solver
                    } 
                    else {
                        mazeCopy = baseMaze; // Refresh the maze
                        currentSolver = createSolver(currentAlgoIndex, mazeCopy);
                        state = VizState::Running;
                        stepClock.restart();
                    }
                }
                // Results screen
                else if (state == VizState::ShowingResults) {
                    currentAlgoIndex = 0;
                    results.clear(); // Clear the std::map
                    shortestPath = std::numeric_limits<int>::max();
                    state = VizState::Starting;
                }
            }
        }

        // Update Logic
        if (state == VizState::Running && currentSolver && !currentSolver->isFinished()) {
            if (stepClock.getElapsedTime() > TIME_PER_STEP) {
                stepClock.restart();
                currentSolver->step();
                
                // When it finishes, change state
                if (currentSolver->isFinished()) {
                    state = VizState::Paused;
                    stepClock.restart();

                    // Collect stats for the algorithm that just finished
                    AlgoStats stats;
                    stats.nodesExplored = currentSolver->getNodesExplored();
                    stats.pathLength = currentSolver->getPathLength();
                    stats.timeTakenMs = currentSolver->getTimeTaken().asMilliseconds(); 
                    stats.pathFound = currentSolver->isPathFound();
                    
                    results[titles[currentAlgoIndex]] = stats; 

                    // Update "true" shortest path from complete algorithms
                    if (stats.pathFound && (titles[currentAlgoIndex].find("BFS") != std::string::npos || 
                                           titles[currentAlgoIndex].find("A*") != std::string::npos || 
                                           titles[currentAlgoIndex].find("Dijkstra") != std::string::npos)) 
                    {
                        if (stats.pathLength < shortestPath) {
                            shortestPath = stats.pathLength;
                        }
                    }
                }
            }
        }
        
        window.clear(sf::Color(20, 20, 20));

        if (state == VizState::Starting) {
            // Draw the base maze
            drawMaze(window, baseGrid, font, "Base Maze (Press Space)", sf::Color::Transparent);
        } 
        else if (currentSolver) { 
            // Get the solver's current grid (likely a std::vector<std::string>)
            auto grid = currentSolver->getGrid();
            // Add Start/End back in
            grid[baseStart.first][baseStart.second] = 'S';
            grid[baseGoal.first][baseGoal.second] = 'E';

            // Draw the solver's grid
            drawMaze(window, grid, font, titles[currentAlgoIndex], traversalColors[currentAlgoIndex]);
            
            if (state == VizState::Paused) {
                window.draw(instructionText);
            }
        }
        else if (state == VizState::ShowingResults) {
            window.clear(sf::Color::White); // White background for results

            float yPos = 50.0f;
            float xMargin = 50.0f;

            // Title
            sf::Text title("Final Results", font, 30);
            title.setFillColor(sf::Color::Black);
            title.setPosition(xMargin, yPos);
            window.draw(title);
            yPos += 60;

            // Shortest Distance
            std::string shortestText = "True Shortest Distance: " + 
                                       (shortestPath == std::numeric_limits<int>::max() ? "No Path Found" : std::to_string(shortestPath) + " nodes");
            sf::Text shortest(shortestText, font, 24);
            shortest.setFillColor(sf::Color(0, 0, 150)); // Dark blue
            shortest.setPosition(xMargin, yPos);
            window.draw(shortest);
            yPos += 70;
            sf::Text header("Algorithm", font, 20);
            header.setFillColor(sf::Color::Black);
            header.setStyle(sf::Text::Bold | sf::Text::Underlined);
            header.setPosition(xMargin, yPos);
            window.draw(header);

            header.setString("Time (ms)");
            header.setPosition(xMargin + 300, yPos);
            window.draw(header);

            header.setString("Nodes Explored");
            header.setPosition(xMargin + 500, yPos);
            window.draw(header);

            header.setString("Path Found (Length)");
            header.setPosition(xMargin + 750, yPos);
            window.draw(header);
            yPos += 40;

            // Loop through and draw results for each algorithm 
            for (const std::string& algoName : titles) {
                // Check if the key exists in our 'results' std::map
                if (results.find(algoName) == results.end()) continue; 
                
                const AlgoStats& stats = results.at(algoName);

                sf::Text lineText(algoName, font, 20);
                lineText.setFillColor(sf::Color::Black);
                lineText.setPosition(xMargin, yPos);
                window.draw(lineText);
                
                // Time
                char timeBuffer[32];
                std::snprintf(timeBuffer, sizeof(timeBuffer), "%.4f", stats.timeTakenMs);
                lineText.setString(timeBuffer);
                lineText.setPosition(xMargin + 300, yPos);
                window.draw(lineText);

                // Nodes Explored
                lineText.setString(std::to_string(stats.nodesExplored));
                lineText.setPosition(xMargin + 500, yPos);
                window.draw(lineText);

                // Path Found (Length)
                std::string pathStr = stats.pathFound ? std::to_string(stats.pathLength) + " nodes" : "No Path";
                lineText.setString(pathStr);
                lineText.setFillColor(stats.pathFound ? sf::Color(0, 100, 0) : sf::Color::Red);
                lineText.setPosition(xMargin + 750, yPos);
                window.draw(lineText);

                yPos += 35; // Next line
            }
            
            // Reset instructions
            sf::Text resetText("Press [Space] to restart", font, 22);
            resetText.setFillColor(sf::Color::Black);
            resetText.setPosition(xMargin, windowHeight - 60);
            window.draw(resetText);
        }

        window.display();
    }

    return 0;
}