#pragma once

#include "types.h"
#include "utilities.h"
#include <cassert>

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

inline Move generate_move(Square from, Square to, MoveFlag flags = QUIET)
{
    assert(from >= 0 && from < 64);
    assert(to >= 0 && to < 64);
    assert(flags >= QUIET && flags <= Q_PROM_CAPTURE);
    return ((flags << 12) | (from << 6) | to);
}