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

map<pair<Piece, Color>, char> piece_to_char_color_map = {
    {{PAWN, WHITE}, 'P'},
    {{ROOK, WHITE}, 'R'},
    {{KNIGHT, WHITE}, 'N'},
    {{BISHOP, WHITE}, 'B'},
    {{QUEEN, WHITE}, 'Q'},
    {{KING, WHITE}, 'K'},
    {{PAWN, BLACK}, 'p'},
    {{ROOK, BLACK}, 'r'},
    {{KNIGHT, BLACK}, 'n'},
    {{BISHOP, BLACK}, 'b'},
    {{QUEEN, BLACK}, 'q'},
    {{KING, BLACK}, 'k'},
    {{EMPTY, NO_COLOR}, '-'}
};

char piece_to_char_color(Piece piece, Color color) {
    return piece_to_char_color_map[{piece, color}];
}


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

BB bb_has(BB board, Square square) {
    return board & bb_of(square);
}

BB bb_of(Square square) {
    return (1ULL << square);
}
