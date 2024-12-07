#include "pos.h"
#include <string>
#include <iostream>
#include "types.h"
#include "utilities.h"
#include "move.h"

using namespace std;

Pos::Pos(string fen)
{
    int fen_index = 0;
    int rank = 7;
    int file = 0;

    while (rank != 0 || file != 8)
    {
        char c = fen[fen_index++];

        if (c == '/')
        {
            file = 0;
            rank--;
        }
        else if (c >= '0' && c <= '9')
        {
            file += c - '0';
        }
        else
        {
            Color color = color_of(c);
            Piece piece = char_to_piece(c);
            Square square = Square(rank * 8 + file);

            add_piece(color, piece, square);

            file++;
        }
    }

    fen = fen.substr(fen_index);

    turn = fen.find('w') != string::npos ? WHITE : BLACK;

    if (fen.find('K') != string::npos)
    {
        cr.wkc = true;
    }
    if (fen.find('k') != string::npos)
    {
        cr.bkc = true;
    }
    if (fen.find('Q') != string::npos)
    {
        cr.wqc = true;
    }
    if (fen.find('q') != string::npos)
    {
        cr.bqc = true;
    }

    for (int i = 0; i < 3; i++)
    {
        fen = fen.substr(fen.find(" ") + 1);
    }

    if (fen[0] != '-')
    {
        enpassant_sq = string_to_square(fen.substr(0, 2));
        cout << enpassant_sq << endl;
    }
};

void Pos::add_piece(Color color, Piece piece, Square square)
{
    colors_bbs[color] |= 1ULL << square;
    pieces_bbs[piece] |= 1ULL << square;
}

void Pos::remove_piece(Color color, Piece piece, Square square)
{
    colors_bbs[color] &= 1ULL << square;
    pieces_bbs[piece] &= 1ULL << square;
}

void Pos::print_board()
{
    for (int r = 0; r < 8; r++)
    {
        for (int c = 0; c < 8; c++)
        {
            Square square = square_of(r, c);
            Color color = color_on(square);
            Piece piece = piece_on(square);

            cout << piece_to_char_color(piece, color);

            cout << " ";
        }
        cout << "\n";
    }
    cout << "\n";
}

Color Pos::color_on(Square square)
{
    for (int i = 0; i < 2; i++)
    {

        if (bb_has(colors_bbs[i], square))
        {
            return Color(i);
        }
    }
    return NO_COLOR;
}

Piece Pos::piece_on(Square square)
{
    for (int i = 0; i < 6; i++)
    {
        if (bb_has(pieces_bbs[i], square))
        {
            return Piece(i);
        }
    }
    return EMPTY;
}

Specific_Piece Pos::specific_piece_on(Square square)
{
    return Specific_Piece((color_on(square) * 6) + piece_on(square));
}

void Pos::do_move(Move move)
{
    piece_captured_log.push_back(specific_piece_on(to_square(move)));
    castling_rights_log.push_back(cr);
    enpassant_square_log.push_back(enpassant_sq);
    move_log.push_back(move);

    if (specific_piece_on(from_square(move)) == WHITE_KING)
    {
        cr.wkc = false;
        cr.wqc = false;
    }
    if (specific_piece_on(from_square(move)) == BLACK_KING)
    {
        cr.bkc = false;
        cr.bqc = false;
    }
    if (specific_piece_on(from_square(move)) == WHITE_ROOK)
    {
        if (from_square(move) == H1)
        {
            cr.wkc = false;
        }
        else if (from_square(move) == A1)
        {
            cr.wqc = false;
        }
    }
    if (specific_piece_on(from_square(move)) == BLACK_ROOK)
    {
        if (from_square(move) == H8)
        {
            cr.bkc = false;
        }
        else if (from_square(move) == A8)
        {
            cr.bqc = false;
        }
    }

    enpassant_sq = is_double_pawn_push(move) ? Square((from_square(move) + to_square(move)) / 2) : NONE_SQUARE;

    if (is_promotion(move))
    {
        add_piece(turn, promotion_piece(move), to_square(move));
    }
    else if (is_enpassant(move))
    {
        int direction = (turn == WHITE) ? 1 : -1;
        add_piece(turn, piece_on(from_square(move)), to_square(move));
        remove_piece(color_on(Square(to_square(move) + direction * 8)), piece_on(Square(to_square(move) + direction * 8)), Square(to_square(move) + direction * 8));
    }
    else if (is_king_castle(move))
    {
        add_piece(turn, piece_on(from_square(move)), to_square(move));
        add_piece(turn, ROOK, Square(from_square(move) - 1));
        remove_piece(turn, ROOK, Square(from_square(move) + 3));
    }
    else if (is_queen_castle(move))
    {
        add_piece(turn, piece_on(from_square(move)), to_square(move));
        add_piece(turn, ROOK, Square(from_square(move) + 1));
        remove_piece(turn, ROOK, Square(from_square(move) - 4));
    }
    else
    {
        add_piece(turn, piece_on(from_square(move)), to_square(move));
    }

    remove_piece(turn, piece_on(from_square(move)), from_square(move));
    turn = Color(!turn);
}

