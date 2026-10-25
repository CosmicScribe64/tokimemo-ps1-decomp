#ifndef MAIN_HOLIDAY_H
#define MAIN_HOLIDAY_H

/* T-27061: Days-off activities, appointments, phone, magazine and club flow. */
#include "common.h"

/* T-20940: static data of 80075320.c. holiday_dial_keys is a ring of the last seven keys pressed on the holiday
 * menu (0, 1, 2 from the pad bits 0x20, 0x40, 0x10; cleared to 0xFF by holiday_open_init in a loop IDO unrolled)
 * and holiday_dial_key_pos its write position: holiday_phone_dial_shortcut stores `keys[pos] = key; pos++`, wraps pos at 7 and reads
 * the ring from pos + 7 modulo 7. Two variables, both defined in 80075320.c (T-24303): holiday_open_init stores
 * the position with its own lui $at and the three remainder keys with one shared lui $at, which as1 does only
 * for separate symbols of the same object. The original reloads pos after the store into keys[pos] in
 * holiday_phone_dial_shortcut. appointment_choice_suffix is a 0x14-byte string in .data
 * (strcat argument of appointment_place_menu_build). holiday_sd_set_summer and holiday_sd_set_other map GameState.day_events[].unk_01 (0..7) to a
 * background in holiday_activity_pick_sd_set, one table per hud_in_summer_months() result. */
extern u8 holiday_dial_keys[8];
extern u8 holiday_dial_key_pos;
extern u8 appointment_choice_suffix[0x14];
extern u8 holiday_sd_set_summer[8];
extern u8 holiday_sd_set_other[8];
extern s32 holiday_sd_set_sectors[];
/* T-17000: message table of holiday_magazine_show, indexed by holiday_magazine_hint_index() and passed to sndisp. */
extern u8 *holiday_magazine_hints[];
extern s32 holiday_activity_bg_sectors[];
void holiday_open_init(void);
void holiday_open_wait_bg(void);
void holiday_icons_set_available(void); /* T-20940: called by holiday_open_init before its definition */
void func_80075BFC();
void holiday_menu_frame(void);
s32 holiday_phone_dial_shortcut();
s32 holiday_activity_bg_sector();
s32 holiday_menu_click();
void appointment_place_menu_build(void);
void func_80076450();
void appointment_start(void);
s32 appointment_place_choose(void);
s32 appointment_place_confirm(void);
void appointment_launch(void);
void holiday_activity_load(void);
void holiday_activity_sd_setup(void);
void holiday_activity_play(void);
void holiday_mode_stage_open(void);
void holiday_mode_stage_menu(void);
void holiday_mode_stage_appointment(void);
void holiday_mode_stage_activity(void);
void holiday_mode_stage_magazine(void);
s32 holiday_club(void);
void holiday_mode_stage_club_join(void);
void holiday_magazine_init(void);
void holiday_magazine_show(void);
void magazine_exit(void);
void holiday_tel(void);
void holiday_tel_init(void);
void holiday_tel_call(void);
void telephone_class_init(void);
void holiday_club_join_exit(void);
void func_80078950();
void func_80078970();
s32 func_800789A0();

#endif /* MAIN_HOLIDAY_H */
