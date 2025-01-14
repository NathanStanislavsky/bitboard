#pragma once

#include <string>

#include "types.h"
#include "pos.h"


using namespace std;

int get_piece_value(Piece piece);

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

void print_bitboard(BB bb);

void testMasks();

inline BB rand64() {
    return ((BB)rand() << 48) | ((BB)rand() << 32) | ((BB)rand() << 16) | (BB)rand();
}