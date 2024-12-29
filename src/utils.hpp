#ifndef UTILS_HPP_INCLUDED
#define UTILS_HPP_INCLUDED

#include <array>
#include <iostream>
#include <chrono>


#define GetTickCount()                                                         \
  std::chrono::duration_cast<std::chrono::milliseconds>(                       \
      std::chrono::steady_clock::now().time_since_epoch())                     \
      .count()

namespace {
    static constexpr std::array<bool, 512> init_wincheck() {
    std::array<bool, 512> ret{};
    for (int i = 0; i < 512; ++i) {
        ret[i] = ((i & 7) == 7) || ((i & 56) == 56) || ((i & 448) == 448) ||
                ((i & 73) == 73) || ((i & 146) == 146) || ((i & 292) == 292) ||
                ((i & 273) == 273) || ((i & 84) == 84);
    }
    return ret;
    }

    static constexpr std::array<int, 81> init_square_to_grid_square() {
    std::array<int, 81> ret{};
    for (int i = 0; i < 81; ++i) {
        int row = i / 9, col = i % 9;
        ret[i] = row % 3 * 3 + col % 3;
    }
    return ret;
    }

    static constexpr std::array<int, 81> init_square_to_grid() {
    std::array<int, 81> ret{};
    for (int i = 0; i < 81; ++i) {
        int row = i / 9, col = i % 9;
        ret[i] = row / 3 * 3 + col / 3;
    }
    return ret;
    }

    static constexpr std::array<int, 9> init_grid_to_start_square() {
    std::array<int, 9> ret{};
    for (int i = 0; i < 9; ++i) {
        ret[i] = i / 3 * 27 + i % 3 * 3;
    }
    return ret;
    }
}

constexpr std::array<bool, 512> wincheck = init_wincheck();
constexpr std::array<int, 81> square_to_grid_square =
    init_square_to_grid_square();
constexpr std::array<int, 81> square_to_grid = init_square_to_grid();
constexpr std::array<int, 9> grid_to_start_square = init_grid_to_start_square();

// unsigned seed;

constexpr std::array<char, 3> int_to_char = {'.', 'O', 'X'};

extern void print_board(std::array<int, 9> board, std::array<int, 9> occ);

#endif