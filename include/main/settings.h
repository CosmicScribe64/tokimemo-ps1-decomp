#ifndef MAIN_SETTINGS_H
#define MAIN_SETTINGS_H

#include "common.h"

extern s16 settings_room_mascot_holiday_stamp;
extern s16 settings_room_mascot_stamp;
extern s32 settings_click_speed_ticks;
extern u32 settings_room_radio_clips[16];
extern u32 settings_room_radio_clips_holiday[16];
s32 settings_command_screen_mode_stage_reload(void);
void settings_command_screen_mode_stage_setup(void);
void settings_command_screen_setup_reset(void);
void settings_command_screen_setup_wait(void);
void settings_command_screen_reload_calendar(void);
void settings_command_screen_build();
void settings_room_bg_load_start(void);
void settings_room_bg_load_finish(void);
void settings_command_screen_icons_reset(s32 disable_weekday_commands);
void settings_room_sick_icon_load(void);
void settings_room_sick_icon_show(void);
void settings_command_screen_mode_stage_select();
s32 settings_room_click_window(void);
s32 settings_room_click_help(void);
s32 settings_room_click_radio();
s32 settings_room_click_radio_holiday();
s32 settings_room_click_mascot_holiday(void);
s32 settings_room_click_mascot();
void settings_command_screen_select_frame(void);
s32 settings_command_screen_icon_confirm();
void settings_command_screen_mode_stage_calendar(void);
void settings_command_screen_calendar_init(void);
void settings_command_screen_calendar_wait(void);
void settings_command_screen_calendar_exit(void);
void settings_save_load_mode_stage_dispatch();
void settings_save_load_layout_stash(s32 restore_layout);
void settings_save_load_load_prompt(void);
s32 settings_save_load_load_select(void);
s32 settings_save_load_card_check(void);
void settings_save_load_menu_init(void);
void settings_save_load_save_init(void);
void settings_save_load_load_init(void);
void settings_save_load_save_select(void);
void settings_save_load_save_prompt(void);
void settings_save_load_save_marks(void);
void settings_mouse_click_speed_menu(void);
s32 settings_voice_mode_init(void);
s32 settings_labels_draw(s32 highlighted_setting_item);
s32 settings_text_speed_init(void);
s32 settings_root_init(void);
s32 settings_mascot_menu(void);
void settings_save_load_load_marks();
void settings_mode_stage_dispatch();
s32 settings_new_year_card_sender_count();
s32 settings_new_year_scene_mode_stage_dispatch();
/* T-11190: declarations added by tools/bulk_m2c.py. */
/* T-11190: declarations added by tools/bulk_m2c.py. */

/* T-11190: declarations added by tools/bulk_m2c.py. */

/* T-11190: declarations added by tools/bulk_m2c.py. */

/* T-11190: declarations added by tools/bulk_m2c.py. */

/* T-11190: declarations added by tools/bulk_m2c.py. */

/* T-11190: declarations added by tools/bulk_m2c.py. */

/* T-11190: declarations added by tools/bulk_m2c.py. */
void settings_save_load_menu(void);
void settings_save_load_load_read(void);
s32 settings_save_load_load_finish(void);
void settings_save_load_load_info_read(void);
void settings_save_load_save_info_read(void);
s32 settings_save_load_data_broken(void);
s32 settings_save_load_card_full(void);
s32 settings_save_load_card_unreadable(void);
/* T-11190: declarations added by tools/bulk_m2c.py. */
void settings_root_menu(void);
void settings_text_speed_menu(void);
void settings_text_speed_done(void);
void settings_icon_mode_init(void);
void settings_icon_mode_menu(void);
void settings_icon_mode_done(void);
void settings_mouse_init(void);
void settings_mouse_menu(void);
void settings_mouse_done(void);
void settings_mouse_confirm_init(void);
void settings_mouse_confirm_menu(void);
void settings_mouse_confirm_done(void);
void settings_mouse_click_init(void);
void settings_mouse_click_menu(void);
void settings_mouse_click_done(void);
void settings_mouse_click_speed_init(void);
void settings_mouse_click_speed_done(void);
void settings_menu_mode_init(void);
void settings_menu_mode_menu(void);
void settings_cursor_speed_menu(void);
void settings_menu_mode_done(void);
void settings_cursor_speed_done(void);
void settings_voice_mode_menu(void);
void settings_voice_mode_done(void);
void settings_mascot_init(void);
void settings_mascot_done(void);
void settings_exit(void);

extern u8 D_800C9A60[];
extern u8 D_800C9D60[];
extern s16 D_8011F422;
extern s16 D_8011F426;
extern u8 D_800B59F8[];
extern u8 D_800B5A1C[];

s32 pre_xmas_init(void);
s32 pre_syogatu_init();

extern u8 D_800AFF6C[];
extern u8 D_800AFF78[];
extern u8 D_800B00FC[];
extern u8 D_800B0194[];
extern u8 D_800B0B64[];
extern u8 D_800B0B6C[];
extern u8 D_800E7D2C;

enum SettingsClickMode {
    SETTINGS_CLICK_MODE_SINGLE = 0, /* single-click mode stored value */
    SETTINGS_CLICK_MODE_DOUBLE = 1 /* double-click mode stored value */
};
enum SettingsConfirmButton {
    SETTINGS_CONFIRM_BUTTON_MOUSE_LEFT = 0, /* left mouse button confirms */
    SETTINGS_CONFIRM_BUTTON_MOUSE_RIGHT = 1 /* right mouse button confirms */
};
enum SettingsCursorSpeed {
    SETTINGS_CURSOR_SPEED_SLOWEST = 1, /* slowest stored cursor speed */
    SETTINGS_CURSOR_SPEED_SLOWER = 2, /* slower stored cursor speed */
    SETTINGS_CURSOR_SPEED_NORMAL = 3, /* normal stored cursor speed */
    SETTINGS_CURSOR_SPEED_FASTER = 4, /* faster stored cursor speed */
    SETTINGS_CURSOR_SPEED_FASTEST = 6 /* fastest stored cursor speed; speed menus skip 5; save/debug restoration can supply other values */
};
#endif
