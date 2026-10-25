#ifndef GAME_GAME_STATE_H
#define GAME_GAME_STATE_H


/* ---- aggregate types (T-5000) ----
 * Records whose fields splat names as separate D_ globals. tools/type_recovery.py proposes them from
 * the original asm (indexed accesses with one stride, pointer walks, ordered store/load pairs);
 * evidence and limits: wiki/data-types.md. Field names stay unk_XX until the meaning is known. */

#include "common.h"
#include "game/appointment.h"
#include "game/calendar.h"
#include "game/club.h"
#include "game/day_event.h"
#include "game/draw_records.h"
#include "game/girl.h"
#include "game/menu_records.h"
#include "game/param.h"
#include "game/word_views.h"
#include "game_enums.h"

/* ---- GameState (T-5100) ----
 * The main bss block 0x800E6280..0x800E7D10 is one object of the original: loops over arrays deep
 * inside it keep 0x800E6280 in the base register and reach the arrays through large offsets
 * (search_tpage: +0x1128/+0x1228, OPTION: +0x163C..+0x17BC, ETC: +0x75E), which a compiler can only
 * do when they are one symbol. Layout, evidence and confidence: wiki/game-state.md. O.BIN names the
 * object of this size and position `sys` (hypothesis, not applied). Fields are named by offset;
 * C code reaches them only through game_state (tools/migrate_globals.py rewrites the old D_ names
 * and ninja rejects them). Records are named by their offset in GameState. */










