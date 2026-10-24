#ifndef MAIN_SCHOOL_DAY_H
#define MAIN_SCHOOL_DAY_H

#include "common.h"

extern s32 D_800B6760[];
extern s32 D_800B6780[];
extern s32 D_800B67AC[];
extern s32 D_800B6810[];
/* T-17000: parameter tables of func_80076FDC, school_day_sd_set_select and parameter_up_down. func_80076FDC
 * reads byte [row][col] of two 12-row, two-byte tables in an 11-case switch (`lbu %lo(D_800B6892)` after
 * `addu` of the column); school_day_sd_set_select starts the next table at D_800B68C0. parameter_up_down walks
 * signed nine-byte rows (`lb`, row index * 9): eight rows at school_day_command_deltas end where club_activity_deltas starts. */
extern u8 D_800B6890[12][2];
extern u8 D_800B68A8[12][2];
/* T-25049: event-selector byte regions. Indexed lbu uses offsets
 * 0,2,4,6,9,11 plus the selection column; offset13 is a scalar byte. */
extern u8 D_800B68C0[16];
extern u8 D_800B68D0[16];
extern s8 school_day_command_deltas[8][9];
extern s8 club_activity_deltas[][9];
void func_800737A0(void);
u8 week_day_init(void);
s32 get_weekly_bg_sector(void);
u8 school_day_screen_init(void);
u8 school_day_sd_load(void);
u8 school_day_sd_actors_init(void);
void func_800744A0();
s32 week_day_main0(void);
void week_day_exit0(void);
void func_800748B8();
void func_80074950();
void func_80074A14();
u8 vacation_day_init(void);
u8 school_day_vacation_screen_init(void);
u8 school_day_vacation_sd_load(void);
u8 school_day_vacation_sd_actors_init(void);

#endif
