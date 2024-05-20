#include <iostream>
#include <vector>
#include <fstream>
#include <windows.h>
#include <algorithm>
#include <chrono>
using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

using namespace std;

const unsigned int NUM_COL = 7;
const unsigned int NUM_ROW = 6;

const int EMPTY = 0;
const int PLAYER = -1;
const int AI = 1;

const int MAX_DEPTH = 12;
const int KILLER_MOVES_SLOTS = 2;
const int KILLER_DEPTH = 15;

// vector<vector<int>> board(NUM_ROW, vector<int>(NUM_COL));

// int board[NUM_ROW][NUM_COL];
vector<vector<int>> board(NUM_ROW, vector<int>(NUM_COL));

vector<vector<int>> killerMoves(KILLER_DEPTH, vector<int>(KILLER_MOVES_SLOTS));
bool gameOver;
int currentPlayer;
int turns;

ofstream outfile("moves.txt");

/*
 *   Enable the ANSI colors on the normal terminal
 *
 */
void EnableANSIColors()
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE)
        return;

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode))
        return;

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(hOut, dwMode))
        return;
}
/*
 *   Initialize the board
 */
void init_board()
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

    for (int row = 0; row < NUM_ROW; row++)
    {
        for (int col = 0; col < NUM_COL; col++)
        {
            board[row][col] = EMPTY;
        }
    }
}
/*
 *   Draw the board
 *   @param board - the board to draw
 */
void draw_board(vector<vector<int>> &board)
{

    for (int row = 0; row < NUM_ROW; row++)
    {
        cout << "|";
        for (int col = 0; col < NUM_COL; col++)
        {
            switch (board[NUM_ROW - row - 1][col])
            {
            case EMPTY:
                cout << " ";
                break;
            case PLAYER:
                cout << "\e[33m";
                cout << "O";
                cout << "\e[0m";
                break;
            case AI:
                cout << "\e[31m";
                cout << "O";
                cout << "\e[0m";
                break;
            default:
                break;
            }
            cout << "|";
        }
        cout << row;
        cout << endl;
    }

    for (int col = 0; col < NUM_COL; col++)
    { // write column numbers
        cout << " ";
        cout << col + 1;
    }
    cout << endl;
}
/*
 *   Add a move to the board
 *   @param board - the board to add the move
 *   @param move - the move to add (it represents a column)
 *   @param currentPlayer - the player who made the move
 *   @return - the row in which the move was added
 */
int add_move(vector<vector<int>> &board, int move, int currentPlayer)
{

    for (int row = 0; row < NUM_ROW; row++)
    {
        if (board[row][move] == EMPTY)
        {
            board[row][move] = currentPlayer;
            return row;
        }
    }
    return -1;
}
/*
 *   Check whether the current position is a winning position
 *   @param board - the board
 *   @param currentPlayer - the player to check for the win
 *   @return - true if currentPlayer has won, false otherwise
 */
