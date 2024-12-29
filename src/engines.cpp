#include "engines.hpp"
#include "eval.hpp"
#include "moves.hpp"
#include "utils.hpp"

#include <algorithm>
#include <iostream>

int human_engine(GAMESTATE *gs) {
  MOVES_LIST list[1];
  get_valid_moves(list, gs);
  int row, col;
  do {
    std::cout << "Player " << ((gs->side) ? "X" : "O")
              << ", choose a square in game " << gs->last_square + 1
              << ". (row then column) : ";
    std::cin >> row >> col;
  } while (std::find(begin(list->moves), begin(list->moves) + list->count,
                     (row - 1) * 9 + col - 1) ==
           begin(list->moves) + list->count);
  return (row - 1) * 9 + col - 1;
};

double nega_min_max(GAMESTATE *gs, int depth, int side, SEARCH_INFO *info) {
  if (depth == 0) {
    return side * eval1(gs);
  }
  double bestScore = -INF;
  MOVES_LIST list[1];
  get_valid_moves(list, gs);
  if (!list->count)
    return side * eval1(gs);
  int temp = gs->last_square;
  for (int i = 0; i < list->count; ++i) {
    make_move(gs, list->moves[i]);
    double score = -nega_min_max(gs, depth - 1, -side, info);
    if (score > bestScore) {
      bestScore = score;
      if (depth == info->root)
        info->best_move = list->moves[i];
    }
    undo_move(gs, list->moves[i], temp);
  }
  return bestScore;
}

double nega_max(GAMESTATE *gs, int depth, int side, SEARCH_INFO *info,
                double alpha = -INF, double beta = INF) {
  if (depth == 0) {
    return side * eval1(gs);
  }
  MOVES_LIST list[1];
  get_valid_moves(list, gs);
  if (!list->count)
    return side * eval1(gs);
  // shuffle(list->moves, list->moves + list->count,
  // default_random_engine(seed));
  int temp = gs->last_square;
  for (int i = 0; i < list->count; ++i) {
    make_move(gs, list->moves[i]);
    double score = -nega_max(gs, depth - 1, -side, info, -beta, -alpha);
    undo_move(gs, list->moves[i], temp);
    if (score >= beta)
      return beta;
    if (score > alpha) {
      alpha = score;
      if (depth == info->root)
        info->best_move = list->moves[i];
    }
  }
  return alpha;
}

int nega_engine(GAMESTATE *gs) {
  int DEPTH = 10;

  SEARCH_INFO info[1];
  info->root = DEPTH;
  std::cout << nega_max(gs, DEPTH, (gs->side) ? 1 : -1, info) << '\n';
  return info->best_move;
}

double HC_max(GAMESTATE *gs, int depth, int side, SEARCH_INFO *info,
              double alpha = -INF, double beta = INF) {
  if (depth == 0)
    return side * HC_eval(gs);
  MOVES_LIST list[1];
  get_valid_moves(list, gs);
  if (!list->count)
    return side * HC_eval(gs);
  // shuffle(list->moves, list->moves + list->count,
  // default_random_engine(seed));
  int temp = gs->last_square;
  for (int i = 0; i < list->count; ++i) {
    make_move(gs, list->moves[i]);
    double score = -HC_max(gs, depth - 1, -side, info, -beta, -alpha);
    undo_move(gs, list->moves[i], temp);
    if (score >= beta)
      return beta;
    if (score > alpha) {
      alpha = score;
      if (depth == info->root)
        info->best_move = list->moves[i];
    }
  }
  return alpha;
}

int HC_engine(GAMESTATE *gs) {
  int DEPTH = 10;

  SEARCH_INFO info[1];
  info->root = DEPTH;
  std::cout << HC_max(gs, DEPTH, (gs->side) ? 1 : -1, info) << '\n';
  return info->best_move;
}