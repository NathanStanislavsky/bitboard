#pragma once

#include "types.h"
#include "pos.h"

int search_all_captures(Pos &pos, int alpha, int beta);

void move_order(Pos &pos, std::vector<Move> &moves);

int search(Pos &pos, int depth, int alpha, int beta);

Move get_best_move(Pos &pos, int depth);