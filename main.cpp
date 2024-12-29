#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>
#include <cstring>
#include <array>
#define GetTickCount() std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count()

using namespace std;


enum SIDES {
	SIDE_X, SIDE_O, NO_SIDE
};

enum COLS {
	COL_0, COL_1, COL_2, COL_3, COL_4, COL_5, COL_6, COL_7, COL_8
};

enum ROWS {
	ROW_0, ROW_1, ROW_2, ROW_3, ROW_4, ROW_5, ROW_6, ROW_7, ROW_8
};

typedef struct {
	int moves[81];
	int count = 0;
} MOVES_LIST;

typedef struct {
	int occ[9] = { 0 };
	int board[9] = { 0 };

	int game_occ = 0;
	int main_occ = 0;
	int main_board = 0;
	int side = 1;
	int last_square = -1;
}GAMESTATE;

typedef struct {
	int root;
	int best_move;
}SEARCH_INFO;


// void dfs_helper(int tmp, int a) {
// 	if (a == 9) {
// 		return;
// 	}
// 	dfs_helper(tmp, a + 1);
// 	tmp |= (1 << a);
// 	dfs_helper(tmp, a + 1);
// }

constexpr std::array<bool, 512> init_eval() {
	std::array<bool, 512> ret{};
	for (int i = 0; i < 512; ++i) {
		ret[i] = ((i & 7) == 7) ||
				 ((i & 56) == 56) ||
				 ((i & 448) == 448) ||
				 ((i & 73) == 73) ||
				 ((i & 146) == 146) ||
				 ((i & 292) == 292) ||
				 ((i & 273) == 273) ||
				 ((i & 84) == 84);
	}
	return ret;
}


constexpr std::array<bool, 512> eval = init_eval();

constexpr std::array<int, 81> init_square_to_grid_square() {
    std::array<int, 81> ret{};
    for (int i = 0; i < 81; ++i) {
        int row = i / 9, col = i % 9;
        ret[i] = row % 3 * 3 + col % 3;
    }
    return ret;
}

constexpr std::array<int, 81> init_square_to_grid() {
    std::array<int, 81> ret{};
    for (int i = 0; i < 81; ++i) {
        int row = i / 9, col = i % 9;
        ret[i] = row / 3 * 3 + col / 3;
    }
    return ret;
}

constexpr std::array<int, 9> init_grid_to_start_square() {
    std::array<int, 9> ret{};
    for (int i = 0; i < 9; ++i) {
        ret[i] = i / 3 * 27 + i % 3 * 3;
    }
    return ret;
}

constexpr std::array<int, 81> square_to_grid_square = init_square_to_grid_square();
constexpr std::array<int, 81> square_to_grid = init_square_to_grid();
constexpr std::array<int, 9> grid_to_start_square = init_grid_to_start_square();

// unsigned seed;

constexpr std::array<char, 3> int_to_char = { '.', 'O', 'X' };

void print_board(int board[], int occ[]) {
	cout << "   1 2 3  4 5 6  7 8 9\n";
	for (int i = 0; i < 9; ++i) {
		if (i % 3 == 0) cout << "  ----------------------\n";
		cout << i + 1 << ' ';
		for (int j = 0; j < 9; ++j) {
			if (j % 3 == 0) cout << '|';
			int grid = i / 3 * 3 + j / 3;
			int grid_square = i % 3 * 3 + j % 3;
			cout << int_to_char[2 * (board[grid] >> grid_square & 1) + ((~board[grid] & occ[grid]) >> grid_square & 1)] << ' ';
		}
		cout << "|\n";
	}
	cout << "  ----------------------\n";
}


static constexpr inline bool game_over(GAMESTATE*gs) {
	return gs->game_occ == 511 || eval[gs->main_board] || eval[(~gs->main_board) & gs->main_occ];
}

void get_valid_moves(MOVES_LIST* list, GAMESTATE*gs) {
	if (game_over(gs)) return;
	int a, l;
	if ((gs->game_occ >> gs->last_square) & 1) {
		a = 0, l = 9;
	}
	else {
		a = gs->last_square, l = gs->last_square + 1;
	}
	for (; a < l; ++a) {
		if ((gs->game_occ >> a) & 1) continue;
		int ss = grid_to_start_square[a];
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				if (!(gs->occ[a] >> (i * 3 + j) & 1)) {
					list->moves[list->count++] = ss + i * 9 + j;
				}
			}
		}
	}
}

