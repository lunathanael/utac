#include "eval.hpp"
#include "utils.hpp"

#include <algorithm>
#include <cmath>
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

class BoardHeuristic {
public:
  std::array<double, 9> scores;
  double total_score;

  BoardHeuristic() : scores{}, total_score{} {}
};

static constexpr inline int hash_board(int board, int board_occ) {
  int hash = 0;
  for (int i = 8; i >= 0; --i) {
    hash *= 3;
    if (board_occ & (1 << i)) {
      hash += (board >> i) & 1;
    } else {
      hash += 2;
    }
  }
  return hash;
}

static constexpr inline std::pair<int, int> hash_to_board(int hash) {
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

static void dfs_helper(BoardHeuristic &ret, int board, int board_occ,
                       int square, int depth) {
  if (wincheck[board]) {
    double score = (depth == 0) ? FULL_BOARD_HEURISTIC
                                : OPEN_LANE_HEURISTIC / (depth * depth);
    ret.total_score += score;
    if ((board & 7) == 7) {
      ret.scores[0] += score;
      ret.scores[1] += score;
      ret.scores[2] += score;
    }
    if ((board & 56) == 56) {
      ret.scores[3] += score;
      ret.scores[4] += score;
      ret.scores[5] += score;
    }
    if ((board & 448) == 448) {
      ret.scores[6] += score;
      ret.scores[7] += score;
      ret.scores[8] += score;
    }

    if ((board & 73) == 73) {
      ret.scores[0] += score;
      ret.scores[3] += score;
      ret.scores[6] += score;
    }
    if ((board & 146) == 146) {
      ret.scores[1] += score;
      ret.scores[4] += score;
      ret.scores[7] += score;
    }
    if ((board & 292) == 292) {
      ret.scores[2] += score;
      ret.scores[5] += score;
      ret.scores[8] += score;
    }

    if ((board & 273) == 273) {
      ret.scores[0] += score;
      ret.scores[4] += score;
      ret.scores[8] += score;
    }
    if ((board & 84) == 84) {
      ret.scores[2] += score;
      ret.scores[4] += score;
      ret.scores[6] += score;
    }

    return;
  }
  if (square == 9)
    return;
  for (int i = square; i < 9; ++i) {
    if (board_occ & (1 << i))
      continue;
    dfs_helper(ret, board | (1 << i), board_occ | (1 << i), square + 1,
               depth + 1);
  }
}

static std::array<double, 9> softmax(const std::array<double, 9> &input) {
  std::array<double, 9> output;
  double max_val = *std::max_element(input.begin(), input.end());
  double sum = 0.0;

  for (int i = 0; i < 9; ++i) {
    output[i] = std::exp(input[i] - max_val);
    sum += output[i];
  }
  for (int i = 0; i < 9; ++i) {
    output[i] /= sum;
  }

  return output;
}

static std::array<BoardHeuristic, 0x4ce3> init_open_lane_heuristic() {
  std::array<BoardHeuristic, 0x4ce3> ret{};
  for (int i = 0; i < 0x4ce3; ++i) {
    auto [board, board_occ] = hash_to_board(i);
    int other_board = (~board) & board_occ;

    BoardHeuristic score, other_score;
    if (!wincheck[other_board])
      dfs_helper(score, board, board_occ, 0, 0);
    if (!wincheck[board])
      dfs_helper(other_score, other_board, board_occ, 0, 0);

    for (int i = 0; i < 9; ++i) {
      score.scores[i] -= other_score.scores[i];
    }
    score.total_score -= other_score.total_score;

    score.scores = softmax(score.scores);
    ret[i] = score;
  }
  return ret;
}

static std::array<BoardHeuristic, 0x4ce3> open_lane_heuristic{};

namespace Evals {
void init() { open_lane_heuristic = init_open_lane_heuristic(); }
} // namespace Evals

double HC_eval(GAMESTATE *gs) {
  if (wincheck[gs->main_board])
    return WIN_SCORE; // X VICTORY
  if (wincheck[(~gs->main_board) & gs->main_occ])
    return -WIN_SCORE; // O VICTORY
  if (gs->game_occ == 511)
    return DRAW_SCORE; // DRAW

  double score = 0;
  BoardHeuristic main_score =
      open_lane_heuristic[hash_board(gs->main_board, gs->main_occ)];
  for (int i = 0; i < 9; ++i) {
    BoardHeuristic sub_score =
        open_lane_heuristic[hash_board(gs->board[i], gs->occ[i])];
    score += main_score.scores[i] * sub_score.total_score;
  }
  return score;
}