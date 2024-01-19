#include "pos.h"
#include <string>
#include "utilities.h"

using namespace std;

Pos::Pos(string fen) {
    int fen_index = 0;
    int rank = 7;
    int file = 0;

    while(rank != 0 || file != 8) {
        char c = fen[fen_index++];

        if (c == '/') {
            file = 0;
            rank--;
        } else if (c > '0' && c < '9') {
            file += c - '0';
        } else {
            Color color = color_of(c);
            Piece piece = char_to_piece(c);
            Square square = rank * 8 + file;

            add_piece(color, piece, square);

            file++;
        }
    }
};

void Pos::add_piece(Color color, Piece piece, Square square) {
    colors_bbs[color] |= 1ULL << square;
    pieces_bbs[piece] |= 1ULL << square;
}

void Pos::remove_piece(Color color, Piece piece, Square square) {
    colors_bbs[color] &= 1ULL << square;
    pieces_bbs[piece] &= 1ULL << square;
}