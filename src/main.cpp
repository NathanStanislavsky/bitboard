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

    int depth = 5;
    int alpha = -10000;
    int beta = 10000;

    int evaluation_score = search(pos, depth, alpha, beta);

    std::cout << "Evaluation Score: " << evaluation_score << std::endl;

    return 0;
}