/* text.c: English text runtime (T-39502). Not part of the original executable: only the English build
 * (`ninja english`) compiles it, into the island that src/english/boot.s copies after the main bss. The
 * functions replace load_string, set_kanji_string and disp_string through the jumps that
 * tools/english_link.py writes at their entry points (config/english_hooks.txt). Design and encoding:
 * wiki/english-text-runtime.md.
 *
 * Encoding: a byte 0x20-0x7E or 0xA0-0xDF is one glyph of the proportional English font
 * (translations/en/font/latin12.txt); any other byte from 0x80 up starts a two-byte Shift-JIS character
 * that is drawn from the game's font at the fixed advance; bytes 0x01-0x1F are skipped (control codes of
 * the English script format). Each glyph still takes one VRAM slot, in the order the original uses, so
 * the typewriter (k_disp_inc) and every glyph count keep their meaning; only the x position of a glyph
 * comes from english_glyph_pen instead of index * advance. */
#include "common.h"
#include "game_enums.h"
#include "libapi.h"
#include "libgpu.h"
#include "main/draw.h"
#include "main/import_views/entry_ui.h"
#include "main/prim.h"
#include "main/state.h"
#include "main/text.h"
#include "main/vram.h"
#include "psx_types.h"
#include "english/text.h"

#ifdef TOKIMEMO_ENGLISH

s16 english_glyph_pen[ENGLISH_GLYPH_SLOTS];

/* Expands one 18-byte 1bpp glyph to 16 x 12 4bpp in the upload ring and loads it into the next glyph
 * slot, as vload_font does for the game's font. */
static void english_glyph_load(u8 *bits) {
    u32 *out;
    u32 *p;
    u32 left;
    u32 right;
    s32 row;
    s32 col;
    s32 n;
    s32 slot;
    s32 page_x;

    slot = text_state.total;
    out = text_glyph_upload_ring + (slot & 0xF) * 0x18;
    p = out;
    n = 0;
    for (row = 0; row < 12; row++) {
        left = 0;
        right = 0;
        for (col = 0; col < 12; col++, n++) {
            if (bits[n >> 3] & (0x80 >> (n & 7))) {
                if (col < 8) {
                    left |= 1 << (col * 4);
                } else {
                    right |= 1 << ((col - 8) * 4);
                }
            }
        }
        *p++ = left;
        *p++ = right;
    }
    if (slot < 0x80) {
        page_x = 0x320;
    } else if (slot < 0x100) {
        page_x = 0x360;
    } else {
        page_x = 0x3A0;
    }
    LoadSquare((slot & 7) * 4 + page_x, (slot & 0x78) * 2 + 0x100, 4, 0xC, out);
    func_8009C674(0);
    text_state.total += 1;
}

/* Lays out one string: loads a glyph slot per character and records its pen position; returns the
 * number of glyphs (the TextEntry glyph count). */
s32 english_load_string(u8 *text) {
    s32 first_glyph;
    s32 pen;
    s32 slot;
    s32 advance;
    s32 i;
    u32 c;
    u32 code;

    first_glyph = text_state.total;
    pen = 0;
    while ((c = *text) != 0) {
        slot = text_state.total;
        if (c < 0x80 || (c >= 0xA0 && c < 0xE0)) {
            text++;
            if (english_font_index[c] == 0xFF) {
                continue;
            }
            english_glyph_load(&english_font_glyphs[english_font_index[c] * 18]);
            advance = english_font_advance[english_font_index[c]];
        } else {
            if (text[1] == 0) {
                break;
            }
            code = (c << 8) + text[1];
            text += 2;
            if (code < text_font_range_table[0].first) {
                printf("<kanji.c>error(No-sjis)");
                break;
            }
            for (i = 1; i < 46; i++) {
                if (code < text_font_range_table[i].first) {
                    vload_font((code - text_font_range_table[i - 1].first) + text_font_range_table[i - 1].glyph,
                               code - text_font_range_table[i - 1].first);
                    break;
                }
            }
            if (text_state.total == slot) {
                continue;
            }
            advance = text_state.glyph_advance;
        }
        if (slot < ENGLISH_GLYPH_SLOTS) {
            english_glyph_pen[slot] = pen;
        }
        pen += advance;
    }
    return text_state.total - first_glyph;
}

