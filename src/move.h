#pragma once

#include "types.h"
#include "utilities.h"

MoveFlag flags(Move move);

bool is_capture(Move move);
bool is_double_pawn_push(Move move);
bool is_enpassant(Move move);
bool is_king_castle(Move move);
bool is_promotion(Move move);
bool is_queen_castle(Move move);

Square to_square(Move move);
Square from_square(Move move);
Piece promotion_piece(Move move);
Square capture_square(Move move);

inline Move make_move(Square from, Square to, MoveFlag flags = QUIET)
{
    // from is shifted left by 6, to is in bits 0–5, flags in bits 12–15
    return static_cast<Move>((static_cast<int>(from) << 6) 
                           | (static_cast<int>(to) & 0x3F) 
                           | (static_cast<int>(flags) << 12));
}