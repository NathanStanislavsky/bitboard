#include "move_gen.h"
#include "types.h"
#include <vector>

#define get_bit(bitboard, square) (bitboard & (1ULL << square))
#define set_bit(bitboard, square) (bitboard |= (1ULL << square))
#define remove_bit(bitboard, square) (get_bit(bitboard, square) ? bitboard ^= (1ULL << square) : 0)

// A column
static const BB not_A_column = 18374403900871474942ULL;

// 11111110
// 11111110
// 11111110
// 11111110
// 11111110
// 11111110
// 11111110
// 11111110

// 1111111011111110111111101111111011111110111111101111111011111110

// H column
static const BB not_H_column = 9187201950435737471ULL;

// 11111110
// 11111110
// 11111110
// 11111110
// 11111110
// 11111110
// 11111110
// 11111110

// 111111101111111011111110111111101111111011111110111111101111111

// HG column
static const BB not_HG_column = 4557430888798830399ULL;

// 11111100
// 11111100
// 11111100
// 11111100
// 11111100
// 11111100
// 11111100
// 11111100

// 11111100111111001111110011111100111111001111110011111100111111

// AB column
static const BB not_AB_column = 18229723555195321596ULL;

// 0 0 1 1 1 1 1 1
// 0 0 1 1 1 1 1 1
// 0 0 1 1 1 1 1 1
// 0 0 1 1 1 1 1 1
// 0 0 1 1 1 1 1 1
// 0 0 1 1 1 1 1 1
// 0 0 1 1 1 1 1 1
// 0 0 1 1 1 1 1 1

// 1111110011111100111111001111110011111100111111001111110011111100

// Generate pawn attacks
BB mask_pawn_attacks(Color side, Square square)
{
    // attacks result bitboard
    BB attacks = 0ULL;

    // piece bitboard
    BB bitboard = 0ULL;

    // set piece on board
    set_bit(bitboard, square);

    // white pawns
    if (!side)
    {
        if ((bitboard << 7) & not_H_column)
        {
            // attack south west
            attacks |= (bitboard << 7);
        }

        if ((bitboard << 9) & not_A_column)
        {
            // attack south east
            attacks |= (bitboard << 9);
        }
    }
    // black pawns
    else
    {
        if ((bitboard >> 7) & not_A_column)
        {
            // attack north east
            attacks |= (bitboard >> 7);
        }

        if ((bitboard >> 9) & not_H_column)
        {
            // attack north west
            attacks |= (bitboard >> 9);
        }
    }

    // return attack map
    return attacks;
}

BB mask_knight_attacks(Square square)
{
    // attacks result bitboard
    BB attacks = 0ULL;

    // piece bitboard
    BB bitboard = 0ULL;

    // set piece on board
    set_bit(bitboard, square);

    // Generate knight attacks
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

    // return attack map
    return attacks;
}

// Generate pawn attacks
BB mask_king_attacks(Square square)
{
    // attacks result bitboard
    BB attacks = 0ULL;

    // piece bitboard
    BB bitboard = 0ULL;

    // set piece on board
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

    // return attack map
    return attacks;
}

BB mask_bishop_attacks(int square, BB block)
{
    // result attacks bitboard
    BB attacks = 0ULL;

    // init row & file
    int r, f;

    // init target rank & files
    int tr = square / 8;
    int tf = square % 8;

    // south east
    for (r = tr + 1, f = tf + 1; r <= 7 && f <= 7; r++, f++)
    {
        attacks |= (1ULL << (r * 8 + f));
        if ((1ULL << (r * 8 + f)) & block)
            break;
    }

    // north east
    for (r = tr - 1, f = tf + 1; r >= 0 && f <= 7; r--, f++)
    {
        attacks |= (1ULL << (r * 8 + f));
        if ((1ULL << (r * 8 + f)) & block)
            break;
    }

    // south west
    for (r = tr + 1, f = tf - 1; r <= 7 && f >= 0; r++, f--)
    {
        attacks |= (1ULL << (r * 8 + f));
        if ((1ULL << (r * 8 + f)) & block)
            break;
    }

    // north west
    for (r = tr - 1, f = tf - 1; r >= 0 && f >= 0; r--, f--)
    {
        attacks |= (1ULL << (r * 8 + f));
        if ((1ULL << (r * 8 + f)) & block)
            break;
    }

    // return attack map
    return attacks;
}

