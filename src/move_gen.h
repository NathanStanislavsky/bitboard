#include "types.h"

// Pawn attacks [side][square]
BB pawn_attacks[2][64];

BB knight_attacks[64];

BB king_attacks[64];

// Generate pawn attacks
BB mask_pawn_attacks(Color side, Square square);

BB mask_knight_attacks(Square square);

// Generate pawn attacks
BB mask_king_attacks(Square square);

BB mask_king_attacks(Square square);

BB mask_bishop_attacks(int square, BB block);

BB mask_rook_attacks(int square, BB block);

BB mask_queen_attacks(int square, BB block);

// generate leaper piece attacks
void init_leapers_attacks();