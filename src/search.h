#pragma once

int eval(Pos& pos);

int count_material(Pos& pos, Color side);

int search(Pos& pos, int depth);