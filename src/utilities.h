#pragma once
#include "types.h"
#include <string>
#include <map>

using namespace std;

Piece char_to_piece(char c);

Color color_of(char piece);

Square string_to_square(string str);

Square square_of(int rank, int file);

char piece_to_char_color(Piece piece, Color color);

BB bb_has(BB board, Square square);

BB bb_of(Square square);

Piece specific_piece_to_piece(Specific_Piece spiece);

string move_to_string(Move move);

string square_to_string(Square square);

char specific_piece_to_char(Specific_Piece spiece);