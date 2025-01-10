#pragma once

namespace PieceSquareTable
{
    extern const int pawns[64];
    extern const int knights[64];
    extern const int bishops[64];
    extern const int rooks[64];
    extern const int queens[64];
    extern const int kingMiddle[64];
    extern const int kingEnd[64];

    int readValue(const int *table, int square, bool isBlack);

}