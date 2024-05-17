#include <iostream>
#include <vector>
#include <fstream>

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

using namespace std;

unsigned int NUM_COL = 7;
unsigned int NUM_ROW = 6;

const int EMPTY = 0;
const int PLAYER = 1;
const int AI = 2;
const int DRAW = 3;

unsigned int MAX_DEPTH = 8;

vector<vector<int>> board(NUM_ROW, vector<int>(NUM_COL));
int gameOver;
int currentPlayer;
int turns;

ofstream outfile("moves.txt");

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
int check_end(vector<vector<int>> &board, int currentPlayer)
{
    int count;

    // check horizontal win
    for (int row = 0; row < NUM_ROW; row++)
    {
        count = 0;
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
                    count = 0;
                }
                if (count == 4)
                {
                    cout << "Horizontal Win";
                    cout << endl;
                    return currentPlayer;
                }
            }
        }
    }
    // check vertical win
    for (int col = 0; col < NUM_COL; col++)
    {
        count = 0;
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
                    count = 0;
                }
                if (count == 4)
                {
                    cout << "Vertical Win";
                    cout << endl;
                    return currentPlayer;
                }
            }
        }
    }

    // check diagonal win
    for (unsigned int c = 0; c < NUM_COL - 3; c++)
    {
        for (unsigned int r = 3; r < NUM_ROW; r++)
        {
            for (int i = 0; i < 4; i++)
            {
                if ((unsigned int)board[r - i][c + i] == currentPlayer)
                {
                    count++;
                }
                else
                {
                    count = 0;
                }
                if (count == 4)
                {
                    cout << "Diagonal Win";
                    cout << endl;
                    return currentPlayer;
                }
            }
            count = 0;
        }
    }
    for (unsigned int c = 0; c < NUM_COL - 3; c++)
    {
        for (unsigned int r = 0; r < NUM_ROW - 3; r++)
        {
            for (int i = 0; i < 4; i++)
            {
                if ((unsigned int)board[r + i][c + i] == currentPlayer)
                {
                    count++;
                }
                else
                {
                    count = 0;
                }

                if (count == 4)
                {
                    cout << "Diagonal Win";
                    cout << endl;
                    return currentPlayer;
                }
            }
            count = 0;
        }
    }
    if (turns == NUM_COL * NUM_ROW)
    {
        return DRAW;
    }

    return -1;
}
int evaluateLine(int countPlayer, int countOpponent, int empty)
{

    if (countPlayer == 4) // winning move
    {
        return 10001;
    }
    else if (countPlayer == 3 && countOpponent == 0) // three in a row
    {
        return 1000;
    }
    else if (countPlayer == 2 && countOpponent == 0)
    {
        return 500;
    }
    else if (countOpponent == 2 && empty == 2)
    {
        return -501;
    }
    else if (countOpponent == 3 && empty == 1)
    {
        return -1001;
    }
    else if (countOpponent == 4)
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
        countPlayer = 0;
        countOpponent = 0;
        empty = 0;
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

            score += evaluateLine(countPlayer, countOpponent, empty);
            countPlayer = 0;
            countOpponent = 0;
            empty = 0;
        }
    }

    for (int col = 0; col < NUM_COL; col++)
    {
        countPlayer = 0;
        countOpponent = 0;
        empty = 0;
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
            score += evaluateLine(countPlayer, countOpponent, empty);
            countPlayer = 0;
            countOpponent = 0;
            empty = 0;
        }
    }

    for (unsigned int c = 0; c < NUM_COL - 3; c++)
    {
        for (unsigned int r = 3; r < NUM_ROW; r++)
        {
            for (int i = 0; i < 4; i++)
            {
                if ((unsigned int)board[r - i][c + i] == currentPlayer)
                {
                    countPlayer++;
                }
                else if ((unsigned int)board[r - i][c + i] != EMPTY)
                {
                    countOpponent++;
                }
                else
                {
                    empty++;
                }
            }

            score += evaluateLine(countPlayer, countOpponent, empty);
            countPlayer = 0;
            countOpponent = 0;
            empty = 0;
        }
    }
    for (unsigned int c = 0; c < NUM_COL - 3; c++)
    {
        for (unsigned int r = 0; r < NUM_ROW - 3; r++)
        {
            for (int i = 0; i < 4; i++)
            {
                if ((unsigned int)board[r + i][c + i] == currentPlayer)
                {
                    countPlayer++;
                }
                else if ((unsigned int)board[r + i][c + i] != EMPTY)
                {
                    countOpponent++;
                }
                else
                {
                    empty++;
                }
            }

            score += evaluateLine(countPlayer, countOpponent, empty);
            countPlayer = 0;
            countOpponent = 0;
            empty = 0;
        }
    }

    return score;
}

/*
 *   @param board - board to generate the next possible moves from
 *   @return vector<int> - the columns where a piece placement is possible
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
vector<vector<int>> copyBoard(vector<vector<int>> board)
{
    return board;
}
/*
 *
 * @return - {score, column to place piece}
 */
pair<int, int> minimax(vector<vector<int>> &board, int depth, int alpha, int beta, int currentPlayer)
{
    vector<int> nextMoves = generateMoves(board);

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
                break;
            }
        }
        return bestScore;
    }
}
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
int aiMove()
{
    cout << endl;
    cout << "\t- AI turn... -";
    cout << endl;
    int move = minimax(board, MAX_DEPTH, INT_MIN, INT_MAX, AI).second;
    return move;
}
void play_game()
{
    gameOver = -1;
    draw_board(board); // draw empty board
    currentPlayer = PLAYER;
    turns = 1;
    int move = -1;

    while (gameOver < 0)
    {
        move = currentPlayer == PLAYER ? userMove() : aiMove();
        if (currentPlayer == PLAYER)
        {
            outfile << move+1 << endl;
        }
        add_move(board, move, currentPlayer);
        draw_board(board);
        gameOver = check_end(board, currentPlayer); // detect win or draw

        if (gameOver < 0)
        {
            currentPlayer = (currentPlayer == PLAYER) ? AI : PLAYER;
            turns++;
        }
    }
    if (gameOver == PLAYER)
    {

        cout << "You won!";
        cout << endl;
    }
    else if (gameOver == AI)
    {
        cout << "You lost!";
        cout << endl;
    }
    else
    {
        cout << "Draw!";
        cout << endl;
    }
}

int main()
{
    init_board();

    play_game();
    outfile.close();
    cout << endl;
    cout << "END";
    return 0;
}
