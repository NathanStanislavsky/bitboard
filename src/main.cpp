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
    init_leapers_attacks();

    // Pos pos("");
    // pos.print_board();

    // Timestamp start = get_current_ms();

    // cout << perft(pos, 6, true) << endl;

    // print_time_diff(start);

    test_fens();

    return 0;
}