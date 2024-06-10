#ifndef HEURISTIC
#define HEURISTIC
#include "Board.hpp"

class Board;
using namespace std;
const int KILLER_MOVES_SLOTS = 2;
const int KILLER_DEPTH = 18;
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
    bool isKillerMove(int move, int depth)
    {
        for (int slot = 0; slot < killerMoves[depth].size(); slot++)
        {
            if (move == killerMoves[depth][slot])
            {
                return true;
            }
        }
        return false;
    }
    vector<vector<int>> killerMoves;

private:
};
#endif