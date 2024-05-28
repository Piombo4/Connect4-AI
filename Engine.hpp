#ifndef ENGINE
#define ENGINE
#include <iostream>
#include <vector>
#include <algorithm>
#include "Board.hpp"
#include "Heuristic.hpp"

class Heuristic;
using namespace std;
class Engine
{
public:
    Engine(Heuristic h) : heuristic(h) {}
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

        vector<int> nextMoves = board.generateMoves();
        sortByCenter(nextMoves);
        for (int slot = 0; slot < heuristic.killerMoves[depth].size(); slot++)
        {
            int killerMove = heuristic.killerMoves[depth][slot];

            for (int i = 0; i < nextMoves.size(); i++)
                if (nextMoves[i] == killerMove)
                {
                    nextMoves.erase(remove(nextMoves.begin(), nextMoves.end(), nextMoves[i]), nextMoves.end());
                    nextMoves.insert(nextMoves.begin(), nextMoves[i]);
                    break;
                }
        }
        if (nextMoves.empty() || depth == 0)
        {
            return {heuristic.evaluateBoard(board.board, AI), -1};
        }

        if (currentPlayer == C::AI)
        {
            pair<int, int> bestScore = {INT_MIN, -1};
            for (int col : nextMoves)
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
            return bestScore;
        }
        else
        {
            pair<int, int> bestScore = {INT_MAX, -1};
            for (int col : nextMoves)
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
            return bestScore;
        }
    }

private:
    Heuristic heuristic;
    bool customSort(int a, int b, double center)
    {
        double diffA = abs(a - center);
        double diffB = abs(b - center);
        if (diffA == diffB)
        {
            return a < b;
        }
        return diffA < diffB;
    }
    void sortByCenter(vector<int> &numbers)
    {
        double center = (numbers.front() + numbers.back()) / 2.0;
        sort(numbers.begin(), numbers.end(),
             [this, center](int a, int b)
             { return customSort(a, b, center); });
    }
};

#endif