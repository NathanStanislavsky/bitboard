#include <iostream>
#include <vector>
#include "pos.h"
#include "move_gen.h"
#include "move.h"
#include "types.h"
#include "test.h"
#include "timer.h"

int main()
{
    // Pos pos("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
    // pos.print_board();

    // Timestamp start = get_current_ms();

    // cout << perft(pos, 5, true) << endl;

    // print_time_diff(start);

    test_fens();

    return 0;
}