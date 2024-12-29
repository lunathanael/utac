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
extern double HC_eval(GAMESTATE *gs);

#endif