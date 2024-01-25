#pragma once

#include <stdint.h>

typedef unsigned long long BB;

typedef uint16_t Move;
typedef uint8_t MoveFlag;

enum Color { BLACK, WHITE, NO_COLOR };

enum Piece { PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING, EMPTY};

enum Square { A1, B1, C1, D1, E1, F1, G1, H1,
              A2, B2, C2, D2, E2, F2, G2, H2,
              A3, B3, C3, D3, E3, F3, G3, H3, 
              A4, B4, C4, D4, E4, F4, G4, H4, 
              A5, B5, C5, D5, E5, F5, G5, H5, 
              A6, B6, C6, D6, E6, F6, G6, H6, 
              A7, B7, C7, D7, E7, F7, G7, H7, 
              A8, B8, C8, D8, E8, F8, G8, H8, NONE_SQUARE };

enum Move_Flag {QUIET=0, DOUBLE_PAWN_PUSH, KING_CASTLE, QUEEN_CASTLE, CAPTURE, EP, N_PROM=8, B_PROM, R_PROM, Q_PROM, N_PROM_CAPTURE, B_PROM_CAPTURE, R_PROM_CAPTURE, Q_PROM_CAPTURE};

enum Specific_Piece { BLACK_PAWN, BLACK_KNIGHT, BLACK_BISHOP, BLACK_ROOK, BLACK_QUEEN, BLACK_KING, WHITE_PAWN, WHITE_KNIGHT, WHITE_BISHOP, WHITE_ROOK, WHITE_QUEEN, WHITE_KING, S_EMPTY };