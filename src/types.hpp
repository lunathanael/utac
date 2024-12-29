#ifndef TYPES_HPP_INCLUDED
#define TYPES_HPP_INCLUDED

#include <array>

constexpr double INF = 1e9;
constexpr double WIN_SCORE = 1'000'000;
constexpr double DRAW_SCORE = 0;

enum SIDES { SIDE_X, SIDE_O, NO_SIDE };

enum COLS { COL_0, COL_1, COL_2, COL_3, COL_4, COL_5, COL_6, COL_7, COL_8 };

enum ROWS { ROW_0, ROW_1, ROW_2, ROW_3, ROW_4, ROW_5, ROW_6, ROW_7, ROW_8 };

typedef struct {
  std::array<int, 81> moves{};
  int count = 0;
} MOVES_LIST;

typedef struct {
  std::array<int, 9> occ{};
  std::array<int, 9> board{};

  int game_occ = 0;
  int main_occ = 0;
  int main_board = 0;
  int side = 1;
  int last_square = -1;
} GAMESTATE;

typedef struct {
  int root;
  int best_move;
} SEARCH_INFO;

#endif