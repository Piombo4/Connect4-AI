#ifndef ENGINE
#define ENGINE
#include <iostream>
#include <vector>
#include <algorithm>
#include "Board.hpp"
#include "KillerMoves.hpp"

using namespace std;
class Engine
{
private:
    KillerMoves killerMovesPlaceholder;
    vector<int> orderedMoves;

public:
    Engine(KillerMoves h) : killerMovesPlaceholder(h) {}
    /**
     * Applies the minimax algorithm with alpha-beta pruning to determine the best move.
     * @param board - the current board
     * @param depth - the current depth of the search tree
     * @param alpha - the best value that the maximizer can guarantee at the current level or above
     * @param beta  - the best value that the minimizer can guarantee at the current level or above
     * @param isMaximizing - true if it's AI turn, false otherwise
     * @return - a pair of integers where the first element is the score of the board for the current player,
     *           and the second element is the best move's position on the board.
     */
    pair<int, int> minimax(Board &board, int depth, int alpha, int beta, bool isMaximizing)
    {
        vector<int> moves = board.generate_moves();
        for (int slot = 0; slot < killerMovesPlaceholder.killerMoves[depth].size(); slot++)
        {
            int killerMove = killerMovesPlaceholder.killerMoves[depth][slot];

            for (int i = 0; i < moves.size(); i++)
            {
                if (moves[i] == killerMove)
                {
                    moves.erase(moves.begin() + i);
                    moves.insert(moves.begin(), killerMove);
                    break;
                }
            }
        }
        if (depth == 0)
        {
            return {board.evaluateBoard(C::AI), -1};
        }
        else if (board.n_moves() == Board::NUM_COL * Board::NUM_ROW)
        {
            return {0, -1};
        }
        else if (board.has_won(C::AI))
        {
            return {100000+depth, -1};
        }
        else if (board.has_won(C::PLAYER))
        {
            return {-(100000+depth), -1};
        }

        if (isMaximizing)
        {
            pair<int, int> bestScore = {INT_MIN, -1};

            for (int col : moves)
            {
                if (board.can_play(col))
                {

                    int row = board.make_move(col, isMaximizing ? C::AI : C::PLAYER);
                    int currentScore = minimax(board, depth - 1, alpha, beta, false).first;

                    if (currentScore > bestScore.first)
                    {
                        bestScore = {currentScore, col};
                    }
                    alpha = max(alpha, bestScore.first);

                    board.unmake_move(row, col);

                    if (alpha >= beta)
                    {
                        killerMovesPlaceholder.store_killer_moves(col, depth);
                        break;
                    }
                }
            }
            return bestScore;
        }
        else
        {
            pair<int, int> bestScore = {INT_MAX, -1};

            for (int col : moves)
            {
                if (board.can_play(col))
                {
                    int row = board.make_move(col, isMaximizing ? C::AI : C::PLAYER);
                    int currentScore = minimax(board, depth - 1, alpha, beta, true).first;

                    if (currentScore < bestScore.first)
                    {
                        bestScore = {currentScore, col};
                    }
                    beta = min(beta, bestScore.first);

                    board.unmake_move(row, col);

                    if (alpha >= beta)
                    {

                        killerMovesPlaceholder.store_killer_moves(col, depth);
                        break;
                    }
                }
            }
            return bestScore;
        }
    }
};

#endif