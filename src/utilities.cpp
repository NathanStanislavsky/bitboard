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