/* set_kanji_string without the two-byte minimum: a one-letter English string is valid. */
s32 english_set_kanji_string(s16 x, s16 y, u8 color_index, u8 *text, s32 ordering_slot) {
    TextEntry *entry;

    if (text_state.count >= 0x34) {
        return -1;
    }
    entry = (TextEntry *)text_list + text_state.count;
    if (text == 0 || text[0] == 0) {
        entry->active = 0;
    } else {
        entry->y = y;
        entry->attribute = color_index;
        entry->x = x;
        entry->glyph_count = english_load_string(text);
        entry->extra = ordering_slot;
        entry->active = 1;
    }
    return ++text_state.count;
}

/* disp_string with the x of each glyph taken from its pen position. */
void english_disp_string(void) {
    TextEntry *entry;
    SPRT_8 *p;
    s32 i;
    u32 k;
    s32 j;
    u32 ot;
    s32 flag;
    s32 x;
    s32 y;

    j = 0;
    ot = ((TextEntry *)text_list)[0].extra;
    if (ot == 0) {
        ot = 1;
    }
    flag = 1;
    for (i = 0, k = 0; i < text_state.total && i < text_state.pos; i++, k++) {
        if (k >= ((TextEntry *)text_list)[j].glyph_count) {
            k = 0;
            j++;
        }
        entry = (TextEntry *)text_list + j;
        if (entry->active != 1) {
            continue;
        }
        if (ot != entry->extra) {
            ot = entry->extra;
            flag = 1;
            if (ot == 0) {
                ot = 1;
            }
        }
        x = entry->x + game_state.draw_origin_x[draw_buffer];
        if (i < ENGLISH_GLYPH_SLOTS) {
            x += english_glyph_pen[i];
        }
        y = entry->y + game_state.draw_origin_y[draw_buffer];

        p = (SPRT_8 *)GetWorkBase(0x10, draw_buffer);
        func_8009F090(p);
        SetSemiTrans(p, 0);
        SetShadeTex(p, 0);
        p->r0 = game_state.screen_brightness;
        p->g0 = game_state.screen_brightness;
        p->b0 = game_state.screen_brightness;
        p->x0 = x;
        p->y0 = y;
        p->u0 = (i & 7) * 0x10 + 0x80;
        p->v0 = (i & 0x78) * 2;
        p->clut = func_8009ED74((entry->attribute & 8) * 4 + 0x100, (entry->attribute & 7) + 0x1F8);
        AddPrim(&draw_ot_tags[(draw_buffer << 10) + ot * 4], p);

        /* drop shadow one pixel right and down, drawn first (AddPrim puts it in front in the list) */
        p = (SPRT_8 *)GetWorkBase(0x10, draw_buffer);
        func_8009F090(p);
        SetSemiTrans(p, 0);
        SetShadeTex(p, 0);
        p->r0 = 0;
        p->g0 = 0;
        p->b0 = 0;
        p->x0 = x + 1;
        p->y0 = y + 1;
        p->u0 = (i & 7) * 0x10 + 0x80;
        p->v0 = (i & 0x78) * 2;
        p->clut = func_8009ED74((entry->attribute & 8) * 4 + 0x110, (entry->attribute & 7) + 0x1F8);
        AddPrim(&draw_ot_tags[(draw_buffer << 10) + ot * 4], p);

        if (i == 0x7F) {
            goto_tpage(0x300, 0x100, ot, 0, 0);
            flag = 0;
        } else if (i == 0xFF) {
            goto_tpage(0x340, 0x100, ot, 0, 0);
            flag = 0;
        } else if (flag != 0) {
            flag = 0;
            if (i >= 0x100) {
                goto_tpage(0x380, 0x100, ot, 0, 0);
            } else if (i >= 0x80) {
                goto_tpage(0x340, 0x100, ot, 0, 0);
            } else {
                goto_tpage(0x300, 0x100, ot, 0, 0);
            }
        }
    }
    if (i >= 0x100) {
        goto_tpage(0x380, 0x100, ot, 0, 0);
    } else if (i >= 0x80) {
        goto_tpage(0x340, 0x100, ot, 0, 0);
    } else {
        goto_tpage(0x300, 0x100, ot, 0, 0);
    }
}

#endif
