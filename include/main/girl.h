#ifndef MAIN_GIRL_H
#define MAIN_GIRL_H

#include "common.h"

/* T-20940: name table, 16 string pointers up to D_800B3578 (12 girls, then four more). telephone_class_init
 * reads entry i - 1 for menu row i (splat's D_800B3534 is that -4 relocation) and entries 12 and 13 as
 * D_800B3568, D_800B356C; TEL and OPTION index it by girl. */
extern u8 *girl_full_name_table[16];
extern u8 *girl_short_name_table[];
extern u8 *girl_short_name_rei;
extern u8 *girl_short_name_yumi_given;
/* T-20940: the 13 telephone numbers, seven digits each (0x5B bytes, D_800B37D4 follows after one pad
 * byte): holiday_tel_exit plays digit k of number game_state.current_girl (row 10 as D_800B37BE), and
 * holiday_phone_dial_shortcut compares each row with the last seven keys in holiday_dial_keys. */
extern u8 girl_phone_digits[13][7];
extern u8 girl_tokimeki_weight[13];
extern u8 girl_bonus_param_mask[13];
extern u8 girl_param_ladder_kind[13];
extern s8 girl_param_bonus[13][8];
extern u8 girl_param_ladder_base[7];
extern u8 girl_affection_weight[];
extern u8 girl_closeness_grid[20][20];
extern u8 girl_shiori_param_minimum_tail[]; /* table read by girl_points_shiori */
extern s8 girl_syoushin_weight[12];
extern u8 girl_points_mask_strong[12];
extern u8 girl_points_mask_normal[12];
extern s8 girl_points_param_curve[15];
extern s8 girl_points_stress_curve[15];
extern u8 girl_name_buf[];
s32 girl_player_sports_club_check();
s32 girl_state_rank(s32 a);
#ifndef MAIN_API_OVERRIDE_girl_scores
s32 girl_scores();
#endif
/* T-11190: declarations added by tools/bulk_m2c.py. */
/* T-11190: declarations added by tools/bulk_m2c.py. */

/* T-11190: declarations added by tools/bulk_m2c.py. */

/* T-11190: declarations added by tools/bulk_m2c.py. */

/* T-11190: declarations added by tools/bulk_m2c.py. */

/* T-11190: declarations added by tools/bulk_m2c.py. */

/* T-11190: declarations added by tools/bulk_m2c.py. */

/* T-11190: declarations added by tools/bulk_m2c.py. */

/* T-11190: declarations added by tools/bulk_m2c.py. */

/* T-11190: declarations added by tools/bulk_m2c.py. */
extern u8 girl_name_suffix_familiar;
extern u8 girl_name_suffix_polite;

#ifndef MAIN_API_OVERRIDE_set_c_girl
void set_c_girl(u8 girl_id);
#endif
s32 get_g_name(u8 *name_buf, s32 girl_id);
void get_p_name();
#ifndef MAIN_API_OVERRIDE_get_g_zyotai_s
s32 get_g_zyotai_s(s32 girl_id);
#endif
#ifndef MAIN_API_OVERRIDE_get_g_zyotai_h
s32 get_g_zyotai_h(s32 girl_id);
#endif
#ifndef MAIN_API_OVERRIDE_get_h_tokimeki
u32 get_h_tokimeki(s32 girl_id);
#endif
#ifndef MAIN_API_OVERRIDE_get_h_yuukou
u32 get_h_yuukou(s32 girl_id);
#endif

#endif
