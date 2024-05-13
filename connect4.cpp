#include <iostream>
#include <vector>

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

using namespace std;

unsigned int NUM_COL = 7;
unsigned int NUM_ROW = 6;

const int EMPTY = 0;
const int YELLOW = 1;
const int RED = 2;

const int PLAYER = 0;
const int AI = 1;

vector<vector<int>> board(NUM_ROW, vector<int>(NUM_COL));
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
            case YELLOW:
                cout << "\x1b[33m";
                cout << "O";
                cout << "\x1b[0m";
                break;
            case RED:
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

void add_move(vector<vector<int>> &board, int move)
{
    for (int row = 0; row < NUM_ROW; row++)
    {
        if (board[row][move - 1] == EMPTY)
        {
            board[row][move - 1] = RED;
            break;
        }
    }
}
void play_game()
{
    draw_board(board); // draw empty board
    int move = -1;
    while (true)
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
        add_move(board, move);

        draw_board(board);
    }
}

int main()
{

    init_board();
    play_game();
    return 0;
}
