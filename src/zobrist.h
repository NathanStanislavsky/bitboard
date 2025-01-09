#pragma once

#include "types.h"
#include "pos.h"

extern BB piece_square_table[N_COLORS][N_PIECES][N_SQUARES];
extern BB enpassant_table[N_SQUARES + 1];
extern BB castling_table[N_SQUARES];
extern BB white_to_move;

void init_zobrist_keys();

BB compute_zobrist_hash(Pos &pos);
