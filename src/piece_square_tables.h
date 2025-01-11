#pragma once

namespace PieceSquareTable
{
    extern const int pawns_eval_table[64];
    extern const int knights_eval_table[64];
    extern const int bishops_eval_table[64];
    extern const int rooks_eval_table[64];
    extern const int queens_eval_table[64];
    extern const int king_middle_eval_table[64];
    extern const int king_end_eval_table[64];

    int readValue(const int *table, int square, bool isWhite);

}