BB mask_rook_attacks(int square, BB block)
{
    // result attacks bitboard
    BB attacks = 0ULL;

    // init ranks & files
    int r, f;

    // init target rank & files
    int tr = square / 8;
    int tf = square % 8;

    // down
    for (r = tr + 1; r <= 7; r++)
    {
        attacks |= (1ULL << (r * 8 + tf));
        if ((1ULL << (r * 8 + tf)) & block)
            break;
    }
    // up
    for (r = tr - 1; r >= 0; r--)
    {
        attacks |= (1ULL << (r * 8 + tf));
        if ((1ULL << (r * 8 + tf)) & block)
            break;
    }
    // right
    for (f = tf + 1; f <= 7; f++)
    {
        attacks |= (1ULL << (tr * 8 + f));
        if ((1ULL << (tr * 8 + f)) & block)
            break;
    }
    // left
    for (f = tf - 1; f >= 0; f--)
    {
        attacks |= (1ULL << (tr * 8 + f));
        if ((1ULL << (tr * 8 + f)) & block)
            break;
    }

    // return attack map
    return attacks;
}

// generate rook attacks on the fly
BB mask_queen_attacks(int square, BB block)
{
    // result attacks bitboard
    BB attacks = 0ULL;

    attacks |= mask_rook_attacks(square, block);
    attacks |= mask_bishop_attacks(square, block);

    return attacks;
}

// generate leaper piece attacks
void init_leapers_attacks()
{
    for (int square = 0; square < 64; square++)
    {
        pawn_attacks[WHITE][square] = mask_pawn_attacks(WHITE, Square(square));
        pawn_attacks[BLACK][square] = mask_pawn_attacks(BLACK, Square(square));

        knight_attacks[square] = mask_knight_attacks(Square(square));

        king_attacks[square] = mask_king_attacks(Square(square));
    }
}

