/*
Name: Muhammad Hamza Ijaz Tung
Roll No: 21I-1562
Section: DS-B
Assigment: 02
*/

// Game: The Unseen Journey Quest 

#include <iostream>
#include <cstdlib>  // For random
#include <ctime>    
#include <cstdio>   // For getchar() to capture special inputs
using namespace std;

class Node {
public:
    bool isKey, isExit, isCoin, isBomb;
    Node() : isKey(false), isExit(false), isCoin(false), isBomb(false) {}
};

// Player Class: Manages player's current state
class Player {
public:
    int row, col;   // player's position 
    int moves;
    bool hasKey;
    int score;
    int prevRow, prevCol; // previous position for undo to work

    Player(int startRow, int startCol)
        : row(startRow), col(startCol), moves(50), hasKey(false), score(0) {}
};

// Maze Class: Manages the entire game logic, grid, player movement, and object placements
class Maze {
private:
    Node grid[20][20]; // fixed-size grid for simplicity
    Player player;
    int rows, cols;

public:
    Maze(int difficulty) : player(0, 0) {
        setupDifficulty(difficulty);
        placeObjects();
    }

    void setupDifficulty(int difficulty) {
        if (difficulty == 1) {
            rows = 10; cols = 10;
            player.moves = 50;
        } 
        else if (difficulty == 2) {
            rows = 15; cols = 15;
            player.moves = 75;
        } 
        else {
            rows = 20; cols = 20;
            player.moves = 100;
        }
    }

    void placeObjects() {
        placeObject("Key", &grid[rand() % rows][rand() % cols].isKey);
        placeObject("Exit", &grid[rand() % rows][rand() % cols].isExit);
        for (int i = 0; i < 3; i++) {
            placeObject("Bomb", &grid[rand() % rows][rand() % cols].isBomb);
        }
        for (int i = 0; i < 5; i++) {
            placeObject("Coin", &grid[rand() % rows][rand() % cols].isCoin);
        }
    }

    void placeObject(const char* type, bool* attribute) {
        while (*attribute) {
            int randomRow = rand() % rows;
            int randomCol = rand() % cols;
            attribute = &grid[randomRow][randomCol].isKey; // changes according to the type
        }
        *attribute = true; // place the object
    }

    void displayMaze() {
        system("clear"); // clears the screen 
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (player.row == i && player.col == j) {
                    cout << "P ";  // Display Player as 'P'
                } 
                else if (grid[i][j].isKey) {
                    cout << "K ";  // Display Key as 'K'
                } 
                else if (grid[i][j].isExit) {
                    cout << "E ";  // Display Exit as 'E'
                } 
                else if (grid[i][j].isCoin) {
                    cout << "C ";  // Display Coin as 'C'
                } 
                else if (grid[i][j].isBomb) {
                    cout << "B ";  // Display Bomb as 'B'
                } 
                else {
                    cout << ". ";  // to show empty cell
                }
            }
            cout << endl;
        }
        cout << "Score: " << player.score << " | Moves left: " << player.moves << endl; // display score and number of moves left
    }

    void handleInput() {
        char ch1, ch2;
        int newRow = player.row;
        int newCol = player.col;
        // stores previous position
        player.prevRow = player.row;
        player.prevCol = player.col;
        // reads the first character
        ch1 = getchar();
        
        // Check if the first character indicates an escape sequence
        if (ch1 == 27) { // Escape character
            ch2 = getchar(); // Read the next character
            if (ch2 == '[') { // Arrow keys
                ch1 = getchar(); // Read the arrow direction
                switch(ch1) {
                    case 'A': newRow--; break; // Up arrow
                    case 'B': newRow++; break; // Down arrow
                    case 'D': newCol--; break; // Left arrow
                    case 'C': newCol++; break; // Right arrow
                }
            }
        } 
        else {
            switch(ch1) {
                case 'u': // undo move
                    player.row = player.prevRow;
                    player.col = player.prevCol;
                    return; 
                default: cout << "Invalid input. Use arrow keys or 'u' for undo." << endl; return;
            }
        }

        // Check boundaries
        if (newRow >= 0 && newRow < rows && newCol >= 0 && newCol < cols) {
            player.row = newRow;
            player.col = newCol;
            player.moves--;
            // Check for objects
            if (grid[player.row][player.col].isKey) {
                player.hasKey = true;
                grid[player.row][player.col].isKey = false; // collect the key
            }
            if (grid[player.row][player.col].isCoin) {
                player.score += 10; // increase score for collecting a coin
                grid[player.row][player.col].isCoin = false; // collect the coin
            }
            if (grid[player.row][player.col].isBomb) {
                cout << "Game Over! You hit a bomb!" << endl; 
                exit(0); 
            }
            if (grid[player.row][player.col].isExit && player.hasKey) {
                cout << "You have exited the maze! You win! Your score: " << player.score << endl; 
                exit(0); 
            }
        }
    }

    void startGame() {
        while (player.moves > 0) {
            displayMaze();
            handleInput();
        }
        // When moves are exhausted
        cout << "Game Over! No moves left." << endl;
    }
};

int main() {
    srand(static_cast<unsigned int>(time(0))); // random number generator
    int difficulty;
    cout << "Choose difficulty level (1: Easy, 2: Medium, 3: Hard): ";
    cin >> difficulty;
    cin.ignore(); 
    Maze mazeGame(difficulty);
    mazeGame.startGame();

    return 0;
}