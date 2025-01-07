#include "zobrist.h"
#include "types.h"
#include "utilities.h"

BB piece_square_table[N_COLORS][N_PIECES][N_SQUARES];
BB enpassant_table[N_SQUARES + 1];
BB castling_table[N_SQUARES];
BB white_to_move;

void init_zobrist_keys()
{
    srand(38217392);

    for (int square = 0; square <= 63; square++)
    {
        for (Color color : {WHITE, BLACK})
            for (int piece = 0; piece <= 5; piece++)
                piece_square_table[color][piece][square] = rand64();

        enpassant_table[square] = rand64();
        castling_table[square] = rand64();
    }

    white_to_move = rand64();

    enpassant_table[NONE_SQUARE] = 0;
}

BB compute_zobrist_hash(Pos &pos)
{
    BB zobrist_key = 0;

    for (int square = 0; square <= 63; square++)
    {
        Piece piece = pos.piece_on(Square(square));
        if (piece != EMPTY)
        {
            Color color = pos.color_on(Square(square));
            zobrist_key ^= piece_square_table[color][piece][square];
        }
    }

    if (pos.enpassant_sq != NONE_SQUARE)
    {
        zobrist_key ^= enpassant_table[pos.enpassant_sq];
    }

    zobrist_key ^= castling_table[pos.get_castling_rights()];

    if (pos.turn == WHITE)
    {
        zobrist_key ^= white_to_move;
    }

    return zobrist_key;
}