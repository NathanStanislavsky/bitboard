#pragma once

#include "types.h"
#include "pos.h"

int quiescence_search(Pos &pos, int alpha, int beta);

void move_order(Pos &pos, std::vector<Move> &moves);

int search(Pos &pos, int depth, int alpha, int beta, int ply);

Move get_best_move(Pos &pos, int depth);