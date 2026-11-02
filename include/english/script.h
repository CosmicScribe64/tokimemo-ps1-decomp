#ifndef ENGLISH_SCRIPT_H
#define ENGLISH_SCRIPT_H

/* English script format (T-39506, wiki/english-text-runtime.md). In the English build the dialogue
 * parsers read these control bytes in place of the original single-byte letters and the 0x20 line
 * end, so that ASCII letters and spaces can be text. The text writers (T-39507) emit them. Every
 * byte below 0x20 has no glyph in the English font, so english_load_string skips any that reach it. */

#include "common.h"

#define ENGLISH_LINE_END 0x0A     /* original 0x20: end of a display line */
#define ENGLISH_OPTION_END 0x0B   /* original `q`: next choice option or reply variant */
#define ENGLISH_OPTION_EMPTY 0x0C /* original `n`: empty option, skipped reply, `nn` branch-zero tail */

/* Name and value codes, original letters `P p m F Y G S O W A M` in this order. */
#define ENGLISH_CODE_PLAYER_FORM 0x10 /* P: player name as the current girl says it */
#define ENGLISH_CODE_FAMILY 0x11      /* p: player family name */
#define ENGLISH_CODE_GIVEN 0x12       /* m: player given name */
#define ENGLISH_CODE_FULL 0x13        /* F: family name, space, given name */
#define ENGLISH_CODE_YOSHIO 0x14      /* Y: Yoshio */
#define ENGLISH_CODE_GIRL 0x15        /* G: current girl's name with honorific */
#define ENGLISH_CODE_S 0x16           /* S O W A M: context buffers name_code_s .. name_code_m */
#define ENGLISH_CODE_O 0x17
#define ENGLISH_CODE_W 0x18
#define ENGLISH_CODE_A 0x19
#define ENGLISH_CODE_M 0x1A

/* Inline speaker labels (original `>`, `-`, `+` with 4, 6 or 2 label bytes, and `<` with 4): the
 * marker is the first byte of the line, the label runs to ENGLISH_LABEL_END, the speech follows. */
#define ENGLISH_LABEL_SPEECH 0x1C  /* `>` `-` `+`: label, then the speech after an opening quote */
#define ENGLISH_LABEL_THOUGHT 0x1D /* `<`: label, then the thought in parentheses */
#define ENGLISH_LABEL_END 0x1F

/* Stored byte limits, excluding NUL. Pair-encoded English names may contain more
 * letters while GameState, name_code buffers and saves keep their original sizes. */
#define ENGLISH_NAME_LETTERS 7
/* name_code_player_form, which receives the nickname, is 12 bytes. */
#define ENGLISH_NICKNAME_LETTERS 11

/* Replacements reached through config/english_hooks.txt. Parameters are the promoted K&R ones. */
s32 english_talk_render(s32 x, s32 y, s32 text_color, u8 *input_text, s32 ordering_slot, u8 *line_suffix,
                        u8 *speaker_label, s32 label_enabled);
s32 english_talk_speak_part(u8 *line_text, s32 speak_code, s32 choice, void *voice_table, s32 group_index,
                            s32 line_index, s32 text_color);
s32 english_make_three_select(u8 *source_text);
void english_talk_names_set(s32 girl_id);
void english_gnsx(u8 *speaker_name);
void english_sndisp(u8 *text, s32 first_row, s32 speaker_style);
void english_sndi(u8 *text, s32 row, s32 speaker_style);
void english_k_reset(s32 entry_count);
void english_k_sub_disp_start(s32 entry_offset);
s32 english_get_g_name(u8 *name_buf, s32 girl_id);
void english_get_p_name(u8 *name_buf, s32 girl_form);

/* Helpers shared with the other English code (NAME_ENT uses them too). */
void english_copy(u8 *dst, u8 *src, s32 size);
void english_append(u8 *dst, u8 *src, s32 size);
/* Overlay replacements of strcpy into name_code_girl_short (8 bytes) and girl_name_buf (12 bytes): the
 * full name is kept for the parsers, the buffer gets a bounded copy. */
void english_girl_short_set(u8 *name);
void english_girl_name_set(u8 *name);
/* The same for the O and W values (name_code_o, 8 bytes; name_code_w, 12 bytes): New Year fortune and wish;
 * code is ENGLISH_CODE_O or ENGLISH_CODE_W. */
void english_value_set(s32 code, u8 *text);
/* x of a speaker label right-aligned in the label column left of the quote mark (at least -0x9E) */
s32 english_speaker_label_x(u8 *name);

#endif
