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

    Pos pos("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    pos.print_board();

    int depth = 7;

    Move best_move = get_best_move(pos, depth);

    std::string best_move_str = move_to_string(best_move);

    std::cout << "Best Move: " << best_move_str << std::endl;

    return 0;
}