bool winning_move(vector<vector<int>> &board, int currentPlayer)
{
    int count;

    // check horizontal win
    for (int row = 0; row < NUM_ROW; row++)
    {

        for (int col = 0; col < NUM_COL - 3; col++)
        {
            count = 0;
            for (int i = 0; i < 4; i++)
            {
                if (board[row][col + i] == currentPlayer)
                {
                    count++;
                }
                else
                {
                    break;
                }
                if (count == 4)
                {
                    return true;
                }
            }
        }
    }
    // check vertical win
    for (int col = 0; col < NUM_COL; col++)
    {

        for (int row = 0; row < NUM_ROW - 3; row++)
        {
            count = 0;
            for (int i = 0; i < 4; i++)
            {
                if (board[row + i][col] == currentPlayer)
                {
                    count++;
                }
                else
                {

                    break;
                }
                if (count == 4)
                {
                    return true;
                }
            }
        }
    }

    // check diagonal win
    for (unsigned int c = 0; c < NUM_COL - 3; c++)
    {
        for (unsigned int r = 3; r < NUM_ROW; r++)
        {
            count = 0;
            for (int i = 0; i < 4; i++)
            {
                if ((unsigned int)board[r - i][c + i] == currentPlayer)
                {
                    count++;
                }
                else
                {

                    break;
                }
                if (count == 4)
                {
                    return true;
                }
            }
        }
    }
    for (unsigned int c = 0; c < NUM_COL - 3; c++)
    {
        for (unsigned int r = 0; r < NUM_ROW - 3; r++)
        {
            count = 0;
            for (int i = 0; i < 4; i++)
            {
                if ((unsigned int)board[r + i][c + i] == currentPlayer)
                {
                    count++;
                }
                else
                {

                    break;
                }

                if (count == 4)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

/*
 *   Heuristic function
 *   If possible it chooses the winning move. It prefers to block rather than placing consecutive pieces
 *   @param countPlayer - The number of consecutive pieces of the player
 *   @param countOpponent -The number of consecutive pieces of the opponent
 *   @param empty -  The number of empty spaces
 *   @return - the score of the move
 */
int evaluateChunk(int countPlayer, int countOpponent, int empty)
{

    if (countPlayer == 4) // winning move
    {
        return 10001;
    }
    else if (countPlayer == 3 && countOpponent == 0) // three consecutive
    {
        return 1000;
    }
    else if (countPlayer == 2 && countOpponent == 0) // two consecutive
    {
        return 100;
    }
    else if (countOpponent == 2 && countPlayer == 0) // two consecutive of the opponent
    {
        return -101;
    }
    else if (countOpponent == 3 && countPlayer == 0) // three consecutive of the opponent
    {
        return -1001;
    }
    else if (countOpponent == 4) // winning move for the opponent
    {
        return -10000;
    }
    return 0;
}
/*
 * Evaluates the position of the board
 * @param board - the board to evaluate
 * @param currentPlayer - the player who has to move
 * @return - a score based on how good or bad the position is
 */
int evaluateBoard(vector<vector<int>> &board, int currentPlayer)
{

    int score = 0;
    int last_column = NUM_COL - 3;
    int last_row = NUM_ROW - 3;
    int countPlayer = 0;
    int countOpponent = 0;
    int empty = 0;
    for (int row = 0; row < NUM_ROW; row++)
    {
        int countPlayer = 0;
        int countOpponent = 0;
        int empty = 0;
        for (int col = 0; col < last_column; col++)
        {

            for (int i = 0; i < 4; i++)
            {
                if (board[row][col + i] == currentPlayer)
                {
                    countPlayer++;
                }
                else if (board[row][col + i] != EMPTY)
                {
                    countOpponent++;
                }
                else
                {
                    empty++;
                }
            }
            score += evaluateChunk(countPlayer, countOpponent, empty);
            countPlayer = 0;
            countOpponent = 0;
            empty = 0;
        }
    }

    for (int col = 0; col < NUM_COL; col++)
    {
        int countPlayer = 0;
        int countOpponent = 0;
        int empty = 0;
        for (int row = 0; row < last_row; row++)
        {

            for (int i = 0; i < 4; i++)
            {
                if (board[row + i][col] == currentPlayer)
                {
                    countPlayer++;
                }
                else if (board[row + i][col] != EMPTY)
                {
                    countOpponent++;
                }
                else
                {
                    empty++;
                }
            }
            score += evaluateChunk(countPlayer, countOpponent, empty);
            countPlayer = 0;
            countOpponent = 0;
            empty = 0;
        }
    }

    for (int c = 0; c < NUM_COL - 3; c++)
    {
        int countPlayer = 0;
        int countOpponent = 0;
        int empty = 0;
        for (int r = 3; r < NUM_ROW; r++)
        {

            for (int i = 0; i < 4; i++)
            {
                if (board[r - i][c + i] == currentPlayer)
                {
                    countPlayer++;
                }
                else if (board[r - i][c + i] != EMPTY)
                {
                    countOpponent++;
                }
                else
                {
                    empty++;
                }
            }

            score += evaluateChunk(countPlayer, countOpponent, empty);
            countPlayer = 0;
            countOpponent = 0;
            empty = 0;
        }
    }
    for (int c = 0; c < NUM_COL - 3; c++)
    {
        int countPlayer = 0;
        int countOpponent = 0;
        int empty = 0;
        for (int r = 0; r < NUM_ROW - 3; r++)
        {

            for (int i = 0; i < 4; i++)
            {
                if (board[r + i][c + i] == currentPlayer)
                {
                    countPlayer++;
                }
                else if (board[r + i][c + i] != EMPTY)
                {
                    countOpponent++;
                }
                else
                {
                    empty++;
                }
            }

            score += evaluateChunk(countPlayer, countOpponent, empty);
            countPlayer = 0;
            countOpponent = 0;
            empty = 0;
        }
    }
    return score;
}

/*
 *   Generates all the possible moves from a certain position
 *   @param board - board to generate the next possible moves from
 *   @return - the columns where a piece placement is possible
 */
vector<int> generateMoves(vector<vector<int>> &board)
{
    vector<int> possibleMoves;
    for (int col = 0; col < NUM_COL; col++)
    {
        for (int row = 0; row < NUM_ROW; row++)
        {
            if (board[row][col] == EMPTY)
            {
                possibleMoves.push_back(col);
                break;
            }
        }
    }
    return possibleMoves;
}
void store_killer_moves(int move, int depth)
{

    for (int i = KILLER_MOVES_SLOTS - 2; i >= 0; i--)
    {
        killerMoves[depth][i + 1] = killerMoves[depth][i];
    }
    killerMoves[depth][0] = move;
}

/*
 * Functions to sort values so that the values in the center are now first
 */
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
         [center](int a, int b)
         { return customSort(a, b, center); });
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

pair<int, int> minimax(vector<vector<int>> &board, int depth, int alpha, int beta, int currentPlayer)
{

    vector<int> nextMoves = generateMoves(board);
    sortByCenter(nextMoves);
    for (int slot = 0; slot < killerMoves[depth].size(); slot++)
    {     
        int killerMove = killerMoves[depth][slot];

        for (int i = 0; i < nextMoves.size(); i++)
            if (nextMoves[i] == killerMove)
            {
                nextMoves.erase(remove(nextMoves.begin(), nextMoves.end(), nextMoves[i]), nextMoves.end());
                nextMoves.insert(nextMoves.begin(), killerMove);
                break;
            }
    }

    if (nextMoves.empty() || depth == 0)
    {
        return {evaluateBoard(board, AI), -1};
    }

    if (currentPlayer == AI)
    {
        pair<int, int> bestScore = {INT_MIN, -1};
        for (int col : nextMoves)
        {
            int row = add_move(board, col, currentPlayer);
            int currentScore = minimax(board, depth - 1, alpha, beta, PLAYER).first;

            if (currentScore > bestScore.first)
            {
                bestScore = {currentScore, col};
            }
            alpha = max(alpha, bestScore.first);

            board[row][col] = EMPTY; // Undo move

            if (alpha >= beta)
            {
                store_killer_moves(col, depth);
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

            // memcpy(tempBoard, board, NUM_COL * NUM_ROW * sizeof(int));
            int row = add_move(board, col, currentPlayer);
            int currentScore = minimax(board, depth - 1, alpha, beta, AI).first;

            if (currentScore < bestScore.first)
            {
                bestScore = {currentScore, col};
            }
            beta = min(beta, bestScore.first);

            board[row][col] = EMPTY; // Undo move

            if (alpha >= beta)
            {

                store_killer_moves(col, depth);
                break;
            }
        }
        return bestScore;
    }
}
/*
 * A move made by a human player
 * @return - the column to place the piece
 */
int userMove()
{
    int move;
    cout << endl;
    cout << "\t- Your turn! -";
    cout << endl;

    while (true)
    {
        cout << "Insert the column: ";
        cin >> move;

        if (!cin)
        { // check non-integer values
            cout << "Wrong";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
        }
        else if ((unsigned int)move > NUM_COL || (unsigned int)move <= -0) // outside bounds
        {

            cout << "Please insert a number between 1 and 7";
        }
        else if (board[NUM_ROW - 1][move - 1] != EMPTY) // column full
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
/*
 * A move made by an AI
 * @return - the column to place the piece
 */
int aiMove()
{
    cout << endl;
    cout << "\t- AI turn... -";
    cout << endl;

    auto t1 = high_resolution_clock::now();
    pair<int, int> move = minimax(board, MAX_DEPTH, INT_MIN, INT_MAX, AI);
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
void play_game()
{
    gameOver = false;
    // draw empty board
    draw_board(board);
    // player starts
    currentPlayer = PLAYER;
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
        move = currentPlayer == PLAYER ? userMove() : aiMove();
        if (currentPlayer == PLAYER)
        {
            outfile << move + 1 << endl;
        }
        add_move(board, move, currentPlayer);
        draw_board(board);
        gameOver = winning_move(board, currentPlayer); // detect win or draw
        if (!gameOver)
        {
            currentPlayer = (currentPlayer == PLAYER) ? AI : PLAYER;
            turns++;
        }
    }
    if (currentPlayer == PLAYER)
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

    EnableANSIColors();
    init_board();
    play_game();
    outfile.close();
    cout << endl;
    cout << "END";
    return 0;
}
