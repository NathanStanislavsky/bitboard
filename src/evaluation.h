#pragma once

#include "types.h"
#include "pos.h"

int count_material(Pos &pos, Color side);

int get_material_count_without_pawns(Pos &pos, Color side);

float endgame_phase_weight(int materialCountWithoutPawns);

int force_king_to_corner_endgame_eval(Pos &pos, float endgame_weight);

int piece_square_table_score(Pos &pos, float endgame_weight);

int eval(Pos& pos);