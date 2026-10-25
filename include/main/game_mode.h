#ifndef MAIN_GAME_MODE_H
#define MAIN_GAME_MODE_H

/* T-27061: Game mode, stage and step progression. */
#include "common.h"

s32 mode_stage_next(void);
s32 mode_step_next(void);
void game_mode_set();
s32 mode_stage_set(s32 arg0);
s32 mode_step_set(s32 arg0);
s32 game_mode_request_overlay(void);

s32 game_mode_reset_to_title(void);

void game_mode_tt_game_save_step(void);
void wait_sub_sub(s16 wait_frames);

#endif /* MAIN_GAME_MODE_H */
