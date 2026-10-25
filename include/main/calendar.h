#ifndef MAIN_CALENDAR_H
#define MAIN_CALENDAR_H

#include "common.h"

void calendar_screen_init(void);
s16 calendar_cell_x(s32 month_side, s32 day_index);
s16 calendar_cell_y(s32 month_side, s32 day_index);
void calendar_month_load(u8 calendar_year, u8 calendar_month, s32 populate_next_month);
void calendar_view_update();

#ifndef MAIN_API_OVERRIDE_month_data_table
extern s32 **month_data_table;
#endif

s32 birth_day_check_days(s32 birthday_subject, s32 month, s32 day);
void cal_base_show(void);
void cal_sprite_init(void);

void calendar_marks_build(void);

#endif
