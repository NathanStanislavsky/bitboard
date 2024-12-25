#include <iostream>
#include <vector>
#include "pos.h"
#include "move_gen.h"
#include "move.h"
#include "types.h"

int main()
{
    Pos pos("8/8/8/3p4/4P3/8/8/8 b - - 0 1");
    pos.print_board();

    cout << perft(pos, 2, true) << endl;
    
    return 0;
}