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
    Pos pos("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    pos.print_board();

    Timestamp start = get_current_ms();

    cout << perft(pos, 6, true) << endl;

    print_time_diff(start);

    // test_fens();

    return 0;
}