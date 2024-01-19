#pragma once
#include "types.h"
#include <string>

using namespace std;

class Pos {
    BB colors_bbs[2] = {};  
    BB pieces_bbs[6] = {};

    Color turn = WHITE;


    Pos(string fen);

    void add_piece(Color color, Piece piece, Square square);

    void remove_piece(Color color, Piece piece, Square square);
};