#ifndef MAIN_TEXT_H
#define MAIN_TEXT_H

#include "common.h"

/* 8-byte record of the array at text_list (stride 8; see k_disp_switch). */
typedef struct TextEntry {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ u8 attribute;
    /* 0x05 */ u8 glyph_count;
    /* 0x06 */ u8 extra;
    /* 0x07 */ s8 active;
} TextEntry; /* size 0x08 */

/* Text display state, defined in src/main/text_system.c (T-9150). */
typedef struct TextState {
    /* 0x0 */ s16 count;   /* number of entries in text_list */
    /* 0x2 */ s16 total;
    /* 0x4 */ s16 mark;
    /* 0x6 */ u8 speed;
    /* 0x7 */ u8 pad_07;
    /* 0x8 */ s16 pos;
    /* 0xA */ u8 glyph_advance;
    /* 0xB */ u8 pad_0B;
} TextState; /* size 0xC */

/* Shift-JIS range table of load_string at text_font_range_table: first code of each range and the glyph number
 * of its first character (T-17020); 46 entries up to text_font_range_table_end. */
typedef struct FontRange {
    /* 0x0 */ u16 first;
    /* 0x2 */ u16 glyph;
} FontRange; /* size 0x4 */

extern u8 text_list[];
extern u8 text_list_active0[];  /* stride 8 from k_disp_switch: likely a field of an 8-byte struct array */
extern u8 text_list_last[];
extern TextState text_state;
extern FontRange text_font_range_table[46];
/* Font and cursor images that text_system_reset uploads with LoadSquare (T-17020). */
extern u8 text_blank_strip[0x40];
extern u8 text_palette_0[0x20];
extern u8 text_palette_1[0x20];
extern u8 text_shadow_palette_0[0x20];
extern u8 text_shadow_palette_1[0x20];
s32 load_string(); /* set_kanji_string passes three arguments, the definition takes one */
void vload_font();
/* Four signed menu row positions, read by x_taku_menu_set. */
extern s16 text_row_y[4];
extern u8 text_font_glyphs[];
extern u32 text_glyph_upload_ring[];
void disp_string(void);
u8 *get_k_work(s32 entry_index);
void text_glyph_expand();
void text_glyph_pages_clear(s32 page_count);
void text_system_reset(void);
#ifndef MAIN_API_OVERRIDE_set_kanji_string
s32 set_kanji_string(s16 x, s16 y, u8 color_index, u8 *text, s32 ordering_slot);
#endif
void k_disp_start(s32 first_entry);
void k_disp_switch(s32 entry_index, s32 visible);
void k_reset(s32 entry_count);
void k_sub_reset_point_set(void);
#ifndef MAIN_API_OVERRIDE_k_sub_reset
void k_sub_reset(void);
#endif
void k_sub_disp_start(s32 entry_offset);
s16 k_disp_goto_line_end(void);
void k_speed_set(u8 reveal_speed);
u8 get_k_speed(void);
s32 k_disp_inc(void);
s32 check_end_k(void);
#ifndef MAIN_API_OVERRIDE_get_now_k
u8 get_now_k(void);
#endif
void text_wait_confirm();
void k_disp_inc2();
void text_marker_hide(void);

enum TextSpeed {
    TEXT_SPEED_FAST = 0, /* T-27062 one glyph per frame */
    TEXT_SPEED_NORMAL = 1, /* T-27062 one glyph every two frames */
    TEXT_SPEED_SLOW = 3, /* T-27062 one glyph every four frames */
    TEXT_SPEED_EVERY_SIX_FRAMES = 5, /* T-27062 one glyph every six frames */
    TEXT_SPEED_EVERY_NINE_FRAMES = 8, /* T-27062 one glyph every nine frames */
    TEXT_SPEED_EVERY_FIFTEEN_FRAMES = 14, /* T-27062 one glyph every fifteen frames */
    TEXT_SPEED_EVERY_SEVENTEEN_FRAMES = 16, /* T-27062 one glyph every seventeen frames */
    TEXT_SPEED_EVERY_THIRTY_ONE_FRAMES = 30 /* T-27062 one glyph every thirty-one frames */
};
#endif /* MAIN_TEXT_H */
