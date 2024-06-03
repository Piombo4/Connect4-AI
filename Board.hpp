#ifndef BOARD
#define BOARD
#include <vector>
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

    Board() : _bitboards{0}, _moves{-1}
    {
        _counter = 0;
        std::fill(_col_heights, _col_heights + NUM_COL, 0);
    }

    /*
     * Draws the board
     */
    void draw()
    {
        for (int i = NUM_ROW - 1; i >= 0; i--)
        {
            std::cout << "|";
            for (int j = 0; j < NUM_COL; j++)
            {
                uint64_t pos = UINT64_C(1) << (j * (NUM_ROW + 1) + i);
                if ((_bitboards[0] & pos) == pos)
                {
                    std::cout << "\e[33m";
                    std::cout << "O";
                    std::cout << "\e[0m";
                }
                else if ((_bitboards[1] & pos) == pos)
                {
                    std::cout << "\e[31m";
                    std::cout << "O";
                    std::cout << "\e[0m";
                }
                else
                {
                    std::cout << " ";
                }
                std::cout << "|";
            }
            std::cout << std::endl;
        }

        for (int col = 0; col < NUM_COL; col++)
        { // write column numbers
            std::cout << " ";
            std::cout << col + 1;
        }
        std::cout << std::endl;
    }

    /**
     *   Checks whether a move can be played
     *   @param col - index of column to play
     *   @return true if the move can be played, false otherwise
     */
    bool canPlay(int col)
    {
        uint64_t pos = UINT64_C(1) << (NUM_ROW - 1) << col * (NUM_ROW + 1);
        return ((_bitboards[0] & pos) == 0) && ((_bitboards[1] & pos) == 0);
    }
    /**
     *   Plays a move on the board
     *   It should not be called without checking if the column is full
     *   @param col - index of column to play
     */
    void makeMove(int col)
    {
        uint64_t pos = UINT64_C(1) << (col * (NUM_ROW + 1)) + _col_heights[col];
        _bitboards[_counter & 1] |= _bitboards[_counter & 1] + pos;
        _col_heights[col]++;
        _moves[_counter++] = col;
    }
    /**
     *   Undo a move on the board
     */
    void unmakeMove()
    {
        int col = _moves[--_counter];
        uint64_t pos = UINT64_C(1) << (col * (NUM_ROW + 1) + (--_col_heights[col]));
        _bitboards[_counter & 1] &= ~pos; // Remove the last move from the mask
    }
    /**
     *  Check if an alignment of 4 has been made by the current player
     *  @return true if an alignment of 4 has been made, false otherwise
     */
    bool isWin()
    {
        // Since after making a move the counter increases,
        // I virtually increase it again to get the player who has made the last move
        uint64_t currentPlayerBB = _bitboards[(_counter + 1) & 1];
        int directions[] = {1, 7, 6, 8};
        long bb;
        for (int dir : directions)
        {
            bb = currentPlayerBB & (currentPlayerBB >> dir);
            if ((bb & (bb >> (2 * dir))) != 0)
            {
                return true;
            }
        }
        return false;
    }
    /**
     *  Check if the current player is the human player.
     *  This function assumes that the human player starts first.
     *  Whenever the human player makes a move, the counter becomes an odd number
     *  @return true if an alignment of 4 has been made, false otherwise
     */
    bool isPlayer()
    {
        return _counter % 2 != 0;
    }
    /**
     *  @return all the playable moves
     */
    std::vector<int> generateMoves()
    {
        std::vector<int> moves;
        for (int i = 0; i < NUM_COL; i++)
        {
            if (canPlay(i))
            {
                moves.push_back(i);
            }
        }
        return moves;
    }
    /**
     *  @return the number of moves played until now
     */
    int nMoves()
    {
        return _counter;
    }
    uint64_t getMask()
    {
        return _bitboards[0] | _bitboards[1];
    }
    int evalBoard()
    {
        uint64_t currentPlayerBB = _bitboards[0];
        uint64_t opponentBB = _bitboards[1];
        int directions[] = {1, 7, 6, 8};
        int score = 0;
        for (int dir : directions)
        {
            int countPlayer = 0;
            int countOpponent = 0;
            for (int i = 0; i < 4; i++)
            {
                if((currentPlayerBB & (currentPlayerBB >> dir*i)) != 0){
                    countPlayer++;
                }
                if((opponentBB & (opponentBB >> dir*i)) != 0){
                    countOpponent++;
                }
            }
            score +=evaluateChunk(countPlayer,countOpponent);

        }
        return score;
    }
    int evaluateChunk(int countPlayer, int countOpponent)
    {
        
        if (countPlayer == 4)
            return 10000; // Winning move
        else if (countPlayer == 3 && countOpponent == 0)
        {
            return 1000;
        }
        else if (countPlayer == 2 && countOpponent == 0)
        {
            return 100;
        }
        else if (countOpponent == 2 && countPlayer == 0)
        {
            return -100;
        }
        else if (countOpponent == 3 && countPlayer == 0)
        {
            return -1000;
        }
        else if (countOpponent == 4)
        {
            return -10000;
        }
        return 0;
    }

private:
    uint64_t _bitboards[2];
    int _col_heights[NUM_COL]; // Track the heights of each column
    int _moves[NUM_COL * NUM_ROW];
    int _counter;
};

#endif
