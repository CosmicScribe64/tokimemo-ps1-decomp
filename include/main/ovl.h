#ifndef MAIN_OVL_H
#define MAIN_OVL_H

/* T-27061: Overlay loading, entry selection and loader state. */
#include "common.h"

extern u8 ovl_load_screen_mode;
extern u8 ovl_load_state;
extern u8 ovl_load_sectors;
extern s32 ovl_load_buf;
extern s32 ovl_load_lba;
extern u32 ovl_entry;
void ovl_load_reset(void);
void ovl_load_screen_draw();
s32 ovl_load_start();
void ovl_load_poll(void);
void ovl_load_verify(void);
void ovl_load_enter();
s32 game_mode_is_tt_game(void);

#endif /* MAIN_OVL_H */
