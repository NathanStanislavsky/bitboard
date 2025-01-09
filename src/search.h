#pragma once

#include "types.h"
#include "pos.h"
#include "transposition.h"

int quiescence_search(Pos &pos, int alpha, int beta);

void move_order(Pos &pos, std::vector<Move> &moves);

std::pair<int, Move> search(Pos &pos, int depth, int alpha, int beta, int ply, TranspositionTable &tt);
