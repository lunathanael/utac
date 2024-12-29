#ifndef ENGINES_HPP_INCLUDED
#define ENGINES_HPP_INCLUDED

#include "types.hpp"

extern int human_engine(GAMESTATE *gs);
extern int nega_min_max(GAMESTATE *gs, int depth, int side, SEARCH_INFO *info);

int nega_engine(GAMESTATE *gs);

#endif