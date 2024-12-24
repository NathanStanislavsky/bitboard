#include <iostream>
#include <vector>
#include "pos.h"
#include "move_gen.h"
#include "move.h"
#include "types.h"

int main()
{
    init_leapers_attacks();

    Pos pos("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    pos.print_board();

    int depth = 2;

    uint64_t total = perft_divide(pos, depth);

    return 0;
}