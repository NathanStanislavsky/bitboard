#include <iostream>
#include <vector>
#include "pos.h"
#include "move_gen.h"
#include "move.h"
#include "types.h"

int main()
{
    // Suppose you have a standard FEN for the initial position
    Pos pos("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    pos.print_board();

    // Depth at which you want to do the 'divide' output
    int depth = 2; // or 2, 3, etc.

    // This will print each move and how many positions it generates
    uint64_t total = perft_divide(pos, depth);

    // Optionally compare 'total' with known perft results
    // e.g., perft(1) = 20 from the initial position, etc.

    return 0;
}