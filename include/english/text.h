#ifndef ENGLISH_TEXT_H
#define ENGLISH_TEXT_H

/* English text runtime (T-39502, wiki/english-text-runtime.md). Only the English build
 * (`ninja english`, -DTOKIMEMO_ENGLISH) compiles and links these symbols: tools/english_link.py
 * appends them to the main executable's code and data, which the free relink moves up (T-39505,
 * wiki/relink.md). */

#include "common.h"

/* VRAM glyph slots of the text system: three 128-slot pages at x 0x320, 0x360 and 0x3A0. */
#define ENGLISH_GLYPH_SLOTS 384

/* Font data written by tools/english_font.py from translations/en/font/latin12.txt. */
extern u8 english_font_index[256];   /* single-byte code -> glyph number, 0xFF: none */
extern u8 english_font_advance[];    /* advance in pixels per glyph number */
extern u8 english_font_glyphs[];     /* 18-byte 12 x 12 1bpp bitmaps per glyph number */

/* x offset in pixels of each loaded glyph slot from the start of its text entry */
extern s16 english_glyph_pen[ENGLISH_GLYPH_SLOTS];

/* Nonzero: english_load_string places each glyph centered in a cell of this many pixels instead of at
 * its proportional advance (grids such as the name entry letter page). Set it around the
 * set_kanji_string calls that need it and clear it again; disp_string reads the pen positions that
 * were recorded at load time. */
extern s32 english_text_cell;

/* Width in pixels of a string as english_load_string lays it out (proportional mode). */
s32 english_text_width(u8 *text);

/* Replacements reached through the jumps in config/english_hooks.txt. */
s32 english_set_kanji_string(s16 x, s16 y, u8 color_index, u8 *text, s32 ordering_slot);
s32 english_load_string(u8 *text);
void english_disp_string(void);

#endif
