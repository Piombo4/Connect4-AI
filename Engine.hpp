#include "Board.hpp"
#include <iostream>
class Engine
{
public:
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
            bestValue = std::max(bestValue, value);
            alpha = std::max(alpha, bestValue);
            if (alpha >= beta)
            {
                break;
            }
        }
        return {bestValue, bestMove};
    }

private:
    unsigned long long nodeCount;    // counter of explored nodes.
    int columnOrder[Board::NUM_COL]; // column exploration order
    int negamax(Board &board, int depth, int alpha, int beta, int color)
    {
        nodeCount++;

        if (depth == 0)
        {
            // return board evaluation
        }
        else if (board.nMoves() == Board::NUM_COL * Board::NUM_ROW) // check for draw game
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