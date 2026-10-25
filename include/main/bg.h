#ifndef MAIN_BG_H
#define MAIN_BG_H

#include "common.h"

extern s32 bg_lba_cache[4];
#ifndef MAIN_API_OVERRIDE_dec_bg_show
extern u8 dec_bg_show[];  /* flags; bg_brightness and bg_fade_amount are also declared as scalars */
#endif
extern u8 bg_brightness;
extern u8 bg_fade_amount;
extern s32 bg_wipe_count;
extern s32 dec_bg_last_screen_mode;
extern s32 bg_decode_result;
extern u32 bg_layers;
void bg_layers_clear(s32 arg0, s32 arg1);
void bg_still_buffers_set(s32 arg0, s32 arg1, s32 arg2);
s32 bg_still_decode_to_vram();
#ifndef MAIN_API_OVERRIDE_set_dec_bri
void set_dec_bri(u8 arg0);
#endif
s32 get_last_gamen_mode(void);
void dec_bg_reset(void);
void dec_bg_show_switch(s32 arg0);
void dec_bg_show_set(s32 arg0, s32 arg1);
s32 dec_bg_cd_read(s32 arg0, s32 arg1);
s32 dec_bg_decode_to_vram();
void bg_capture_screen();
s32 bg_wipe_step(void);
void dec_bg_draw(s32 a);
void bg_fade_draw(s32);
void bg_crossfade_draw();
void bg_read_sub2(s32 arg0);

extern s32 bg_still_variable_length_code_buffer;
extern s32 bg_still_out_buffer;

#ifndef MAIN_API_OVERRIDE_prim_bg_slot_setup
void prim_bg_slot_setup();
#endif
void change_dec_bg(void);
s32 dec_init(void);

extern s32 D_800B5950[];
extern s32 D_800B5960[];

#endif
