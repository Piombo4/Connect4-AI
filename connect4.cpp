#include <iostream>
#include <vector>

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

using namespace std;

unsigned int NUM_COL = 7;
unsigned int NUM_ROW = 6;

const int EMPTY = 0;
const int PLAYER = 1;
const int AI = 2;

vector<vector<int>> board(NUM_ROW, vector<int>(NUM_COL));
bool gameOver;
int currentPlayer;
int turns;

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
                cout << "\x1b[33m";
                cout << "O";
                cout << "\x1b[0m";
                break;
            case AI:
                cout << "\x1b[31m";
                cout << "O";
                cout << "\x1b[0m";
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
void add_move(vector<vector<int>> &board, int move, int currentPlayer)
{
    for (int row = 0; row < NUM_ROW; row++)
    {
        if (board[row][move - 1] == EMPTY)
        {
            board[row][move - 1] = currentPlayer;
            break;
        }
    }
}
bool detect_win(vector<vector<int>> &board)
{
    int count;
    int last_column = NUM_COL - 1;
    int last_row = NUM_ROW - 1;

    // check horizontal win
    for (int row = 0; row < NUM_ROW; row++)
    {
        count = 1;
        for (int col = 0; col < last_column; col++)
        {
            if (count == 4)
            {
                return true;
            }
            if (board[row][col] != EMPTY && board[row][col] == board[row][col + 1])
            {
                count++;
            }
            else
            {
                count = 1;
            }
        }
    }

    // check vertical win
    for (int col = 0; col < NUM_COL; col++)
    {
        count = 1;
        for (int row = 0; row < last_row; row++)
        {
            if (count == 4)
            {
                return true;
            }
            if (board[row][col] != EMPTY && board[row][col] == board[row + 1][col])
            {
                count++;
            }
            else
            {
                count = 1;
            }
        }
    }
    // check diagonal win
    for (int col = 0; col < last_column; col++)
    {
        count = 1;
        for (int row = 0; row < last_row; row++)
        {
            if (count == 4)
            {
                return true;
            }
            if (board[row][col] != EMPTY && board[row][col] == board[row + 1][col + 1])
            {
                count++;
            }
            else
            {
                count = 1;
            }
        }
    }
    for (int col = last_column; col > 0; col--)
    {
        count = 1;
        for (int row = last_row; row > 0; row--)
        {
            if (count == 4)
            {
                return true;
            }
            if (board[row][col] != EMPTY && board[row][col] == board[row - 1][col - 1])
            {
                count++;
            }
            else
            {
                count = 1;
            }
        }
    }

    return false;
}
void play_game()
{
    gameOver = false;
    draw_board(board); // draw empty board
    int move = -1;
    currentPlayer = PLAYER;
    turns = 1;

    while (!gameOver)
    {

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
            else if ((unsigned int)move > NUM_COL || (unsigned int)move <= 0)
            { // outside bounds

                cout << "Please insert a number between 1 and 7";
            }
            else if (board[NUM_ROW - 1][move - 1] != EMPTY)
            {
                cout << "Column full!";
            }
            else
            { // valid input
                break;
            }
            cout << endl
                 << endl;
        }
        add_move(board, move, currentPlayer);
        draw_board(board);
        gameOver = detect_win(board);
        if (!gameOver)
        {
            currentPlayer = (currentPlayer == PLAYER) ? AI : PLAYER;
        }
        turns++;
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
    init_board();
    play_game();
    return 0;
}
