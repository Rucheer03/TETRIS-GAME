#include <iostream>
#include <vector>
#include <termios.h>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <fcntl.h>

using namespace std;

#define BLOCK "\u2588\u2588" // Double width for square blocks
// ANSI color codes
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_ORANGE  "\x1b[38;5;208m"
#define ANSI_COLOR_WHITE   "\x1b[37m"

int speed=300000;
int highest_score=0;
class Tetris {
private:
    struct Point {
        int x, y;
    };

    struct Tetromino {
        std::vector<Point> shape;
        char symbol;

        void Rotate() {
            for (auto &p : shape) {
                int temp = p.x;
                p.x = -p.y;
                p.y = temp;
                p.x++;
            }
        }
    };

    const int boardWidth = 10;
    const int boardHeight = 20;
    char board[20][10];
    bool gameOver;
    int score;
    Tetromino currentPiece;
    Point position;
    termios oldt;
    void InitializeBoard() {
        for (int i = 0; i < boardHeight; i++) {
            for (int j = 0; j < boardWidth; j++) {
                board[i][j] = ' ';
            }
        }
    }

    Tetromino CreateTetromino() {
        Tetromino t;
        int type = rand() % 7;
        switch (type) {
            case 0: 
                t.shape = { {0, 0}, {1, 0}, {0, 1}, {1, 1} };
                t.symbol = 'O';
                break; 
            case 1: 
                t.shape = { {0, 0}, {1, 0}, {2, 0}, {3, 0} };
                t.symbol = 'I';
                break; 
            case 2: 
                t.shape = { {0, 0}, {1, 0}, {2, 0}, {1, 1} };
                t.symbol = 'T';
                break; 
            case 3: 
                t.shape = { {0, 0}, {1, 0}, {1, 1}, {2, 1} };
                t.symbol = 'S';
                break; 
            case 4: 
                t.shape = { {0, 1}, {1, 1}, {1, 0}, {2, 0} };
                t.symbol = 'Z';
                break; 
            case 5: 
                t.shape = { {0, 0}, {0, 1}, {1, 1}, {2, 1} };
                t.symbol = 'L';
                break; 
            case 6: 
                t.shape = { {0, 1}, {1, 1}, {2, 1}, {2, 0} };
                t.symbol = 'J';
                break; 
        }
        return t;
    }

    bool CheckCollision() {
        for (auto &p : currentPiece.shape) {
            int x = position.x + p.x;
            int y = position.y + p.y;

            if (x < 0 || x >= boardWidth || y >= boardHeight || (y >= 0 && board[y][x] != ' ')) {
                return true;
            }
        }
        return false;
    }

    void PlacePiece() {
        for (auto &p : currentPiece.shape) {
            int x = position.x + p.x;
            int y = position.y + p.y;
            if (y >= 1) {
                board[y][x] = currentPiece.symbol;
            }
            else {
                gameOver = true;
            }
        }
        position = {boardWidth/2,-1};
        currentPiece = CreateTetromino();
    }

    void ClearBoard() {
        for (auto &p : currentPiece.shape) {
            int x = position.x + p.x;
            int y = position.y + p.y;
            if (y >= 0) {
                board[y][x] = ' ';
            }
        }
    }

    bool kbhit() {
        struct termios newt;
        int ch;
        int oldf;

        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        oldf = fcntl(STDIN_FILENO, F_GETFL);
        fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

        ch = getchar();

        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        fcntl(STDIN_FILENO, F_SETFL, oldf);

        if (ch != EOF) {
            ungetc(ch, stdin);
            return true;
        }

        return false;
    }

    void RemoveFullLines() {
        for (int i = 0; i < boardHeight; i++) {
            bool isFull = true;
            for (int j = 0; j < boardWidth; j++) {
                if (board[i][j] == ' ') {
                    isFull = false;
                    break;
                }
            }

            if (isFull) {
                for (int k = i; k > 0; k--) {
                    for (int j = 0; j < boardWidth; j++) {
                        board[k][j] = board[k-1][j];
                    }
                }

                for (int j = 0; j < boardWidth; j++) {
                    board[0][j] = ' ';
                }

                score += 100;
                if(score%100==0&&score!=0){
                    speed=max(speed-20000,50000);
                }
            }
        }
    }

public:
    Tetris() : gameOver(false), score(0), position({boardWidth/2, 0}) {
        srand(static_cast<unsigned>(time(0)));
        InitializeBoard();
        currentPiece = CreateTetromino();
        disableEcho();
    }

    ~Tetris() {
        enableEcho();
    }

    void disableEcho() {
        struct termios newt;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    }

    void enableEcho() {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    }

