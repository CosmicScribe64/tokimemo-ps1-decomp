#ifndef MAIN_STATE_H
#define MAIN_STATE_H

#include "common.h"
#include "game/game_state.h"
#include "game/system_data.h"

extern GameState game_state;
#ifndef MAIN_API_OVERRIDE_system_data
extern SystemData system_data;
#endif

#endif
