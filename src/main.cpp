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

    Pos pos("3r4/3r4/4k3/8/8/4K3/8/8 b - - 79 41");
    pos.print_board();

    int depth = 9;
    int INF = 2147483647;

    Timestamp start = get_current_ms();

    auto [score, bestMove] = search(pos, depth, -INF, INF, 0);

    print_time_diff(start);

    std::cout << "Best move: " << move_to_string(bestMove)
              << " with eval: " << score << std::endl;

    return 0;
}