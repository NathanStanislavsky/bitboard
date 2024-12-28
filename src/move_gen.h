#pragma once

#include "types.h"
#include <vector>
#include "pos.h"
#include "utilities.h"

// Masking
BB mask_pawn_attacks(Color side, Square square);
BB mask_knight_attacks(Square square);
BB mask_king_attacks(Square square);
BB mask_bishop_attacks(int square, BB block);
BB mask_rook_attacks(int square, BB block);
BB mask_queen_attacks(int square, BB block);

void init_leapers_attacks();

// lookup tables for leaper pieces
BB pawn_attacks[2][64];
BB knight_attacks[64];
BB king_attacks[64];

// Move generation
std::vector<Move> generate_psuedo_moves(const Pos &pos);
std::vector<Move> generate_legal_moves(Pos &pos);

// Perft test
int perft(Pos &pos, int depth, bool verbose);