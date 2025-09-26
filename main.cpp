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
    Board(int s = 3) : size(s) // size is const
    // TODO default constructor // hana
    {
        grid.resize(size, vector<int>(size, 0));
    }
    void display() const;
    // TODO display the borad //malak

    bool makeMove(int row, int col, char symbol);
    // TODO boolean to indicate move success(symbol X/O) // hamody

    bool isValidMove(int row, int col) const
    {
        // TODO bool indicates if move is valid// fares
        if (row < 0 || row >= size || col < 0 || col >= size)
        {
            return false;
        }
        return grid[row][col] == 0; // indicates empty cell
    }

    bool checkWin(char symbol) const;
    // TODO bool check all win conditions // ramy

    bool isFull() const
    // TODO check if all cells are full //hana
    {
        for (int row = 0; row < size; row++)
        {
            for (int col = 0; col < size; col++)
            {
                if (grid[row][col] == 0)
                {
                    return false; // empty cell->not a full board
                }
            }
        }
        return true; // no empty cell->full board
    }
    char getCell(int row, int col) const;
    // TODO getter return the symbol at specific coordinate // malak

    void reset();
    // TODO clear all cells to empty state// hamody

    int getSize() const
    {
        // TODO  getter function returns dimension of the board // fares
        return size;
    }
};

class Player
{
private:
    string name;
    char symbol;

public:
    Player(const string &n, char s) : name(n), symbol(s)
    // TODO parameterized constructor //hana
    {
    }

    virtual void getMove(const Board &board, int &row, int &col) = 0;
    // TODO pure virtual implement in derived classes

    string getName() const;
    // TODO getter of player name //hamody

    char getSymbol() const
    {
        // TODO getter return player's symbol //fares
        return symbol;
    }

    void setName(const string &name);
    // TODO setter of player's name // malak
};

class HumanPlayer : public Player
{
public:
    HumanPlayer(const string &name, char symbol) : Player(name, symbol) {}

    void getMove(const Board &board, int &row, int &col) override
    {
        cout << "Enter row: ";
        cin >> row;
        cout << "Enter column: ";
        cin >> col;
    }
};

class AIPlayer : public Player
{
private:
    Difficulty difficulty;

public:
    AIPlayer(const string &name, char symbol, Difficulty difficulty);
    // TODO constructor

    void getMove(const Board &board, int &row, int &col) override
    {
        // TODO AI move depending on difficulty/////// fares
        switch (difficulty)
        {
        case Difficulty::Easy:
            getRandomMOve(board, row, col);
            break;
        case Difficulty::Medium:
            if (rand() % 2 == 0) // 0 logic work on easy mode, 1 logic work on hard mode, it's divided 50 50
                getRandomMOve(board, row, col);
            else
                getBestMove(board, row, col);
            break;
        case Difficulty::Hard:
            getBestMove(board, row, col);
            break;
        }
    }

    void setDifficulty(Difficulty newDifficulty); // ramy
    // TODO change AI difficulty settings

    void getRandomMOve(const Board &board, int &row, int &col) const;
    // TODO select random valid move for easy difficulty (can be used in getMove)//malak

    // minimax algorithm
    int minimax(Board board, bool isMaximizing, char aiSymbol, char humanSymbol) const
    {
        // Base cases
        if (board.checkWin(humanSymbol))
            return -1; // Human win
        if (board.checkWin(aiSymbol))
            return 1;
        if (board.isFull())
            return 0;

        // AI (max)
        if (isMaximizing)
        {
            int bestScore = -1000;

            for (int i = 0; i < board.getSize(); i++)
            {
                for (int j = 0; j < board.getSize(); j++)
                {
                    if (board.isValidMove(i, j))
                    {
                        Board temp = board;
                        temp.makeMove(i, j, aiSymbol);

                        int score = minimax(temp, false, aiSymbol, humanSymbol);
                        bestScore = max(bestScore, score);
                    }
                }
            }
            return bestScore;
        }

        // Human(mini)
        else
        {
            int bestScore = 1000;

            for (int i = 0; i < board.getSize(); i++)
            {
                for (int j = 0; j < board.getSize(); j++)
                {
                    if (board.isValidMove(i, j))
                    {
                        Board temp = board;
                        temp.makeMove(i, j, humanSymbol);

                        int score = minimax(temp, true, aiSymbol, humanSymbol);
                        bestScore = min(bestScore, score);
                    }
                }
            }
            return bestScore;
        }
    }

