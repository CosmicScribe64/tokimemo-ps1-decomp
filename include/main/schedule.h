#ifndef MAIN_SCHEDULE_H
#define MAIN_SCHEDULE_H

#include "common.h"

extern s8 schedule_last_picked_girl;
extern s32 schedule_command_rank_bar_liberal_arts_athletics;
extern s32 schedule_command_rank_bar_art;
extern s32 schedule_command_rank_bar_trivia;
extern s32 schedule_command_rank_bar_shiori_athletics;
extern s32 schedule_command_rank_bar_science_looks;
void _schedule_init(void);
void schedule_step_prepare(void);
void schdeule_girls_param(void);
void schedule_step_advance_day(void);
void last_date_spot_timer_dec(void);
void restore_bgm(void);
s32 schedule_memory_card_save_poll(void);
void schedule_girl_spots_expire(void);
void schedule_progress_state_update(void);
s32 schedule_meeting_select_catchup(u8 *arg0, u8 *arg1);
s32 schedule_meeting_select_normal(u8 *arg0, u8 *arg1);
void schedule_girls_gakko(void);
void yuukou_down(void);
void syoushin_up(void);
void schedule_day_screen_step_dispatch(void);
void schedule_ending_start(void);
void calendar_advance_day(void);
void schedule_vacation_notice_step(void);
void func_80072734();
s32 schedule_month_start_step_dispatch(void);
s32 schedule_month_data_load_step(void);
void schedule_month_start_music_wait(void);
void schedule_week_end_hud_init(void);
void schedule_health_step_dispatch(void);
void func_80072FE8();
s32 schedule_meeting_select_club(u8 *arg0, u8 *arg1);

#endif
