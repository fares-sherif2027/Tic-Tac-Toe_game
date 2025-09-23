#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

enum class Difficulty
{
    Easy,
    Medium,
    Hard
};

class Board
{

private:
    vector<vector<int>> grid;
    const int size;

public:
    Board(int size = 3);
    // TODO default constructor // hana

    void display() const;
    // TODO display the borad //malak

    bool makeMove(int row, int col, char symbol);
    // TODO boolean to indicate move success(symbol X/O) // hamody

    bool isValidMove(int row, int col) const;
    // TODO bool indicates if move is valid// fares

    bool checkWin(char symbol) const;
    // TODO bool check all win conditions // ramy

    bool isFull() const;
    // TODO check if all cells are full //hana

    char getCell(int row, int col) const;
    // TODO getter return the symbol at specific coordinate // malak

    void reset();
    // TODO clear all cells to empty state// hamody

    int getSize() const;
    // TODO  getter function returns dimension of the board // fares
};

class Player
{
private:
    string name;
    char symbol;

public:
    Player(const string &name, char symbol);
    // TODO parameterized constructor //hana

    virtual void getMove(int &row, int &col) = 0;
    // TODO pure virtual implement in derived classes

    string getName() const;
    // TODO getter of player name //hamody

    char getSymbol() const;
    // TODO getter return player's symbol //fares

    void setName(const string &name);
    // TODO setter of player's name // malak
};

class AIPlayer : public Player
{
private:
    Difficulty difficulty;

public:
    AIPlayer(const string &name, char symbol, Difficulty difficulty);
    // TODO constructor

    void getMove(int &row, int &col) override;
    // TODO AI move depending on difficulty/////// fares

    void setDifficulty(Difficulty newDifficulty); // ramy
    // TODO change AI difficulty settings

    void getRandomMOve(const Board &board, int &row, int &col) const;
    // TODO select random valid move for easy difficulty (can be used in getMove)//malak

    void getBestMove(Board &board, int &row, int &col) const;
    // TODO find optimal move usng minimax algorithm///////hana

    int evaluateBoard(const Board &board) const;
    // TODO evaluate board score(win/loss/draw)//////hamody
};

class Game
{
    Board board;
    Player *player1;
    Player *player2;
    Player *currentPlayer;
    Player *winner;
    Difficulty difficulty;
    bool isPvP; // flag: true = PvP, false = PvC

public:
    Game();
    // TODO default constructor

    void start();
    // TODO main game entry conrol overall flow  /// ramy

    void showMenu();
    // TODO display selection menu   //hana

    void setupPvP();
    // TODO configure player vs player mode  //fares

    void setupPvc(Difficulty difficulty);
    // TODO configure player vs computer mode and concern difficulty // hamody

    void switchPlayer();
    // TODO alternate current player with other players  //malak

    void handleHumanMove(Player &player);
    // TODO process human move and validate it // ramy

    void handleAIMove(AIPlayer aiPlayer); // hana
    // TODO execute AI move

    bool checkGameEnd();
    // TODO check win conditions and board status// fares

    void displayResult() const;
    // TODO show game outcome message // hamody

    void reset();
    // TODO reset game for new round// malak
};

int main()
{
    cout << "==============================\n";
    cout << "     Welcome to Tic-Tac-Toe   \n";
    cout << "==============================\n\n";

    Game game;    // create a new game object
    game.start(); // let the game handle menu, setup, loop

    cout << "\nThanks for playing!\n";
    return 0;
}

void AIPlayer::setDifficulty(Difficulty newDifficulty)
{
    this->difficulty = newDifficulty;
}

bool Board::checkWin(char symbol) const
{
    for (int i = 0; i < size; ++i)
    {
        bool rowWin = true;
        bool colWin = true;
        for (int j = 0; j < size; ++j)
        {
            if (grid[i][j] != symbol)
                rowWin = false;
            if (grid[j][i] != symbol)
                colWin = false;
        }
        if (rowWin || colWin)
            return true;
    }

    bool diag1Win = true;
    for (int i = 0; i < size; ++i)
    {
        if (grid[i][i] != symbol)
        {
            diag1Win = false;
            break;
        }
    }
    if (diag1Win)
        return true;

    bool diag2Win = true;
    for (int i = 0; i < size; ++i)
    {
        if (grid[i][size - 1 - i] != symbol)
        {
            diag2Win = false;
            break;
        }
    }
    if (diag2Win)
        return true;

    return false; 
}

void Game::start()
{
    showMenu();

    bool gameOver = false;
    while (!gameOver)
    {
        board.display();

        AIPlayer* aiPlayer = dynamic_cast<AIPlayer*>(currentPlayer);
        if (aiPlayer) {
            handleAIMove(*aiPlayer);
        } else {
            handleHumanMove(*currentPlayer);
        }

        gameOver = checkGameEnd();
        if (!gameOver)
        {
            switchPlayer();
        }
    }

    displayResult();
}
