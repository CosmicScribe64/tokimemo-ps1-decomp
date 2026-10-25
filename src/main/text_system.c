/* text_system.c: src/main/8004E500.c; T-27059: object [0x8004E500,0x8004F6B0); text list/typewriter, glyph lookup and upload, text palette and per-frame glyph drawing; preserve complete object (reuse reviewed m-menutext filename). Original object 0x8004E500-0x8004F6B0 (config/objects/main.txt). */
#include "common.h"
#include "game.h"
#include "psx_types.h"
/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
INCLUDE_RODATA("asm/data/main/text_system.data", text_list);
INCLUDE_RODATA("asm/data/main/text_system.data", text_list_last);
TextState text_state = { 0, 0, 0, 0, 0, 0, 0 };
INCLUDE_RODATA("asm/data/main/text_system.data", text_blank_strip);
INCLUDE_RODATA("asm/data/main/text_system.data", text_palette_0);
INCLUDE_RODATA("asm/data/main/text_system.data", text_palette_1);
INCLUDE_RODATA("asm/data/main/text_system.data", text_shadow_palette_0);
INCLUDE_RODATA("asm/data/main/text_system.data", text_shadow_palette_1);
INCLUDE_RODATA("asm/data/main/text_system.data", text_font_range_table);
INCLUDE_RODATA("asm/data/main/text_system.data", D_800B4030);
INCLUDE_RODATA("asm/data/main/text_system.data", text_font_range_table_end);

void text_glyph_pages_clear(s32 page_count) {
    s32 i;
    RECT r;

    for (i = 0; i < page_count; i++) {
        r.x = 0x320 + i * 0x40;
        r.y = 0x100;
        r.w = 0x20;
        r.h = 0x100;
        func_8009C7F8(&r, 0, 0, 0);
    }
    func_8009C674(0);
}

void text_system_reset(void) {
    s32 i;
    RECT r;

    LoadSquare(0x100, 0x1F8, 2, 8, text_palette_0);
    LoadSquare(0x120, 0x1F8, 2, 8, text_palette_1);
    LoadSquare(0x110, 0x1F8, 2, 8, text_shadow_palette_0);
    LoadSquare(0x130, 0x1F8, 2, 8, text_shadow_palette_1);
    LoadSquare(0x320, 0x1FF, 0x20, 1, text_blank_strip);
    LoadSquare(0x360, 0x1FF, 0x20, 1, text_blank_strip);
    text_glyph_pages_clear(2);
    r.x = 0x102;
    r.y = 0x1F8;
    r.w = 2;
    r.h = 8;
    func_8009C7F8(&r, 0x80, 0x80, 0x80);
    for (i = 0; i < 0x34; i++) {
        ((TextEntry *)text_list)[i].x = 0;
        ((TextEntry *)text_list)[i].y = 0;
        ((TextEntry *)text_list)[i].attribute = 0;
        ((TextEntry *)text_list)[i].glyph_count = 0;
        ((TextEntry *)text_list)[i].extra = 0;
        ((TextEntry *)text_list)[i].active = 0;
    }
    text_state.total = 0;
    text_state.count = 0;
    text_state.pos = 0;
    text_state.mark = 0;
    /* FAKE: T-17020. The two compares read the byte through a TextState pointer view. With a direct
     * read the build loads it after the four zero stores and puts the li in t6 (tools/cvt_pass.py
     * keeps the widening CVT because the first read comes after a call). The pointer view gives the
     * original's order: lbu before the stores, li in v0. */
    if (((TextState *)&text_state)->unk_6A != 0xE && ((TextState *)&text_state)->unk_6A != 0x10) {
        text_state.unk_6A = 0xE;
    }
}

void text_set_advance(u8 glyph_advance) {
    if (glyph_advance == 0x10) {
        text_state.unk_6A = 0x10;
    } else {
        text_state.unk_6A = 0xE;
    }
}

void text_unused_stub(void) {
}

/* Queue one string (x, y, attribute, text, extra) in the 52-entry text list for drawing */
s32 set_kanji_string(s16 x, s16 y, u8 color_index, u8 *text, s32 ordering_slot) {
    if (text_state.count >= 0x34) {
        return -1;
    }
    if (text == 0) {
        ((TextEntry *)text_list)[text_state.count].active = 0;
    } else if (text[0] == 0 || text[1] == 0) {
        ((TextEntry *)text_list)[text_state.count].active = 0;
    } else {
        ((TextEntry *)text_list)[text_state.count].y = y;
        ((TextEntry *)text_list)[text_state.count].attribute = color_index;
        ((TextEntry *)text_list)[text_state.count].x = x;
        ((TextEntry *)text_list)[text_state.count].glyph_count = load_string(text, y, color_index);
        ((TextEntry *)text_list)[text_state.count].extra = ordering_slot;
        ((TextEntry *)text_list)[text_state.count].active = 1;
    }
    return ++text_state.count;
}

/* Start the typewriter at entry n (skips the glyphs of earlier entries) */
void k_disp_start(s32 first_entry) {
    s32 i;

    text_state.pos = 0;
    for (i = 0; i < first_entry; i++) {
        text_state.pos += text_list[i * 8 + 5];
    }
}

