#include "utilities.h"
#include <string>
#include "move.h"

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
    {'k', KING}};

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
    {{EMPTY, NO_COLOR}, '-'}};

map<Specific_Piece, Piece> spiece_to_piece = {
    {WHITE_PAWN, PAWN},
    {WHITE_ROOK, ROOK},
    {WHITE_KNIGHT, KNIGHT},
    {WHITE_BISHOP, BISHOP},
    {WHITE_QUEEN, QUEEN},
    {WHITE_KING, KING},
    {BLACK_PAWN, PAWN},
    {BLACK_ROOK, ROOK},
    {BLACK_KNIGHT, KNIGHT},
    {BLACK_BISHOP, BISHOP},
    {BLACK_QUEEN, QUEEN},
    {BLACK_KING, KING},
    {S_EMPTY, EMPTY}};

map<Specific_Piece, char> spiece_map = {
    {WHITE_PAWN, 'P'},
    {WHITE_ROOK, 'R'},
    {WHITE_KNIGHT, 'N'},
    {WHITE_BISHOP, 'B'},
    {WHITE_QUEEN, 'Q'},
    {WHITE_KING, 'K'},
    {BLACK_PAWN, 'p'},
    {BLACK_ROOK, 'r'},
    {BLACK_KNIGHT, 'n'},
    {BLACK_BISHOP, 'b'},
    {BLACK_QUEEN, 'q'},
    {BLACK_KING, 'k'},
    {S_EMPTY, '-'}};

map<Piece, char> piece_to_char_map = {
    {PAWN, 'p'},
    {KNIGHT, 'n'},
    {BISHOP, 'b'},
    {ROOK, 'r'},
    {QUEEN, 'q'},
    {KING, 'k'},
};

char piece_to_char_color(Piece piece, Color color)
{
    return piece_to_char_color_map[{piece, color}];
}

Piece char_to_piece(char c)
{
    return c_to_p[c];
}

Color color_of(char c)
{
    if (c >= 'A' && c <= 'Z')
    {
        return WHITE;
    }
    else
    {
        return BLACK;
    }
}

Square string_to_square(string str)
{
    return square_of(str[1] - '1', str[0] - 'a');
}

Square square_of(int rank, int file)
{
    return Square(rank * 8 + file);
}

BB bb_has(BB board, Square square)
{
    return board & bb_of(square);
}

BB bb_of(Square square)
{
    return (1ULL << square);
}

Piece specific_piece_to_piece(Specific_Piece spiece)
{
    return spiece_to_piece[spiece];
}

char specific_piece_to_char(Specific_Piece spiece)
{
    return spiece_map[spiece];
}

string square_to_string(Square square)
{
    return string(1, (square % 8) + 'a') + string(1, (square / 8) + '1');
}

string move_to_string(Move move)
{
    return square_to_string(from_square(move)) + square_to_string(to_square(move)) + (is_promotion(move) ? "=" + string(1, piece_to_char_map[promotion_piece(move)]) : "");
}

bool is_square_attacked(Square sq, Color side, const Pos& pos)
{
    BB side_pawns    = pos.pieces_bbs[PAWN]   & pos.colors_bbs[side];
    BB side_knights  = pos.pieces_bbs[KNIGHT] & pos.colors_bbs[side];
    BB side_bishops  = pos.pieces_bbs[BISHOP] & pos.colors_bbs[side];
    BB side_rooks    = pos.pieces_bbs[ROOK]   & pos.colors_bbs[side];
    BB side_queens   = pos.pieces_bbs[QUEEN]  & pos.colors_bbs[side];
    BB side_king     = pos.pieces_bbs[KING]   & pos.colors_bbs[side];

    BB all_pieces = pos.colors_bbs[WHITE] | pos.colors_bbs[BLACK];

    while (side_pawns)
    {
        int from = __builtin_ctzll(side_pawns);
        side_pawns &= side_pawns - 1;

        if ( (pawn_attacks[side][from] & (1ULL << sq)) != 0 )
            return true;
    }

    while (side_knights)
    {
        int from = __builtin_ctzll(side_knights);
        side_knights &= side_knights - 1;

        if ( (knight_attacks[from] & (1ULL << sq)) != 0 )
            return true;
    }

    BB diagonals = side_bishops | side_queens;
    while (diagonals)
    {
        int from = __builtin_ctzll(diagonals);
        diagonals &= diagonals - 1;

        BB attacks = mask_bishop_attacks(from, all_pieces);
        if ( (attacks & (1ULL << sq)) != 0 )
            return true;
    }

    BB straights = side_rooks | side_queens;
    while (straights)
    {
        int from = __builtin_ctzll(straights);
        straights &= straights - 1;

        BB attacks = mask_rook_attacks(from, all_pieces);
        if ( (attacks & (1ULL << sq)) != 0 )
            return true;
    }

    if (side_king)
    {
        int from = __builtin_ctzll(side_king);

        if ( (king_attacks[from] & (1ULL << sq)) != 0 )
            return true;
    }

    return false;
}