#include "move_gen.h"
#include "types.h"
#include <vector>
#include <iostream>
#include <cassert>

using namespace std;

#define get_bit(bitboard, square) (bitboard & (1ULL << square))
#define set_bit(bitboard, square) (bitboard |= (1ULL << square))
#define remove_bit(bitboard, square) (bitboard & ~(1ULL << square))

static const BB not_A_column = 18374403900871474942ULL;
static const BB not_H_column = 9187201950435737471ULL;
static const BB not_HG_column = 4557430888798830399ULL;
static const BB not_AB_column = 18229723555195321596ULL;

BB mask_pawn_attacks(Color side, Square square)
{
    BB attacks = 0ULL;

    BB bitboard = 0ULL;
    set_bit(bitboard, square);

    if (side == WHITE)
    {
        // Attack down right
        if ((bitboard << 7) & not_H_column)
        {
            attacks |= (bitboard << 7);
        }

        // Attack down left
        if ((bitboard << 9) & not_A_column)
        {
            attacks |= (bitboard << 9);
        }
    }
    else // BLACK
    {
        // Attack up right
        if ((bitboard >> 9) & not_H_column)
        {
            attacks |= (bitboard >> 9);
        }

        // Attack up left
        if ((bitboard >> 7) & not_A_column)
        {
            attacks |= (bitboard >> 7);
        }
    }

    return attacks;
}

BB mask_knight_attacks(Square square)
{
    BB attacks = 0ULL;

    BB bitboard = 0ULL;
    set_bit(bitboard, square);

    if ((bitboard >> 17) & not_H_column) // Up-left
    {
        attacks |= (bitboard >> 17);
    }
    if ((bitboard >> 15) & not_A_column) // Up-right
    {
        attacks |= (bitboard >> 15);
    }
    if ((bitboard >> 10) & not_HG_column) // Right-up
    {
        attacks |= (bitboard >> 10);
    }
    if ((bitboard >> 6) & not_AB_column) // Left-up
    {
        attacks |= (bitboard >> 6);
    }
    if ((bitboard << 17) & not_A_column) // Down-left
    {
        attacks |= (bitboard << 17);
    }
    if ((bitboard << 15) & not_H_column) // Down-right
    {
        attacks |= (bitboard << 15);
    }
    if ((bitboard << 10) & not_AB_column) // Right-down
    {
        attacks |= (bitboard << 10);
    }
    if ((bitboard << 6) & not_HG_column) // Left-down
    {
        attacks |= (bitboard << 6);
    }

    return attacks;
}

BB mask_king_attacks(Square square)
{
    BB attacks = 0ULL;

    BB bitboard = 0ULL;
    set_bit(bitboard, square);

    // north
    if (bitboard >> 8)
    {
        attacks |= (bitboard >> 8);
    }
    // north west
    if ((bitboard >> 9) & not_H_column)
    {
        attacks |= (bitboard >> 9);
    }
    // north east
    if ((bitboard >> 7) & not_A_column)
    {
        attacks |= (bitboard >> 7);
    }
    // west
    if ((bitboard >> 1) & not_H_column)
    {
        attacks |= (bitboard >> 1);
    }

    // south
    if (bitboard << 8)
    {
        attacks |= (bitboard << 8);
    }
    // south east
    if ((bitboard << 9) & not_A_column)
    {
        attacks |= (bitboard << 9);
    }
    // south west
    if ((bitboard << 7) & not_H_column)
    {
        attacks |= (bitboard << 7);
    }
    // east
    if ((bitboard << 1) & not_A_column)
    {
        attacks |= (bitboard << 1);
    }

    return attacks;
}

BB mask_bishop_attacks(int square, BB block)
{
    BB attacks = 0ULL;

    int rank, file;

    // init target rank & files
    int to_rank = square / 8;
    int to_file = square % 8;

    // south east
    for (rank = to_rank + 1, file = to_file + 1; rank <= 7 && file <= 7; rank++, file++)
    {
        attacks |= (1ULL << (rank * 8 + file));
        if ((1ULL << (rank * 8 + file)) & block)
            break;
    }

    // north east
    for (rank = to_rank - 1, file = to_file + 1; rank >= 0 && file <= 7; rank--, file++)
    {
        attacks |= (1ULL << (rank * 8 + file));
        if ((1ULL << (rank * 8 + file)) & block)
            break;
    }

    // south west
    for (rank = to_rank + 1, file = to_file - 1; rank <= 7 && file >= 0; rank++, file--)
    {
        attacks |= (1ULL << (rank * 8 + file));
        if ((1ULL << (rank * 8 + file)) & block)
            break;
    }

    // north west
    for (rank = to_rank - 1, file = to_file - 1; rank >= 0 && file >= 0; rank--, file--)
    {
        attacks |= (1ULL << (rank * 8 + file));
        if ((1ULL << (rank * 8 + file)) & block)
            break;
    }

    return attacks;
}

