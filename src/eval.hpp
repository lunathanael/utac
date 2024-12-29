#ifndef EVAL_HPP_INCLUDED
#define EVAL_HPP_INCLUDED

#include "types.hpp"
#include "utils.hpp"

#include <array>

constexpr double FULL_BOARD_HEURISTIC = 1000;
constexpr double OPEN_LANE_HEURISTIC = 10;

namespace Evals {
void init();
}

extern double eval1(GAMESTATE *gs);

extern int hash_board(int board, int occ);
extern std::pair<int, int> hash_to_board(int hash);

extern std::array<double, 0x4ce3> open_lane_heuristic;

inline double eval_board(int board, int occ) {
  return open_lane_heuristic[hash_board(board, occ)];
}

#endif