void Pos::undo_move()
{
    Move move = move_log.back();
    move_log.pop_back();

    cr = castling_rights_log.back();
    castling_rights_log.pop_back();

    enpassant_sq = enpassant_square_log.back();
    enpassant_square_log.pop_back();

    if (is_promotion(move))
    {
        add_piece(Color(!turn), PAWN, from_square(move));
        if (piece_captured_log.back() != S_EMPTY)
        {
            add_piece(Color(turn), specific_piece_to_piece(piece_captured_log.back()), to_square(move));
        }
        remove_piece(Color(!turn), promotion_piece(move), to_square(move));
    }
    else if (is_enpassant(move))
    {
        int offset = (turn == WHITE) ? 8 : -8;
        add_piece(Color(!turn), PAWN, from_square(move));
        remove_piece(Color(!turn), PAWN, to_square(move));
        add_piece(turn, specific_piece_to_piece(piece_captured_log.back()), Square(to_square(move) + offset));
    }
    else if (is_king_castle(move))
    {
        add_piece(Color(!turn), KING, from_square(move));
        remove_piece(Color(!turn), KING, to_square(move));
        add_piece(Color(!turn), ROOK, Square(from_square(move) + 3));
        remove_piece(Color(!turn), ROOK, Square(from_square(move) + 1));
    }
    else if (is_queen_castle(move))
    {
        add_piece(Color(!turn), KING, from_square(move));
        remove_piece(Color(!turn), KING, to_square(move));
        add_piece(Color(!turn), ROOK, Square(from_square(move) - 4));
        remove_piece(Color(!turn), ROOK, Square(from_square(move) - 1));
    }
    else
    {
        Piece piece_moved = piece_on(to_square(move));
        add_piece(Color(!turn), piece_on(to_square(move)), from_square(move));
        if (piece_captured_log.back() != S_EMPTY)
        {
            add_piece(Color(turn), specific_piece_to_piece(piece_captured_log.back()), to_square(move));
        }
        remove_piece(Color(!turn), piece_moved, to_square(move));
    }

    piece_captured_log.pop_back();
    turn = Color(!turn);
}

void Pos::print_logs()
{
    for (int i = 0; i < piece_captured_log.size(); i++)
    {
        cout << specific_piece_to_char(piece_captured_log[i]) + string(" ");
    }
    cout << "" << endl;

    for (int i = 0; i < castling_rights_log.size(); i++)
    {
        cout << to_string(castling_rights_log[i].wkc) + to_string(castling_rights_log[i].wqc) + to_string(castling_rights_log[i].bkc) + to_string(castling_rights_log[i].bqc) + " ";
    }
    cout << "" << endl;

    for (int i = 0; i < enpassant_square_log.size(); i++)
    {
        cout << to_string(enpassant_square_log[i]) + " ";
    }
    cout << "" << endl;

    for (int i = 0; i < move_log.size(); i++)
    {
        cout << move_to_string(move_log[i]) + " ";
    }
    cout << "" << endl;
}
