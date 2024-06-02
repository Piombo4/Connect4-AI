#ifndef ENGINE
#define ENGINE
#include <iostream>
#include <vector>
#include <algorithm>
#include "Board.hpp"
#include "Heuristic.hpp"

using namespace std;
class Engine
{
private:
    Heuristic heuristic;

    vector<int> orderedMoves;

public:
    Engine(Heuristic h) : heuristic(h) {}

    int negamax(Board &board, int depth, int alpha, int beta, int currentPlayer)
    {
        if (depth == 0)
        {
            return heuristic.evaluateBoard(board.board, currentPlayer);
        }
        else if (board.checkDraw())
        {
            return 0;
        }
        else if (board.winning_move(currentPlayer))
        {
            return currentPlayer * INT_MAX;
        }

        int bestValue = INT_MIN;
        vector<int> moves = board.getMoves();
        for (int i = 0; i < moves.size(); i++)
        {
            if (board.canPlay(moves[i]))
            {
                int row = board.add_move(moves[i], currentPlayer);
                int currValue = -negamax(board, depth - 1, -beta, -alpha, -currentPlayer);
                board.undoMove(row, moves[i]);
                bestValue = max(bestValue, currValue);
                alpha = max(alpha, bestValue);
                if (alpha >= beta)
                {
                    break;
                }
            }
        }
        return bestValue;
    }
    pair<int, int> negamaxHandler(Board &board, int depth, int alpha, int beta, int currentPlayer)
    {
        int bestValue = INT_MIN;
        int bestMove = -1;
        vector<int> moves = board.getMoves();
        for (int i = 0; i < moves.size(); i++)
        {
            if (board.canPlay(moves[i]))
            {
                int row = board.add_move(moves[i], currentPlayer);
                int currValue = -negamax(board, depth - 1, -beta, -alpha, -currentPlayer);
                board.undoMove(row, moves[i]);
                if (currValue > bestValue)
                {
                    bestValue = currValue;
                    bestMove = moves[i];
                }
                alpha = max(alpha, bestValue);
                if (alpha >= beta)
                {
                    break;
                }
            }
        }
        return {bestValue, bestMove};
    }

    /*
     * Applies the minimax algorithm with alpha-beta pruning to determine the best move.
     * @param board - the current board
     * @param depth - the current depth of the search tree
     * @param alpha - the best value that the maximizer can guarantee at the current level or above
     * @param beta  - the best value that the minimizer can guarantee at the current level or above
     * @param currentPlayer - the player whose move is being considered
     * @return - a pair of integers where the first element is the score of the board for the current player,
     *           and the second element is the best move's position on the board.
     */
    pair<int, int> minimax(Board &board, int depth, int alpha, int beta, int currentPlayer)
    {
         vector<int> moves = board.getMoves();
        /*for (int slot = 0; slot < heuristic.killerMoves[depth].size(); slot++)
        {
            int killerMove = heuristic.killerMoves[depth][slot];

            for (int i = 0; i < moves.size(); i++)
            {
                if (moves[i] == killerMove)
                {
                    moves.erase(moves.begin() + i);
                    moves.insert(moves.begin(), killerMove);
                    break;
                }
            }
        }*/
        if (depth == 0)
        {
            return {heuristic.evaluateBoard(board.board, AI), -1};
        }
        else if (board.checkDraw())
        {
            return {0, -1};
        }
       
        if (currentPlayer == C::AI)
        {
            pair<int, int> bestScore = {INT_MIN, -1};
            if (board.winning_move(C::PLAYER))
            {
                return bestScore;
            }

            for (int col : moves)
            {
                if (board.canPlay(col))
                {
                    int row = board.add_move(col, currentPlayer);
                    int currentScore = minimax(board, depth - 1, alpha, beta, -currentPlayer).first;

                    if (currentScore > bestScore.first)
                    {
                        bestScore = {currentScore, col};
                    }
                    alpha = max(alpha, bestScore.first);

                    board.undoMove(row, col);

                    if (alpha >= beta)
                    {
                        heuristic.store_killer_moves(col, depth);
                        break;
                    }
                }
            }
            return bestScore;
        }
        else
        {
            pair<int, int> bestScore = {INT_MAX, -1};
            if (board.winning_move(C::AI))
            {
                return bestScore;
            }
            for (int col : board.possibleMoves)
            {
                if (board.canPlay(col))
                {
                    int row = board.add_move(col, currentPlayer);
                    int currentScore = minimax(board, depth - 1, alpha, beta, -currentPlayer).first;

                    if (currentScore < bestScore.first)
                    {
                        bestScore = {currentScore, col};
                    }
                    beta = min(beta, bestScore.first);

                    board.undoMove(row, col);

                    if (alpha >= beta)
                    {

                        heuristic.store_killer_moves(col, depth);
                        break;
                    }
                }
            }
            return bestScore;
        }
    }
};

#endif