static inline int eval1(GAMESTATE* gs) {
	if (eval[gs->main_board]) return 1000; // X VICTORY
	if (eval[(~gs->main_board) & gs->main_occ]) return -1000; // O VICTORY
	if (gs->game_occ == 511) return 0; // DRAW

	// speculative eval
	return (__builtin_popcount(gs->main_board) - __builtin_popcount((~gs->main_board) & gs->main_occ));
}

int human_engine(GAMESTATE* gs) {
	MOVES_LIST list[1];
	get_valid_moves(list, gs);
	int row, col;
	do {
		cout << "Player " << ((gs->side) ? "X" : "O") << ", choose a square in game " << gs->last_square + 1 << ". (row then column) : ";
		cin >> row >> col;
	} while (find(list->moves, list->moves + list->count, (row - 1) * 9 + col - 1) == list->moves + list->count);
	return (row - 1) * 9 + col - 1;
};


static inline void make_move(GAMESTATE* gs, const int square) {
	int curr_square = square_to_grid[square];
	int grid_square = square_to_grid_square[square];
	gs->occ[curr_square] |= (1 << grid_square);
	gs->board[curr_square] |= (gs->side << grid_square);
	gs->side ^= 1;

	//update gs
	gs->game_occ |= (eval[gs->board[curr_square]] || eval[(~gs->board[curr_square]) & gs->occ[curr_square]] || gs->occ[curr_square] == 511) << curr_square;
	gs->main_occ |= (eval[gs->board[curr_square]] || eval[(~gs->board[curr_square]) & gs->occ[curr_square]]) << curr_square;
	gs->main_board |= eval[gs->board[curr_square]] << curr_square;
	
	gs->last_square = grid_square;
	return;
}


static inline void undo_move(GAMESTATE* gs, const int square, const int prev_last) {
	int curr_square = square_to_grid[square];
	int grid_square = square_to_grid_square[square];
	gs->occ[curr_square] &= ~(1 << grid_square);
	gs->board[curr_square] &= ~(1 << grid_square);
	gs->side ^= 1;

	//update gs
	gs->game_occ &= ~(1 << curr_square);
	gs->main_occ &= ~(1 << curr_square);
	gs->main_board &= ~(1 << curr_square);

	gs->last_square = prev_last;
	return;
}

int game(int(*engX)(GAMESTATE* gs), int(*engO)(GAMESTATE * gs), bool print) {
	GAMESTATE gs[1];

	// set to random later
	gs->last_square = (rand() % 9);
	if (print) print_board(gs->board, gs->occ);

	while (!game_over(gs)) {
		int chosen_square = (gs->side) ? engX(gs) : engO(gs);
		// verify move
		MOVES_LIST list[1];
		get_valid_moves(list, gs);
		if (find(list->moves, list->moves + list->count, chosen_square) == list->moves + list->count) {
			cout << "INVALID: " << chosen_square << '\n'; for (int i : list->moves) { cout << i << ' '; } return -100;
		}
		make_move(gs, chosen_square);
		if (print) {
			print_board(gs->board, gs->occ);
			cout << "Player ";
			cout << ((gs->side) ? "O" : "X") << " chose square on row " << chosen_square / 9 + 1 << " and column " << chosen_square % 9 + 1 << ".\n";
		}
	}
	int res = (eval[gs->main_board]) - (eval[(~gs->main_board) & gs->main_occ]);
	if (print) {
		cout << "GAME OVER!\n";
		cout << "Result was a ";
		if (res != 0) {
			cout << "victory for player " << ((res == 1) ? "X" : "O") << ".\n";
		}
		else {
			cout << "draw!\n";
		}

	}
	return res;
}



long long p_nodes;

static inline void perft_driver(int depth, GAMESTATE* gs)
{
	if (depth == 0)
	{
		++p_nodes;
		return;
	}

	MOVES_LIST list[1];
	get_valid_moves(list, gs);

	for (int move_count = 0; move_count < list->count; ++move_count)
	{
		int temp = gs->last_square;
		make_move(gs, list->moves[move_count]);
		perft_driver(depth - 1, gs);
		undo_move(gs, list->moves[move_count], temp);
	}
}

