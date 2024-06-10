#ifndef BOARD
#define BOARD
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

const unsigned int NUM_COL = 7;
const unsigned int NUM_ROW = 6;

enum C : int16_t
{
    EMPTY = 0,
    PLAYER = -1,
    AI = 1
};

class Board
{
public:
    static const int NUM_COL = 7; // width of the board
    static const int NUM_ROW = 6; // height of the board

    Board() : board(NUM_ROW, vector<int>(NUM_COL)) { _counter = 0;}
    /*
     *   Initializes the board
     */
    void init_board()
    {
        for (int row = 0; row < NUM_ROW; row++)
        {
            for (int col = 0; col < NUM_COL; col++)
            {
                board[row][col] = C::EMPTY;
            }
        }
    }
    /*
     *   Draws the board
     */
    void draw_board()
    {

        for (int row = 0; row < NUM_ROW; row++)
        {
            cout << "|";
            for (int col = 0; col < NUM_COL; col++)
            {
                switch (board[NUM_ROW - row - 1][col])
                {
                case C::EMPTY:
                    cout << " ";
                    break;
                case C::PLAYER:
                    cout << "\e[33m";
                    cout << "O";
                    cout << "\e[0m";
                    break;
                case C::AI:
                    cout << "\e[31m";
                    cout << "O";
                    cout << "\e[0m";
                    break;
                default:
                    break;
                }
                cout << "|";
            }

            cout << endl;
        }

        for (int col = 0; col < NUM_COL; col++)
        { // write column numbers
            cout << " ";
            cout << col + 1;
        }
        cout << endl;
    }
    /**
     *   Checks if a move is playable i.e. if the column is not full
     *   @param col - the column to play the move
     *   @return true if the column is playable, false otherwise
     */
    bool can_play(int col)
    {

        if (board[NUM_ROW - 1][col] == C::EMPTY)
        {
            return true;
        }

        return false;
    }
    /**
     *   Adds a move to the board
     *   @param move - the move to add (it represents a column)
     *   @param currentPlayer - the player who made the move
     *   @return - the row in which the move was added
     */
    int make_move(int move, int currentPlayer)
    {
        for (int row = 0; row < NUM_ROW; row++)
        {
            if (board[row][move] == C::EMPTY)
            {
                board[row][move] = currentPlayer;
                _counter++;
                return row;
            }
        }
        return -1;
    }
    /**
     *   Checks whether the current position is a winning position
     *   @param currentPlayer - the player to check for the win
     *   @return - true if currentPlayer has won, false otherwise
     */
    bool winning_move(int currentPlayer)
    {
        int count;

        // check horizontal win
        for (int row = 0; row < NUM_ROW; row++)
        {

            for (int col = 0; col < NUM_COL - 3; col++)
            {
                count = 0;
                for (int i = 0; i < 4; i++)
                {
                    if (board[row][col + i] == currentPlayer)
                    {
                        count++;
                    }
                    else
                    {
                        break;
                    }
                    if (count == 4)
                    {
                        return true;
                    }
                }
            }
        }
        // check vertical win
        for (int col = 0; col < NUM_COL; col++)
        {

            for (int row = 0; row < NUM_ROW - 3; row++)
            {
                count = 0;
                for (int i = 0; i < 4; i++)
                {
                    if (board[row + i][col] == currentPlayer)
                    {
                        count++;
                    }
                    else
                    {

                        break;
                    }
                    if (count == 4)
                    {
                        return true;
                    }
                }
            }
        }

        // check diagonal win
        for (unsigned int c = 0; c < NUM_COL - 3; c++)
        {
            for (unsigned int r = 3; r < NUM_ROW; r++)
            {
                count = 0;
                for (int i = 0; i < 4; i++)
                {
                    if (board[r - i][c + i] == currentPlayer)
                    {
                        count++;
                    }
                    else
                    {

                        break;
                    }
                    if (count == 4)
                    {
                        return true;
                    }
                }
            }
        }
        for (unsigned int c = 0; c < NUM_COL - 3; c++)
        {
            for (unsigned int r = 0; r < NUM_ROW - 3; r++)
            {
                count = 0;
                for (int i = 0; i < 4; i++)
                {
                    if (board[r + i][c + i] == currentPlayer)
                    {
                        count++;
                    }
                    else
                    {

                        break;
                    }

                    if (count == 4)
                    {
                        return true;
                    }
                }
            }
        }
        return false;
    }
    /**
     *   Generates all the possible moves from a certain position
     *   @return - the columns where a piece placement is possible
     */
    vector<int> generate_moves()
    {
        return possibleMoves;
    }

