#include <iostream>
#include <vector>
#include <fstream>
#include <windows.h>
#include <algorithm>
#include <chrono>
#include "Board.hpp"
#include "Engine.hpp"
#include "Heuristic.hpp"
using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

using namespace std;

const int MAX_DEPTH = 10;

Board board;
Heuristic heuristic;
Engine engine = Engine(heuristic);
bool gameOver;
int currentPlayer;
int turns;

ofstream outfile("moves.txt");

/*
 * A move made by a human player
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
            cout << "Wrong";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
        }
        else if ((unsigned int)move > NUM_COL || (unsigned int)move <= 0) // outside bounds
        {

            cout << "Please insert a number between 1 and 7";
        }
        else if (board.board[NUM_ROW - 1][move - 1] != C::EMPTY) // column full
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

int aiMove2(Board &board)
{
    cout << endl;
    cout << "AI is thinking...";
    cout << endl;

    auto t1 = high_resolution_clock::now();
    pair<int, int> move = engine.negamaxHandler(board, MAX_DEPTH, INT_MIN, INT_MAX, C::AI);
    auto t2 = high_resolution_clock::now();
    duration<double, milli> ms_double = t2 - t1;

    // int move = searchMove(board, MAX_DEPTH);
    cout << endl;
    cout << "Elaboration speed: " << ms_double.count() << "ms";
    cout << endl;
    cout << "Move played: " << move.second + 1 << "\n";
    cout << "Move score: " << move.first << "\n";
    cout << endl;
    return move.second;
    // return move;
}
/*
 * A move made by an AI
 * @return - the column to place the piece
 */
int aiMove(Board &board)
{
    cout << endl;
    cout << "AI is thinking...";
    cout << endl;

    auto t1 = high_resolution_clock::now();
    pair<int, int> move = engine.minimax(board, MAX_DEPTH, INT_MIN, INT_MAX, C::AI);
    auto t2 = high_resolution_clock::now();
    duration<double, milli> ms_double = t2 - t1;

    // int move = searchMove(board, MAX_DEPTH);
    cout << endl;
    cout << "Elaboration speed: " << ms_double.count() << "ms";
    cout << endl;
    cout << "Move played: " << move.second + 1 << "\n";
    cout << "Move score: " << move.first << "\n";
    cout << endl;
    return move.second;
    // return move;
}
/*
 * Starts the game
 */
void playGame(Board &board)
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
    currentPlayer = C::AI;
    turns = 0;
    int move = -1;

    while (!gameOver)
    {
        if (turns == NUM_COL * NUM_ROW)
        {
            cout << "Draw!";
            cout << endl;
            return;
        }
        move = currentPlayer == C::PLAYER ? userMove(board) : aiMove2(board);
        if (currentPlayer == C::PLAYER)
        {
            outfile << move + 1 << endl;
        }
        board.add_move(move, currentPlayer);
        board.draw_board();
        gameOver = board.winning_move(currentPlayer);
        if (!gameOver)
        {
            currentPlayer = (currentPlayer == C::PLAYER) ? AI : C::PLAYER;
            turns++;
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

int main()
{

    playGame(board);
    outfile.close();
    cout << endl;
    cout << "END";
    return 0;
}