/* Show or hide a text list entry */
void k_disp_switch(s32 entry_index, s32 visible) {
    if (visible > 0) {
        text_list_active0[entry_index * 8] = 1;
    } else {
        text_list_active0[entry_index * 8] = 0;
    }
}

/* Return the text list entry n */
u8 *get_k_work(s32 entry_index) {
    if (entry_index >= 0x35) {
        return text_list_last;
    }
    return text_list + entry_index * 8;
}

s16 *get_ksys(void) {
    return (u8 *)&text_state;
}

void set_k_work(s32 entry_index, TextEntry entry_value) {
    TextEntry *p = (TextEntry *)text_list + entry_index;

    p->x = entry_value.x;
    p->y = entry_value.y;
    p->attribute = entry_value.attribute;
    p->glyph_count = entry_value.glyph_count;
    p->extra = entry_value.extra;
    p->active = entry_value.active;
}

/* Cut the text list to n entries and recompute the glyph totals */
void k_reset(s32 entry_count) {
    s16 glyph_total;
    s32 i;

    glyph_total = 0;
    for (i = 0; i < entry_count; i++) {
        glyph_total += text_list[i * 8 + 5];
    }
    text_state.count = entry_count;
    text_state.total = glyph_total;
    text_state.pos = glyph_total;
}

/* Remember the current text list length as the mark */
void k_sub_reset_point_set(void) {
    text_state.mark = text_state.count;
}

/* Cut the text list back to the saved mark */
void k_sub_reset(void) {
    k_reset(text_state.mark);
}

/* Start the typewriter at mark + n */
void k_sub_disp_start(s32 entry_offset) {
    k_disp_start(text_state.mark + entry_offset);
}

s16 k_disp_goto_line_end(void) {
    s32 i;
    s32 glyph_end;

    glyph_end = 0;
    for (i = 0; i < 0x34; i++) {
        glyph_end += text_list[i * 8 + 5];
        if (glyph_end >= text_state.pos) {
            text_state.pos = glyph_end;
            return glyph_end;
        }
    }
    return -1;
}

void k_speed_set(u8 reveal_speed) {
    text_state.speed = reveal_speed;
    if ((u32)reveal_speed >= 0x81) {
        text_state.speed = 0x7F;
    }
}

u8 get_k_speed(void) {
    return text_state.speed;
}

/* Reveal one more glyph every speed + 1 frames; returns 1 when everything is shown */
s32 k_disp_inc(void) {
    if (game_state.frame_count % (text_state.speed + 1) == 0) {
        if (text_state.pos < text_state.total) {
            text_state.pos += 1;
            return 0;
        }
        return 1;
    }
    if (text_state.pos == text_state.total) {
        return 1;
    }
    return 0;
}

/* The operand order sets the load order of the two globals (T-0017). */
/* Return nonzero when the typewriter has shown the whole text */
s32 check_end_k(void) {
    if (text_state.pos == text_state.total) {
        return 1;
    }
    return 0;
}

u8 check_set_k(void) {
    return *(u8 *)&text_state;
}

u8 get_now_k(void) {
    s32 i;
    s32 glyph_end;

    glyph_end = 0;
    for (i = 0; i < text_state.pos; i++) {
        glyph_end += text_list[i * 8 + 5];
        if (glyph_end >= text_state.pos) {
            return i;
        }
    }
    return 0xFF;
}

void disp_string(void) {
    SPRT_8 *p;
    s32 i;
    u32 k;
    s32 j;
    u32 ot;
    s32 flag;

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
        if (((TextEntry *)text_list)[j].active == 1) {
            if (ot != ((TextEntry *)text_list)[j].extra) {
                ot = ((TextEntry *)text_list)[j].extra;
                flag = 1;
                if (ot == 0) {
                    ot = 1;
                }
            }
            p = (SPRT_8 *)GetWorkBase(0x10, draw_buffer);
            func_8009F090(p);
            SetSemiTrans(p, 0);
            SetShadeTex(p, 0);
            p->r0 = game_state.screen_brightness;
            p->g0 = game_state.screen_brightness;
            p->b0 = game_state.screen_brightness;
            p->x0 = ((TextEntry *)text_list)[j].x + game_state.draw_origin_x[draw_buffer] + k * text_state.unk_6A;
            p->y0 = ((TextEntry *)text_list)[j].y + game_state.draw_origin_y[draw_buffer];
            /* FAKE: T-24141, same-line UV/Y stores reproduce original as1 instruction scheduling. */
            p->u0 = (i & 7) * 0x10 + 0x80; p->v0 = (i & 0x78) * 2;
            p->clut = func_8009ED74((((TextEntry *)text_list)[j].attribute & 8) * 4 + 0x100, (((TextEntry *)text_list)[j].attribute & 7) + 0x1F8);
            AddPrim(&draw_ot_tags[(draw_buffer << 10) + ot * 4], p);

            p = (SPRT_8 *)GetWorkBase(0x10, draw_buffer);
            func_8009F090(p);
            SetSemiTrans(p, 0);
            SetShadeTex(p, 0);
            p->r0 = 0;
            p->g0 = 0;
            p->b0 = 0;
            p->x0 = ((TextEntry *)text_list)[j].x + game_state.draw_origin_x[draw_buffer] + k * text_state.unk_6A + 1;
            /* FAKE: T-24141, same-line Y/UV stores reproduce original as1 instruction scheduling. */
            p->y0 = ((TextEntry *)text_list)[j].y + game_state.draw_origin_y[draw_buffer] + 1; p->u0 = (i & 7) * 0x10 + 0x80; p->v0 = (i & 0x78) * 2;
            p->clut = func_8009ED74((((TextEntry *)text_list)[j].attribute & 8) * 4 + 0x110, (((TextEntry *)text_list)[j].attribute & 7) + 0x1F8);
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
    }
    if (i >= 0x100) {
        goto_tpage(0x380, 0x100, ot, 0, 0);
    } else if (i >= 0x80) {
        goto_tpage(0x340, 0x100, ot, 0, 0);
    } else {
        goto_tpage(0x300, 0x100, ot, 0, 0);
    }
}

