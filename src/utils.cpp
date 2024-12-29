#include "utils.hpp"

void print_board(std::array<int, 9> board, std::array<int, 9> occ) {
  std::cout << "   1 2 3  4 5 6  7 8 9\n";
  for (int i = 0; i < 9; ++i) {
    if (i % 3 == 0)
      std::cout << "  ----------------------\n";
    std::cout << i + 1 << ' ';
    for (int j = 0; j < 9; ++j) {
      if (j % 3 == 0)
        std::cout << '|';
      int grid = i / 3 * 3 + j / 3;
      int grid_square = i % 3 * 3 + j % 3;
      std::cout << int_to_char[2 * (board[grid] >> grid_square & 1) +
                               ((~board[grid] & occ[grid]) >> grid_square & 1)]
                << ' ';
    }
    std::cout << "|\n";
  }
  std::cout << "  ----------------------\n";
}