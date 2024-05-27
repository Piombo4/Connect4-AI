#ifndef HEURISTIC
#define HEURISTIC
#include "Board.hpp"

class Board;
using namespace std;

const int KILLER_MOVES_SLOTS = 2;
const int KILLER_DEPTH = 15;

class Heuristic
{
public:
    Heuristic() : killerMoves(KILLER_DEPTH, vector<int>(KILLER_MOVES_SLOTS)) {}

    void store_killer_moves(int move, int depth)
    {

        for (int i = KILLER_MOVES_SLOTS - 2; i >= 0; i--)
        {
            killerMoves[depth][i + 1] = killerMoves[depth][i];
        }
        killerMoves[depth][0] = move;
    }
    bool isKillerMove(int move, int depth){
        for (int slot = 0; slot < killerMoves[depth].size(); slot++)
        {
            if(move == killerMoves[depth][slot]){
                return true;
            }
        }
        return false;
    }
    /*
     * Evaluates the position of the board
     * @param board - the board to evaluate
     * @param currentPlayer - the player who has to move
     * @return - a score based on how good or bad the position is
     */
    int evaluateBoard(vector<vector<int>> &board, int currentPlayer)
    {

        int score = 0;
        int last_column = NUM_COL - 3;
        int last_row = NUM_ROW - 3;
        int countPlayer = 0;
        int countOpponent = 0;
        int empty = 0;
        for (int row = 0; row < NUM_ROW; row++)
        {
            int countPlayer = 0;
            int countOpponent = 0;
            int empty = 0;
            for (int col = 0; col < last_column; col++)
            {

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
                countPlayer = 0;
                countOpponent = 0;
                empty = 0;
            }
        }

        for (int col = 0; col < NUM_COL; col++)
        {
            int countPlayer = 0;
            int countOpponent = 0;
            int empty = 0;
            for (int row = 0; row < last_row; row++)
            {

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
                countPlayer = 0;
                countOpponent = 0;
                empty = 0;
            }
        }

        for (int c = 0; c < NUM_COL - 3; c++)
        {
            int countPlayer = 0;
            int countOpponent = 0;
            int empty = 0;
            for (int r = 3; r < NUM_ROW; r++)
            {

                for (int i = 0; i < 4; i++)
                {
                    if (board[r - i][c + i] == currentPlayer)
                    {
                        countPlayer++;
                    }
                    else if (board[r - i][c + i] != EMPTY)
                    {
                        countOpponent++;
                    }
                    else
                    {
                        empty++;
                    }
                }

                score += evaluateChunk(countPlayer, countOpponent, empty);
                countPlayer = 0;
                countOpponent = 0;
                empty = 0;
            }
        }
        for (int c = 0; c < NUM_COL - 3; c++)
        {
            int countPlayer = 0;
            int countOpponent = 0;
            int empty = 0;
            for (int r = 0; r < NUM_ROW - 3; r++)
            {

                for (int i = 0; i < 4; i++)
                {
                    if (board[r + i][c + i] == currentPlayer)
                    {
                        countPlayer++;
                    }
                    else if (board[r + i][c + i] != EMPTY)
                    {
                        countOpponent++;
                    }
                    else
                    {
                        empty++;
                    }
                }

                score += evaluateChunk(countPlayer, countOpponent, empty);
                countPlayer = 0;
                countOpponent = 0;
                empty = 0;
            }
        }
        return score;
    }

private:
    vector<vector<int>> killerMoves;
    /*
     *   Heuristic function
     *   If possible it chooses the winning move. It prefers to block rather than placing consecutive pieces
     *   @param countPlayer - The number of consecutive pieces of the player
     *   @param countOpponent -The number of consecutive pieces of the opponent
     *   @param empty -  The number of empty spaces
     *   @return - the score of the move
     */
    int evaluateChunk(int countPlayer, int countOpponent, int empty)
    {

        if (countPlayer == 4) // winning move
        {
            return 10001;
        }
        else if (countPlayer == 3 && countOpponent == 0) // three consecutive
        {
            return 1000;
        }
        else if (countPlayer == 2 && countOpponent == 0) // two consecutive
        {
            return 100;
        }
        else if (countOpponent == 2 && countPlayer == 0) // two consecutive of the opponent
        {
            return -101;
        }
        else if (countOpponent == 3 && countPlayer == 0) // three consecutive of the opponent
        {
            return -1001;
        }
        else if (countOpponent == 4) // winning move for the opponent
        {
            return -10000;
        }
        return 0;
    }
};
#endif