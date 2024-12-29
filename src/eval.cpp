#include "eval.hpp"
#include "utils.hpp"

int eval1(GAMESTATE *gs) {
  if (wincheck[gs->main_board])
    return WIN_SCORE; // X VICTORY
  if (wincheck[(~gs->main_board) & gs->main_occ])
    return -WIN_SCORE; // O VICTORY
  if (gs->game_occ == 511)
    return DRAW_SCORE; // DRAW

  // speculative eval
  return (__builtin_popcount(gs->main_board) -
          __builtin_popcount((~gs->main_board) & gs->main_occ));
}