vector<Move> generate_psuedo_moves(const Pos &pos)
{
    std::vector<Move> moves;

    Color side = pos.turn;
    Color enemy = Color(!side);

    BB current_side_pieces = pos.pieces_bbs[side];
    BB enemy_side_pieces = pos.pieces_bbs[enemy];
    BB all_pieces = current_side_pieces | enemy_side_pieces;
    BB empty_squares = ~all_pieces;

    auto add_moves = [&](Square from, BB targets, Piece piece)
    {
        while (targets)
        {
            int to = __builtin_ctzll(targets);
            targets &= targets - 1;

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
                    moves.push_back(make_move(from, Square(to), (MoveFlag)Q_PROM | (flag == CAPTURE ? CAPTURE : 0)));
                    moves.push_back(make_move(from, Square(to), (MoveFlag)R_PROM | (flag == CAPTURE ? CAPTURE : 0)));
                    moves.push_back(make_move(from, Square(to), (MoveFlag)B_PROM | (flag == CAPTURE ? CAPTURE : 0)));
                    moves.push_back(make_move(from, Square(to), (MoveFlag)N_PROM | (flag == CAPTURE ? CAPTURE : 0)));
                    continue;
                }
            }

            // Normal move
            moves.push_back(make_move(from, Square(to), flag));
        }
    };

    // Generate Pawn Moves
    // -------------------
    BB pawns = pos.pieces_bbs[PAWN] & current_side_pieces;

    // Direction and double-push rank depend on side
    int forward = (side == WHITE) ? 8 : -8;
    int start_rank = (side == WHITE) ? 1 : 6; // White pawns start at rank 2 (index 1), Black at rank 7 (index 6)

    while (pawns)
    {
        Square from = (Square)__builtin_ctzll(pawns);
        pawns &= pawns - 1;

        Piece piece = PAWN;

        // Pawn captures
        BB captures = pawn_attacks[side][from] & enemy_side_pieces;
        add_moves(from, captures, piece);

        // En passant (not shown here, requires pos.enpassant_sq check)
        // if (pos.enpassant_sq != NONE_SQUARE) { ... }

        // Pawn forward moves
        int to_int = from + forward;
        if (to_int >= A1 && to_int <= H8 && bb_has(empty_squares, Square(to_int)))
        {
            // Single push
            Square to_s = Square(to_int);
            // Promotion handled in add_moves if last rank
            moves.push_back(make_move(from, to_s));

            // Double push
            int rank = from / 8;
            if (rank == start_rank)
            {
                int to_int2 = from + 2 * forward;
                if (to_int2 >= A1 && to_int2 <= H8 && bb_has(empty_squares, Square(to_int2)))
                {
                    moves.push_back(make_move(from, Square(to_int2), DOUBLE_PAWN_PUSH));
                }
            }
        }
    }

    // Knights
    // -------
    BB knights = pos.pieces_bbs[KNIGHT] & current_side_pieces;
    while (knights)
    {
        Square from = (Square)__builtin_ctzll(knights);
        knights &= knights - 1;
        BB attacks = knight_attacks[from] & ~current_side_pieces;
        add_moves(from, attacks, KNIGHT);
    }

    // Bishops
    // -------
    BB bishops = pos.pieces_bbs[BISHOP] & current_side_pieces;
    while (bishops)
    {
        Square from = (Square)__builtin_ctzll(bishops);
        bishops &= bishops - 1;
        BB attacks = mask_bishop_attacks(from, all_pieces) & ~current_side_pieces;
        add_moves(from, attacks, BISHOP);
    }

    // Rooks
    // -----
    BB rooks = pos.pieces_bbs[ROOK] & current_side_pieces;
    while (rooks)
    {
        Square from = (Square)__builtin_ctzll(rooks);
        rooks &= rooks - 1;
        BB attacks = mask_rook_attacks(from, all_pieces) & ~current_side_pieces;
        add_moves(from, attacks, ROOK);
    }

    // Queens
    // ------
    BB queens = pos.pieces_bbs[QUEEN] & current_side_pieces;
    while (queens)
    {
        Square from = (Square)__builtin_ctzll(queens);
        queens &= queens - 1;
        BB attacks = mask_queen_attacks(from, all_pieces) & ~current_side_pieces;
        add_moves(from, attacks, QUEEN);
    }

    // King
    // ----
    BB king = pos.pieces_bbs[KING] & current_side_pieces;
    if (king)
    {
        Square from = (Square)__builtin_ctzll(king);
        BB attacks = king_attacks[from] & ~current_side_pieces;
        add_moves(from, attacks, KING);

        if (side == WHITE)
        {
            if (pos.cr.wkc)
            {
                bool squaresEmpty = !bb_has(all_pieces, F1) && !bb_has(all_pieces, G1);

                bool notAttacked = true;
                if (is_square_attacked(E1, enemy, pos))
                    notAttacked = false;
                if (is_square_attacked(F1, enemy, pos))
                    notAttacked = false;
                if (is_square_attacked(G1, enemy, pos))
                    notAttacked = false;

                if (squaresEmpty && notAttacked)
                {
                    moves.push_back(make_move(E1, G1, KING_CASTLE));
                }
            }

            if (pos.cr.wqc)
            {
                bool squaresEmpty = !bb_has(all_pieces, D1) && !bb_has(all_pieces, C1) && !bb_has(all_pieces, B1);

                bool notAttacked = true;
                if (is_square_attacked(E1, enemy, pos))
                    notAttacked = false;
                if (is_square_attacked(D1, enemy, pos))
                    notAttacked = false;
                if (is_square_attacked(C1, enemy, pos))
                    notAttacked = false;

                if (squaresEmpty && notAttacked)
                {
                    moves.push_back(make_move(E1, C1, QUEEN_CASTLE));
                }
            }
        }
        else 
        {
            if (pos.cr.bkc)
            {
                bool squaresEmpty = !bb_has(all_pieces, F8) && !bb_has(all_pieces, G8);

                bool notAttacked = true;
                if (is_square_attacked(E8, enemy, pos))
                    notAttacked = false;
                if (is_square_attacked(F8, enemy, pos))
                    notAttacked = false;
                if (is_square_attacked(G8, enemy, pos))
                    notAttacked = false;

                if (squaresEmpty && notAttacked)
                {
                    moves.push_back(make_move(E8, G8, KING_CASTLE));
                }
            }

            if (pos.cr.bqc)
            {
                bool squaresEmpty = !bb_has(all_pieces, D8) && !bb_has(all_pieces, C8) && !bb_has(all_pieces, B8);

                bool notAttacked = true;
                if (is_square_attacked(E8, enemy, pos))
                    notAttacked = false;
                if (is_square_attacked(D8, enemy, pos))
                    notAttacked = false;
                if (is_square_attacked(C8, enemy, pos))
                    notAttacked = false;

                if (squaresEmpty && notAttacked)
                {
                    moves.push_back(make_move(E8, C8, QUEEN_CASTLE));
                }
            }
        }
    }

    return moves;
}