    void DrawBoard() {
        std::cout << "\033[2J\033[H";
        std::cout << "\033[2J\033[H";
        cout << "\033[1;31m--------------------------------------\033[0m" << endl;
        cout << "\033[1;32m             TETRIS GAME\033[0m" << endl;
        cout << "\033[1;31m--------------------------------------\033[0m" << endl;
        cout << "Controls: '->' - Right | '<-' - LEFT | '↑' - Rotate | '↓' - SoftFall" << endl;
        cout << "Controls: 'SpaceBar' - HardFall | 'AnyOtherKey - Quit'"<<endl;
        cout << "\033[1;35m--------------------------------------\033[0m" << endl;
        std::cout << "Score: " << score << endl;
        std::cout << ANSI_COLOR_WHITE;
        for (int x = 0; x < boardWidth + 2; ++x) std::cout << BLOCK;
        std::cout << ANSI_COLOR_RESET << std::endl;
        for (int i = 0; i < boardHeight; i++) {
            std::cout << BLOCK;
            for (int j = 0; j < boardWidth; j++) {
                if (board[i][j] != ' ') {
                  if(board[i][j]=='I'){
                        std::cout << ANSI_COLOR_BLUE;   
                    }
                    else if(board[i][j]=='O'){
                        std::cout << ANSI_COLOR_CYAN;
                    }
                    else if(board[i][j]=='T'){
                        std::cout << ANSI_COLOR_YELLOW;
                    }
                    else if(board[i][j]=='S'){
                        std::cout << ANSI_COLOR_MAGENTA;
                    }
                    else if(board[i][j]=='Z'){
                        std::cout << ANSI_COLOR_GREEN ;
                    }
                    else if(board[i][j]=='L'){
                        std::cout <<ANSI_COLOR_RED ;
                    }
                    else if(board[i][j]=='J'){
                        std::cout <<ANSI_COLOR_ORANGE;
                    }
                    std::cout << BLOCK;
                    std::cout << ANSI_COLOR_RESET ;

                } else {
                    std::cout << board[i][j] << " ";
                }
            }

            std::cout << BLOCK << std::endl;
        }
        for (int x = 0; x < boardWidth + 2; ++x) std::cout << BLOCK;
        std::cout << ANSI_COLOR_RESET << std::endl;

    }

    void Input() {
        if (kbhit()) {
            char key = getchar();
            if (key == '\033') {
                getchar();
                switch (getchar()) {
                    case 'A':  // Rotate piece
                        currentPiece.Rotate();
                        if (CheckCollision()) {
                            for (int i = 0; i < 3; i++) currentPiece.Rotate();
                        }
                        break;
                    case 'B':  // Move down
                        position.y++;
                        break;
                    case 'C':  // Move right
                        position.x++;
                        if (CheckCollision()) position.x--;
                        break;
                    case 'D':  // Move left
                        position.x--;
                        if (CheckCollision()) position.x++;
                        break;
                }
            } else if (key == ' ') {  // Spacebar (hard drop)
                while (!CheckCollision()) {
                    position.y++;
                }
                position.y--;  // Move back one step before placing

                PlacePiece();
                RemoveFullLines();
            } else {  // Escape key (exit game)
                gameOver = true;
            }
        }
    }
    

    void Update() {
        for (auto &p : currentPiece.shape) {
            int x = position.x + p.x;
            int y = position.y + p.y;
            if (y == 0 && board[y][x] != ' ') {
                gameOver = true;
                return;
            }
        }

        if (!gameOver) {
            position.y++;
            if (CheckCollision()) {
                position.y--;
                PlacePiece();
                RemoveFullLines();
            }
        }
    }

    void DrawPiece() {
        for (auto &p : currentPiece.shape) {
            int x = position.x + p.x;
            int y = position.y + p.y;
            if (y >= 0) {
                board[y][x] = currentPiece.symbol;
            }
        }
    }

    void Run() {
        cout << "\033[2J\033[H"; // Clear screen
        std::cout << "\033[2J\033[H";
        cout << "\033[1;31m--------------------------------------\033[0m" << endl;
        cout << "\033[1;32m             TETRIS GAME\033[0m" << endl;
        cout << "\033[1;31m--------------------------------------\033[0m" << endl;
        cout << "Controls: '->' - Right | '<-' - LEFT | '↑' - Rotate | '↓' - SoftFall" << endl;
        cout << "Controls: 'SpaceBar' - HardFall | 'AnyOtherKey - Quit'"<<endl;
        cout << "\033[1;35m--------------------------------------\033[0m" << endl;
        int choice;
        cout << "\nSelect Difficulty Level:\n";
        cout << " 1. Easy   (Slow speed)\n";
        cout << " 2. Medium (Normal speed)\n";
        cout << " 3. Hard   (Fast speed)\n";
        cout << "Enter choice (1-3): ";

        choice = getchar() - '0';  // Convert char to int

        switch (choice) {
            case 1: speed = 400000; break; // Slow speed
            case 2: speed = 300000; break; // Normal speed
            case 3: speed = 200000; break; // Fast speed
            default: speed = 300000; break; // Default to Medium
        }

        while (!gameOver) {
            ClearBoard();
            Input();
            Update();
            DrawPiece();
            DrawBoard();
            
            usleep(speed);
        }
        if(score>highest_score){
            highest_score=score;
        }
        cout << "Game Over! Final Score: " << score << endl;
        cout << "Highest Score: " << highest_score << endl;
        char restart;
        cout << "Do you want to restart the game? (y/n): ";
        cin >> restart;

        if (restart == 'y' || restart == 'Y') {
            // Restart the game by resetting necessary variables
            score = 0;
            speed = 300000;
            gameOver = false;
            InitializeBoard();
            currentPiece = CreateTetromino();
            Run();  // Recursively restart the game
        }
    
    }
};

int main() {
    Tetris game;
    game.Run();
    return 0;
}
