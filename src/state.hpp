#ifndef STATE_HPP_INCLUDED
#define STATE_HPP_INCLUDED

#include "types.hpp"
#include <array>
#include <vector>

class State {
private:
    GAMESTATE gs;
public:
    State();
    State(const GAMESTATE& gs);
    State(const State& state);
    void make_move(int move);
    std::array<std::array<int, 81>, 4> get_obs() const;
    std::vector<int> get_valid_moves() const;
    std::array<int, 81> get_valid_mask() const;
    bool is_terminal() const;
    int terminal_value() const;
    int current_player() const;
    void print() const;

    GAMESTATE _get_gs() const;
};

#endif
