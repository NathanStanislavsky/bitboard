#include <iostream>
#include <vector>
#include "pos.h"
#include "move_gen.h"
#include "move.h"
#include "types.h"

int main()
{
    Pos pos("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    init_leapers_attacks();

    std::vector<Move> pseudoMoves = generate_psuedo_moves(pos);

    std::cout << "Pseudolegal moves:\n";
    for (Move m : pseudoMoves)
    {
        std::cout << move_to_string(m) << " ";
    }
    std::cout << std::endl;

    std::vector<Move> legalMoves = generate_legal_moves(pos);
    std::cout << "Legal moves:\n";
    for (Move m : legalMoves)
    {
        std::cout << move_to_string(m) << " ";
    }
    std::cout << std::endl;

    return 0;
}