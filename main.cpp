#include <bits/stdc++.h>
#include <algorithm>

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

bool eval[512];
int square_to_grid_square[81];
int square_to_grid[81];
int grid_to_start_square[9];


void dfs_helper(int& tmp, int a) {
	if (a == 9) {
		eval[tmp] = ((tmp & 7) == 7);
		eval[tmp] |= ((tmp & 56) == 56);
		eval[tmp] |= ((tmp & 448) == 448);
		eval[tmp] |= ((tmp & 273) == 273);
		eval[tmp] |= ((tmp & 84) == 84);
		return;
	}
	dfs_helper(tmp, a + 1);
	tmp |= (1 << a);
	dfs_helper(tmp, a + 1);
	tmp &= (~(1 << a));
}

void init_eval() {
	int tmp = 0;
	dfs_helper(tmp, 0);
}

void init_stg() {
	for (int i = 0; i < 81; ++i) {
		int row = i / 9, col = i % 9, grid_square = row % 3 * 3 + col % 3;
		square_to_grid_square[i] = grid_square;
		square_to_grid[i] = row / 3 * 3 + col / 3;
	}
	for (int i = 0; i < 9; ++i) {
		int square = i / 3 * 27 + i % 3 * 3;
		grid_to_start_square[i] = square;
	}
}

int evaluate(int board[], int board_occ[]) {
	int tmp = 0;
	int occ = 0;
	for (int i = 0; i < 9; ++i) {
		tmp |= ((eval[board[i]]) << i);
		occ |= ((eval[board[i]]) << i);
		occ |= ((eval[~board[i] & board_occ[i]]) << i);
	}
	return (eval[tmp]) - (eval[~tmp & occ]);
}

char int_to_char[] = { '.', 'O', 'X' };
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

void get_valid_moves_old(MOVES_LIST* list, GAMESTATE* gs) {
	if ((gs->game_occ >> square_to_grid[gs->last_square]) & 1) {
		for (int a = 0; a < 9; ++a) {
			if (gs->game_occ >> a & 1) continue;
			int ss = grid_to_start_square[a];
			for (int i = 0; i < 3; ++i) {
				for (int j = 0; j < 3; ++j) {
					if (!(gs->occ[a] >> (i * 3 + j) & 1)) list->moves[list->count++] = ss + i * 9 + j;
				}
			}
		}
	}
	else {
		int ss = grid_to_start_square[square_to_grid[gs->last_square]];
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				if (!(gs->occ[square_to_grid[gs->last_square]] >> (i * 3 + j) & 1)) list->moves[list->count++] = ss + i * 9 + j;
			}
		}
	}

}
void get_valid_moves(MOVES_LIST* list, int occ[], const int square, const int game_occ) {
	int a, l;
	if ((game_occ >> square) & 1) {
		a = 0, l = 9;
	}
	else {
		a = square, l = square + 1;
	}
	for (; a < l; ++a) {
		if (game_occ >> a & 1) continue;
		int ss = grid_to_start_square[a];
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				if (!(occ[a] >> (i * 3 + j) & 1)) list->moves[list->count++] = ss + i * 9 + j;
			}
		}
	}
}

void get_valid_moves(MOVES_LIST* list, GAMESTATE*gs) {
	int a, l;
	if ((gs->game_occ >> gs->last_square) & 1) {
		a = 0, l = 9;
	}
	else {
		a = gs->last_square, l = gs->last_square + 1;
	}
	for (; a < l; ++a) {
		if (gs->game_occ >> a & 1) continue;
		int ss = grid_to_start_square[a];
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				if (!(gs->occ[a] >> (i * 3 + j) & 1)) list->moves[list->count++] = ss + i * 9 + j;
			}
		}
	}
}

int top_left_engine(const int square, int board[], int occ[], const int game_occ) {
	MOVES_LIST list[1];
	get_valid_moves(list, occ, square, game_occ);
	return list->moves[0];
};


int bottom_right_engine(const int square, int board[], int occ[], const int game_occ) {
	MOVES_LIST list[1];
	get_valid_moves(list, occ, square, game_occ);
	return list->moves[list->count - 1];
};

int bottom_right_engine(GAMESTATE*gs) {
	MOVES_LIST list[1];
	get_valid_moves(list, gs);
	return list->moves[list->count - 1];
};


