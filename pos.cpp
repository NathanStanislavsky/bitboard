#include "pos.h"
#include <string>
#include <iostream>
#include "types.h"
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
        } else if (c >= '0' && c <= '9') {
            file += c - '0';
        } else {
            Color color = color_of(c);
            Piece piece = char_to_piece(c);
            Square square = Square(rank * 8 + file);

            add_piece(color, piece, square);

            file++;
        }
    }

    fen = fen.substr(fen_index);

    turn = fen.find('w') != string::npos ? WHITE : BLACK;

    if (fen.find('K') != string::npos) {
        cr.wkc = true;
    }
    if (fen.find('k') != string::npos) {
        cr.bkc = true;
    }
    if (fen.find('Q') != string::npos) {
        cr.wqc = true;
    }
    if (fen.find('q') != string::npos) {
        cr.bqc = true;
    }

    for (int i = 0; i < 3; i++) {
        fen = fen.substr(fen.find(" ") + 1);
    }
    
    if (fen[0] != '-') {
        enpassant_sq = string_to_square(fen.substr(0, 2));
        cout << enpassant_sq << endl;
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