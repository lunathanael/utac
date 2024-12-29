#include "perft.hpp"
#include "moves.hpp"
#include "types.hpp"
#include "utils.hpp"

#include <iostream>

long long p_nodes;

static inline void perft_driver(int depth, GAMESTATE *gs) {
  if (depth == 0) {
    ++p_nodes;
    return;
  }

  MOVES_LIST list[1];
  get_valid_moves(list, gs);

  for (int move_count = 0; move_count < list->count; ++move_count) {
    int temp = gs->last_square;
    make_move(gs, list->moves[move_count]);
    perft_driver(depth - 1, gs);
    undo_move(gs, list->moves[move_count], temp);
  }
}

void perft_test(int depth) {
  p_nodes = 0;
  std::cout << "\n     Performance test\n\n";
  std::cout << "\n     Move:    Nodes:\n";

  GAMESTATE gs[1];
  gs->last_square = (rand() % 9);

  // create move list instance
  MOVES_LIST move_list[1];

  // generate moves
  get_valid_moves(move_list, gs);

  // init start time
  long start = GetTickCount();

  // loop over generated moves
  for (int move_count = 0; move_count < move_list->count; move_count++) {
    // preserve board state
    int temp = gs->last_square;
    // make move
    make_move(gs, move_list->moves[move_count]);

    // cummulative nodes
    long long cummulative_nodes = p_nodes;

    // call perft driver recursively
    perft_driver(depth - 1, gs);

    // old nodes
    long long old_nodes = p_nodes - cummulative_nodes;

    // take back
    undo_move(gs, move_list->moves[move_count], temp);
    // print move
    std::cout << "     " << move_list->moves[move_count] / 9 + 1 << ' '
              << move_list->moves[move_count] % 9 + 1 << "  " << old_nodes
              << "\n";
  }

  long time = GetTickCount() - start;
  // print results
  std::cout << "\n    Depth: " << depth << "ply\n";
  std::cout << "    Nodes: " << p_nodes << "\n";
  std::cout << "     Time: " << time << " ms\n";
  std::cout << "      Nps: " << ((p_nodes / time) / 1000) << "MN / s\n\n";
}