int human_engine(const int square, int board[], int occ[], const int game_occ) {
	print_board(board, occ);
	if (square != -1) {
		cout << "Pick a move in game " << square<< ". (row then column indexed by 0):  ";
	}
	else {
		cout << ("Pick any square") << ". (row then column indexed by 0):  ";
	}
	MOVES_LIST list[1];
	get_valid_moves(list, occ, square, game_occ);
	for (int i = 0; i < list->count; ++i) {
		cout << '\n' << list->moves[i] << ' ' << list->moves[i] / 9 + 1 << list->moves[i] % 9 + 1;
	}
	cout << '\n';

	int row, col;
	cin >> row >> col;
	return (row-1) * 9 + col - 1;
};

static inline bool game_over(GAMESTATE*gs) {
	return gs->game_occ == 511 || eval[gs->main_board] || eval[~gs->main_board & gs->main_occ];
}

static inline int make_move(GAMESTATE* gs, const int square) {
	int curr_square = square_to_grid[square];
	int grid_square = square_to_grid_square[square];
	gs->occ[curr_square] |= (1 << grid_square);
	gs->board[curr_square] |= (gs->side << grid_square);
	gs->side ^= 1;

	//update gs
	gs->game_occ |= (eval[gs->board[curr_square]] || eval[~gs->board[curr_square] & gs->occ[curr_square]] || gs->occ[curr_square] == 511) << curr_square;
	gs->main_occ |= (eval[gs->board[curr_square]] || eval[~gs->board[curr_square] & gs->occ[curr_square]]) << curr_square;
	gs->main_board |= eval[gs->board[curr_square]] << curr_square;
	return grid_square;
}

int test(int(*engX)(GAMESTATE* gs), int(*engO)(GAMESTATE * gs)) {
	GAMESTATE gs[1];

	// set to random later
	gs->last_square = 4;

	while (!game_over(gs)) {
		print_board(gs->board, gs->occ);
		int chosen_square = (gs->side) ? engX(gs) : engO(gs);
		// verify move
		MOVES_LIST list[1];
		get_valid_moves(list, gs);
		for (int i = 0; i < list->count; ++i) {
			cout << list->moves[i] << ' ';
		}
		cout << '\n';
		if (!find(list->moves, list->moves + list->count, chosen_square)) {
			cout << "INVALID: " << chosen_square << '\n'; for (int i : list->moves) { cout << i << ' '; } return -100;
		}
		gs->last_square = make_move(gs, chosen_square);
	}
	return (eval[gs->main_board]) - (eval[~gs->main_board & gs->main_occ]);
}



int game(int(*engX)(const int, int[], int[], const int), int(*engO)(const int, int[], int[], const int)) {
	int board_occ[9] = { 0 };
	int board[9] = { 0 };

	int game_occ = 0;
	int main_occ = 0;
	int main_board = 0;

	bool side = 1;
	int curr_square = 4;
	while (game_occ != 511 && !eval[main_board] && !eval[~main_board & main_occ]) {
		int chosen_square = (side) ? engX(curr_square, board, board_occ, game_occ) : engO(curr_square, board, board_occ, game_occ);
		MOVES_LIST list[1];
		get_valid_moves(list, board_occ, curr_square, game_occ);
		int row = chosen_square / 9, col = chosen_square % 9, grid_square = row % 3 * 3 + col % 3;
		if (!find(list->moves, list->moves + list->count, chosen_square)) {
			cout << "INVALID: " << chosen_square << '\n'; for (int i : list->moves) { cout << i << ' '; } return -100;
		}
		curr_square = row / 3 * 3 + col / 3;
		board_occ[curr_square] |= (1 << (grid_square));
		board[curr_square] |= (side << grid_square);
		side ^= 1;
		if (eval[board[curr_square]] || eval[~board[curr_square] & board_occ[curr_square]] || board_occ[curr_square] == 511) {
			game_occ |= 1 << curr_square;
			main_occ |= (eval[board[curr_square]] || eval[~board[curr_square] & board_occ[curr_square]]) << curr_square;
			main_board |= eval[board[curr_square]] << curr_square;
		}
		curr_square = grid_square;//((game_occ >> grid_square) & 1) ? -1 : 
	}
	print_board(board, board_occ);
	cout << "eval: " << (eval[main_board]) - (eval[~main_board & main_occ]) << '\n';
	return evaluate(board, board_occ);
}


int main()
{
	init_eval();
	init_stg();

	cout << test(&bottom_right_engine, &bottom_right_engine);
	//cout << game(&bottom_right_engine, &bottom_right_engine);
	cout << "GAME OVER";
	return 0;
}
