#include "moves.hpp"

void make_move(GAMESTATE *gs, const int square) {
  int curr_square = square_to_grid[square];
  int grid_square = square_to_grid_square[square];
  gs->occ[curr_square] |= (1 << grid_square);
  gs->board[curr_square] |= (gs->side << grid_square);
  gs->side ^= 1;

  // update gs
  gs->game_occ |= (wincheck[gs->board[curr_square]] ||
                   wincheck[(~gs->board[curr_square]) & gs->occ[curr_square]] ||
                   gs->occ[curr_square] == 511)
                  << curr_square;
  gs->main_occ |= (wincheck[gs->board[curr_square]] ||
                   wincheck[(~gs->board[curr_square]) & gs->occ[curr_square]])
                  << curr_square;
  gs->main_board |= wincheck[gs->board[curr_square]] << curr_square;

  gs->last_square = grid_square;
  return;
}

void undo_move(GAMESTATE *gs, const int square, const int prev_last) {
  int curr_square = square_to_grid[square];
  int grid_square = square_to_grid_square[square];
  gs->occ[curr_square] &= ~(1 << grid_square);
  gs->board[curr_square] &= ~(1 << grid_square);
  gs->side ^= 1;

  // update gs
  gs->game_occ &= ~(1 << curr_square);
  gs->main_occ &= ~(1 << curr_square);
  gs->main_board &= ~(1 << curr_square);

  gs->last_square = prev_last;
  return;
}

void get_valid_moves(MOVES_LIST *list, const GAMESTATE *gs) {
  if (game_over(gs))
    return;
  int a, l;
  if ((gs->game_occ >> gs->last_square) & 1 || gs->last_square == -1) {
    a = 0, l = 9;
  } else {
    a = gs->last_square, l = gs->last_square + 1;
  }
  for (; a < l; ++a) {
    if ((gs->game_occ >> a) & 1)
      continue;
    int ss = grid_to_start_square[a];
    for (int i = 0; i < 3; ++i) {
      for (int j = 0; j < 3; ++j) {
        if (!(gs->occ[a] >> (i * 3 + j) & 1)) {
          list->moves[list->count++] = ss + i * 9 + j;
        }
      }
    }
  }
}

void get_valid_mask(MOVES_LIST *list, const GAMESTATE *gs) {
  if (game_over(gs))
    return;
  int a, l;
  if ((gs->game_occ >> gs->last_square) & 1 || gs->last_square == -1) {
    a = 0, l = 9;
  } else {
    a = gs->last_square, l = gs->last_square + 1;
  }
  for (; a < l; ++a) {
    if ((gs->game_occ >> a) & 1)
      continue;
    int ss = grid_to_start_square[a];
    for (int i = 0; i < 3; ++i) {
      for (int j = 0; j < 3; ++j) {
        if (!(gs->occ[a] >> (i * 3 + j) & 1)) {
          list->moves[ss + i * 9 + j] = 1;
        }
      }
    }
  }
}