    void getBestMove(const Board &board, int &row, int &col) const
    // TODO find optimal move usng minimax algorithm///////hana
    {
        int bestScore = -1000;
        row = -1;
        col = -1;
        char aiSymbol = this->getSymbol();
        char humanSymbol = (aiSymbol == 'X') ? 'O' : 'X';

        for (int i = 0; i < board.getSize(); i++)
        {
            for (int j = 0; j < board.getSize(); j++)
            {
                if (board.isValidMove(i, j))
                {
                    Board temp = board;
                    temp.makeMove(i, j, aiSymbol);

                    int score = minimax(temp, true, aiSymbol, humanSymbol);

                    if (score > bestScore)
                    {
                        bestScore = score;
                        row = i;
                        col = j;
                    }
                }
            }
        }
    }

    int evaluateBoard(const Board &board) const;
    // TODO evaluate board score(win/loss/draw)//////hamody
};

class Game
{
private:
    Board board;
    AIPlayer aiPlayer;
    HumanPlayer *player1;
    HumanPlayer *player2;
    Player *currentPlayer;
    Player *winner;
    Difficulty difficulty;
    bool isPvP; // flag: true = PvP, false = PvC

public:
    Game();
    // TODO default constructor

    void start();
    // TODO main game entry conrol overall flow  /// ramy

    void showMenu()
    // TODO display selection menu   //hana
    {
        cout << "1. Player vs Player" << endl;
        cout << "2. Player vs Computer (Easy)" << endl;
        cout << "3. Player vs Computer (Hard)" << endl;
        cout << "4. Exit" << endl;
        cout << "==============================\n";
        cout << "Select game mode: " << endl;
        int choice;
        cin >> choice;

        switch (choice)
        {
        case 1:
            isPvP = true;
            setupPvP();
            break;

        case 2:
            isPvP = false;
            difficulty = Difficulty::Easy;
            setupPvc(difficulty);
            break;

        case 3:
            isPvP = false;
            difficulty = Difficulty::Hard;
            setupPvc(difficulty);
            break;

        case 4:
            cout << "Exiting gane. Goodbye!" << endl;
            exit(0);

        default:
            cout << "Invalid choice." << endl;
            break;
        }
    }
    void setupPvP()
    {
        // TODO configure player vs player mode  //fares
        if (isPvP)
        {
            cout << "PvP mode" << endl;
            string player1_Name;
            string player2_Name;
            char symbol1 = 'X';
            char symbol2 = 'O';
            cout << "Enter player1's name: ";
            cin >> player1_Name;
            cout << "Enter player2's name: ";
            cin >> player2_Name;
            player1 = new HumanPlayer(player1_Name, symbol1);
            player2 = new HumanPlayer(player2_Name, symbol2);
            currentPlayer = (currentPlayer == player1) ? player2 : player1;
        }
    }

    void setupPvc(Difficulty difficulty);
    // TODO configure player vs computer mode and concern difficulty // hamody

    void switchPlayer();
    // TODO alternate current player with other players  //malak

    void handleHumanMove(Player &player);
    // TODO process human move and validate it // ramy

    void handleAIMove(AIPlayer &aiPlayer) // hana
    // TODO execute AI move
    {
        int row, col;
        aiPlayer.getMove(board, row, col);

        if (board.makeMove(row, col, aiPlayer.getSymbol()))
        {
            cout << aiPlayer.getName() << "(AI)" << aiPlayer.getSymbol() << "at (" << row << " , " << col << " )" << endl;
        }
        else
        {
            cout << "Invalid move at (" << row << " , " << col << " )" << endl;
        }
    }

    bool
    checkGameEnd()
    {
        // TODO check win conditions and board status// fares
        if (board.checkWin('X') || board.checkWin('O'))
        {
            return true; // Someone won
        }
        if (board.isFull())
        {
            return true; // Draw
        }
        return false; // Game continues
    }

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