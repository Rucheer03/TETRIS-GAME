# Tetris Game 🎮

### Created by:
- **Rucheer Patel**
- **Preet Patel**

---

## Introduction

The Tetris game is a classic puzzle game where players rotate and move falling tetrominoes (shapes made up of blocks) to fit them into empty spaces on a 2D grid. The goal is to form horizontal lines of blocks without gaps, which will then disappear, earning the player points. The game ends when the tetrominoes stack up to the top of the grid.

---

## Objective

The goal of the game is to control the falling tetrominoes, rotate them to fit into the available spaces, and complete as many lines as possible. Each time a line is filled, it disappears, and the player earns points. The game ends when there is no space left to place a new tetromino.

---

## Requirements

### Platform

- **macOS**.

### Compiler

- A C++ compiler that supports the C++11 standard or later:
  - **Clang** for macOS.

### Libraries

- **termios.h** for controlling terminal input/output.
- **unistd.h** for handling sleep and delays.

---

## How to Compile and Run

### On Mac

#### Using Clang:
1. Open the Terminal and navigate to the folder where the game file is saved.
2. Use the following command to compile the code:
   ```bash
   clang++ tetris_game.cpp -o tetris_game

## Key Controls

- **Arrow Keys** for controlling the tetromino:
  - **Up Arrow**: Rotate the tetromino.
  - **Left Arrow**: Move the tetromino left.
  - **Right Arrow**: Move the tetromino right.
  - **Down Arrow**: Move the tetromino down faster.
  - **Spacebar**: Hard drop (move tetromino to the bottom instantly).
- **Esc Key**: Exit the game.

## Display of the Game 
The game is displayed in the console using ASCII characters:
![img alt](https://github.com/Rucheer03/TETRIS-GAME/blob/main/tetris.jpeg)

## Key Features

- **Classic Tetris Gameplay**: Rotate and move falling tetrominoes to form complete lines.
- **Score Tracking**: The score is displayed at the top of the screen and increases every time the snake eats food. The score is updated in real-time, offering instant feedback to the player. The final score is shown at the end of the game.
- **Game Over Condition**:The game ends when the snake collides with the boundaries of the grid or its own body. At this point, the player is shown the final score and choose either want to play again or not.
- **Difficulty Adjustment**: As the score increases, the speed of falling tetrominoes increases.
- **Colorful Display**: Each tetromino has a distinct color for better visual representation.

## Data Structures Used

### 1. **Class: Point**
- **Attributes**:
  - `int x, y`: Coordinates of a point, representing a single block of a tetromino or a position on the grid.
  
### 2. **Class: Tetromino**
- **Attributes**:
  - `vector<Point> shape`: The shape of the tetromino, stored as a list of `Point` objects.
  - `char symbol`: The character used to represent the tetromino (like 'I', 'O', etc.).
  - `int color`: Color used to display the tetromino.

- **Methods**:
  - `void Rotate()`: Rotates the tetromino by 90 degrees.
  
### 3. **Class: Tetris**
- **Attributes**:
  - `char board[20][10]`: The game grid, with dimensions 20x10.
  - `Tetromino currentPiece`: The current falling tetromino.
  - `int score`: The player's score.
  - `bool gameOver`: A flag to indicate whether the game has ended.

- **Methods**:
  - `void InitializeBoard()`: Initializes the game grid.
  - `void DrawBoard()`: Draws the game grid and the current state of the tetrominoes.
  - `void Input()`: Handles user input for controlling the tetromino.
  - `void Update()`: Updates the game state, moves the tetromino down, checks for collisions.
  - `void PlacePiece()`: Places the tetromino on the board and starts a new piece.
  - `void RemoveFullLines()`: Removes completed lines and updates the score.
  - `void Run()`: The main game loop that repeatedly handles input, updates the game state, and draws the board.

## Code Structure and Flow

1. **Game Initialization**:
   - The game begins by initializing the game grid (20x10).
   - A random tetromino is selected to start falling.

2. **Game Loop**:
   - The game runs in a continuous loop where the following functions are repeatedly called:
     - **Input**: Detects user input (arrow keys, spacebar, etc.) for controlling the tetromino.
     - **Update**: Moves the current tetromino down the grid. If it collides with the bottom or another tetromino, it places the piece on the grid.
     - **Remove Full Lines**: After placing a piece, the game checks for any completed lines and removes them, updating the score.
     - **Draw**: The grid, tetrominoes, and score are drawn on the screen.

3. **Input Handling**:
   - **Arrow Keys**: Control the movement and rotation of the tetromino.
   - **Spacebar**: Causes the tetromino to instantly drop to the bottom.
   - **Esc Key**: Allows the player to exit the game.

4. **Collision Detection**:
   - The game checks for collisions between the current tetromino and the filled blocks on the grid.
   - If a collision occurs, the current piece is placed on the grid, and a new tetromino begins falling.

5. **Game Over Condition**:
   - The game ends when a new tetromino cannot be placed at the top of the grid due to the presence of other pieces.

6. **Game Speed**:
   - The game speed increases as the player progresses, making the tetrominoes fall faster as the score increases.


