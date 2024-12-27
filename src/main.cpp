#include <iostream>
#include <vector>
#include "pos.h"
#include "move_gen.h"
#include "move.h"
#include "types.h"
#include "test.h"

int main()
{
    Pos pos("r3k2r/Pppp1ppp/1b3nbN/nPB5/B1P1P3/q4N2/Pp1P2PP/R2Q1RK1 b kq - 1 1");

    pos.print_board();

    cout << perft(pos, 1, true) << endl;
    
    return 0;
}