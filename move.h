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