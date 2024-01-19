#pragma once
#include "types.h"
#include <string>
#include <map>

using namespace std;

Piece char_to_piece(char c);

Color color_of(char piece);

Square string_to_square(string str);

Square square_of(int rank, int file);