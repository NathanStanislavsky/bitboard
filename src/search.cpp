#include "search.h"
#include "types.h"
#include "pos.h"

#include <map>

std::map<Piece, int> piece_value = {
    {PAWN, 100},
    {BISHOP, 300},
    {KNIGHT, 300},
    {ROOK, 500},
    {QUEEN, 900}};

int INF = 2147483647;

int count_material(Pos &pos, Color side)
{
    int material = 0;
    for (int i = 0; i < 6; i++)
    {
        material += __builtin_popcount(pos.pieces_bbs[i] & pos.colors_bbs[side]) * piece_value[Piece(i)];
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

void move_order(Pos &pos, std::vector<Move> &moves)
{
    std::vector<std::pair<int, Move>> scored_moves;
    scored_moves.reserve(moves.size());

    for (const Move &move : moves)
    {
        int move_score_guess = 0;
        Piece piece_moved = pos.piece_on(from_square(move));
        Piece piece_captured = pos.piece_on(to_square(move));

        // Prioritize capturing pieces of higher values with lower value pieces
        if (is_capture(move))
        {
            move_score_guess = 10 * piece_value[piece_captured] - piece_value[piece_moved];
        }

        // Prioritize promotions
        if (is_promotion(move))
        {
            move_score_guess += piece_value[promotion_piece(move)];
        }

        // Prioritize not moving pieces to squares attacked by pawns
        if (pos.is_attacked_by_pawn(to_square(move), Color(!pos.turn)))
        {
            move_score_guess -= piece_value[piece_moved];
        }

        scored_moves.emplace_back(move_score_guess, move);
    }

    // Sort the moves in descending order of score
    std::sort(scored_moves.begin(), scored_moves.end(),
              [](const std::pair<int, Move> &a, const std::pair<int, Move> &b) -> bool
              {
                  return a.first > b.first;
              });

    // Extract the sorted moves back into the original vector
    moves.clear();
    moves.reserve(scored_moves.size());
    for (const auto &pair : scored_moves)
    {
        moves.emplace_back(pair.second);
    }
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
            return -INF;
        }
        else
        {
            return 0;
        }
    }

    move_order(pos, legal_moves);

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

Move get_best_move(Pos &pos, int depth)
{
    std::vector<Move> legal_moves = generate_legal_moves(pos);

    if (legal_moves.size() == 0)
    {
        if (pos.is_in_check(pos.turn))
        {
            return -INF;
        }
        else
        {
            return 0;
        }
    }

    int maxEval = -INF;
    Move bestMove = legal_moves[0];

    for (const Move &move : legal_moves)
    {
        pos.do_move(move);

        int eval = -search(pos, depth - 1, -INF, INF);

        pos.undo_move();

        if (eval > maxEval)
        {
            maxEval = eval;
            bestMove = move;
        }
    }

    return bestMove;
}