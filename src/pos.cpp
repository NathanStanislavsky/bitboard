#include "pos.h"
#include <string>
#include <iostream>
#include "types.h"
#include "utilities.h"
#include "move.h"
#include "move_gen.h"

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
    colors_bbs[color] &= ~(1ULL << square);
    pieces_bbs[piece] &= ~(1ULL << square);
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

    Square from = from_square(move);
    Square to   = to_square(move);

    Specific_Piece movedSP    = specific_piece_on(from);
    Specific_Piece capturedSP = specific_piece_on(to); 
    Color sideMoving          = turn;
    Piece movedPiece          = specific_piece_to_piece(movedSP);

    // if the king or rook moves, we lose castling rights
    if (movedSP == WHITE_KING)
    {
        cr.wkc = false;
        cr.wqc = false;
    }
    if (movedSP == BLACK_KING)
    {
        cr.bkc = false;
        cr.bqc = false;
    }
    if (movedSP == WHITE_ROOK)
    {
        if (from == H1)
        {
            cr.wkc = false;
        }
        else if (from == A1)
        {
            cr.wqc = false;
        }
    }
    if (movedSP == BLACK_ROOK)
    {
        if (from == H8)
        {
            cr.bkc = false;
        }
        else if (from == A8)
        {
            cr.bqc = false;
        }
    }
    // Check if a rook is captured and update castling rights
    if (capturedSP == BLACK_ROOK)
    {
        if (to == H8)
        {
            cr.bkc = false;
        }
        else if (to == A8)
        {
            cr.bqc = false;
        }
    }
    if (capturedSP == WHITE_ROOK)
    {
        if (to == H1)
        {
            cr.wkc = false;
        }
        else if (to == A1)
        {
            cr.wqc = false;
        }
    }

    if (is_double_pawn_push(move))
    {
        enpassant_sq = Square((from + to) / 2);
    }
    else
    {
        enpassant_sq = NONE_SQUARE;
    }

    remove_piece(sideMoving, movedPiece, from);

    if (is_promotion(move))
    {
        add_piece(sideMoving, promotion_piece(move), to);
    }
    else if (is_enpassant(move))
    {
        add_piece(sideMoving, movedPiece, to);
        int direction = (turn == WHITE) ? 1 : -1;
        Square capSq  = Square(to + direction * 8); 
        remove_piece(color_on(capSq), piece_on(capSq), capSq);
    }
    else if (is_king_castle(move))
    {
        add_piece(sideMoving, movedPiece, to);
        remove_piece(sideMoving, ROOK, Square(from + 3));
        add_piece(sideMoving, ROOK, Square(from + 1));
    }
    else if (is_queen_castle(move))
    {
        add_piece(sideMoving, movedPiece, to);
        remove_piece(sideMoving, ROOK, Square(from - 4));
        add_piece(sideMoving, ROOK, Square(from - 1));
    }
    else
    {
        add_piece(sideMoving, movedPiece, to);
    }

    turn = Color(!turn);
}

