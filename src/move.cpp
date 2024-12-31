#include "move.h"
#include "types.h"
#include "utilities.h"

MoveFlag flags(Move move)
{
    return move >> 12;
}

bool is_capture(Move move)
{
    return flags(move) & CAPTURE;
}

bool is_double_pawn_push(Move move)
{
    return flags(move) == DOUBLE_PAWN_PUSH;
}

bool is_enpassant(Move move)
{
    return flags(move) == EP;
}

bool is_king_castle(Move move)
{
    return flags(move) == KING_CASTLE;
}

bool is_promotion(Move move)
{
    return flags(move) & N_PROM;
}

bool is_queen_castle(Move move)
{
    return flags(move) == QUEEN_CASTLE;
}

Square to_square(Move move)
{
    return Square(move & 0b111111);
}

Square from_square(Move move)
{
    return Square((move >> 6) & 0b111111);
}

Piece promotion_piece(Move move)
{
    return Piece((flags(move) & 0b0011) + KNIGHT);
}

Square capture_square(Move move)
{
    Square from_square_int = from_square(move);
    Square to_square_int = to_square(move);
    return is_enpassant(move) ? square_of(from_square_int / 8, to_square_int % 8) : to_square_int;
}
