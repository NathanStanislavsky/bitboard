#pragma once

#include "types.h"
#include <vector>
#include "pos.h"
#include "utilities.h"

// Generate pawn attacks
BB mask_pawn_attacks(Color side, Square square);
BB mask_knight_attacks(Square square);
BB mask_king_attacks(Square square);
BB mask_bishop_attacks(int square, BB block);
BB mask_rook_attacks(int square, BB block);
BB mask_queen_attacks(int square, BB block);

// Move generation
std::vector<Move> generate_psuedo_moves(const Pos &pos);
std::vector<Move> generate_legal_moves(Pos &pos);

int perft(Pos &pos, int depth, bool verbose);