#include <iostream>
#include <vector>

#include "pos.h"
#include "move_gen.h"
#include "move.h"
#include "types.h"
#include "test.h"
#include "timer.h"
#include "search.h"

int main()
{
    init_leapers_attacks();

    Pos pos("3r4/8/8/4k3/8/8/5K2/3r4 b - - 30 16");
    pos.print_board();

    int depth = 8;

    Timestamp start = get_current_ms();

    Move best_move = get_best_move(pos, depth);

    print_time_diff(start);

    std::string best_move_str = move_to_string(best_move);

    std::cout << "Best Move: " << best_move_str << std::endl;

    // std::cout << search(pos, depth, -2147483647, 2147483647) << std::endl;


    // perft(pos, 8, true);
    return 0;
}