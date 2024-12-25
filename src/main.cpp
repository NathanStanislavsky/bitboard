#include <iostream>
#include <vector>
#include "pos.h"
#include "move_gen.h"
#include "move.h"
#include "types.h"

int main()
{

    Pos pos("8/8/8/3p4/4P3/8/8/8 b - - 0 1");

    pos.print_board();

    // testMasks();

    // pos.do_move(make_move(E2, E4));
    // pos.do_move(make_move(E7, E5));
    // pos.do_move(make_move(D2, D4));
    // pos.do_move(make_move(D7, D5));
    // pos.do_move(make_move(B1, C3));
    // pos.do_move(make_move(B8, C6));
    // pos.do_move(make_move(C3, D5));

    int depth = 2;

    uint64_t total = perft_divide(pos, depth);

    return 0;
}