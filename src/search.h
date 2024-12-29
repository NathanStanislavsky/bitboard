#pragma once

#include "types.h"
#include "pos.h"
#include "move_gen.h"

int eval(Pos& pos);

int count_material(Pos& pos, Color side);

void move_order(Pos &pos, std::vector<Move> &moves);

int search(Pos &pos, int depth, int alpha, int beta);

Move get_best_move(Pos &pos, int depth);