BB mask_rook_attacks(int square, BB block)
{
    BB attacks = 0ULL;

    int rank, file;

    int to_rank = square / 8;
    int to_file = square % 8;

    // down
    for (rank = to_rank + 1; rank <= 7; rank++)
    {
        attacks |= (1ULL << (rank * 8 + to_file));
        if ((1ULL << (rank * 8 + to_file)) & block)
            break;
    }
    // up
    for (rank = to_rank - 1; rank >= 0; rank--)
    {
        attacks |= (1ULL << (rank * 8 + to_file));
        if ((1ULL << (rank * 8 + to_file)) & block)
            break;
    }
    // right
    for (file = to_file + 1; file <= 7; file++)
    {
        attacks |= (1ULL << (to_rank * 8 + file));
        if ((1ULL << (to_rank * 8 + file)) & block)
            break;
    }
    // left
    for (file = to_file - 1; file >= 0; file--)
    {
        attacks |= (1ULL << (to_rank * 8 + file));
        if ((1ULL << (to_rank * 8 + file)) & block)
            break;
    }

    return attacks;
}

BB mask_queen_attacks(int square, BB block)
{
    BB attacks = 0ULL;

    attacks |= mask_rook_attacks(square, block);
    attacks |= mask_bishop_attacks(square, block);

    return attacks;
}

