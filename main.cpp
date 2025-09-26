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

    bool makeMove(int row, int col, char symbol)
    {
    // TODO boolean to indicate move success(symbol X/O) // hamody
        if(isValidMove(row, col))
        {
            grid[row][col] = symbol; 
            return true;
        }
        return false;
    }
    bool isValidMove(int row, int col) const
    {
        // TODO bool indicates if move is valid// fares
        if (row < 0 || row >= size || col < 0 || col >= size)
        {
            return false;
            return grid[row][col] == 0; // indicates empty cell
        }
    }

    bool checkWin(char symbol) const;
    // TODO bool check all win conditions // ramy

    bool isFull() const;
    // TODO check if all cells are full //hana

    char getCell(int row, int col) const;
    // TODO getter return the symbol at specific coordinate // malak

    void reset()
    {
    // TODO clear all cells to empty state// hamody
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                grid[i][j] = 0; 
            }
        }
    }

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
    Player(const string &name, char symbol);
    // TODO parameterized constructor //hana

    virtual void getMove(const Board &board, int &row, int &col) = 0;
    // TODO pure virtual implement in derived classes

    string getName() const
    {
    // TODO getter of player name //hamody
        return name;
    }

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

    void getBestMove(const Board &board, int &row, int &col) const;
    // TODO find optimal move usng minimax algorithm///////hana

    int evaluateBoard(const Board &board) const
    {
    // TODO evaluate board score(win/loss/draw)//////hamody
        if (board.checkWin(getSymbol()))
            return +10; //computer wins 
        else if (board.checkWin(getSymbol() == 'X' ? 'O' : 'X'))
            return -10; // draw or still no winning ( i dont know what is the right logic yet :< )
        return 0;

    }
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

    void showMenu();
    // TODO display selection menu   //hana

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

    void setupPvc(Difficulty difficulty)
    {
    // TODO configure player vs computer mode and concern difficulty // hamody
        cout << "PvC mode" << endl;
        string playerName;
        char humanSymbol = 'X';
        char aiSymbol = 'O';

        cout << "Enter your name: ";
        cin >> playerName;

        player1 = new HumanPlayer(playerName, humanSymbol);
        aiPlayer = AIPlayer("Computer", aiSymbol, difficulty);

        currentPlayer = player1; 
        isPvP = false;

    }

    void switchPlayer();
    // TODO alternate current player with other players  //malak

    void handleHumanMove(Player &player);
    // TODO process human move and validate it // ramy

    void handleAIMove(AIPlayer aiPlayer); // hana
    // TODO execute AI move

    bool checkGameEnd()
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

    void displayResult() const
    {
    // TODO show game outcome message // hamody
        if (board.checkWin('X'))
        cout << "Player with symbol X wins\n";
        else if (board.checkWin('O'))
            cout << "Player with symbol O wins\n";
        else
            cout << "It's a draw!\n";
    }
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
void Game::handleHumanMove(Player &player)
{
    int row, col;
    bool validMove = false;
    while (!validMove)
    {
        cout << player.getName() << " (" << player.getSymbol() << "), enter your move (row and column): ";
        cin >> row >> col;

        if (cin.fail())
        {
            cout << "Invalid input. Please enter numbers only." << endl;
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            continue;
        }

        if (board.isValidMove(row, col))
        {
            board.makeMove(row, col, player.getSymbol());
            validMove = true;
        }
        else
        {
            cout << "Invalid move. Either the cell is taken or out of bounds. Try again." << endl;
        }
    }
}
