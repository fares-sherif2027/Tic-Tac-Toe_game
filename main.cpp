#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>
#include <cstdlib>
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
    // TODO display the borad malak
    void display() const
    {
        cout << "  ";
        for (int col = 0; col < size; col++)
        {
            cout << col << " ";
        }
        cout << endl;

        for (int row = 0; row < size; row++)
        {
            cout << row << " ";
            for (int col = 0; col < size; col++)
            {
                char cellChar = grid[row][col] == 0 ? '.' : grid[row][col];
                cout << cellChar << " ";
            }
            cout << endl;
        }
    }
    bool makeMove(int row, int col, char symbol)
    {
        // TODO boolean to indicate move success(symbol X/O) // hamody
        if (isValidMove(row, col))
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
        }
        return grid[row][col] == 0; // indicates empty cell
    }
    bool checkWin(char symbol) const
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
    // TODO getter return the symbol at specific coordinate // malak
    char getCell(int row, int col) const
    {
        if (row < 0 || row >= size || col < 0 || col >= size)
        {
            return '\0'; // invalid position
        }
        return grid[row][col];
    }

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
    Player(const string &n, char s) : name(n), symbol(s)
    // TODO parameterized constructor //hana
    {
    }

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
    // TODO setter of player's name // malak
    void setName(const string &name)
    {
        this->name = name;
    }
};

class HumanPlayer : public Player
{
public:
    HumanPlayer(const string &name, char symbol) : Player(name, symbol) {}

    void getMove(const Board &board, int &row, int &col) override
    {
        (void)board; // suppress unused parameter warning
        cout << "Enter row: ";
        cin >> row;
        cout << "Enter column: ";
        cin >> col;
    }

    int evaluateBoard(const Board &board) const
    {
        if (board.checkWin(getSymbol()))
            return +10; // this player wins
        else if (board.checkWin(getSymbol() == 'X' ? 'O' : 'X'))
            return -10; // opponent wins
        return 0;       // draw or ongoing
    }
};

class AIPlayer : public Player
{
private:
    Difficulty difficulty;

public:
    AIPlayer(const string &name, char symbol, Difficulty difficulty)
        : Player(name, symbol), difficulty(difficulty)
    {
    }

    void getMove(const Board &board, int &row, int &col) override
    {
        // TODO AI move depending on difficulty/////// fares
        switch (difficulty)
        {
        case Difficulty::Easy:
            getRandomMOve(board, row, col);
            break;
        case Difficulty::Medium:
            getMediumMove(board, row, col);
            break;
        case Difficulty::Hard:
            getBestMove(board, row, col);
            break;
        }
    }

    void setDifficulty(Difficulty newDifficulty)
    {
        this->difficulty = newDifficulty;
    }

    // TODO change AI difficulty settings
    // TODO select random valid move for easy difficulty (can be used in getMove)//malak
    void getRandomMOve(const Board &board, int &row, int &col) const
    {
        vector<pair<int, int>> availableMoves;

        for (int i = 0; i < board.getSize(); i++)
        {
            for (int j = 0; j < board.getSize(); j++)
            {
                if (board.isValidMove(i, j))
                {
                    availableMoves.push_back({i, j});
                }
            }
        }

        if (!availableMoves.empty())
        {
            int randomIndex = rand() % availableMoves.size();
            row = availableMoves[randomIndex].first;
            col = availableMoves[randomIndex].second;
        }
    }

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

                    // FIX: After AI's move, it's human's turn
                    int score = minimax(temp, false, aiSymbol, humanSymbol);

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

    // For medium mode: 50% random, 50% minimax
    void getMediumMove(const Board &board, int &row, int &col) const
    {
        if (rand() % 2 == 0)
        {
            getRandomMOve(board, row, col);
        }
        else
        {
            getBestMove(board, row, col);
        }
    }

    int evaluateBoard(const Board &board) const
    {
        // TODO evaluate board score(win/loss/draw)//////hamody
        if (board.checkWin(getSymbol()))
            return +10; // computer wins
        else if (board.checkWin(getSymbol() == 'X' ? 'O' : 'X'))
            return -10; // draw or still no winning
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
    Game()
        // TODO default constructor
        : board(3),
          aiPlayer("Computer", 'O', Difficulty::Easy),
          player1(nullptr),
          player2(nullptr),
          currentPlayer(nullptr),
          winner(nullptr),
          difficulty(Difficulty::Easy),
          isPvP(false)
    {
    }

    void start()
    {
        // TODO main game entry conrol overall flow  /// ramy

        showMenu();

        bool gameOver = false;
        while (!gameOver)
        {
            board.display();
            showBoardScore();

            AIPlayer *aiPlayer = dynamic_cast<AIPlayer *>(currentPlayer);
            if (aiPlayer)
            {
                handleAIMove(*aiPlayer);
            }
            else
            {
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

    void showMenu()
    // TODO display selection menu   //hana
    {
        cout << "1. Player vs Player" << endl;
        cout << "2. Player vs Computer (Easy)" << endl;
        cout << "3. Player vs Computer (Medium)" << endl;
        cout << "4. Player vs Computer (Hard)" << endl;
        cout << "5. Exit" << endl;
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
            difficulty = Difficulty::Medium;
            setupPvc(difficulty);
            break;

        case 4:
            isPvP = false;
            difficulty = Difficulty::Hard;
            setupPvc(difficulty);
            break;

        case 5:
            cout << "Exiting game. Goodbye!" << endl;
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
    // TODO alternate current player with other players  //malak
    void switchPlayer()
    {
        if (isPvP)
        {
            currentPlayer = (currentPlayer == player1) ? player2 : player1;
        }
        else
        {
            currentPlayer = (currentPlayer == player1) ? static_cast<Player *>(&aiPlayer) : player1;
        }
    }

    void handleHumanMove(Player &player)
    {
        // TODO process human move and validate it // ramy
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

    void displayResult() const
    {
        cout << "\n=== FINAL RESULT ===\n";
        if (board.checkWin('X'))
            cout << "Player with symbol X wins with score: " << player1->evaluateBoard(board) << endl;
        else if (board.checkWin('O'))
            cout << "Player with symbol O wins with score: " << player2->evaluateBoard(board) << endl;
        else
            cout << "It's a draw!\n";
        cout << "====================\n";
    }
    // TODO reset game for new round// malak
    void reset()
    {
        board.reset();
        currentPlayer = player1;
        winner = nullptr;
    }

    // Show board evaluation score for both players (PvP or PvC)
    void showBoardScore() const
    {
        if (isPvP && player1 && player2)
        {
            HumanPlayer temp1(player1->getName(), player1->getSymbol());
            HumanPlayer temp2(player2->getName(), player2->getSymbol());
            int score1 = temp1.evaluateBoard(board);
            int score2 = temp2.evaluateBoard(board);
            cout << "Current board evaluation score (" << player1->getName() << ": " << player1->getSymbol() << ") = " << score1 << endl;
            cout << "Current board evaluation score (" << player2->getName() << ": " << player2->getSymbol() << ") = " << score2 << endl;
        }
        else
        {
            int aiScore = aiPlayer.evaluateBoard(board);
            char humanSymbol = (aiPlayer.getSymbol() == 'X') ? 'O' : 'X';
            std::string humanName = player1 ? player1->getName() : "Human";
            HumanPlayer tempHuman(humanName, humanSymbol);
            int humanScore = tempHuman.evaluateBoard(board);
            cout << "Current board evaluation score (AI): " << aiScore << endl;
            cout << "Current board evaluation score (" << humanName << "): " << humanScore << endl;
        }
    }
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
