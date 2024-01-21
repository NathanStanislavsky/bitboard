#pragma once
#include "types.h"
#include <string>
#include "utilities.h"
#include "castling.h"

using namespace std;

class Pos {
    public:
        BB colors_bbs[2] = {};  
        BB pieces_bbs[6] = {};

        Color turn = WHITE;
        Castling cr;
        Square enpassant_sq;


        Pos(string fen);

        void add_piece(Color color, Piece piece, Square square);

        void remove_piece(Color color, Piece piece, Square square);

        void print_board();

        Color color_on(Square square);

        Piece piece_on(Square square);
};