#ifndef MAIN_BOOT_H
#define MAIN_BOOT_H

#include "common.h"

void boot_loop();
void boot_mode_step(void);
void boot_hud_textures_step(void);
void boot_hud_textures_load(void);
s32 boot_data_load_step(void);
void boot_hud_init(void);

extern s32 D_800C6020;
extern s32 D_800C62C0[32];

void game_state_init(s32 arg0);
void boot_game_data_init(void);
void boot_calendar_init(void);
void boot_player_init(void);
void boot_girls_init(void);
void boot_event_state_init(void);
void boot_settings_defaults_set(void);
void boot_frame_begin(void);

#endif
