#include <algorithm>
#include <chrono>
#include <iostream>
#include "Board.hpp"
int userMove(Board &board)
{
    int move;

    std::cout << std::endl
              << "Your turn! " << std::endl;

    while (true)
    {
        std::cout << std::endl
                  << "Insert the column: ";
        std::cin >> move;

        if (!std::cin)
        { // check non-integer values
            std::cout << "Please insert a number between 1 and 7";
            std::cin.clear();
            std::cin.ignore(INT_MAX, '\n');
        }
        else if ((unsigned int)move > board.NUM_COL || (unsigned int)move <= 0) // outside bounds
        {

            std::cout << "Please insert a number between 1 and 7";
        }
        else if (!board.canPlay(move - 1)) // column full
        {
            std::cout << "Column full!";
        }
        else // valid input
        {
            break;
        }
        std::cout << std::endl
                  << std::endl;
    }
    return move - 1;
}
void playGame(Board &board)
{
    std::cout << "  ____                            _     _____                \n";
    std::cout << " / ___|___  _ __  _ __   ___  ___| |_  |  ___|__  _   _ _ __ \n";
    std::cout << "| |   / _ \\| '_ \\| '_ \\ / _ \\/ __| __| | |_ / _ \\| | | | '__|\n";
    std::cout << "| |__| (_) | | | | | | |  __/ (__| |_  |  _| (_) | |_| | |   \n";
    std::cout << " \\____\\___/|_| |_|_| |_|\\___|\\___|\\__| |_|  \\___/ \\__,_|_|\n";
    std::cout << "\n";
    std::cout << "You can choose the column to insert the token into by specifying its number:";
    std::cout << "\n";
    std::cout << std::endl;

    board.draw();
    bool gameOver = false;
    int currentPlayer = C::PLAYER;
    int turns = 0;
    while (!gameOver)
    {
        int move = userMove(board);
        board.makeMove(move);
        board.draw();
        gameOver = board.isWin();
    }
    if (board.isPlayer())
    {
        std::cout << "You won!" << std::endl;
    }
    else
    {
        std::cout << "You lost!" << std::endl;
    }
}
int main(int argc, char *argv[])
{
    Board board;
    playGame(board);

    return 0;
}