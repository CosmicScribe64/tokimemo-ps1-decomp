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
/* T-14501: _schedule_init callees defined later in scheduler.c. */
void schedule_step_prepare(void);
void schdeule_girls_param(void);
void schedule_step_advance_day(void);
void last_date_spot_timer_dec(void);
void restore_bgm(void);
s32 schedule_memory_card_save_poll(void);
void schedule_girl_spots_expire(void);
void schedule_progress_state_update(void);
s32 schedule_meeting_select_catchup(u8 *selected_girl, u8 *scene_variant);
s32 schedule_meeting_select_normal(u8 *selected_girl, u8 *scene_variant);
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
s32 schedule_meeting_select_club(u8 *selected_girl, u8 *scene_variant);

/* T-27056 R4 exact meeting eligibility thresholds, branch order unchanged. */
enum ScheduleMeetingMinimum {
    SCHEDULE_MEETING_CATCHUP_MIO_MINIMUM = 55, /* catchup mio eligibility threshold; same-girl paired stats share this value. */
    SCHEDULE_MEETING_CATCHUP_YUINA_MINIMUM = 60, /* catchup yuina eligibility threshold; same-girl paired stats share this value. */
    SCHEDULE_MEETING_CATCHUP_AYAKO_MINIMUM = 50, /* catchup ayako eligibility threshold; same-girl paired stats share this value. */
    SCHEDULE_MEETING_CATCHUP_YUKARI_MINIMUM = 81, /* catchup yukari eligibility threshold; same-girl paired stats share this value. */
    SCHEDULE_MEETING_CATCHUP_NOZOMI_MINIMUM = 31, /* catchup nozomi eligibility threshold; same-girl paired stats share this value. */
    SCHEDULE_MEETING_CATCHUP_MIRA_MINIMUM = 101, /* catchup mira eligibility threshold; same-girl paired stats share this value. */
    SCHEDULE_MEETING_CATCHUP_SAKI_MINIMUM = 36, /* catchup saki eligibility threshold; same-girl paired stats share this value. */
    SCHEDULE_MEETING_NORMAL_MIO_MINIMUM = 75, /* normal mio eligibility threshold; same-girl paired stats share this value. */
    SCHEDULE_MEETING_NORMAL_YUINA_MINIMUM = 85, /* normal yuina eligibility threshold; same-girl paired stats share this value. */
    SCHEDULE_MEETING_NORMAL_AYAKO_MINIMUM = 65, /* normal ayako eligibility threshold; same-girl paired stats share this value. */
    SCHEDULE_MEETING_NORMAL_YUKARI_MINIMUM = 84, /* normal yukari eligibility threshold; same-girl paired stats share this value. */
    SCHEDULE_MEETING_NORMAL_NOZOMI_MINIMUM = 71, /* normal nozomi eligibility threshold; same-girl paired stats share this value. */
    SCHEDULE_MEETING_NORMAL_MIRA_MINIMUM = 126, /* normal mira eligibility threshold; same-girl paired stats share this value. */
    SCHEDULE_MEETING_NORMAL_SAKI_MINIMUM = 96, /* normal saki eligibility threshold; same-girl paired stats share this value. */
    SCHEDULE_MEETING_YUKO_STEP_0_TRIVIA_MINIMUM = 36, /* Unmet Yuko story-step trivia requirement, shared by normal and catchup selectors. */
    SCHEDULE_MEETING_YUKO_STEP_1_TRIVIA_MINIMUM = 56, /* Unmet Yuko story-step trivia requirement, shared by normal and catchup selectors. */
    SCHEDULE_MEETING_YUKO_STEP_2_TRIVIA_MINIMUM = 76, /* Unmet Yuko story-step trivia requirement, shared by normal and catchup selectors. */
    SCHEDULE_MEETING_YUKO_STEP_3_TRIVIA_MINIMUM = 91 /* Unmet Yuko story-step trivia requirement, shared by normal and catchup selectors. */
};

#endif
