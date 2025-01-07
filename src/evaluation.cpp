#include <algorithm>

#include "evaluation.h"

const float endgame_material_start = 500 * 2 + 300 + 300; // rook value, knight value, bishop value

int count_material(Pos &pos, Color side)
{
    int material = 0;
    for (int i = 0; i < 6; i++)
    {
        material += __builtin_popcountll(pos.pieces_bbs[i] & pos.colors_bbs[side]) * get_piece_value(Piece(i));
    }
    return material;
}

int get_material_count_without_pawns(Pos &pos, Color side)
{
    int material = 0;
    // start at 1 to exclude pawns
    for (int i = 1; i < 6; i++)
    {
        material += __builtin_popcountll(pos.pieces_bbs[i] & pos.colors_bbs[side]) * get_piece_value(Piece(i));
    }
    return material;
}

float endgame_phase_weight(int material_count_without_pawns)
{
    const float multiplier = 1 / endgame_material_start;
    return 1 - std::min(1.0f, material_count_without_pawns * multiplier);
}

int force_king_to_corner_endgame_eval(Pos &pos, float endgame_weight)
{
    Square friendly_king_square = Square(__builtin_ctzll(pos.pieces_bbs[KING] & pos.colors_bbs[pos.turn]));
    Square enemy_king_square = Square(__builtin_ctzll(pos.pieces_bbs[KING] & pos.colors_bbs[Color(!pos.turn)]));

    int evaluation = 0;

    int enemy_king_rank = enemy_king_square / 8;
    int enemy_king_file = enemy_king_square % 8;

    int enemy_king_distance_center_file = max(3 - enemy_king_file, enemy_king_file - 4);
    int enemy_king_distance_center_rank = max(3 - enemy_king_rank, enemy_king_rank - 4);
    int enemy_king_distance_center = enemy_king_distance_center_file + enemy_king_distance_center_rank;

    evaluation += enemy_king_distance_center;

    int friendly_king_rank = friendly_king_square / 8;
    int friendly_king_file = friendly_king_square % 8;

    int distance_between_kings_file = abs(friendly_king_file - enemy_king_file);
    int distance_between_kings_rank = abs(friendly_king_rank - enemy_king_rank);
    int distance_between_kings = distance_between_kings_file + distance_between_kings_rank;
    evaluation += 14 - distance_between_kings;

    return (int)(evaluation * 10 * endgame_weight);
}

int eval(Pos &pos)
{
    Color side = pos.turn;
    Color enemy = Color(!side);

    int current_player_material = count_material(pos, side);
    int enemy_player_material = count_material(pos, enemy);
    int material_score = current_player_material - enemy_player_material;

    int material_score_without_pawns = get_material_count_without_pawns(pos, side);
    float endgame_weight = endgame_phase_weight(material_score_without_pawns);
    int endgame_score = force_king_to_corner_endgame_eval(pos, endgame_weight);
    
    return material_score + endgame_score;
}