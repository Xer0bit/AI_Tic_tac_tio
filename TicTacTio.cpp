#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
using namespace std;

class TicTacToe;
class AI;
void play_game(TicTacToe game);

class TicTacToe
{
private:
    char board[9] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
    char current_turn = 'X';
    char winner = ' '; // ' ' refers to None
    int state = -1;    // -1 refers to running
    vector<int> move_stack;
    void swap_turn();
    void update_state();

public:
    friend class AI;
    friend void play_game(TicTacToe game);

    void print_board() const;
    int play_move(int index);
    void undo_move();
    vector<int> get_possible_moves();
};

class AI
{
private:
    int max(TicTacToe board, char max_symbol, int depth);
    int min(TicTacToe board, char max_symbol, int depth);

public:
    int minmax(TicTacToe board, char max_symbol);
};
// main
int main()
{
    TicTacToe game;
    play_game(game);
    return 0;
}

void play_game(TicTacToe game)
{
    bool playing = true;
    int move;
    int count = 0;
    AI my_ai;

    while (playing)
    {
        game.print_board();
        if (count % 2 == 1)
        {
            move = my_ai.minmax(game, game.current_turn);
        }
        else
        {
            cout << "Enter your move (1-9)\n";
            cin >> move;
            if (!cin)
            {
                cerr << "Input error\n";
                return;
            }
            --move;
        }

        if (game.play_move(move) == 0)
        {
            cout << "Box already occupied\n";
            continue;
        }

        if (game.state == 1)
        {
            game.print_board();
            cout << game.winner << " wins the game!\n";
            playing = false;
        }
        else if (game.state == 0)
        {
            game.print_board();
            cout << "Draw!\n";
            playing = false;
        };
        ++count;
    };
};

void TicTacToe::print_board() const
{
    system("clear");
    // read di.txt line radomly
    ifstream file("di.txt");
    string line;
    int random = rand() % 7;
    for (int i = 0; i < random; i++)
    {
        getline(file, line);
    }
    cout << "***********" << line << "**********" << endl;

    // cout << "Player 1 (X)  -  Player 2 (O)" << endl << endl;
    cout << endl;

    cout << "     |     |     " << endl;
    cout << "  " << board[0] << "  |  " << board[1] << "  |  " << board[2] << endl;

    cout << "_____|_____|_____" << endl;
    cout << "     |     |     " << endl;

    cout << "  " << board[3] << "  |  " << board[4] << "  |  " << board[5] << endl;

    cout << "_____|_____|_____" << endl;
    cout << "     |     |     " << endl;

    cout << "  " << board[6] << "  |  " << board[7] << "  |  " << board[8] << endl;

    cout << "     |     |     " << endl
         << endl;
};

void TicTacToe::swap_turn()
{
    current_turn = (current_turn == 'X') ? 'O' : 'X';
}

int TicTacToe::play_move(int index)
{
    if (index >= 0 && index < 9)
    {
        if (board[index] == ' ')
        {
            board[index] = current_turn;
            move_stack.push_back(index);
            update_state();
            swap_turn();
            return 1;
        }
    }
    return 0;
};

void TicTacToe::undo_move()
{
    int move = move_stack.back();
    board[move] = ' ';
    move_stack.pop_back();
    update_state();
    swap_turn();
};

vector<int> TicTacToe::get_possible_moves()
{
    vector<int> possible_moves;
    for (int i = 0; i < 9; ++i)
    {
        bool found = (find(move_stack.begin(), move_stack.end(), i) != move_stack.end());
        if (!found)
        {
            possible_moves.push_back(i);
        }
    }
    return possible_moves;
}

void TicTacToe::update_state()
{
    if (
        // Horizontal checks
        (board[0] == current_turn && board[1] == current_turn && board[2] == current_turn) ||
        (board[3] == current_turn && board[4] == current_turn && board[5] == current_turn) ||
        (board[6] == current_turn && board[7] == current_turn && board[8] == current_turn) ||
        // Vertical Checks
        (board[0] == current_turn && board[3] == current_turn && board[6] == current_turn) ||
        (board[1] == current_turn && board[4] == current_turn && board[7] == current_turn) ||
        (board[2] == current_turn && board[5] == current_turn && board[8] == current_turn) ||
        // Diagonal Checks
        (board[0] == current_turn && board[4] == current_turn && board[8] == current_turn) ||
        (board[2] == current_turn && board[4] == current_turn && board[6] == current_turn))
    {
        state = 1;
        winner = current_turn;
    }
    else
    {
        bool draw = true;
        for (int i = 0; i < 9; ++i)
        {
            if (board[i] == ' ')
            {
                draw = false;
                break;
            }
        };
        if (draw)
        {
            state = 0;
        }
        else
        {
            winner = ' ';
            state = -1;
        }
    }
};

int AI::minmax(TicTacToe board, char max_symbol)
{
    int best_score = -100;
    int best_move = -1;
     // -1 refers to none

    for (auto move : board.get_possible_moves())
    {
        board.play_move(move);
        int score = AI::min(board, max_symbol, 0);
        if (score > best_score)
        {
            best_score = score;
            best_move = move;
        }
        board.undo_move();
    }

    return best_move;
}

int AI::max(TicTacToe board, char max_symbol, int depth)
{
    if (board.state == 1)
    {
        if (board.winner == max_symbol)
        {
            return 10 - depth;
        }
        else
        {
            return -10 + depth;
        }
    }
    else if (board.state == 0)
    {
        return 0;
    }

    int best_score = -100;

    for (auto move : board.get_possible_moves())
    {
        board.play_move(move);
        int score = AI::min(board, max_symbol, depth + 1);
        if (score > best_score)
        {
            best_score = score;
        }
        board.undo_move();
    }
    return best_score;
}

int AI::min(TicTacToe board, char max_symbol, int depth)
{
    if (board.state == 1)
    {
        if (board.winner == max_symbol)
        {
            return 10 - depth;
        }
        else
        {
            return -10 + depth;
        }
    }
    else if (board.state == 0)
    {
        return 0;
    }

    int best_score = 100;

    for (auto move : board.get_possible_moves())
    {
        board.play_move(move);
        int score = AI::max(board, max_symbol, depth + 1);
        if (score < best_score)
        {
            best_score = score;
        }
        board.undo_move();
    }
    return best_score;
}