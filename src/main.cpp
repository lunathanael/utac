#include <algorithm>
#include <array>
#include <cstring>
#include <iostream>

#include "types.hpp"
#include "utils.hpp"
#include "moves.hpp"
#include "engines.hpp"
#include "perft.hpp"

using namespace std;


int game(int (*engX)(GAMESTATE *gs), int (*engO)(GAMESTATE *gs), bool print) {
  GAMESTATE gs[1];

  // set to random later
  gs->last_square = (rand() % 9);
  if (print)
    print_board(gs->board, gs->occ);

  while (!game_over(gs)) {
    int chosen_square = (gs->side) ? engX(gs) : engO(gs);
    // verify move
    MOVES_LIST list[1];
    get_valid_moves(list, gs);
    if (find(begin(list->moves), begin(list->moves) + list->count, chosen_square) ==
        begin(list->moves) + list->count) {
      cout << "INVALID: " << chosen_square << '\n';
      for (int i : list->moves) {
        cout << i << ' ';
      }
      return -100;
    }
    make_move(gs, chosen_square);
    if (print) {
      print_board(gs->board, gs->occ);
      cout << "Player ";
      cout << ((gs->side) ? "O" : "X") << " chose square on row "
           << chosen_square / 9 + 1 << " and column " << chosen_square % 9 + 1
           << ".\n";
    }
  }
  int res =
      (wincheck[gs->main_board]) - (wincheck[(~gs->main_board) & gs->main_occ]);
  if (print) {
    cout << "GAME OVER!\n";
    cout << "Result was a ";
    if (res != 0) {
      cout << "victory for player " << ((res == 1) ? "X" : "O") << ".\n";
    } else {
      cout << "draw!\n";
    }
  }
  return res;
}

//
//
// static inline void iterative_deepen(GAMESTATE*gs, int depth)
// {
// 	int bestScore = -100000;
// 	int currentDepth = 1;
// 	for (; currentDepth <= depth; ++currentDepth)
// 	{

// 		bestScore = nega_max(gs, depth, (gs->side) ? 1 : -1, depth);

// 		long time = get_time_ms() - info->starttime;
// 		U64 nps = info->nodes / (time + !time) * 1000;

// 		if (bestScore > -VALUE_MATE && bestScore <
// VALUE_MATED_IN_MAX_PLY)
// 		{
// 			std::cout << "info score mate " << -(bestScore +
// VALUE_MATE) / 2 << " depth " << currentDepth << " nodes " << info->nodes <<
// 				" nps " << nps << " time " << time << " pv ";
// 		}
// 		else if (bestScore > VALUE_MATE_IN_MAX_PLY && bestScore <
// VALUE_MATE)
// 		{
// 			std::cout << "info score mate " << (VALUE_MATE -
// bestScore) / 2 + 1 << " depth " << currentDepth << " nodes " << info->nodes
// << 				" nps " << nps << " time " << time << " pv ";
// 		}
// 		else {
// 			std::cout << "info score cp " << bestScore << " depth "
// << currentDepth << " nodes " << info->nodes << 				" nps " << nps << " time " <<
// time << " pv ";
// 		}
// 		std::cout << Pr_move(info->bestMove) << std::endl;
// 	}
// 	std::cout << "bestmove " << Pr_move(info->bestMove) << "\n";
// }

int main() {
  // seed = GetTickCount();
  // srand(seed);
  game(&nega_engine, &nega_engine, true);
  return 0;
}
