#ifndef STATE_HPP_INCLUDED
#define STATE_HPP_INCLUDED

#include "types.hpp"
#include <array>

class State {
private:
    GAMESTATE gs;
public:
    State();
    void make_move(int move);
    std::array<std::array<int, 81>, 2> get_obs() const;
    std::array<int, 81> get_valid_moves() const;
    std::array<int, 81> get_valid_mask() const;
    bool is_terminal() const;
    int terminal_value() const;
    void print() const;
};

#endif