#include <iostream>
#include <vector>

#include "pos.h"
#include "move_gen.h"
#include "move.h"
#include "types.h"
#include "test.h"
#include "timer.h"
#include "search.h"
#include "zobrist.h"

int main()
{
    init_leapers_attacks();
    init_zobrist_keys();

    Pos pos("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    pos.print_board();

    std::cout << compute_zobrist_hash(pos) << std::endl;

    // int depth = 8;
    // int INF = 2147483647;

    // Timestamp start = get_current_ms();

    // Move best_move = get_best_move(pos, depth);

    // print_time_diff(start);

    // std::string best_move_str = move_to_string(best_move);

    // std::cout << "Best Move: " << best_move_str << std::endl;

    // std::cout << search(pos, depth, -INF, INF) << std::endl;


    // std::cout << perft(pos, 8, true) << std::endl;

    return 0;
}