    /**
     *   Unmakes a move
     *   @param row - the row
     *   @param col - the column
     */
    void unmake_move(int row, int col)
    {
        board[row][col] = C::EMPTY;
        _counter--;
    }

    /**
     *   @return the number of moves played until now 
     */
    int n_moves(){
        return _counter;
    }
    /**
     *   Evaluates the board positions for the currentPlayer.
     *   It counts how many pieces in a row the currentPlayer has in the four directions (horizontal, vertical, diagonal, antidiagonal) and gives a score based on that.
     *   Then it subtracts the score of the opponent (calculated in the same way) and returns the remaining value.
     *   @param currentPlayer - the current player
     *   @return a score based on the board position.
     */
    int evaluateBoard(int currentPlayer)
    {
        int score = 0;
        int last_column = NUM_COL - 3;
        int last_row = NUM_ROW - 3;
        int countPlayer = 0;
        int countOpponent = 0;
        int empty = 0;
        // Horizontal Check
        for (int row = 0; row < NUM_ROW; row++)
        {
            for (int col = 0; col < last_column; col++)
            {
                countPlayer = 0;
                countOpponent = 0;
                empty = 0;
                for (int i = 0; i < 4; i++)
                {
                    if (board[row][col + i] == currentPlayer)
                    {
                        countPlayer++;
                    }
                    else if (board[row][col + i] != EMPTY)
                    {
                        countOpponent++;
                    }
                    else
                    {
                        empty++;
                    }
                }
                score += evaluateChunk(countPlayer, countOpponent, empty);
            }
        }

        // Vertical Check
        for (int col = 0; col < NUM_COL; col++)
        {
            for (int row = 0; row < last_row; row++)
            {
                countPlayer = 0;
                countOpponent = 0;
                empty = 0;
                for (int i = 0; i < 4; i++)
                {
                    if (board[row + i][col] == currentPlayer)
                    {
                        countPlayer++;
                    }
                    else if (board[row + i][col] != EMPTY)
                    {
                        countOpponent++;
                    }
                    else
                    {
                        empty++;
                    }
                }
                score += evaluateChunk(countPlayer, countOpponent, empty);
            }
        }

        // Diagonal Check (Bottom-left to top-right)
        for (int col = 0; col < last_column; col++)
        {
            for (int row = 3; row < NUM_ROW; row++)
            {
                countPlayer = 0;
                countOpponent = 0;
                empty = 0;
                for (int i = 0; i < 4; i++)
                {
                    if (board[row - i][col + i] == currentPlayer)
                    {
                        countPlayer++;
                    }
                    else if (board[row - i][col + i] != EMPTY)
                    {
                        countOpponent++;
                    }
                    else
                    {
                        empty++;
                    }
                }
                score += evaluateChunk(countPlayer, countOpponent, empty);
            }
        }

        // Diagonal Check (Top-left to bottom-right)
        for (int col = 0; col < last_column; col++)
        {
            for (int row = 0; row < last_row; row++)
            {
                countPlayer = 0;
                countOpponent = 0;
                empty = 0;
                for (int i = 0; i < 4; i++)
                {
                    if (board[row + i][col + i] == currentPlayer)
                    {
                        countPlayer++;
                    }
                    else if (board[row + i][col + i] != EMPTY)
                    {
                        countOpponent++;
                    }
                    else
                    {
                        empty++;
                    }
                }
                score += evaluateChunk(countPlayer, countOpponent, empty);
            }
        }

        return score;
    }

private:
    int evaluateChunk(int countPlayer, int countOpponent, int empty)
    {
        int score = 0;
        if (countPlayer == 4)
            return 20000; // Winning move
        else if (countPlayer == 3 && empty == 1)
        {
            return 1000;
        }
        else if (countPlayer == 2 && empty == 2)
        {
            return 100;
        }
        else if (countOpponent == 2 && empty == 2)
        {
            return -100;
        }
        else if (countOpponent == 3 && empty == 1)
        {
            return -2000;
        }
        else if (countOpponent == 4)
        {
            return -10000;
        }
        return score;
    }
    vector<vector<int>> board;
    vector<int> possibleMoves = {4, 3, 5, 2, 1, 6, 0};
    int _counter;
};

#endif