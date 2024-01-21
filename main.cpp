#include "utilities.h"
#include "types.h"
#include "pos.h"
#include "castling.h"


int main() {
    Pos pos("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    pos.print_board();
    
    return 0;
}