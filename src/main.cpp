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
#include "transposition.h"

int main()
{
    init_leapers_attacks();
    init_zobrist_keys();
    TranspositionTable tt(1 << 24);

    Pos pos("7r/3r4/4k3/8/5K2/8/8/8 b - - 81 42");
    pos.print_board();

    int depth = 12;
    int INF = 2147483647;

    Timestamp start = get_current_ms();

    auto [score, bestMove] = search(pos, depth, -INF, INF, 0, tt);

    print_time_diff(start);

    std::cout << "Best move: " << move_to_string(bestMove)
              << " with eval: " << score << std::endl;

    return 0;
}
