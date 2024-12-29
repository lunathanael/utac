#include "eval.hpp"
#include "utils.hpp"

double eval1(GAMESTATE *gs) {
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

static int hash_board(int board, int board_occ) {
  int hash = 0;
  for (int i = 8; i >= 0; --i) {
    hash *= 3;
    if (board_occ & (1 << i)) {
      hash += board & (1 << i);
    } else {
      hash += 2;
    }
  }
  return hash;
}

static std::pair<int, int> hash_to_board(int hash) {
  int board = 0;
  int board_occ = 0;
  for (int i = 0; i < 9; ++i) {
    int q = hash % 3;
    hash /= 3;
    if (q == 2)
      continue;
    board_occ |= (1 << (i));
    board |= (q << (i));
  }
  return std::make_pair(board, board_occ);
}

static constexpr double dfs_helper(int board, int board_occ, int square, int depth) {
  if (wincheck[board])
    return OPEN_LANE_HEURISTIC / (depth * depth);
  if (square == 9)
    return 0;
  double score = 0;
  for (int i = square; i < 9; ++i) {
    if (board_occ & (1 << i))
      continue;
    score += dfs_helper(board | (1 << i), board_occ | (1 << i), square + 1,
                        depth + 1);
  }
  return score;
}

static std::array<double, 0x4ce3> init_open_lane_heuristic() {
  std::array<double, 0x4ce3> ret{};
  for (int i = 0; i < 0x4ce3; ++i) {
    auto [board, board_occ] = hash_to_board(i);
    int other_board = (~board) & board_occ;

    if (wincheck[board]) {
      ret[i] = FULL_BOARD_HEURISTIC;
      continue;
    }
    if (wincheck[other_board]) {
      ret[i] = -FULL_BOARD_HEURISTIC;
      continue;
    }

    double score = 0;
    score += dfs_helper(board, board_occ, 0, 0);
    score -= dfs_helper(other_board, board_occ, 0, 0);
    ret[i] = score;
  }
  return ret;
}

static const std::array<double, 0x4ce3> open_lane_heuristic = init_open_lane_heuristic();


static constexpr inline double eval_board(int board, int occ) {
  return open_lane_heuristic[hash_board(board, occ)];
}
