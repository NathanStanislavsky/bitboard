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