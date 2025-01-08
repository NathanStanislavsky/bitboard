#include <iostream>

#include "search.h"
#include "evaluation.h"
#include "move_gen.h"

int INF = 2147483647;
int CHECKMATE_SCORE = 30000;

std::vector<Move> generate_legal_captures(Pos &pos)
{
    std::vector<Move> legal_moves = generate_legal_moves(pos);
    std::vector<Move> legal_captures;
    legal_captures.reserve(legal_moves.size());

    for (Move m : legal_moves)
    {
        if (is_capture(m))
        {
            legal_captures.push_back(m);
        }
    }

    return legal_captures;
}

int quiescence_search(Pos &pos, int alpha, int beta)
{
    int evaluation = eval(pos);

    if (evaluation >= beta)
    {
        // If evaluation is huge (like a mate score), return it exactly
        // if (evaluation >= CHECKMATE_SCORE - 100) // or some threshold
        //     return evaluation;
        return beta;
    }
    alpha = max(alpha, evaluation);

    std::vector<Move> capture_moves = generate_legal_captures(pos);

    move_order(pos, capture_moves);

    for (Move m : capture_moves)
    {
        pos.do_move(m);
        int evaluation = -quiescence_search(pos, -beta, -alpha);
        pos.undo_move();

        if (evaluation >= beta)
        {
            return beta;
        }
        alpha = max(alpha, evaluation);
    }

    return alpha;
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
            move_score_guess = 10 * get_piece_value(piece_captured) - get_piece_value(piece_moved);
        }

        // Prioritize promotions
        if (is_promotion(move))
        {
            move_score_guess += get_piece_value(promotion_piece(move));
        }

        // Prioritize not moving pieces to squares attacked by pawns
        if (pos.is_attacked_by_pawn(to_square(move), Color(!pos.turn)))
        {
            move_score_guess -= get_piece_value(piece_moved);
        }

        scored_moves.emplace_back(move_score_guess, move);
    }

    // Sort the moves in descending order of score
    std::sort(scored_moves.begin(), scored_moves.end(), std::greater<std::pair<int, Move>>());

    // Extract the sorted moves back into the original vector
    moves.clear();
    moves.reserve(scored_moves.size());
    for (const auto &pair : scored_moves)
    {
        moves.emplace_back(pair.second);
    }
}

int search(Pos &pos, int depth, int alpha, int beta, int ply)
{
    if (depth == 0)
    {
        // keep going until no captures are left (quiescence search)
        return quiescence_search(pos, alpha, beta);
    }

    vector<Move> legal_moves = generate_legal_moves(pos);

    // checkmate/stalemate
    if (legal_moves.size() == 0)
    {
        if (pos.is_in_check(pos.turn))
        {
            return -(CHECKMATE_SCORE - ply);
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
        int childEval = -search(pos, depth - 1, -beta, -alpha, ply + 1);
        pos.undo_move();

        if (childEval >= beta)
        {
            // Move was too good for the opponent so we avoid this position
            return beta;
        }
        alpha = max(alpha, childEval);
    }
    return alpha;
}

Move get_best_move(Pos &pos, int depth)
{
    std::vector<Move> legal_moves = generate_legal_moves(pos);

    if (legal_moves.size() == 0)
    {
        return Move();
    }

    int maxEval = -INF;
    Move bestMove = legal_moves[0];

    for (const Move &move : legal_moves)
    {
        pos.do_move(move);;
        int eval = -search(pos, depth - 1, -INF, INF, 1);
        pos.undo_move();

        if (eval > maxEval)
        {
            maxEval = eval;
            bestMove = move;
            std::cout << "Best move: " << move_to_string(move) << " with eval: " << eval << std::endl;
        }
    }

    return bestMove;
}