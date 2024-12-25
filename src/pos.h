#pragma once
#include "types.h"
#include <string>
#include "utilities.h"
#include "castling.h"
#include <vector>
#include "move.h"

using namespace std;

class Pos
{
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

    Specific_Piece specific_piece_on(Square square);

    void do_move(Move move);

    void undo_move();

    void print_logs();

    vector<Specific_Piece> piece_captured_log;
    vector<Castling> castling_rights_log;
    vector<Square> enpassant_square_log;
    vector<Move> move_log;

    bool is_square_attacked(Square sq, Color side) const;

    bool is_in_check(Color side) const;

    void debug_bitboards();
};