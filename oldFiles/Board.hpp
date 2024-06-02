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
    vector<vector<int>> board;
    vector<int> possibleMoves = {4, 3, 5, 2, 1, 6, 0};
    Board() : board(NUM_ROW, vector<int>(NUM_COL)) {}
    /*
     *   Initialize the board
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
     *   Draw the board
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

    bool canPlay(int col)
    {

        if (board[NUM_ROW - 1][col] == C::EMPTY)
        {
            return true;
        }

        return false;
    }

    bool checkDraw()
    {
        for (int i = 0; i < possibleMoves.size(); i++)
        {
            if (canPlay(possibleMoves[i]))
            {
                return false;
            }
        }
        return true;
    }
    /*
     *   Add a move to the board
     *   @param move - the move to add (it represents a column)
     *   @param currentPlayer - the player who made the move
     *   @return - the row in which the move was added
     */
    int add_move(int move, int currentPlayer)
    {
        for (int row = 0; row < NUM_ROW; row++)
        {
            if (board[row][move] == C::EMPTY)
            {
                board[row][move] = currentPlayer;
                return row;
            }
        }
        return -1;
    }
    /*
     *   Check whether the current position is a winning position
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
    /*
     *   Generates all the possible moves from a certain position
     *   @return - the columns where a piece placement is possible
     */
    vector<int> getMoves()
    {
        return possibleMoves;
    }

    /*
     *   @return - the board
     */
    const vector<vector<int>> &getBoard() const
    {
        return board;
    }
    /*
     *   Add move in a particular cell
     *   @param row - the row
     *   @param col - the column
     */
    void undoMove(int row, int col)
    {
        board[row][col] = C::EMPTY;
    }

private:
};

#endif