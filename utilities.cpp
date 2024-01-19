#include "utilities.h"

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