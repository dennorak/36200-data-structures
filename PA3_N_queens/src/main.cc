#include <stack>
#include <vector>
#include <cmath> // std::abs
#include <iostream>
#include <fstream>

#define N 8
#define INVALID_POS (N * N)

// replaces out with in
void ChangeBoards(bool* in, bool* out)
{
    for (int i = 0; i < N * N; i++)
        out[i] = in[i];
}

struct Queen
{
    unsigned int pos;
    bool* board;
    Queen(unsigned int in_pos, bool* in_board)
    {
        pos = in_pos;
        board = new bool[N * N];
        ChangeBoards(in_board, board);
    }

    ~Queen()
    {
        delete[] board;
    }
};

// Sets spaces on the board as taken by a queen
//  at the given position q
void QueenTakesSpace(unsigned int q, bool* board)
{
    int x = q % N;
    int y = q / N;
    for (int i = 0; i < N; i++)
    {
        // horiz
        board[i + (y * N)] = true;
        // vert
        board[x + (i * N)] = true;
        // diag (+)
        if ((x - i) >= 0 && (y - i) >= 0)
            board[(x - i) + ((y - i) * N)] = true;
        if ((x + i) < N * N && (y + i) < N * N)
            board[(x + i) + ((y + i) * N)] = true;
        // diag (-)
        if ((x + i) < N * N && (y - i) >= 0)
            board[(x + i) + ((y - i) * N)] = true;
        if ((x - i) >= 0 && (y + i) < N * N)
            board[(x - i) + ((y + i) * N)] = true;
    }
}

// Finds a position that's not taken in the given column, offset by the given;
//  If no valid position is found, return INVALID_POS
unsigned int QueenInCol(unsigned int col, bool* board, unsigned int offset=0)
{
    for (int i = offset; i < N; i++)
    {
        if (col + (i * N) < N * N && !board[col + (i * N)])
        {
            return col + (i * N);
        }
    }
    return INVALID_POS;
}

// Prints the board with (-) for taken spaces,
//  and (+) for open ones. Not used anymore.
void PrintBoard(bool* board)
{
    for (int y = 0; y < N; y++)
    {
        for (int x = 0; x < N; x++)
                std::cout << (board[x+(y * N)] ? "-" : "+") << ' ';
        std::cout << '\n';
    }
}

// Sets the entire board to free
void ClearBoard(bool* board)
{
    for (int i = 0; i < N * N; i++)
        board[i] = false;
}

int main()
{
    // initialize vars
    std::stack<Queen> stack; // queens stack
    bool* board = new bool[N * N]; // board of taken spaces
    ClearBoard(board);
    unsigned int filled = 0; // # of queens already set
    unsigned int offset = 0; // offset for QueenInCol() if backtracked
    unsigned int col = 0; // current column being filled
    unsigned int pos; // current candidate position for queen

    while (filled < N)
    {
        std::cout << "Working on " << filled + 1 << " of " << N << "\n";

        // check if the offset is outside the
        //  board, and return INVALID_POS if so
        if (offset > N)
        {
            pos = INVALID_POS;
        } else {
            // get the next available spot
            //  and reset the offset
            pos = QueenInCol(col, board, offset);
            offset = 0;
        }

        // check if a valid position was found
        if (pos != INVALID_POS)
        {
            // Found: Update the board with new taken spaces,
            //  add queen to stack, + increment filled + col
            std::cout << "Found position for " << filled << " @ (" << pos % N << ", " << pos / N << ")\n";
            QueenTakesSpace(pos, board);
            stack.emplace(pos, board);
            filled++;
            col++;
        } else {
            // Not Found: Backtrack by one, update the board,
            //  and retry the prev. with a new offset
            std::cout << "No valid position for " << filled << ", retrying previous.\n";
            offset = (stack.top().pos / N) + 1;
            stack.pop();
            filled--;
            col--;
            if (!stack.empty()) ChangeBoards(stack.top().board, board);
            else ClearBoard(board);
        }
    }

    // serialize the data to csv for vis + print
    std::ofstream f;
    f.open("data.csv", std::ios::trunc);
    f << N << "\n"; // add board size at top of file

    while (!stack.empty())
    {
        // Print each queen to console + file
        std::cout << "[*] Queen : (" << stack.top().pos % N << ", " << stack.top().pos / N << ")\n";
        f << stack.top().pos % N << ", " << stack.top().pos / N << "\n";
        stack.pop();
    }

    f.close();

    return 0;
}