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
    vector<int> possibleMoves = {4, 3, 5, 2, 1, 6, 0};
    vector<int> orderedMoves;

public:
    Engine(Heuristic h) : heuristic(h) {}
    bool checkDraw(Board &board)
    {
        for (int i = 0; i < possibleMoves.size(); i++)
        {
            if (board.canPlay(possibleMoves[i]))
            {
                return false;
            }
        }
        return true;
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
        /*orderedMoves = possibleMoves;
        for (int slot = 0; slot < heuristic.killerMoves[depth].size(); slot++)
        {
            int killerMove = heuristic.killerMoves[depth][slot];

            for (int i = 0; i < orderedMoves.size(); i++)
            {
                if (orderedMoves[i] == killerMove)
                {
                    orderedMoves.erase(orderedMoves.begin() + i);          // Rimuovi l'elemento
                    orderedMoves.insert(orderedMoves.begin(), killerMove); // Inserisci l'elemento all'inizio
                    break;
                }
            }
        }*/ 
        if (depth == 0)
        {
            return {heuristic.evaluateBoard(board.board, AI), -1};
        }
        else if (checkDraw(board))
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
            for (int col : possibleMoves)
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
            for (int col : possibleMoves)
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