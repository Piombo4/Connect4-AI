#include <iostream>
#include <vector>
#include <fstream>
#include <windows.h>
#include <algorithm>
#include <sstream>
#include <string>
#include <chrono>
#include "src/Board.hpp"
#include "src/Engine.hpp"
#include "src/KillerMoves.hpp"
using std::chrono::duration;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

using namespace std;

int MAX_DEPTH = 1;

bool gameOver;
int currentPlayer;
int turns;
float sum;
int aiMoves;
/**
 * A move made by a human player
 * @param board the board
 * @return - the column to place the piece
 */
int userMove(Board &board)
{
    int move;
    cout << endl;
    cout << "Your turn! ";
    cout << endl;

    while (true)
    {
        cout << endl;
        cout << "Insert the column: ";
        cin >> move;

        if (!cin)
        { // check non-integer values
            cout << "Please insert a number between 1 and 7";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
        }
        else if ((unsigned int)move > Board::NUM_COL || (unsigned int)move <= 0) // outside bounds
        {

            cout << "Please insert a number between 1 and 7";
        }
        else if (!board.can_play(move - 1)) // column full
        {
            cout << "Column full!";
        }
        else // valid input
        {
            break;
        }
        cout << endl
             << endl;
    }
    return move - 1;
}

/**
 * A move made by an AI
 * @return - the column to place the piece
 */
int aiMove(Board &board, Engine &engine)
{
    cout << endl;
    cout << "AI is thinking...";
    cout << endl;

    auto t1 = high_resolution_clock::now();
    pair<int, int> move = engine.minimax(board, MAX_DEPTH, INT_MIN, INT_MAX, true);
    auto t2 = high_resolution_clock::now();
    duration<double, milli> ms_double = t2 - t1;

    // int move = searchMove(board, MAX_DEPTH);
    cout << endl;
    cout << "Elaboration speed: " << ms_double.count() << "ms";
    cout << endl;
    cout << "Move played: " << move.second + 1 << "\n";
    cout << "Move score: " << move.first << "\n";
    cout << endl;
    sum += ms_double.count();
    aiMoves++;
    return move.second;
    // return move;
}
/**
 * Starts the game
 */
void playGame(Board &board, Engine &engine)
{
    cout << "  ____                            _     _____                \n";
    cout << " / ___|___  _ __  _ __   ___  ___| |_  |  ___|__  _   _ _ __ \n";
    cout << "| |   / _ \\| '_ \\| '_ \\ / _ \\/ __| __| | |_ / _ \\| | | | '__|\n";
    cout << "| |__| (_) | | | | | | |  __/ (__| |_  |  _| (_) | |_| | |   \n";
    cout << " \\____\\___/|_| |_|_| |_|\\___|\\___|\\__| |_|  \\___/ \\__,_|_|\n";
    cout << "\n";
    cout << "You can choose the column to insert the token into by specifying its number:";
    cout << "\n";
    cout << endl;

    board.init_board();
    gameOver = false;
    // draw empty board
    board.draw_board();
    // player starts
    currentPlayer = C::PLAYER;
    int move = -1;

    while (!gameOver)
    {
        if (board.n_moves() == Board::NUM_COL * Board::NUM_ROW)
        {
            cout << "Draw!";
            cout << endl;
            return;
        }
        move = currentPlayer == C::PLAYER ? userMove(board) : aiMove(board, engine);
        board.make_move(move, currentPlayer);
        board.draw_board();
        gameOver = board.has_won(currentPlayer);
        if (!gameOver)
        {
            currentPlayer = (currentPlayer == C::PLAYER) ? AI : C::PLAYER;
        }
    }
    if (currentPlayer == C::PLAYER)
    {
        cout << "You won!";
        cout << endl;
    }
    else
    {
        cout << "You lost!";
        cout << endl;
    }
}

bool isInteger(char *s)
{
    std::istringstream iss(s);
    int x;
    // Try to convert the string to an integer
    return (iss >> x) && (iss.eof());
}
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cout << "usage: ./cf depth" << endl;
        return -1;
    }
    else
    {
        if (!isInteger(argv[1]))
        {
            cout << "usage: ./cf depth" << endl;
            return -1;
        }
        else
        {
            MAX_DEPTH = atoi(argv[1]);
        }
    }
    aiMoves = 0;
    sum = 0;
    Board board;
    KillerMoves killerMovesPlaceholder;
    Engine engine = Engine(killerMovesPlaceholder);
    playGame(board, engine);
    cout << endl;
    cout << "Average elaboration speed: " << sum / aiMoves << "ms";
    cout << endl;
    cout << endl;
    cout << "\nGame Over\n";
    return 0;
}
