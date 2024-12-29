#ifndef MOVES_HPP_INCLUDED
#define MOVES_HPP_INCLUDED

#include "types.hpp"
#include "utils.hpp"

inline constexpr bool game_over(GAMESTATE *gs) {
  return gs->game_occ == 511 || wincheck[gs->main_board] ||
         wincheck[(~gs->main_board) & gs->main_occ];
}

extern void make_move(GAMESTATE *gs, const int square);
extern void undo_move(GAMESTATE *gs, const int square, const int prev_last);
extern void get_valid_moves(MOVES_LIST *list, GAMESTATE *gs);

#endif