#include "utilities.h"
#include <string>

using namespace std;

map<char, Piece> c_to_p = {
    {'P', PAWN},
    {'R', ROOK},
    {'N', KNIGHT},
    {'B', BISHOP},
    {'Q', QUEEN},
    {'K', KING},
    {'p', PAWN},
    {'r', ROOK},
    {'n', KNIGHT},
    {'b', BISHOP},
    {'q', QUEEN},
    {'k', KING}
}; 

Piece char_to_piece(char c) {
    return c_to_p[c];
}

Color color_of(char c) {
    if (c >= 'A' && c <= 'Z') {
        return WHITE;
    } else {
        return BLACK;
    }
}

Square string_to_square(string str) {
    return square_of(str[1] - '1', str[0] - 'a');
}

Square square_of(int rank, int file) {
    return Square(rank * 8 + file);
}