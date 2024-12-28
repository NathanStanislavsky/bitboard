#include "search.h"
#include "types.h"
#include "pos.h"

#include <map>

std::map<Piece, int> point_value = {
    {PAWN, 100},
    {BISHOP, 300},
    {KNIGHT, 300},
    {ROOK, 500},
    {QUEEN, 900}};

int count_material(Pos &pos, Color side)
{
    int material = 0;
    for (int i = 0; i < 6; i++)
    {
        material += __builtin_popcount(pos.pieces_bbs[i] & pos.colors_bbs[side]) * point_value[Piece(i)];
    }
    return material;
}

int eval(Pos &pos)
{
    Color side = pos.turn;
    Color enemy = Color(!side);

    int current_player_material = count_material(pos, side);
    int enemy_player_material = count_material(pos, enemy);

    return current_player_material - enemy_player_material;
}

int search(Pos &pos, int depth, int alpha, int beta)
{
    if (depth == 0)
    {
        return eval(pos);
    }

    vector<Move> legal_moves = generate_legal_moves(pos);

    // checkmate/stalemate
    if (legal_moves.size() == 0)
    {
        if (pos.is_in_check(pos.turn))
        {
            return -10000;
        }
        else
        {
            return 0;
        }
    }

    for (Move move : legal_moves)
    {
        pos.do_move(move);
        int eval = -search(pos, depth - 1, -beta, -alpha);
        pos.undo_move();

        if (eval >= beta)
        {
            // Move was too good for the opponent so we avoid this position
            return beta;
        }
        alpha = max(alpha, eval);
    }
    return alpha;
}