typedef struct GameState {
    /* 0x000 */ u16 video_width;
    /* 0x002 */ u16 video_height;
    /* 0x004 */ u16 video_interlace; /* HYPOTHESIS: third argument of the display-mode call, 0 everywhere it is set */
    /* 0x006 */ u8 unk_006[0xE];
    /* 0x014 */ s16 draw_origin_x[2];        /* indexed by draw_buffer */
    /* 0x018 */ s16 draw_origin_y[2];        /* indexed by draw_buffer */
    /* 0x01C */ PrimArena prim_arena[2];  /* indexed by draw_buffer (SetWorkBase) */
    /* 0x034 */ u8 sound_active;
    /* 0x035 */ u8 memory_card_busy;
    /* 0x036 */ u8 clear_color_r;
    /* 0x037 */ u8 clear_color_g;
    /* 0x038 */ u8 clear_color_b;
    /* 0x039 */ u8 back_clear;
    /* 0x03A */ u8 screen_brightness;
    /* 0x03B */ u8 brightness_step;
    /* 0x03C */ u8 voice_mode; /* VoiceMode */
    /* 0x03D */ u8 vacation; /* Vacation */
    /* 0x03E */ u8 year;            /* 0x5F at init; with +0x3F..+0x41 a date (year, month, day, weekday?) */
    /* 0x03F */ u8 month;            /* 4 at init; compared with GirlRecord.birthday bits 0-3 */
    /* 0x040 */ u8 day;            /* 4 at init; compared with GirlRecord.birthday bits 4-8 */
    /* 0x041 */ u8 weekday;
    /* 0x042 */ u16 day_count;
    /* 0x044 */ MonthCalendar month_calendar[2];
    /* 0x0C4 */ u8 player_full_name[0x10];      /* +0xC4..+0xF4: text buffers (NAME_ENT) */
    /* 0x0D4 */ u8 player_family_name[8];
    /* 0x0DC */ u8 player_given_name[8];
    /* 0x0E4 */ u8 player_nickname[0x10];
    /* 0x0F4 */ HalfView player_flags;
    /* 0x0F6 */ HalfView player_flags_upper;
    /* 0x0F8 */ s32 player_birthday;
    /* 0x0FC */ ParamValue param_condition;        /* +0xFC..+0x120: nine records, members (an array sums in another order, ETC func_8014A32C) */
    /* 0x100 */ ParamValue param_liberal_arts;
    /* 0x104 */ ParamValue param_science;
    /* 0x108 */ ParamValue param_art;
    /* 0x10C */ ParamValue param_athletics;
    /* 0x110 */ ParamValue param_trivia;
    /* 0x114 */ ParamValue param_looks;
    /* 0x118 */ ParamValue param_guts;
    /* 0x11C */ ParamValue param_stress;
    /* 0x120 */ u8 progress_state[0x9C]; /* HYPOTHESIS: 0x9C bytes = 21 one-shot flag bytes (+0..0x14, set by HUD code), 9 command counters (+0x15, ++ per chosen command), and the 0x7C-byte profile snapshot at +0x20 that parameter_show_init compares with the live values to colour changes */
    /* 0x1BC */ GirlRecord girls[16];
    /* 0x53C */ u8 girl_cleared[0x10]; /* HYPOTHESIS: per-girl flag; the mascot menu lists girl i when this or the saved SystemData.flags[i] is set; ETC graduation counts them */
    /* 0x54C */ AppointmentSlots u54C;
    /* 0x56C */ u8 scene_flags[0xFC];
    /* 0x668 */ u8 saved_settings[4];
    /* 0x66C */ ClubRecord clubs[12];
    /* 0x69C */ DayEvent day_events[32];
    /* 0x71C */ u8 day_event_index;
    /* 0x71D */ u8 day_event_count_today; /* HYPOTHESIS: ++ each time a girl encounter record (types 0x44/0x46/0x47) is run; cleared by day_event_today_clear each day */
    /* 0x71E */ u8 day_event_done_bits;
    /* 0x71F */ u8 daily_check_done; /* HYPOTHESIS: set to 1 before the telephone/appointment prompt (mode 0x43) and by the day-start code; cleared when a new queue is built */
    /* 0x720 */ u8 saved_game_mode; /* GameMode */
    /* 0x721 */ u8 saved_mode_stage;
    /* 0x722 */ u8 saved_mode_step;
    /* 0x723 */ u8 unk_723;
    /* 0x724 */ u8 appointment_place_0; /* HYPOTHESIS: recent appointment places; unk_725 is the newest (appointment_launch shifts the others), compared with unk_724 to detect a changed plan */
    /* 0x725 */ u8 appointment_place_1;
    /* 0x726 */ u8 appointment_place_2;
    /* 0x727 */ u8 appointment_place_3;
    /* 0x728 */ u8 date_place_history[4]; /* HYPOTHESIS: 4 places of the latest dates; DATE compares [0]==[1]==chosen place to detect a repeated place (sets player_flags bit 19) */
    /* 0x72C */ u8 girl_last_date_spot[0x10];      /* +0x72C, +0x73C, +0x74C: one byte per girl (loops of 11) */
    /* 0x73C */ u8 girl_last_date_spot_timer[0x10];
    /* 0x74C */ u8 girl_scene_bits[0x10]; /* HYPOTHESIS: one byte per girl; GEKO tests/sets bits 1, 2, 4 for scenes 0xE, 0xF, 7 */
    /* 0x75C */ u8 appointment_flag;
    /* 0x75D */ u8 date_girl; /* GirlId */
    /* 0x75E */ EventLogEntry event_log[256];
    /* 0xF5E */ u8 event_log_count;
    /* 0xF5F */ u8 current_girl;            /* index of the current girl's GirlRecord record */ /* GirlId */
    /* 0xF60 */ u8 text_lines_total; /* HYPOTHESIS: count of dialogue text entries (check_set_k) used by the line-scroll step talk_text_step */
    /* 0xF61 */ u8 text_first_line; /* HYPOTHESIS: index of the first visible dialogue line in the text list */
    /* 0xF62 */ u8 text_scroll_state; /* HYPOTHESIS: bit 7 = scrolling active, low bits = phase of the two-line scroll */
    /* 0xF63 */ s8 unk_F63;
    /* 0xF64 */ u8 quarter_flags[4]; /* HYPOTHESIS: QuarterFlagBits bit view: set by hud_magazine_mark_read by year and month group; at months 3/6/9/12 bit 14 takes bit 13 (schedule_quarter_flags_rotate); TEL also sets bits */
    /* 0xF68 */ WordView last_command;
    /* 0xF6C */ u8 icon_mode; /* IconMode */
    /* 0xF6D */ u8 icon_count;
    /* 0xF6E */ u8 sd_anim_fast;
    /* 0xF6F */ u8 cursor_speed; /* SettingsCursorSpeed */
    /* 0xF70 */ u8 click_mode; /* SettingsClickMode */
    /* 0xF71 */ s8 click_timer;
    /* 0xF72 */ u8 click_button;
    /* 0xF73 */ u8 double_click_interval;
    /* 0xF74 */ u8 input_device; /* InputDevice */
    /* 0xF75 */ u8 confirm_button; /* SettingsConfirmButton */
    /* 0xF76 */ u8 mouse_held;
    /* 0xF77 */ u8 mouse_buttons;
    /* 0xF78 */ u8 mouse_held_previous;
    /* 0xF79 */ u8 unk_F79;
    /* 0xF7A */ s16 mouse_x;
    /* 0xF7C */ s16 mouse_y;
    /* 0xF7E */ u8 unk_F7E[2];
    /* 0xF80 */ s32 pad_held;
    /* 0xF84 */ s32 pad_prev;
    /* 0xF88 */ s32 pad_pressed;
    /* 0xF8C */ s32 pad_released;
    /* 0xF90 */ MenuItem menu_items[32];
    /* 0x1090 */ u8 menu_count_0;
    /* 0x1091 */ u8 menu_count_1;
    /* 0x1092 */ u8 menu_mode; /* MenuMode */
    /* 0x1093 */ s8 menu_cursor_0;           /* menu_check indexes +0x1093 by menu number (lb/sb) */
    /* 0x1094 */ s8 menu_cursor_1;
    /* 0x1095 */ s8 menu_hit[0xD];
    /* 0x10A2 */ s8 tile_layer_enabled;
    /* 0x10A3 */ u8 unk_10A3;
    /* 0x10A4 */ u8 unk_10A4;
    /* 0x10A5 */ u8 quad_layer[0x43];    /* one byte per entry of the SpriteQuad table at sprite_quads */
    /* 0x10E8 */ s32 quads_enabled;
    /* 0x10EC */ u8 bg_layer_order[4];
    /* 0x10F0 */ s32 frame_scanline_limit; /* HYPOTHESIS: 0x106 scanline count; the frame waits for vsync only while the elapsed count is below it */
    /* 0x10F4 */ s32 timer_ticks;
    /* 0x10F8 */ u32 frame_count;
    /* 0x10FC */ s32 game_mode_timer;
    /* 0x1100 */ s32 mode_stage_timer;
    /* 0x1104 */ WordView mode_step_timer;      /* the move/place selector (normal_date_move_place); BUNKA_SD reads it as u32 */
    /* 0x1108 */ u8 game_mode;
    /* 0x1109 */ u8 mode_stage;
    /* 0x110A */ u8 mode_step;
    /* 0x110B */ s8 game_mode_scratch;
    /* 0x110C */ s8 mode_stage_scratch;
    /* 0x110D */ u8 substep;
    /* 0x110E */ u8 ovl_target_game_mode; /* GameMode */
    /* 0x110F */ u8 restart_request;
    /* 0x1110 */ u8 paused;
    /* 0x1111 */ u8 pause_allowed;
    /* 0x1112 */ u8 actor_draw_enabled;
    /* 0x1113 */ u8 draw_3d_enabled;
    /* 0x1114 */ u8 draw_2d_enabled;
    /* 0x1115 */ u8 memory_card_result;
    /* 0x1116 */ u8 unk_1116;
    /* 0x1117 */ u8 album_slot;
    /* 0x1118 */ u8 save_slot;
    /* 0x1119 */ u8 unk_1119;
    /* 0x111A */ s8 boot_hold; /* HYPOTHESIS: mode 1 waits until it is 0 and then starts mode 0 */
    /* 0x111B */ u8 memory_card_job;
    /* 0x111C */ u8 memory_card_step;
    /* 0x111D */ u8 memory_card_retry_count;
    /* 0x111E */ u8 unk_111E[2];
    /* 0x1120 */ s32 memory_card_timer;
    /* 0x1124 */ u8 options_dirty;
    /* 0x1125 */ u8 unk_1125[3];
    /* 0x1128 */ s32 tpage_slot_state[64];     /* search_tpage: set from +0x1228 */
    /* 0x1228 */ s32 tpage_buf[64];
    /* 0x1328 */ PaletteLoad palette_queue[32];
    /* 0x1428 */ u8 palette_queue_count;
    /* 0x1429 */ u8 csr_queue_count;
    /* 0x142A */ u8 unk_142A[2];
    /* 0x142C */ CsrLoad csr_queue[33];
    /* 0x163C */ s32 scratch_words[128];    /* OPTION fills it as words; +0x163C..+0x167C saved by memcpy */ /* HYPOTHESIS: 128-word scratch used by TACO and the VRAM loaders; its first 0x50 bytes also hold the saved free-icon layout (main 8005A0B0) */
    /* 0x183C */ u8 scratch_buffer[0x200]; /* HYPOTHESIS: 0x200-byte overlay scratch; NAME_ENT stages a 128-word palette in it before load_palette (a copy of scratch_words or the col2sepia output), TACO keeps 25 (x, y) word pairs of a drawing effect in it */
    /* 0x1A3C */ s32 ovl_tpage_table;         /* +0x1A3C..+0x1A84: addresses set by NAME_ENT */ /* HYPOTHESIS: pointer an overlay stores (TEL, NAME_ENT) and copies into Actor.tpage_table of its actors */
    /* 0x1A40 */ u8 unk_1A40[0xC];
    /* 0x1A4C */ s32 ovl_frame_table; /* HYPOTHESIS: pointer an overlay stores (TEL, NAME_ENT) and copies into Actor.frame_table of its actors */
    /* 0x1A50 */ u8 unk_1A50[0xC];
    /* 0x1A5C */ s32 ovl_sequence_table; /* HYPOTHESIS: pointer an overlay stores (TEL, NAME_ENT) and copies into Actor.sequence_table of its actors */
    /* 0x1A60 */ u8 unk_1A60[0xC];
    /* 0x1A6C */ s32 ovl_palette_a; /* HYPOTHESIS: pointer to a palette block that TEL, NAME_ENT and TACO pass to load_palette; TACO also copies 128 words from it into scratch_words */
    /* 0x1A70 */ u8 unk_1A70[0xC];
    /* 0x1A7C */ s32 ovl_image_buffer; /* HYPOTHESIS: base address of six 0x4000-byte image blocks (NAME_ENT) */
    /* 0x1A80 */ u8 unk_1A80[4];
    /* 0x1A84 */ s32 ovl_palette_b; /* HYPOTHESIS: pointer to a second palette block that NAME_ENT passes to func_80043914 (slot 0x18) */
    /* 0x1A88 */ u8 unk_1A88[8];
} GameState; /* size 0x1A90 */

#define unk_54C u54C.a
#define unk_54C_0 u54C.e0

#endif /* GAME_GAME_STATE_H */