/* Declaration order: IDO gives every local a stack slot once one local (start) lives in memory, and the
 * locals declared before it sit above it; these six reproduce the original frame (T-17020, T-3330). The
 * original passes the offset into the range as a second argument to vload_font (a1). */
s32 load_string(text)
u8 *text;
{
    s32 i;
    u8 *p;
    u8 *q;
    u16 c;
    s32 hi;
    u8 lo;
    s32 first_glyph;

    first_glyph = text_state.total;
    while (1) {
        q = text++;
        hi = *q;
        p = text++;
        if (hi == 0 || (lo = *p) == 0) {
            return text_state.total - first_glyph;
        }
        c = lo + (hi << 8);
        if (c < text_font_range_table[0].first) {
            printf("<kanji.c>error(No-sjis)");
            return text_state.total - first_glyph;
        }
        for (i = 1; i < 46; i++) {
            if (c < text_font_range_table[i].first) {
                vload_font((c - text_font_range_table[i - 1].first) + text_font_range_table[i - 1].glyph,
                           c - text_font_range_table[i - 1].first);
                break;
            }
        }
    }
}

void vload_font(arg0)
s32 arg0;
{
    text_glyph_expand(arg0, text_glyph_upload_ring + (text_state.total & 0xF) * 0x18);
    if (text_state.total < 0x80) {
        text_state.total = text_state.total;
        LoadSquare((text_state.total & 7) * 4 + 0x320, (text_state.total & 0x78) * 2 + 0x100, 4, 0xC,
                   text_glyph_upload_ring + (text_state.total & 0xF) * 0x18);
    } else if (text_state.total < 0x100) {
        text_state.total = text_state.total;
        LoadSquare((text_state.total & 7) * 4 + 0x360, (text_state.total & 0x78) * 2 + 0x100, 4, 0xC,
                   text_glyph_upload_ring + (text_state.total & 0xF) * 0x18);
    } else {
        text_state.total = text_state.total;
        LoadSquare((text_state.total & 7) * 4 + 0x3A0, (text_state.total & 0x78) * 2 + 0x100, 4, 0xC,
                   text_glyph_upload_ring + (text_state.total & 0xF) * 0x18);
    }
    func_8009C674(0);
    text_state.total += 1;
}

void text_glyph_expand(s32 arg0, s32 *arg1) {
    s32 i;
    s32 j;
    u8 buf[144];
    u8 *p;
    u8 *q;

    p = &text_font_glyphs[arg0 * 18];
    for (i = 0; i < 18; i++) {
        buf[i * 8 + 0] = (p[i] & 0x80) >> 7;
        buf[i * 8 + 1] = (p[i] & 0x40) >> 6;
        buf[i * 8 + 2] = (p[i] & 0x20) >> 5;
        buf[i * 8 + 3] = (p[i] & 0x10) >> 4;
        buf[i * 8 + 4] = (p[i] & 0x08) >> 3;
        buf[i * 8 + 5] = (p[i] & 0x04) >> 2;
        buf[i * 8 + 6] = (p[i] & 0x02) >> 1;
        buf[i * 8 + 7] = p[i] & 0x01;
    }
    for (i = 0, q = buf; i < 6; i++) {
        *arg1 = 0;
        for (j = 0; j < 0x20; j += 4) {
            *arg1 += *q << j;
            q++;
        }
        arg1++;
        *arg1 = 0;
        for (j = 0; j < 0x10; j += 4) {
            *arg1 += *q << j;
            q++;
        }
        arg1++;
        *arg1 = 0;
        for (j = 0; j < 0x20; j += 4) {
            *arg1 += *q << j;
            q++;
        }
        arg1++;
        *arg1 = 0;
        for (j = 0; j < 0x10; j += 4) {
            *arg1 += *q << j;
            q++;
        }
        arg1++;
    }
    /* FAKE: T-24135, keeps arg0 live in $a0 across the second loop so j and the loop bound take $a2/$a3. */
    if (arg0 == i) {}
}