void Pos::undo_move()
{
    // Retrieve the last move
    Move move = move_log.back();
    move_log.pop_back();

    // Restore castling rights & enpassant from logs
    cr = castling_rights_log.back();
    castling_rights_log.pop_back();

    enpassant_sq = enpassant_square_log.back();
    enpassant_square_log.pop_back();

    // Retrieve which piece was captured
    // This is the piece that was on 'to_square' (could be S_EMPTY if no capture)
    Specific_Piece capturedSP = piece_captured_log.back();
    piece_captured_log.pop_back();

    Square from = from_square(move);
    Square to   = to_square(move);

    // Flip turn back
    turn = Color(!turn);

    // Identify what piece moved (on the board after 'do_move')
    Specific_Piece movedSP = specific_piece_on(to);
    Piece movedPiece       = specific_piece_to_piece(movedSP);

    // Reverse the special moves
    if (is_promotion(move))
    {
        // Remove the promoted piece from 'to'
        remove_piece(turn, promotion_piece(move), to);

        // Restore the pawn on 'from'
        add_piece(turn, PAWN, from);

        // If a capture occurred, restore it
        if (capturedSP != S_EMPTY)
        {
            add_piece(Color(!turn), specific_piece_to_piece(capturedSP), to);
        }
    }
    else if (is_enpassant(move))
    {
        // Remove the moved pawn from 'to'
        remove_piece(turn, movedPiece, to);

        // Place the pawn back on 'from'
        add_piece(turn, movedPiece, from);

        // Restore the captured enemy pawn behind 'to'
        int offset = (turn == WHITE) ? -8 : 8;
        Square capSq = Square(to + offset);

        add_piece(Color(!turn), specific_piece_to_piece(capturedSP), capSq);
    }
    else if (is_king_castle(move))
    {
        // Remove king from 'to', place it back on 'from'
        remove_piece(turn, KING, to);
        add_piece(turn, KING, from);

        // Move rook back from (from+1) to (from+3)
        remove_piece(turn, ROOK, Square(from + 1));
        add_piece(turn, ROOK, Square(from + 3));
    }
    else if (is_queen_castle(move))
    {
        // Remove king from 'to', place it back on 'from'
        remove_piece(turn, KING, to);
        add_piece(turn, KING, from);

        // Move rook back from (from-1) to (from-4)
        remove_piece(turn, ROOK, Square(from - 1));
        add_piece(turn, ROOK, Square(from - 4));
    }
    else
    {
        // Normal move
        // Remove the moved piece from 'to', place it back on 'from'
        remove_piece(turn, movedPiece, to);
        add_piece(turn, movedPiece, from);

        // If there was a capture, restore it on 'to'
        if (capturedSP != S_EMPTY)
        {
            add_piece(Color(!turn), specific_piece_to_piece(capturedSP), to);
        }
    }
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

bool Pos::is_square_attacked(Square sq, Color side) const
{
    BB side_pawns = pieces_bbs[PAWN] & colors_bbs[side];
    BB side_knights = pieces_bbs[KNIGHT] & colors_bbs[side];
    BB side_bishops = pieces_bbs[BISHOP] & colors_bbs[side];
    BB side_rooks = pieces_bbs[ROOK] & colors_bbs[side];
    BB side_queens = pieces_bbs[QUEEN] & colors_bbs[side];
    BB side_king = pieces_bbs[KING] & colors_bbs[side];

    BB all_pieces = colors_bbs[WHITE] | colors_bbs[BLACK];

    while (side_pawns)
    {
        int from = __builtin_ctzll(side_pawns);
        side_pawns &= side_pawns - 1;

        if ((pawn_attacks[side][from] & (1ULL << sq)) != 0)
            return true;
    }

    while (side_knights)
    {
        int from = __builtin_ctzll(side_knights);
        side_knights &= side_knights - 1;

        if ((knight_attacks[from] & (1ULL << sq)) != 0)
            return true;
    }

    BB diagonals = side_bishops | side_queens;
    while (diagonals)
    {
        int from = __builtin_ctzll(diagonals);
        diagonals &= diagonals - 1;

        BB attacks = mask_bishop_attacks(from, all_pieces);
        if ((attacks & (1ULL << sq)) != 0)
            return true;
    }

    BB straights = side_rooks | side_queens;
    while (straights)
    {
        int from = __builtin_ctzll(straights);
        straights &= straights - 1;

        BB attacks = mask_rook_attacks(from, all_pieces);
        if ((attacks & (1ULL << sq)) != 0)
            return true;
    }

    if (side_king)
    {
        int from = __builtin_ctzll(side_king);

        if ((king_attacks[from] & (1ULL << sq)) != 0)
            return true;
    }

    return false;
}

bool Pos::is_in_check(Color side) const
{
    Square king_location = static_cast<Square>(
        __builtin_ctzll(pieces_bbs[KING] & colors_bbs[side])
    );

    return is_square_attacked(king_location, Color(!side));
}
