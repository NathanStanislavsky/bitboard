#include "move_gen.h"
#include "types.h"

#define get_bit(bitboard, square) (bitboard & (1ULL << square))
#define set_bit(bitboard, square) (bitboard |= (1ULL << square))
#define remove_bit(bitboard, square) (get_bit(bitboard, square) ? bitboard ^= (1ULL << square) : 0)

// A column
const BB not_A_column = 18374403900871474942ULL;

// H column
const BB not_H_column = 9187201950435737471ULL;

// HG column
const BB not_HG_column = 4557430888798830399ULL;

// AB column
const BB not_AB_column = 18229723555195321596ULL;

// Generate pawn attacks
BB mask_pawn_attacks(Color side, Square square) {
    // attacks result bitboard
    BB attacks = 0ULL;

    // piece bitboard
    BB bitboard = 0ULL;

    // set piece on board
    set_bit(bitboard, square);
    
    // white pawns
    if (!side) {
        if ((bitboard << 7) & not_H_column) {
            attacks |= (bitboard << 7);
        }

        if ((bitboard << 9) & not_A_column) {
            attacks |= (bitboard << 9);
        }
    }
    // black pawns
    else {
        if ((bitboard >> 7) & not_A_column) {
            attacks |= (bitboard >> 7);
        }

        if ((bitboard >> 9) & not_H_column) {
            attacks |= (bitboard >> 9);
        }
    }


    // return attack map
    return attacks;
}

BB mask_knight_attacks(Square square) {
    // attacks result bitboard
    BB attacks = 0ULL;

    // piece bitboard
    BB bitboard = 0ULL;

    // set piece on board
    set_bit(bitboard, square);

    // Generate knight attacks
    if ((bitboard >> 17) & not_H_column) {
        attacks |= (bitboard >> 17);
    }
    if ((bitboard >> 15) & not_A_column) {
        attacks |= (bitboard >> 15);
    }
    if ((bitboard >> 10) & not_HG_column) {
        attacks |= (bitboard >> 10);
    }
    if ((bitboard >> 6) & not_AB_column) {
        attacks |= (bitboard >> 6);
    }
    if ((bitboard << 17) & not_A_column) {
        attacks |= (bitboard << 17);
    }
    if ((bitboard << 15) & not_H_column) {
        attacks |= (bitboard << 15);
    }
    if ((bitboard << 10) & not_AB_column) {
        attacks |= (bitboard << 10);
    }
    if ((bitboard << 6) & not_HG_column) {
        attacks |= (bitboard << 6);
    }

    // return attack map
    return attacks;
}

// Generate pawn attacks
BB mask_king_attacks(Square square) {
    // attacks result bitboard
    BB attacks = 0ULL;

    // piece bitboard
    BB bitboard = 0ULL;

    // set piece on board
    set_bit(bitboard, square);
    
    if (bitboard >> 8) {
        attacks |= (bitboard >> 8);
    }
    if ((bitboard >> 9) & not_H_column) {
        attacks |= (bitboard >> 9);
    }
    if ((bitboard >> 7) & not_A_column) {
        attacks |= (bitboard >> 7);
    }
    if ((bitboard >> 1) & not_H_column) {
        attacks |= (bitboard >> 1);
    }

    if (bitboard << 8) {
        attacks |= (bitboard << 8);
    }
    if ((bitboard << 9) & not_A_column) {
        attacks |= (bitboard << 9);
    }
    if ((bitboard << 7) & not_H_column) {
        attacks |= (bitboard << 7);
    }
    if ((bitboard << 1) & not_A_column) {
        attacks |= (bitboard << 1);
    }
    
    // return attack map
    return attacks;
}

// generate leaper piece attacks
void init_leapers_attacks() {
    for (int square = 0; square < 64; square++) {
        pawn_attacks[WHITE][square] = mask_pawn_attacks(WHITE, Square(square));
        pawn_attacks[BLACK][square] = mask_pawn_attacks(BLACK, Square(square));

        knight_attacks[square] = mask_knight_attacks(Square(square));

        king_attacks[square] = mask_king_attacks(Square(square));
    }
}
