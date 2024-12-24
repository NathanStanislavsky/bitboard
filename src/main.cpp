#include <iostream>
#include <vector>
#include "pos.h"        // Contains your Pos class, definitions, etc.
#include "move_gen.h"   // Contains generate_pseudolegal_moves, init_leapers_attacks, etc.
#include "move.h"       // Contains make_move, from_square, to_square, etc.
#include "types.h"      // Contains Color, Piece, Square, etc.

int main()
{
    Pos pos("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    init_leapers_attacks();

    std::vector<Move> pseudoMoves = generate_psuedo_moves(pos);

    std::cout << "Pseudolegal moves:\n";
    for (Move m : pseudoMoves)
    {
        std::cout << move_to_string(m) << " "; // or however you print a move
    }
    std::cout << std::endl;

    // 5) (Optional) Filter them to get only legal moves
    //    This requires you have a function like generate_legal_moves(pos).
    //    If you haven’t written that yet, skip it for now.
    /*
    std::vector<Move> legalMoves = generate_legal_moves(pos);
    std::cout << "Legal moves:\n";
    for (Move m : legalMoves)
    {
        std::cout << move_to_string(m) << " ";
    }
    std::cout << std::endl;
    */

    return 0;
}