#include "state.hpp"
#include "moves.hpp"
#include "utils.hpp"

State::State() {
    gs.last_square = -1;
}

void State::make_move(int move) {
    ::make_move(&gs, move);
}

State::State(const GAMESTATE& gs) : gs(gs) {}

State::State(const State& state) : gs(state.gs) {}

std::array<std::array<int, 81>, 4> State::get_obs() const {
    std::array<std::array<int, 81>, 4> obs{};

    for (int i = 0; i < 81; ++i) {
        int grid = square_to_grid[i];
        int grid_square = square_to_grid_square[i];
        if ((gs.board[grid] >> grid_square & 1)) {
            obs[0][i] = 1;
        } else if ((~gs.board[grid] & gs.occ[grid]) >> grid_square & 1) {
            obs[0][i] = -1;
        } else {
            obs[0][i] = 0;
        }
        int subgame = square_to_grid[i];
        if (gs.game_occ >> subgame & 1) {
            obs[1][i] = 1;
            if (gs.main_board >> subgame & 1) {
                obs[2][i] = 1;
            }
            if ((~gs.main_board & gs.main_occ) >> subgame & 1) {
                obs[2][i] = -1;
            }
        }
    }

    obs[3] = get_valid_mask();

    return obs;
}

std::vector<int> State::get_valid_moves() const {
    MOVES_LIST list[1];
    ::get_valid_moves(list, &gs);
    return std::vector<int>(list->moves.begin(), list->moves.begin() + list->count);
}

std::array<int, 81> State::get_valid_mask() const {
    MOVES_LIST list[1];
    ::get_valid_mask(list, &gs);
    return list->moves;
}

bool State::is_terminal() const {
    return game_over(&gs);
}

int State::terminal_value() const {
    int res =
        (wincheck[gs.main_board]) - (wincheck[(~gs.main_board) & gs.main_occ]);
    return res;
}

void State::print() const {
    ::print_board(gs.board, gs.occ);
}

GAMESTATE State::_get_gs() const {
    return gs;
}

int State::current_player() const {
    return gs.side;
}
