#include "Board.hpp"
#include <iostream>
class Engine
{
public:
    Engine()
    {
        nodeCount = 0;
    }
    std::pair<int, int> negamaxHandler(Board &board, int depth, int alpha, int beta, int color)
    {
        int bestValue = INT_MIN;
        int bestMove = -1;
        std::vector<int> moves = board.generateMoves();
        for (int move : moves)
        {
            board.makeMove(move);
            int value = -negamax(board, depth - 1, -beta, -alpha, -color);
            board.unmakeMove();
            if (value > bestValue)
            {
                bestValue = value;
                bestMove = move;
            }
            alpha = std::max(alpha, bestValue);
            if (alpha >= beta)
            {
                break;
            }
        }
        return {bestValue, bestMove};
    }
    std::pair<int, int> minimax(Board &board, int depth, int alpha, int beta, int currentPlayer)
    {

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
            return {board.evalBoard(), -1};
        }
        else if (board.nMoves() == Board::NUM_COL * Board::NUM_ROW) // check for draw game
        {
            return {0, -1};
        }
        std::vector<int> moves = board.generateMoves();
        if (currentPlayer == C::AI)
        {
            std::pair<int, int> bestScore = {INT_MIN, -1};

            for (int col : moves)
            {

                board.makeMove(col);
                int currentScore = minimax(board, depth - 1, alpha, beta, -currentPlayer).first;
                if (currentScore > bestScore.first)
                {
                    bestScore = {currentScore, col};
                }
                alpha = std::max(alpha, bestScore.first);

                board.unmakeMove();

                if (alpha >= beta)
                {
                    // heuristic.store_killer_moves(col, depth);
                    break;
                }
            }
            return bestScore;
        }
        else
        {
            std::pair<int, int> bestScore = {INT_MAX, -1};
            for (int col : moves)
            {

                board.makeMove(col);
                int currentScore = minimax(board, depth - 1, alpha, beta, -currentPlayer).first;

                if (currentScore < bestScore.first)
                {
                    bestScore = {currentScore, col};
                }
                beta = std::min(beta, bestScore.first);

                board.unmakeMove();

                if (alpha >= beta)
                {
                    // heuristic.store_killer_moves(col, depth);
                    break;
                }
            }
            return bestScore;
        }
    }

private:
    unsigned long long nodeCount;    // counter of explored nodes.
    int columnOrder[Board::NUM_COL]; // column exploration order
    int negamax(Board &board, int depth, int alpha, int beta, int color)
    {
        nodeCount++;

        if (depth == 0)
        {
            return color * board.evalBoard();
        }
        else if (board.nMoves() == Board::NUM_COL * Board::NUM_ROW) //check for draw game
        {
            return 0;
        }

        int bestValue = INT_MIN;
        std::vector<int> moves = board.generateMoves();
        for (int move : moves)
        {
            board.makeMove(move);
            int value = -negamax(board, depth - 1, -beta, -alpha, -color);
            board.unmakeMove();
            bestValue = std::max(bestValue, value);
            alpha = std::max(alpha, bestValue);
            if (alpha >= beta)
            {
                break;
            }
        }
        return bestValue;
    }
};