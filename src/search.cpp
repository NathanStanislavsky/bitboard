#include <iostream>

#include "search.h"
#include "evaluation.h"
#include "move_gen.h"
#include "transposition.h"

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

std::pair<int, Move> search(Pos &pos, int depth, int alpha, int beta, int ply, TranspositionTable &tt)
{
    // If we have reached depth 0, drop into quiescence search.
    if (depth == 0)
    {
        int score = quiescence_search(pos, alpha, beta);
        return {score, Move()}; // No particular "best move" in quiescence
    }

    uint64_t key = compute_zobrist_hash(pos);

    int ttScore;
    Move ttBestMove;
    if (tt.probe(key, depth, alpha, beta, ttScore, ttBestMove))
    {
        return {ttScore, ttBestMove};
    }

    // Generate all legal moves
    std::vector<Move> legal_moves = generate_legal_moves(pos);

    // Check for checkmate/stalemate
    if (legal_moves.empty())
    {
        if (pos.is_in_check(pos.turn))
        {
            // Negative mate score factoring in distance to mate
            return {-(CHECKMATE_SCORE - ply), Move()};
        }
        else
        {
            // Stalemate
            return {0, Move()};
        }
    }

    // Order the moves for better alpha-beta performance
    move_order(pos, legal_moves);

    int originalAlpha = alpha;
    int bestEval = -INF;
    Move bestMove;

    for (const Move &move : legal_moves)
    {
        pos.do_move(move);
        auto [childEval, childBestMove] = search(pos, depth - 1, -beta, -alpha, ply + 1, tt);
        childEval = -childEval;
        pos.undo_move();

        // Update alpha/bestEval/bestMove if we found something better
        if (childEval > bestEval)
        {
            bestEval = childEval;
            bestMove = move;
        }
        alpha = std::max(alpha, bestEval);

        // Alpha-Beta cutoff
        if (alpha >= beta)
            break;
    }

    TTFlag flag = TT_EXACT; // By default, assume it's an exact value

    // If the score is too low (i.e., we never raised alpha above alphaOriginal)
    // then it's an upper bound on the actual score
    if (bestEval <= originalAlpha)
    {
        flag = TT_UPPERBOUND;
    }
    // If the score is too high (alpha >= beta), it's a lower bound
    else if (bestEval >= beta)
    {
        flag = TT_LOWERBOUND;
    }

    tt.store(key, depth, bestEval, flag, bestMove);

    // Return the best score found and the corresponding move
    return {bestEval, bestMove};
}