vector<Move> generate_psuedo_moves(const Pos &pos)
{
    std::vector<Move> moves;

    Color side = pos.turn;
    Color enemy = Color(!side);

    BB current_side_pieces = pos.colors_bbs[side];
    BB enemy_side_pieces = pos.colors_bbs[enemy];
    BB all_pieces = current_side_pieces | enemy_side_pieces;
    BB empty_squares = ~all_pieces;

    BB pawns = pos.pieces_bbs[PAWN] & pos.colors_bbs[side];
    BB knights = pos.pieces_bbs[KNIGHT] & pos.colors_bbs[side];
    BB bishops = pos.pieces_bbs[BISHOP] & pos.colors_bbs[side];
    BB rooks = pos.pieces_bbs[ROOK] & pos.colors_bbs[side];
    BB queens = pos.pieces_bbs[QUEEN] & pos.colors_bbs[side];
    BB king = pos.pieces_bbs[KING] & pos.colors_bbs[side];

    auto add_moves = [&](Square from, BB targets, Piece piece)
    {
        while (targets)
        {
            // Get the index of the least significant bit
            int to = __builtin_ctzll(targets);
            targets &= targets - 1;

            // capture
            MoveFlag flag = QUIET;
            if (bb_has(enemy_side_pieces, Square(to)))
            {
                flag = CAPTURE;
            }

            // Handle promotions if this is a pawn moving to last rank
            if (piece == PAWN)
            {
                int rank = to / 8;
                if ((side == WHITE && rank == 7) || (side == BLACK && rank == 0))
                {
                    MoveFlag promotionFlags[] = {
                        (MoveFlag)(Q_PROM | ((flag == CAPTURE) ? CAPTURE : 0)),
                        (MoveFlag)(R_PROM | ((flag == CAPTURE) ? CAPTURE : 0)),
                        (MoveFlag)(B_PROM | ((flag == CAPTURE) ? CAPTURE : 0)),
                        (MoveFlag)(N_PROM | ((flag == CAPTURE) ? CAPTURE : 0))};

                    for (auto promotionFlag : promotionFlags)
                    {
                        moves.push_back(generate_move(from, Square(to), promotionFlag));
                    }
                    continue;
                }
            }

            // Normal move
            moves.push_back(generate_move(from, Square(to), flag));
        }
    };

    // Generate Pawn Moves
    // -------------------
    int forward = (side == WHITE) ? 8 : -8;
    int start_rank = (side == WHITE) ? 1 : 6; // White pawns start at rank 2 (index 1), Black at rank 7 (index 6)

    while (pawns)
    {
        Square from = (Square)__builtin_ctzll(pawns);
        pawns &= pawns - 1;

        Piece piece = PAWN;
        BB pawn_mask = mask_pawn_attacks(side, from);

        // captures
        BB captures = pawn_mask & enemy_side_pieces;
        add_moves(from, captures, piece);

        // enpassant
        if (pos.enpassant_sq != NONE_SQUARE)
        {
            Square ep = pos.enpassant_sq;

            if ((pawn_mask & (1ULL << ep)) != 0)
            {
                moves.push_back(generate_move(from, ep, EP));
            }
        }

        // Pawn forward moves
        int to_int = from + forward;
        if (to_int >= A1 && to_int <= H8 && bb_has(empty_squares, Square(to_int)))
        {
            BB single_push = 1ULL << to_int;
            int to_rank = to_int / 8;
            int from_rank = from / 8;
            Square to_square = Square(to_int);

            if ((side == WHITE && to_rank == 7) || (side == BLACK && to_rank == 0))
            {
                // promotion single push
                add_moves(from, single_push, piece);
            }
            else
            {
                // normal single push
                moves.push_back(generate_move(from, to_square));
            }

            // Double push
            if (from_rank == start_rank)
            {
                int to_int2 = from + 2 * forward;
                if (to_int2 >= A1 && to_int2 <= H8 && bb_has(empty_squares, Square(to_int2)))
                {
                    moves.push_back(generate_move(from, Square(to_int2), DOUBLE_PAWN_PUSH));
                }
            }
        }
    }

    // Knights
    // -------
    while (knights)
    {
        // least significant bit
        Square from = (Square)__builtin_ctzll(knights);
        knights &= knights - 1;

        BB attacks = mask_knight_attacks(from) & ~current_side_pieces;
        add_moves(from, attacks, KNIGHT);
    }

    // Bishops
    // -------
    while (bishops)
    {
        Square from = (Square)__builtin_ctzll(bishops);
        bishops &= bishops - 1;
        BB attacks = mask_bishop_attacks(from, all_pieces) & ~current_side_pieces;
        add_moves(from, attacks, BISHOP);
    }

    // Rooks
    // -----
    while (rooks)
    {
        Square from = (Square)__builtin_ctzll(rooks);
        rooks &= rooks - 1;
        BB attacks = mask_rook_attacks(from, all_pieces) & ~current_side_pieces;
        add_moves(from, attacks, ROOK);
    }

    // Queens
    // ------
    while (queens)
    {
        Square from = (Square)__builtin_ctzll(queens);
        queens &= queens - 1;
        BB attacks = mask_queen_attacks(from, all_pieces) & ~current_side_pieces;
        add_moves(from, attacks, QUEEN);
    }

    // King
    // ----
    if (king)
    {
        Square from = (Square)__builtin_ctzll(king);

        // normal moves
        BB attacks = mask_king_attacks(from) & ~current_side_pieces;
        add_moves(from, attacks, KING);

        // Castling
        if (side == WHITE)
        {
            if (pos.cr.wkc)
            {
                bool squaresEmpty = !bb_has(all_pieces, F1) && !bb_has(all_pieces, G1);

                bool notAttacked = true;
                if (pos.is_square_attacked(E1, enemy))
                    notAttacked = false;
                if (pos.is_square_attacked(F1, enemy))
                    notAttacked = false;
                if (pos.is_square_attacked(G1, enemy))
                    notAttacked = false;

                if (squaresEmpty && notAttacked)
                {
                    moves.push_back(generate_move(E1, G1, KING_CASTLE));
                }
            }

            if (pos.cr.wqc)
            {
                bool squaresEmpty = !bb_has(all_pieces, D1) && !bb_has(all_pieces, C1) && !bb_has(all_pieces, B1);

                bool notAttacked = true;
                if (pos.is_square_attacked(E1, enemy))
                    notAttacked = false;
                if (pos.is_square_attacked(D1, enemy))
                    notAttacked = false;
                if (pos.is_square_attacked(C1, enemy))
                    notAttacked = false;

                if (squaresEmpty && notAttacked)
                {
                    moves.push_back(generate_move(E1, C1, QUEEN_CASTLE));
                }
            }
        }
        else
        {
            if (pos.cr.bkc)
            {
                bool squaresEmpty = !bb_has(all_pieces, F8) && !bb_has(all_pieces, G8);

                bool notAttacked = true;
                if (pos.is_square_attacked(E8, enemy))
                    notAttacked = false;
                if (pos.is_square_attacked(F8, enemy))
                    notAttacked = false;
                if (pos.is_square_attacked(G8, enemy))
                    notAttacked = false;

                if (squaresEmpty && notAttacked)
                {
                    moves.push_back(generate_move(E8, G8, KING_CASTLE));
                }
            }

            if (pos.cr.bqc)
            {
                bool squaresEmpty = !bb_has(all_pieces, D8) && !bb_has(all_pieces, C8) && !bb_has(all_pieces, B8);

                bool notAttacked = true;
                if (pos.is_square_attacked(E8, enemy))
                    notAttacked = false;
                if (pos.is_square_attacked(D8, enemy))
                    notAttacked = false;
                if (pos.is_square_attacked(C8, enemy))
                    notAttacked = false;

                if (squaresEmpty && notAttacked)
                {
                    moves.push_back(generate_move(E8, C8, QUEEN_CASTLE));
                }
            }
        }
    }

    return moves;
}

std::vector<Move> generate_legal_moves(Pos &pos)
{
    std::vector<Move> pseudoMoves = generate_psuedo_moves(pos);
    std::vector<Move> legalMoves;
    legalMoves.reserve(pseudoMoves.size());

    /*
    for every move in pseudoMoves we do the move 
    and check if the king is in check after the move
    */
    for (Move m : pseudoMoves)
    {
        pos.do_move(m);

        if (!pos.is_in_check(Color(!pos.turn)))
        {
            legalMoves.push_back(m);
        }

        pos.undo_move();
    }

    return legalMoves;
}

int perft(Pos &pos, int depth, bool verbose)
{
    if (depth == 0)
    {
        return 1;
    }

    vector<Move> validMoves = generate_legal_moves(pos);

    int count = 0;

    for (int i = 0; i < validMoves.size(); i++)
    {
        pos.do_move(validMoves[i]);

        int result = perft(pos, depth - 1, false);

        if (verbose)
        {
            cout << move_to_string(validMoves[i]) + " " + to_string(result) << endl;
        }
        count += result;

        pos.undo_move();
    }
    
    return count;
}