static inline void perft_test(int depth)
{
	p_nodes = 0;
	cout << "\n     Performance test\n\n";
	cout << "\n     Move:    Nodes:\n";

	GAMESTATE gs[1];
	gs->last_square = (rand() % 9);

	// create move list instance
	MOVES_LIST move_list[1];

	// generate moves
	get_valid_moves(move_list, gs);

	// init start time
	long start = GetTickCount();

	// loop over generated moves
	for (int move_count = 0; move_count < move_list->count; move_count++)
	{
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
		cout << "     "<< move_list->moves[move_count] / 9 + 1 << ' ' << move_list->moves[move_count] % 9 + 1 << "  "<< old_nodes <<"\n";
	}

	long time = GetTickCount() - start;
	// print results
	cout << "\n    Depth: " << depth << "ply\n";
	cout << "    Nodes: " << p_nodes << "\n";
	cout << "     Time: " << time << " ms\n";
	cout << "      Nps: " << ((p_nodes / time) / 1000) << "MN / s\n\n";
}


int nega_min_max(GAMESTATE* gs, int depth, int side, SEARCH_INFO*info) {
	if (depth == 0) {
		return side * eval1(gs);
	}
	int bestScore = -10000;
	MOVES_LIST list[1];
	get_valid_moves(list, gs);
	if (!list->count) return side * eval1(gs);
	int temp = gs->last_square;
	for (int i = 0; i < list->count; ++i) {
		make_move(gs, list->moves[i]);
		int score = -nega_min_max(gs, depth - 1, -side, info);
		if (score > bestScore) {
			bestScore = score;
			if (depth == info->root) info->best_move = list->moves[i];
		}
		undo_move(gs, list->moves[i], temp);
	}
	return bestScore;
}

int nega_max(GAMESTATE* gs, int depth, int side, SEARCH_INFO* info, int alpha, int beta) {
	if (depth == 0) {
		return side * eval1(gs);
	}
	MOVES_LIST list[1];
	get_valid_moves(list, gs);
	if (!list->count) return side * eval1(gs);
	// shuffle(list->moves, list->moves + list->count, default_random_engine(seed));
	int temp = gs->last_square;
	for (int i = 0; i < list->count; ++i) {
		make_move(gs, list->moves[i]);
		int score = -nega_max(gs, depth - 1, -side, info, -beta, -alpha);
		undo_move(gs, list->moves[i], temp);
		if (score >= beta) return beta;
		if (score > alpha) {
			alpha = score;
			if (depth == info->root) info->best_move = list->moves[i];
		}
	}
	return alpha;
}

int nega_engine(GAMESTATE* gs) {
	SEARCH_INFO info[1];
	info->root = 14;
	cout << nega_max(gs, 14, (gs->side) ? 1 : -1, info, -10000, 10000) << '\n';
	return info->best_move;
}
int nega_engine1(GAMESTATE* gs) {
	SEARCH_INFO info[1];
	info->root = 9;
	cout << nega_max(gs, 9, (gs->side) ? 1 : -1, info, -10000, 10000) << '\n';
	return info->best_move;
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

// 		if (bestScore > -VALUE_MATE && bestScore < VALUE_MATED_IN_MAX_PLY)
// 		{
// 			std::cout << "info score mate " << -(bestScore + VALUE_MATE) / 2 << " depth " << currentDepth << " nodes " << info->nodes <<
// 				" nps " << nps << " time " << time << " pv ";
// 		}
// 		else if (bestScore > VALUE_MATE_IN_MAX_PLY && bestScore < VALUE_MATE)
// 		{
// 			std::cout << "info score mate " << (VALUE_MATE - bestScore) / 2 + 1 << " depth " << currentDepth << " nodes " << info->nodes <<
// 				" nps " << nps << " time " << time << " pv ";
// 		}
// 		else {
// 			std::cout << "info score cp " << bestScore << " depth " << currentDepth << " nodes " << info->nodes <<
// 				" nps " << nps << " time " << time << " pv ";
// 		}
// 		std::cout << Pr_move(info->bestMove) << std::endl;
// 	}
// 	std::cout << "bestmove " << Pr_move(info->bestMove) << "\n";
// }


int main()
{
	// seed = GetTickCount();
	// srand(seed);
	game(&nega_engine, &human_engine, true);
	return 0;
}
