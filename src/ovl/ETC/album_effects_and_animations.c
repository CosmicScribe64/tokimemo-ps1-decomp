/* album_effects_and_animations.c: ; src/ovl/ETC/80132880.c:4-18 begins color split and VRAM blur helpers; same object contains album animation routines. Original object 0x80132880-0x8013B1F0 (config/objects/ETC.txt). */
#include "common.h"
#include "ovl/ETC.h"

void album_rgb555_split(Rgb555Split *arg0) {
    u16 c = arg0->color;

    arg0->r = c & 0x1F;
    arg0->g = c & 0x3E0;
    arg0->b = c & 0x7C00;
}

/* Blur the 274x160 VRAM area at (343, 275): each output pixel mixes arg0% of the pixel with arg1% of
 * the mean of its four neighbours, from three rolling rows of split channels. */
void album_vram_blur(s32 arg0, s32 arg1) {
    RECT rect;
    /* FAKE: three unused words above the buffer put buf at sp+0x90 and the row + 1 spill at
     * sp+0x6C; the original's locals that own these slots are unknown. T-25121 */
    s32 pad[3];
    u16 *src;
    u16 *dst;
    s32 y;
    Rgb555Split buf[3][274];
    s32 x;
    u32 r;
    u32 g;
    u32 b;
    u32 cr;
    u32 cg;
    u32 cb;

    rect.x = 343;
    rect.y = 275;
    rect.w = 274;
    rect.h = 160;
    func_8009C8E0(&rect, (void *)0x80198000);
    func_8009C674(0);
    src = (u16 *)0x80198000;
    dst = (u16 *)0x80198000;
    for (y = 0; y < 160; y++) {
        if (y == 0) {
            for (x = 0; x < 274; x++) {
                buf[2][x].color = *src;
                album_rgb555_split(&buf[2][x]);
                buf[0][x].color = *src;
                album_rgb555_split(&buf[0][x]);
                src++;
            }
        }
        for (x = 0; x < 274; x++) {
            if (y == 159) {
                buf[(y + 1) % 3][x].color = buf[y % 3][x].color;
                album_rgb555_split(&buf[(y + 1) % 3][x]);
            } else {
                buf[(y + 1) % 3][x].color = *src;
                album_rgb555_split(&buf[(y + 1) % 3][x]);
                src++;
            }
        }
        dst++;
        for (x = 1; x < 273; x++) {
            r = (buf[(y + 2) % 3][x].r + buf[(y + 1) % 3][x].r + buf[y % 3][x - 1].r + buf[y % 3][x + 1].r) >> 2;
            g = (buf[(y + 2) % 3][x].g + buf[(y + 1) % 3][x].g + buf[y % 3][x - 1].g + buf[y % 3][x + 1].g) >> 2;
            b = (buf[(y + 2) % 3][x].b + buf[(y + 1) % 3][x].b + buf[y % 3][x - 1].b + buf[y % 3][x + 1].b) >> 2;
            cr = buf[y % 3][x].r * arg0 / 100 + r * arg1 / 100;
            cg = buf[y % 3][x].g * arg0 / 100 + g * arg1 / 100;
            cb = buf[y % 3][x].b * arg0 / 100 + b * arg1 / 100;
            *dst = (cr & 0x1F) | (cg & 0x3E0) | (cb & 0x7C00);
            *dst |= 0x8000;
            dst++;
        }
        dst++;
    }
    func_8009C884(&rect, (void *)0x80198000);
    func_8009C674(0);
}

/* Four semi-transparent POLY_FT4 quads with the shade counter album_fade_shade as colour; the counter steps by 2
 * and wraps at 0x81 (bumping unk_110D). Needs the FAKE volatile view of album_fade_shade in ovl/ETC.h. Notes:
 * drafts/T-25121/ETC/album_fade_quads_draw/NOTES.md. */
/* FAKE: one line per PsyQ setXY4/setUVWH expansion (x0 first) keeps as1's per-line store order and the
 * constant numbering; the original's macros are not in the headers. T-25121 */
s32 album_fade_quads_draw(void) {
    s32 pad[13]; /* FAKE: 13 frame words above y; the original likely had folded coordinate locals. T-25121 */
    s32 y;
    s32 shade;
    s32 y2;
    s32 y0;
    s32 next;
    POLY_FT4 *p0;
    POLY_FT4 *p1;
    POLY_FT4 *p2;
    POLY_FT4 *p3;

    y = (-draw_buffer * 0xF0) + 0xF0;
    p0 = (POLY_FT4 *)GetWorkBase(0x28, draw_buffer);
    p1 = (POLY_FT4 *)GetWorkBase(0x28, draw_buffer);
    p2 = (POLY_FT4 *)GetWorkBase(0x28, draw_buffer);
    p3 = (POLY_FT4 *)GetWorkBase(0x28, draw_buffer);
    func_8009F040(p0);
    func_8009F040(p1);
    func_8009F040(p2);
    func_8009F040(p3);
    SetSemiTrans(p0, 1);
    SetSemiTrans(p1, 1);
    SetSemiTrans(p2, 1);
    SetSemiTrans(p3, 1);
    shade = album_fade_shade;
    p0->r0 = shade;
    p0->g0 = shade;
    p0->b0 = shade;
    p1->b0 = shade;
    p1->g0 = shade;
    p1->r0 = shade;
    p2->b0 = shade;
    p2->g0 = shade;
    p2->r0 = shade;
    p3->b0 = shade;
    p3->g0 = shade;
    p3->r0 = shade;
    /* FAKE: the original stores the unchanged counter back here; real source unknown (T-24012). T-25121 */
    album_fade_shade = shade;
    p0->tpage = func_8009ECB0(2, 2, 0x280, 0);
    p1->tpage = func_8009ECB0(2, 2, 0x300, 0);
    p2->tpage = func_8009ECB0(2, 1, 0x140, 0x100);
    p3->tpage = func_8009ECB0(2, 1, 0x1C0, 0x100);
    y2 = y + 0xB3;
    y0 = y + 0x13;
    p0->x0 = 0x18, p0->y0 = y0, p0->x1 = 0xA0, p0->y1 = y0, p0->x2 = 0x18, p0->y2 = y2, p0->x3 = 0xA0, p0->y3 = y2;
    /* FAKE: empty test splits the uopt block after the first quad's XY stores, as the original's block
     * (16 counted stores) does; the UV constants then start after the tpage store. T-25121 */
    if (p0) {
    }
    /* FAKE: s8 views type u0/u2 as int constants so 0x18 shares the x0 register; real form unknown. T-25121 */
    *(s8 *)&p0->u0 = 0x18, p0->v0 = 0x13, p0->u1 = 0xA0, p0->v1 = 0x13, *(s8 *)&p0->u2 = 0x18, p0->v2 = 0xB3, p0->u3 = 0xA0, p0->v3 = 0xB3;
    p1->x0 = 0xA0, p1->y0 = y0, p1->x1 = 0x128, p1->y1 = y0, p1->x2 = 0xA0, p1->y2 = y2, p1->x3 = 0x128, p1->y3 = y2;
    p1->u0 = 0x20, p1->v0 = 0x13, p1->u1 = 0xA8, p1->v1 = 0x13, p1->u2 = 0x20, p1->v2 = 0xB3, p1->u3 = 0xA8, p1->v3 = 0xB3;
    p2->x0 = 0x18, p2->y0 = y0, p2->x1 = 0xA0, p2->y1 = y0, p2->x2 = 0x18, p2->y2 = y2, p2->x3 = 0xA0, p2->y3 = y2;
    *(s8 *)&p2->u0 = 0x18, p2->v0 = 0x13, p2->u1 = 0xA0, p2->v1 = 0x13, *(s8 *)&p2->u2 = 0x18, p2->v2 = 0xB3, p2->u3 = 0xA0, p2->v3 = 0xB3;
    p3->x0 = 0xA0, p3->y0 = y0, p3->x1 = 0x128, p3->y1 = y0, p3->x2 = 0xA0, p3->y2 = y2, p3->x3 = 0x128, p3->y3 = y2;
    /* FAKE: never-true arm with a u8 store of 0x20: one more use without a reload lifts that constant's
     * priority to 2, so uopt colours it (t0) before 0xA0, 0x128 and 0xA8; ugen drops the arm. T-25201 */
    if ((u32)p0 < 0) {
        p1->code = 0x20;
    }
    p3->u0 = 0x20, p3->v0 = 0x13, p3->u1 = 0xA8, p3->v1 = 0x13, p3->u2 = 0x20, p3->v2 = 0xB3, p3->u3 = 0xA8, p3->v3 = 0xB3;
    AddPrim((draw_buffer << 0xA) + 0x14 + draw_ot_tags, p0);
    AddPrim(draw_ot_tags + (draw_buffer << 0xA) + 0x14, p1);
    AddPrim(draw_ot_tags + (draw_buffer << 0xA) + 4, p2);
    AddPrim(draw_ot_tags + (draw_buffer << 0xA) + 4, p3);
    next = album_fade_shade + 2;
    album_fade_shade = next;
    if (next >= 0x81) {
        game_state.substep++;
        next = 0;
        album_fade_shade = 0;
    }
    /* FAKE: no return and a dead test keep the result in v0 (a return statement puts it in v1). T-25121 */
    if (next) {
    }
}

void album_event_background_load(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    album_event_has_background = 0;
    if (arg0 == 0x14) {
        switch (arg1) {
        case 1:
        case 2:
        case 3:
            if (album_event_year == 0x61) {
                dec_bg_cd_read(0x40F4, 1);
                album_event_has_background = 1;
            } else {
                dec_bg_cd_read(album_background_lba_table[arg0][arg1], 1);
                album_event_has_background = 1;
            }
            break;
        case 4:
            if (album_event_year == 0x61) {
                dec_bg_cd_read(0x467C, 1);
                album_event_has_background = 1;
            } else {
                dec_bg_cd_read(album_background_lba_table[arg0][arg1], 1);
                album_event_has_background = 1;
            }
            break;
        case 17:
        case 18:
        case 19:
        case 20:
            if (album_event_year == 0x60) {
                dec_bg_cd_read(0x4118, 1);
                album_event_has_background = 1;
            } else {
                dec_bg_cd_read(album_background_lba_table[arg0][arg1], 1);
                album_event_has_background = 1;
            }
            break;
        default:
            dec_bg_cd_read(album_background_lba_table[arg0][arg1], 1);
            album_event_has_background = 1;
            break;
        }
    } else if (album_background_lba_table[arg0][arg1] != -1) {
        if (album_background_lba_table[arg0][arg1] == 0x42EB) {
            if ((arg3 == 2) || (arg3 == 3)) {
                dec_bg_cd_read(0x42F4, 1);
                album_event_has_background = 1;
            } else {
                dec_bg_cd_read(album_background_lba_table[arg0][arg1], 1);
                album_event_has_background = 1;
            }
        } else if (album_background_lba_table[arg0][arg1] == 0x42FD) {
            if ((arg3 == 2) || (arg3 == 3)) {
                dec_bg_cd_read(0x4306, 1);
                album_event_has_background = 1;
            } else {
                dec_bg_cd_read(album_background_lba_table[arg0][arg1], 1);
                album_event_has_background = 1;
            }
        } else if (album_background_lba_table[arg0][arg1] == 0x430F) {
            if ((arg3 == 2) || (arg3 == 3)) {
                dec_bg_cd_read(0x4318, 1);
                album_event_has_background = 1;
            } else {
                dec_bg_cd_read(album_background_lba_table[arg0][arg1], 1);
                album_event_has_background = 1;
            }
        } else if (((arg0 != 0) && (arg0 != 1) && (arg0 != 0xB) && (arg0 != 0xA) && (arg0 != 0xD)) || (arg1 != 0)) {
            dec_bg_cd_read(album_background_lba_table[arg0][arg1], 1);
            album_event_has_background = 1;
        }
    }
}

void album_anim_36_tables_set(void) {
    album_anim_36_ptrs = (u8 *)0x801CE0FC;
    D_8014EA74 = (u8 *)0x801CE100;
    D_8014EA78 = (u8 *)0x801CE120;
    D_8014EA7C = *(s16 *)0x801CE13C;
    D_8014EA80 = (u8 *)0x801B0000;
    D_8014EA84 = (u8 *)0x801B2000;
    D_8014EA88 = (u8 *)0x801B6000;
    D_8014EA8C = (u8 *)0x801BA000;
    D_8014EA90 = (u8 *)0x801BE000;
    D_8014EA94 = (u8 *)0x801C2000;
    D_8014EA98 = (u8 *)0x801C6000;
}

void album_anim_37_tables_set(void) {
    album_anim_37_ptrs = (u8 *)0x801CE0F4;
    D_8014EAA0 = (u8 *)0x801CE0F8;
    D_8014EAA4 = (u8 *)0x801CE118;
    D_8014EAA8 = *(s16 *)0x801CE12C;
    D_8014EAAC = (u8 *)0x801B0000;
    D_8014EAB0 = (u8 *)0x801B2000;
    D_8014EAB4 = (u8 *)0x801B6000;
    D_8014EAB8 = (u8 *)0x801BA000;
    D_8014EABC = (u8 *)0x801BE000;
    D_8014EAC0 = (u8 *)0x801C2000;
    D_8014EAC4 = (u8 *)0x801C6000;
}

void album_anim_38_tables_set(void) {
    album_anim_38_ptrs = (u8 *)0x801D2184;
    D_8014EACC = (u8 *)0x801D218C;
    D_8014EAD0 = (u8 *)0x801D21B4;
    D_8014EAD4 = *(s16 *)0x801D21C8;
    D_8014EAD8 = (u8 *)0x801B0000;
    D_8014EADC = (u8 *)0x801B2000;
    D_8014EAE0 = (u8 *)0x801B6000;
    D_8014EAE4 = (u8 *)0x801BA000;
    D_8014EAE8 = (u8 *)0x801BE000;
    D_8014EAEC = (u8 *)0x801C2000;
    D_8014EAF0 = (u8 *)0x801C6000;
}

void album_anim_39_tables_set(void) {
    album_anim_39_ptrs = (u8 *)0x801CE080;
    D_8014EAF8 = (u8 *)0x801CE084;
    D_8014EAFC = (u8 *)0x801CE094;
    D_8014EB00 = *(s16 *)0x801CE0A8;
    D_8014EB04 = (u8 *)0x801B0000;
    D_8014EB08 = (u8 *)0x801B2000;
    D_8014EB0C = (u8 *)0x801B6000;
    D_8014EB10 = (u8 *)0x801BA000;
    D_8014EB14 = (u8 *)0x801BE000;
    D_8014EB18 = (u8 *)0x801C2000;
    D_8014EB1C = (u8 *)0x801C6000;
}

void album_anim_3a_tables_set(void) {
    album_anim_3a_ptrs = (u8 *)0x801CE0C0;
    D_8014EB24 = (u8 *)0x801CE0C4;
    D_8014EB28 = (u8 *)0x801CE0DC;
    D_8014EB2C = *(s16 *)0x801CE0F0;
    D_8014EB30 = (u8 *)0x801B0000;
    D_8014EB34 = (u8 *)0x801B2000;
    D_8014EB38 = (u8 *)0x801B6000;
    D_8014EB3C = (u8 *)0x801BA000;
    D_8014EB40 = (u8 *)0x801BE000;
    D_8014EB44 = (u8 *)0x801C2000;
    D_8014EB48 = (u8 *)0x801C6000;
}

void album_anim_3b_tables_set(void) {
    album_anim_3b_ptrs = (u8 *)0x801D2240;
    D_8014EB50 = (u8 *)0x801D2248;
    D_8014EB54 = (u8 *)0x801D2298;
    D_8014EB58 = *(s16 *)0x801D22B0;
    D_8014EB5C = (u8 *)0x801B0000;
    D_8014EB60 = (u8 *)0x801B2000;
    D_8014EB64 = (u8 *)0x801B6000;
    D_8014EB68 = (u8 *)0x801BA000;
    D_8014EB6C = (u8 *)0x801BE000;
    D_8014EB70 = (u8 *)0x801C2000;
    D_8014EB74 = (u8 *)0x801C6000;
}

void album_anim_00_tables_set(void) {
    album_anim_00_ptrs = (u8 *)0x801D6394;
    D_8014EB7C = (u8 *)0x801D639C;
    D_8014EB80 = (u8 *)0x801D63BC;
    D_8014EB84 = *(s16 *)0x801D63D0;
    D_8014EB88 = (u8 *)0x801B0000;
    D_8014EB8C = (u8 *)0x801D6000;
    D_8014EB90 = (u8 *)0x801B2000;
    D_8014EB94 = (u8 *)0x801B6000;
    D_8014EB98 = (u8 *)0x801BA000;
    D_8014EB9C = (u8 *)0x801BE000;
    D_8014EBA0 = (u8 *)0x801C2000;
    D_8014EBA4 = (u8 *)0x801C6000;
    D_8014EBA8 = (u8 *)0x801D2000;
}

void album_anim_02_tables_set(void) {
    album_anim_02_ptrs = (u8 *)0x801D21D4;
    D_8014EBB0 = (u8 *)0x801D21DC;
    D_8014EBB4 = (u8 *)0x801D21FC;
    D_8014EBB8 = *(s16 *)0x801D2210;
    D_8014EBBC = (u8 *)0x801B0000;
    D_8014EBC0 = (u8 *)0x801B2000;
    D_8014EBC4 = (u8 *)0x801B6000;
    D_8014EBC8 = (u8 *)0x801BA000;
    D_8014EBCC = (u8 *)0x801BE000;
    D_8014EBD0 = (u8 *)0x801C2000;
    D_8014EBD4 = (u8 *)0x801C6000;
}

void album_anim_03_tables_set(void) {
    album_anim_03_ptrs = (u8 *)0x801D21E0;
    D_8014EBDC = (u8 *)0x801D21E8;
    D_8014EBE0 = (u8 *)0x801D2228;
    D_8014EBE4 = *(s16 *)0x801D2240;
    D_8014EBE8 = (u8 *)0x801B0000;
    D_8014EBEC = (u8 *)0x801B2000;
    D_8014EBF0 = (u8 *)0x801B6000;
    D_8014EBF4 = (u8 *)0x801BA000;
    D_8014EBF8 = (u8 *)0x801BE000;
    D_8014EBFC = (u8 *)0x801C2000;
    D_8014EC00 = (u8 *)0x801C6000;
}

void album_anim_04_tables_set(void) {
    album_anim_04_ptrs = (u8 *)0x801D2320;
    D_8014EC08 = (u8 *)0x801D2328;
    D_8014EC0C = (u8 *)0x801D2368;
    D_8014EC10 = *(s16 *)0x801D2380;
    D_8014EC14 = (u8 *)0x801B0000;
    D_8014EC18 = (u8 *)0x801B2000;
    D_8014EC1C = (u8 *)0x801B6000;
    D_8014EC20 = (u8 *)0x801BA000;
    D_8014EC24 = (u8 *)0x801BE000;
    D_8014EC28 = (u8 *)0x801C2000;
    D_8014EC2C = (u8 *)0x801C6000;
}

void album_anim_05_tables_set(void) {
    album_anim_05_ptrs = (u8 *)0x801B0000;
    D_8014EC34 = (u8 *)0x801B2000;
    D_8014EC38 = (u8 *)0x801B6000;
    D_8014EC3C = (u8 *)0x801BA000;
    D_8014EC40 = (u8 *)0x801BE000;
    D_8014EC44 = (u8 *)0x801C2000;
    D_8014EC48 = (u8 *)0x801C6000;
}

void album_anim_06_tables_set(void) {
    album_anim_06_ptrs = (u8 *)0x801DA680;
    D_8014EC50 = (u8 *)0x801DA68C;
    D_8014EC54 = (u8 *)0x801DA72C;
    D_8014EC58 = *(s16 *)0x801DA754;
    D_8014EC5C = (u8 *)0x801B0000;
    D_8014EC60 = (u8 *)0x801DA000;
    D_8014EC64 = (u8 *)0x801B2000;
    D_8014EC68 = (u8 *)0x801B6000;
    D_8014EC6C = (u8 *)0x801BA000;
    D_8014EC70 = (u8 *)0x801BE000;
    D_8014EC74 = (u8 *)0x801C2000;
    D_8014EC78 = (u8 *)0x801C6000;
    D_8014EC7C = (u8 *)0x801D6000;
}

void album_anim_07_tables_set(void) {
    album_anim_07_ptrs = (u8 *)0x801D22D0;
    D_8014EC84 = (u8 *)0x801D22D8;
    D_8014EC88 = (u8 *)0x801D233C;
    D_8014EC8C = *(s16 *)0x801D2358;
    D_8014EC90 = (u8 *)0x801B0000;
    D_8014EC94 = (u8 *)0x801B2000;
    D_8014EC98 = (u8 *)0x801B6000;
    D_8014EC9C = (u8 *)0x801BA000;
    D_8014ECA0 = (u8 *)0x801BE000;
    D_8014ECA4 = (u8 *)0x801C2000;
    D_8014ECA8 = (u8 *)0x801C6000;
}

void album_anim_0e_tables_set(void) {
    album_anim_0e_ptrs = (u8 *)0x801D212C;
    D_8014ECB0 = (u8 *)0x801D2130;
    D_8014ECB4 = (u8 *)0x801D2150;
    D_8014ECB8 = *(s16 *)0x801D216C;
    D_8014ECBC = (u8 *)0x801B0000;
    D_8014ECC0 = (u8 *)0x801B2000;
    D_8014ECC4 = (u8 *)0x801B6000;
    D_8014ECC8 = (u8 *)0x801BA000;
    D_8014ECCC = (u8 *)0x801BE000;
    D_8014ECD0 = (u8 *)0x801C2000;
    D_8014ECD4 = (u8 *)0x801C6000;
}

void album_anim_0f_tables_set(void) {
    album_anim_0f_ptrs = (u8 *)0x801B0000;
    D_8014ECDC = (u8 *)0x801B2000;
    D_8014ECE0 = (u8 *)0x801B6000;
    D_8014ECE4 = (u8 *)0x801BA000;
    D_8014ECE8 = (u8 *)0x801BE000;
    D_8014ECEC = (u8 *)0x801C2000;
    D_8014ECF0 = (u8 *)0x801C6000;
}

void album_anim_10_tables_set(void) {
    album_anim_10_ptrs = (u8 *)0x801CE1A4;
    D_8014ECF8 = (u8 *)0x801CE1A8;
    D_8014ECFC = (u8 *)0x801CE1E0;
    D_8014ED00 = *(s16 *)0x801CE1FC;
    D_8014ED04 = (u8 *)0x801B0000;
    D_8014ED08 = (u8 *)0x801B2000;
    D_8014ED0C = (u8 *)0x801B6000;
    D_8014ED10 = (u8 *)0x801BA000;
    D_8014ED14 = (u8 *)0x801BE000;
    D_8014ED18 = (u8 *)0x801C2000;
    D_8014ED1C = (u8 *)0x801C6000;
}

void album_anim_11_tables_set(void) {
    album_anim_11_ptrs = (u8 *)0x801D2370;
    D_8014ED24 = (u8 *)0x801D2374;
    D_8014ED28 = (u8 *)0x801D23A4;
    D_8014ED2C = *(s16 *)0x801D23C0;
    D_8014ED30 = (u8 *)0x801B0000;
    D_8014ED34 = (u8 *)0x801D2000;
    D_8014ED38 = (u8 *)0x801B2000;
    D_8014ED3C = (u8 *)0x801B6000;
    D_8014ED40 = (u8 *)0x801BA000;
    D_8014ED44 = (u8 *)0x801BE000;
    D_8014ED48 = (u8 *)0x801C2000;
    D_8014ED4C = (u8 *)0x801C6000;
    D_8014ED50 = (u8 *)0x801CE000;
}

void album_anim_12_tables_set(void) {
    album_anim_12_ptrs = (u8 *)0x801D2174;
    D_8014ED58 = (u8 *)0x801D217C;
    D_8014ED5C = (u8 *)0x801D21A8;
    D_8014ED60 = *(s16 *)0x801D21C4;
    D_8014ED64 = (u8 *)0x801B0000;
    D_8014ED68 = (u8 *)0x801B2000;
    D_8014ED6C = (u8 *)0x801B6000;
    D_8014ED70 = (u8 *)0x801BA000;
    D_8014ED74 = (u8 *)0x801BE000;
    D_8014ED78 = (u8 *)0x801C2000;
    D_8014ED7C = (u8 *)0x801C6000;
}

void album_anim_13_tables_set(void) {
    album_anim_13_ptrs = (u8 *)0x801C25B8;
    D_8014ED84 = (u8 *)0x801C25C8;
    D_8014ED88 = (u8 *)0x801C2674;
    D_8014ED8C = *(s16 *)0x801C26A0;
    D_8014ED90 = (u8 *)0x801B0000;
}

void album_anim_50_tables_set(void) {
    album_anim_50_ptrs = (u8 *)0x801B0000;
    D_8014ED98 = (u8 *)0x801B2000;
    D_8014ED9C = (u8 *)0x801B6000;
    D_8014EDA0 = (u8 *)0x801BA000;
    D_8014EDA4 = (u8 *)0x801BE000;
    D_8014EDA8 = (u8 *)0x801C2000;
    D_8014EDAC = (u8 *)0x801C6000;
}

void album_anim_15_tables_set(void) {
    album_anim_15_ptrs = (u8 *)0x801CE120;
    D_8014EDB4 = (u8 *)0x801CE124;
    D_8014EDB8 = (u8 *)0x801CE144;
    D_8014EDBC = *(s16 *)0x801CE158;
    D_8014EDC0 = (u8 *)0x801B0000;
    D_8014EDC4 = (u8 *)0x801B2000;
    D_8014EDC8 = (u8 *)0x801B6000;
    D_8014EDCC = (u8 *)0x801BA000;
    D_8014EDD0 = (u8 *)0x801BE000;
    D_8014EDD4 = (u8 *)0x801C2000;
    D_8014EDD8 = (u8 *)0x801C6000;
}

void album_anim_16_tables_set(void) {
    album_anim_16_ptrs = (u8 *)0x801E2118;
    D_8014EDE0 = (u8 *)0x801E2130;
    D_8014EDE4 = (u8 *)0x801E2154;
    D_8014EDE8 = *(s16 *)0x801E216C;
    D_8014EDEC = (u8 *)0x801B0000;
    D_8014EDF0 = (u8 *)0x801B2000;
    D_8014EDF4 = (u8 *)0x801B6000;
    D_8014EDF8 = (u8 *)0x801BA000;
    D_8014EDFC = (u8 *)0x801BE000;
    D_8014EE00 = (u8 *)0x801C2000;
    D_8014EE04 = (u8 *)0x801C6000;
}

void album_anim_17_tables_set(void) {
    album_anim_17_ptrs = (u8 *)0x801D2160;
    D_8014EE0C = (u8 *)0x801D2168;
    D_8014EE10 = (u8 *)0x801D2188;
    D_8014EE14 = *(s16 *)0x801D21A4;
    D_8014EE18 = (u8 *)0x801B0000;
    D_8014EE1C = (u8 *)0x801B2000;
    D_8014EE20 = (u8 *)0x801B6000;
    D_8014EE24 = (u8 *)0x801BA000;
    D_8014EE28 = (u8 *)0x801BE000;
    D_8014EE2C = (u8 *)0x801C2000;
    D_8014EE30 = (u8 *)0x801C6000;
}

void album_anim_18_tables_set(void) {
    album_anim_18_ptrs = (u8 *)0x801CE124;
    D_8014EE38 = (u8 *)0x801CE128;
    D_8014EE3C = (u8 *)0x801CE148;
    D_8014EE40 = *(s16 *)0x801CE15C;
    D_8014EE44 = (u8 *)0x801B0000;
    D_8014EE48 = (u8 *)0x801B2000;
    D_8014EE4C = (u8 *)0x801B6000;
    D_8014EE50 = (u8 *)0x801BA000;
    D_8014EE54 = (u8 *)0x801BE000;
    D_8014EE58 = (u8 *)0x801C2000;
    D_8014EE5C = (u8 *)0x801C6000;
}

void album_anim_19_tables_set(void) {
    album_anim_19_ptrs = (u8 *)0x801CE080;
    D_8014EE64 = (u8 *)0x801CE084;
    D_8014EE68 = (u8 *)0x801CE094;
    D_8014EE6C = *(s16 *)0x801CE0A8;
    D_8014EE70 = (u8 *)0x801B0000;
    D_8014EE74 = (u8 *)0x801B2000;
    D_8014EE78 = (u8 *)0x801B6000;
    D_8014EE7C = (u8 *)0x801BA000;
    D_8014EE80 = (u8 *)0x801BE000;
    D_8014EE84 = (u8 *)0x801C2000;
    D_8014EE88 = (u8 *)0x801C6000;
}

void album_anim_1a_tables_set(void) {
    album_anim_1a_ptrs = (u8 *)0x801D2384;
    D_8014EE90 = (u8 *)0x801D238C;
    D_8014EE94 = (u8 *)0x801D23F0;
    D_8014EE98 = *(s16 *)0x801D240C;
    D_8014EE9C = (u8 *)0x801B0000;
    D_8014EEA0 = (u8 *)0x801B2000;
    D_8014EEA4 = (u8 *)0x801B6000;
    D_8014EEA8 = (u8 *)0x801BA000;
    D_8014EEAC = (u8 *)0x801BE000;
    D_8014EEB0 = (u8 *)0x801C2000;
    D_8014EEB4 = (u8 *)0x801C6000;
}

void album_anim_2e_tables_set(void) {
    album_anim_2e_ptrs = (u8 *)0x801CE218;
    D_8014EEBC = (u8 *)0x801CE21C;
    D_8014EEC0 = (u8 *)0x801CE25C;
    D_8014EEC4 = *(s16 *)0x801CE27C;
    D_8014EEC8 = (u8 *)0x801B0000;
    D_8014EECC = (u8 *)0x801B2000;
    D_8014EED0 = (u8 *)0x801B6000;
    D_8014EED4 = (u8 *)0x801BA000;
    D_8014EED8 = (u8 *)0x801BE000;
    D_8014EEDC = (u8 *)0x801C2000;
    D_8014EEE0 = (u8 *)0x801C6000;
}

void album_anim_2f_tables_set(void) {
    album_anim_2f_ptrs = (u8 *)0x801CE124;
    D_8014EEE8 = (u8 *)0x801CE128;
    D_8014EEEC = (u8 *)0x801CE148;
    D_8014EEF0 = *(s16 *)0x801CE15C;
    D_8014EEF4 = (u8 *)0x801B0000;
    D_8014EEF8 = (u8 *)0x801B2000;
    D_8014EEFC = (u8 *)0x801B6000;
    D_8014EF00 = (u8 *)0x801BA000;
    D_8014EF04 = (u8 *)0x801BE000;
    D_8014EF08 = (u8 *)0x801C2000;
    D_8014EF0C = (u8 *)0x801C6000;
}

void album_anim_30_tables_set(void) {
    album_anim_30_ptrs = (u8 *)0x801CE0F4;
    D_8014EF14 = (u8 *)0x801CE0F8;
    D_8014EF18 = (u8 *)0x801CE118;
    D_8014EF1C = *(s16 *)0x801CE12C;
    D_8014EF20 = (u8 *)0x801B0000;
    D_8014EF24 = (u8 *)0x801B2000;
    D_8014EF28 = (u8 *)0x801B6000;
    D_8014EF2C = (u8 *)0x801BA000;
    D_8014EF30 = (u8 *)0x801BE000;
    D_8014EF34 = (u8 *)0x801C2000;
    D_8014EF38 = (u8 *)0x801C6000;
}

void album_anim_31_tables_set(void) {
    album_anim_31_ptrs = (u8 *)0x801CE088;
    D_8014EF40 = (u8 *)0x801CE08C;
    D_8014EF44 = (u8 *)0x801CE0A0;
    D_8014EF48 = *(s16 *)0x801CE0B4;
    D_8014EF4C = (u8 *)0x801B0000;
    D_8014EF50 = (u8 *)0x801B2000;
    D_8014EF54 = (u8 *)0x801B6000;
    D_8014EF58 = (u8 *)0x801BA000;
    D_8014EF5C = (u8 *)0x801BE000;
    D_8014EF60 = (u8 *)0x801C2000;
    D_8014EF64 = (u8 *)0x801C6000;
}

void album_anim_32_tables_set(void) {
    album_anim_32_ptrs = (u8 *)0x801CE0F4;
    D_8014EF6C = (u8 *)0x801CE0F8;
    D_8014EF70 = (u8 *)0x801CE118;
    D_8014EF74 = *(s16 *)0x801CE12C;
    D_8014EF78 = (u8 *)0x801B0000;
    D_8014EF7C = (u8 *)0x801B2000;
    D_8014EF80 = (u8 *)0x801B6000;
    D_8014EF84 = (u8 *)0x801BA000;
    D_8014EF88 = (u8 *)0x801BE000;
    D_8014EF8C = (u8 *)0x801C2000;
    D_8014EF90 = (u8 *)0x801C6000;
}

void album_anim_34_tables_set(void) {
    album_anim_34_ptrs = (u8 *)0x801D2430;
    D_8014EF98 = (u8 *)0x801D2434;
    D_8014EF9C = (u8 *)0x801D2498;
    D_8014EFA0 = *(s16 *)0x801D24B4;
    D_8014EFA4 = (u8 *)0x801B0000;
    D_8014EC60 = (u8 *)0x801D2000;
    D_8014EFA8 = (u8 *)0x801B2000;
    D_8014EFAC = (u8 *)0x801B6000;
    D_8014EFB0 = (u8 *)0x801BA000;
    D_8014EFB4 = (u8 *)0x801BE000;
    D_8014EFB8 = (u8 *)0x801C2000;
    D_8014EFBC = (u8 *)0x801C6000;
    D_8014EC7C = (u8 *)0x801CE000;
}

void album_special_anim_46_tables_set(void) {
    album_special_anim_46_ptrs = (u8 *)0x801E8088;
    D_8014EFC4 = (u8 *)0x801E808C;
    D_8014EFC8 = (u8 *)0x801E80A0;
    D_8014EFCC = *(s16 *)0x801E80B4;
    D_8014EFD0 = (u8 *)0x801B0000;
    D_8014EFD4 = (u8 *)0x801B2000;
    D_8014EFD8 = (u8 *)0x801B4000;
    D_8014EFDC = (u8 *)0x801B8000;
    D_8014EFE0 = (u8 *)0x801BC000;
    D_8014EFE4 = (u8 *)0x801C0000;
    D_8014EFE8 = (u8 *)0x801C4000;
    D_8014EFEC = (u8 *)0x801C8000;
    D_8014EFF0 = (u8 *)0x801D0000;
    D_8014EFF4 = (u8 *)0x801D4000;
    D_8014EFF8 = (u8 *)0x801D8000;
    D_8014EFFC = (u8 *)0x801DC000;
    D_8014F000 = (u8 *)0x801E0000;
    D_8014F004 = (u8 *)0x801E4000;
}

void album_anim_08_tables_set(void) {
    album_anim_08_ptrs = (u8 *)0x801CE0F4;
    D_8014F00C = (u8 *)0x801CE0F8;
    D_8014F010 = (u8 *)0x801CE118;
    D_8014F014 = *(s16 *)0x801CE12C;
    D_8014F018 = (u8 *)0x801B0000;
    D_8014F01C = (u8 *)0x801B2000;
    D_8014F020 = (u8 *)0x801B6000;
    D_8014F024 = (u8 *)0x801BA000;
    D_8014F028 = (u8 *)0x801BE000;
    D_8014F02C = (u8 *)0x801C2000;
    D_8014F030 = (u8 *)0x801C6000;
}

void album_anim_09_tables_set(void) {
    album_anim_09_ptrs = (u8 *)0x801D2240;
    D_8014F038 = (u8 *)0x801D2248;
    D_8014F03C = (u8 *)0x801D2288;
    D_8014F040 = *(s16 *)0x801D22A0;
    D_8014F044 = (u8 *)0x801B0000;
    D_8014F048 = (u8 *)0x801B2000;
    D_8014F04C = (u8 *)0x801B6000;
    D_8014F050 = (u8 *)0x801BA000;
    D_8014F054 = (u8 *)0x801BE000;
    D_8014F058 = (u8 *)0x801C2000;
    D_8014F05C = (u8 *)0x801C6000;
}

void album_anim_0a_tables_set(void) {
    album_anim_0a_ptrs = (u8 *)0x801D21E0;
    D_8014F064 = (u8 *)0x801D21E8;
    D_8014F068 = (u8 *)0x801D2228;
    D_8014F06C = *(s16 *)0x801D2240;
    D_8014F070 = (u8 *)0x801B0000;
    D_8014F074 = (u8 *)0x801B2000;
    D_8014F078 = (u8 *)0x801B6000;
    D_8014F07C = (u8 *)0x801BA000;
    D_8014F080 = (u8 *)0x801BE000;
    D_8014F084 = (u8 *)0x801C2000;
    D_8014F088 = (u8 *)0x801C6000;
}

void album_anim_0b_tables_set(void) {
    album_anim_0b_ptrs = (u8 *)0x801CE078;
    D_8014F090 = (u8 *)0x801CE07C;
    D_8014F094 = (u8 *)0x801CE08C;
    D_8014F098 = *(s16 *)0x801CE098;
    D_8014F09C = (u8 *)0x801B0000;
    D_8014F0A0 = (u8 *)0x801B2000;
    D_8014F0A4 = (u8 *)0x801B6000;
    D_8014F0A8 = (u8 *)0x801BA000;
    D_8014F0AC = (u8 *)0x801BE000;
    D_8014F0B0 = (u8 *)0x801C2000;
    D_8014F0B4 = (u8 *)0x801C6000;
}

void album_anim_0c_tables_set(void) {
    album_anim_0c_ptrs = (u8 *)0x801CE148;
    D_8014F0BC = (u8 *)0x801CE14C;
    D_8014F0C0 = (u8 *)0x801CE170;
    D_8014F0C4 = *(s16 *)0x801CE184;
    D_8014F0C8 = (u8 *)0x801B0000;
    D_8014F0CC = (u8 *)0x801B2000;
    D_8014F0D0 = (u8 *)0x801B6000;
    D_8014F0D4 = (u8 *)0x801BA000;
    D_8014F0D8 = (u8 *)0x801BE000;
    D_8014F0DC = (u8 *)0x801C2000;
    D_8014F0E0 = (u8 *)0x801C6000;
}

void album_special_anim_0e_tables_set(void) {
    album_special_anim_0e_ptrs = (u8 *)0x801E60CC;
    D_8014F0E8 = (u8 *)0x801E60D0;
    D_8014F0EC = (u8 *)0x801E60E4;
    D_8014F0F0 = *(s16 *)0x801E60F0;
    D_8014F0F4 = (u8 *)0x801AE000;
    D_8014F0F8 = (u8 *)0x801B0000;
    D_8014F0FC = (u8 *)0x801B2000;
    D_8014F100 = (u8 *)0x801B6000;
    D_8014F104 = (u8 *)0x801BA000;
    D_8014F108 = (u8 *)0x801BE000;
    D_8014F10C = (u8 *)0x801C2000;
    D_8014F110 = (u8 *)0x801C6000;
    D_8014F114 = (u8 *)0x801CE000;
    D_8014F118 = (u8 *)0x801D2000;
    D_8014F11C = (u8 *)0x801D6000;
    D_8014F120 = (u8 *)0x801DA000;
    D_8014F124 = (u8 *)0x801DE000;
    D_8014F128 = (u8 *)0x801E2000;
}

void album_anim_21_tables_set(void) {
    album_anim_21_ptrs = (u8 *)0x801CE0F4;
    D_8014F130 = (u8 *)0x801CE0F8;
    D_8014F134 = (u8 *)0x801CE118;
    D_8014F138 = *(s16 *)0x801CE12C;
    D_8014F13C = (u8 *)0x801B0000;
    D_8014F140 = (u8 *)0x801B2000;
    D_8014F144 = (u8 *)0x801B6000;
    D_8014F148 = (u8 *)0x801BA000;
    D_8014F14C = (u8 *)0x801BE000;
    D_8014F150 = (u8 *)0x801C2000;
    D_8014F154 = (u8 *)0x801C6000;
}

void album_anim_23_tables_set(void) {
    album_anim_23_ptrs = (u8 *)0x801CE080;
    D_8014F15C = (u8 *)0x801CE084;
    D_8014F160 = (u8 *)0x801CE094;
    D_8014F164 = *(s16 *)0x801CE0A8;
    D_8014F168 = (u8 *)0x801B0000;
    D_8014F16C = (u8 *)0x801B2000;
    D_8014F170 = (u8 *)0x801B6000;
    D_8014F174 = (u8 *)0x801BA000;
    D_8014F178 = (u8 *)0x801BE000;
    D_8014F17C = (u8 *)0x801C2000;
    D_8014F180 = (u8 *)0x801C6000;
}

void album_anim_24_tables_set(void) {
    album_anim_24_ptrs = (u8 *)0x801CE31C;
    D_8014F188 = (u8 *)0x801CE320;
    D_8014F18C = (u8 *)0x801CE39C;
    D_8014F190 = *(s16 *)0x801CE3BC;
    D_8014F194 = (u8 *)0x801B0000;
    D_8014F198 = (u8 *)0x801B2000;
    D_8014F19C = (u8 *)0x801B6000;
    D_8014F1A0 = (u8 *)0x801BA000;
    D_8014F1A4 = (u8 *)0x801BE000;
    D_8014F1A8 = (u8 *)0x801C2000;
    D_8014F1AC = (u8 *)0x801C6000;
}

void album_anim_25_tables_set(void) {
    album_anim_25_ptrs = (u8 *)0x801CE124;
    D_8014F1B4 = (u8 *)0x801CE128;
    D_8014F1B8 = (u8 *)0x801CE148;
    D_8014F1BC = *(s16 *)0x801CE15C;
    D_8014F1C0 = (u8 *)0x801B0000;
    D_8014F1C4 = (u8 *)0x801B2000;
    D_8014F1C8 = (u8 *)0x801B6000;
    D_8014F1CC = (u8 *)0x801BA000;
    D_8014F1D0 = (u8 *)0x801BE000;
    D_8014F1D4 = (u8 *)0x801C2000;
    D_8014F1D8 = (u8 *)0x801C6000;
}

void album_anim_26_tables_set(void) {
    album_anim_26_ptrs = (u8 *)0x801B0000;
    D_8014F1E0 = (u8 *)0x801B2000;
    D_8014F1E4 = (u8 *)0x801B6000;
    D_8014F1E8 = (u8 *)0x801BA000;
    D_8014F1EC = (u8 *)0x801BE000;
    D_8014F1F0 = (u8 *)0x801C2000;
    D_8014F1F4 = (u8 *)0x801C6000;
}

void album_anim_27_tables_set(void) {
    album_anim_27_ptrs = (u8 *)0x801D22B8;
    D_8014F1FC = (u8 *)0x801D22C0;
    D_8014F200 = (u8 *)0x801D2310;
    D_8014F204 = *(s16 *)0x801D2328;
    D_8014F208 = (u8 *)0x801B0000;
    D_8014F20C = (u8 *)0x801B2000;
    D_8014F210 = (u8 *)0x801B6000;
    D_8014F214 = (u8 *)0x801BA000;
    D_8014F218 = (u8 *)0x801BE000;
    D_8014F21C = (u8 *)0x801C2000;
    D_8014F220 = (u8 *)0x801C6000;
}

void album_anim_28_tables_set(void) {
    album_anim_28_ptrs = (u8 *)0x801CE124;
    D_8014F228 = (u8 *)0x801CE128;
    D_8014F22C = (u8 *)0x801CE148;
    D_8014F230 = *(s16 *)0x801CE15C;
    D_8014F234 = (u8 *)0x801B0000;
    D_8014F238 = (u8 *)0x801B2000;
    D_8014F23C = (u8 *)0x801B6000;
    D_8014F240 = (u8 *)0x801BA000;
    D_8014F244 = (u8 *)0x801BE000;
    D_8014F248 = (u8 *)0x801C2000;
    D_8014F24C = (u8 *)0x801C6000;
}

void album_anim_29_tables_set(void) {
    album_anim_29_ptrs = (u8 *)0x801CE0DC;
    D_8014F254 = (u8 *)0x801CE0E0;
    D_8014F258 = (u8 *)0x801CE0F8;
    D_8014F25C = *(s16 *)0x801CE10C;
    D_8014F260 = (u8 *)0x801B0000;
    D_8014F264 = (u8 *)0x801B2000;
    D_8014F268 = (u8 *)0x801B6000;
    D_8014F26C = (u8 *)0x801BA000;
    D_8014F270 = (u8 *)0x801BE000;
    D_8014F274 = (u8 *)0x801C2000;
    D_8014F278 = (u8 *)0x801C6000;
}

void album_anim_2a_tables_set(void) {
    album_anim_2a_ptrs = (u8 *)0x801D2100;
    D_8014F280 = (u8 *)0x801D2108;
    D_8014F284 = (u8 *)0x801D2128;
    D_8014F288 = *(s16 *)0x801D2144;
    D_8014F28C = (u8 *)0x801B0000;
    D_8014F290 = (u8 *)0x801B2000;
    D_8014F294 = (u8 *)0x801B6000;
    D_8014F298 = (u8 *)0x801BA000;
    D_8014F29C = (u8 *)0x801BE000;
    D_8014F2A0 = (u8 *)0x801C2000;
    D_8014F2A4 = (u8 *)0x801C6000;
}

void album_anim_2b_tables_set(void) {
    album_anim_2b_ptrs = (u8 *)0x801D2240;
    D_8014F2AC = (u8 *)0x801D2248;
    D_8014F2B0 = (u8 *)0x801D2288;
    D_8014F2B4 = *(s16 *)0x801D22A0;
    D_8014F2B8 = (u8 *)0x801B0000;
    D_8014F2BC = (u8 *)0x801B2000;
    D_8014F2C0 = (u8 *)0x801B6000;
    D_8014F2C4 = (u8 *)0x801BA000;
    D_8014F2C8 = (u8 *)0x801BE000;
    D_8014F2CC = (u8 *)0x801C2000;
    D_8014F2D0 = (u8 *)0x801C6000;
}

void album_anim_2c_tables_set(void) {
    album_anim_2c_ptrs = (u8 *)0x801CE080;
    D_8014F2D8 = (u8 *)0x801CE084;
    D_8014F2DC = (u8 *)0x801CE094;
    D_8014F2E0 = *(s16 *)0x801CE0A8;
    D_8014F2E4 = (u8 *)0x801B0000;
    D_8014F2E8 = (u8 *)0x801B2000;
    D_8014F2EC = (u8 *)0x801B6000;
    D_8014F2F0 = (u8 *)0x801BA000;
    D_8014F2F4 = (u8 *)0x801BE000;
    D_8014F2F8 = (u8 *)0x801C2000;
    D_8014F2FC = (u8 *)0x801C6000;
}

void album_anim_3c_tables_set(void) {
    album_anim_3c_ptrs = (u8 *)0x801D22F4;
    D_8014F304 = (u8 *)0x801D22F8;
    D_8014F308 = (u8 *)0x801D2318;
    D_8014F30C = *(s16 *)0x801D232C;
    D_8014F310 = (u8 *)0x801B0000;
    D_8014EB8C = (u8 *)0x801D2000;
    D_8014F314 = (u8 *)0x801B2000;
    D_8014F318 = (u8 *)0x801B6000;
    D_8014F31C = (u8 *)0x801BA000;
    D_8014F320 = (u8 *)0x801BE000;
    D_8014F324 = (u8 *)0x801C2000;
    D_8014F328 = (u8 *)0x801C6000;
    D_8014EBA8 = (u8 *)0x801CE000;
}

void album_anim_3e_tables_set(void) {
    album_anim_3e_ptrs = (u8 *)0x801CE090;
    D_8014F330 = (u8 *)0x801CE094;
    D_8014F334 = (u8 *)0x801CE0AC;
    D_8014F338 = *(s16 *)0x801CE0C0;
    D_8014F33C = (u8 *)0x801B0000;
    D_8014F340 = (u8 *)0x801B2000;
    D_8014F344 = (u8 *)0x801B6000;
    D_8014F348 = (u8 *)0x801BA000;
    D_8014F34C = (u8 *)0x801BE000;
    D_8014F350 = (u8 *)0x801C2000;
    D_8014F354 = (u8 *)0x801C6000;
}

void album_anim_3f_tables_set(void) {
    album_anim_3f_ptrs = (u8 *)0x801D2240;
    D_8014F35C = (u8 *)0x801D2248;
    D_8014F360 = (u8 *)0x801D2288;
    D_8014F364 = *(s16 *)0x801D22A0;
    D_8014F368 = (u8 *)0x801B0000;
    D_8014F36C = (u8 *)0x801B2000;
    D_8014F370 = (u8 *)0x801B6000;
    D_8014F374 = (u8 *)0x801BA000;
    D_8014F378 = (u8 *)0x801BE000;
    D_8014F37C = (u8 *)0x801C2000;
    D_8014F380 = (u8 *)0x801C6000;
}

void album_anim_40_tables_set(void) {
    album_anim_40_ptrs = (u8 *)0x801CE078;
    D_8014F388 = (u8 *)0x801CE07C;
    D_8014F38C = (u8 *)0x801CE08C;
    D_8014F390 = *(s16 *)0x801CE098;
    D_8014F394 = (u8 *)0x801B0000;
    D_8014F398 = (u8 *)0x801B2000;
    D_8014F39C = (u8 *)0x801B6000;
    D_8014F3A0 = (u8 *)0x801BA000;
    D_8014F3A4 = (u8 *)0x801BE000;
    D_8014F3A8 = (u8 *)0x801C2000;
    D_8014F3AC = (u8 *)0x801C6000;
}

void album_anim_41_tables_set(void) {
    album_anim_41_ptrs = (u8 *)0x801CE0F4;
    D_8014F3B4 = (u8 *)0x801CE0F8;
    D_8014F3B8 = (u8 *)0x801CE118;
    D_8014F3BC = *(s16 *)0x801CE12C;
    D_8014F3C0 = (u8 *)0x801B0000;
    D_8014F3C4 = (u8 *)0x801B2000;
    D_8014F3C8 = (u8 *)0x801B6000;
    D_8014F3CC = (u8 *)0x801BA000;
    D_8014F3D0 = (u8 *)0x801BE000;
    D_8014F3D4 = (u8 *)0x801C2000;
    D_8014F3D8 = (u8 *)0x801C6000;
}

void album_anim_42_tables_set(void) {
    album_anim_42_ptrs = (u8 *)0x801D22D0;
    D_8014F3E0 = (u8 *)0x801D22D8;
    D_8014F3E4 = (u8 *)0x801D233C;
    D_8014F3E8 = *(s16 *)0x801D2358;
    D_8014F3EC = (u8 *)0x801B0000;
    D_8014F3F0 = (u8 *)0x801B2000;
    D_8014F3F4 = (u8 *)0x801B6000;
    D_8014F3F8 = (u8 *)0x801BA000;
    D_8014F3FC = (u8 *)0x801BE000;
    D_8014F400 = (u8 *)0x801C2000;
    D_8014F404 = (u8 *)0x801C6000;
}

void album_anim_1b_tables_set(void) {
    album_anim_1b_ptrs = (u8 *)0x801CE124;
    D_8014F40C = (u8 *)0x801CE128;
    D_8014F410 = (u8 *)0x801CE148;
    D_8014F414 = *(s16 *)0x801CE15C;
    D_8014F418 = (u8 *)0x801B0000;
    D_8014F41C = (u8 *)0x801B2000;
    D_8014F420 = (u8 *)0x801B6000;
    D_8014F424 = (u8 *)0x801BA000;
    D_8014F428 = (u8 *)0x801BE000;
    D_8014F42C = (u8 *)0x801C2000;
    D_8014F430 = (u8 *)0x801C6000;
}

void album_anim_1c_tables_set(void) {
    album_anim_1c_ptrs = (u8 *)0x801CE124;
    D_8014F438 = (u8 *)0x801CE128;
    D_8014F43C = (u8 *)0x801CE148;
    D_8014F440 = *(s16 *)0x801CE15C;
    D_8014F444 = (u8 *)0x801B0000;
    D_8014F448 = (u8 *)0x801B2000;
    D_8014F44C = (u8 *)0x801B6000;
    D_8014F450 = (u8 *)0x801BA000;
    D_8014F454 = (u8 *)0x801BE000;
    D_8014F458 = (u8 *)0x801C2000;
    D_8014F45C = (u8 *)0x801C6000;
}

void album_anim_1d_tables_set(void) {
    album_anim_1d_ptrs = (u8 *)0x801D2240;
    D_8014F464 = (u8 *)0x801D2248;
    D_8014F468 = (u8 *)0x801D2288;
    D_8014F46C = *(s16 *)0x801D22A0;
    D_8014F470 = (u8 *)0x801B0000;
    D_8014F474 = (u8 *)0x801B2000;
    D_8014F478 = (u8 *)0x801B6000;
    D_8014F47C = (u8 *)0x801BA000;
    D_8014F480 = (u8 *)0x801BE000;
    D_8014F484 = (u8 *)0x801C2000;
    D_8014F488 = (u8 *)0x801C6000;
}

void album_anim_1e_tables_set(void) {
    album_anim_1e_ptrs = (u8 *)0x801CE0F4;
    D_8014F490 = (u8 *)0x801CE0F8;
    D_8014F494 = (u8 *)0x801CE118;
    D_8014F498 = *(s16 *)0x801CE12C;
    D_8014F49C = (u8 *)0x801B0000;
    D_8014F4A0 = (u8 *)0x801B2000;
    D_8014F4A4 = (u8 *)0x801B6000;
    D_8014F4A8 = (u8 *)0x801BA000;
    D_8014F4AC = (u8 *)0x801BE000;
    D_8014F4B0 = (u8 *)0x801C2000;
    D_8014F4B4 = (u8 *)0x801C6000;
}

void album_anim_1f_tables_set(void) {
    album_anim_1f_ptrs = (u8 *)0x801CE1A0;
    D_8014F4BC = (u8 *)0x801CE1A4;
    D_8014F4C0 = (u8 *)0x801CE1D4;
    D_8014F4C4 = *(s16 *)0x801CE1F0;
    D_8014F4C8 = (u8 *)0x801B0000;
    D_8014F4CC = (u8 *)0x801B2000;
    D_8014F4D0 = (u8 *)0x801B6000;
    D_8014F4D4 = (u8 *)0x801BA000;
    D_8014F4D8 = (u8 *)0x801BE000;
    D_8014F4DC = (u8 *)0x801C2000;
    D_8014F4E0 = (u8 *)0x801C6000;
}

void album_anim_20_tables_set(void) {
    album_anim_20_ptrs = (u8 *)0x801EA87C;
    D_8014F4E8 = (u8 *)0x801EA89C;
    D_8014F4EC = (u8 *)0x801EA940;
    D_8014F4F0 = *(s16 *)0x801EA968;
    D_8014F4F4 = (u8 *)0x801B0000;
    D_8014F4F8 = (u8 *)0x801B2000;
    D_8014F4FC = (u8 *)0x801B6000;
    D_8014F500 = (u8 *)0x801BA000;
    D_8014F504 = (u8 *)0x801BE000;
    D_8014F508 = (u8 *)0x801C2000;
    D_8014F50C = (u8 *)0x801C6000;
}

void album_anim_43_tables_set(void) {
    album_anim_43_ptrs = (u8 *)0x801D21BC;
    D_8014F514 = (u8 *)0x801D21C4;
    D_8014F518 = (u8 *)0x801D2204;
    D_8014F51C = *(s16 *)0x801D221C;
    D_8014F520 = (u8 *)0x801B0000;
    D_8014F524 = (u8 *)0x801B2000;
    D_8014F528 = (u8 *)0x801B6000;
    D_8014F52C = (u8 *)0x801BA000;
    D_8014F530 = (u8 *)0x801BE000;
    D_8014F534 = (u8 *)0x801C2000;
    D_8014F538 = (u8 *)0x801C6000;
}

void album_anim_45_tables_set(void) {
    album_anim_45_ptrs = (u8 *)0x801CE0F4;
    D_8014F540 = (u8 *)0x801CE0F8;
    D_8014F544 = (u8 *)0x801CE118;
    D_8014F548 = *(s16 *)0x801CE12C;
    D_8014F54C = (u8 *)0x801B0000;
    D_8014F550 = (u8 *)0x801B2000;
    D_8014F554 = (u8 *)0x801B6000;
    D_8014F558 = (u8 *)0x801BA000;
    D_8014F55C = (u8 *)0x801BE000;
    D_8014F560 = (u8 *)0x801C2000;
    D_8014F564 = (u8 *)0x801C6000;
}

void album_anim_46_tables_set(void) {
    album_anim_46_ptrs = (u8 *)0x801CE0F4;
    D_8014F56C = (u8 *)0x801CE0F8;
    D_8014F570 = (u8 *)0x801CE118;
    D_8014F574 = *(s16 *)0x801CE12C;
    D_8014F578 = (u8 *)0x801B0000;
    D_8014F57C = (u8 *)0x801B2000;
    D_8014F580 = (u8 *)0x801B6000;
    D_8014F584 = (u8 *)0x801BA000;
    D_8014F588 = (u8 *)0x801BE000;
    D_8014F58C = (u8 *)0x801C2000;
    D_8014F590 = (u8 *)0x801C6000;
}

void album_anim_47_tables_set(void) {
    album_anim_47_ptrs = (u8 *)0x801CE0F4;
    D_8014F598 = (u8 *)0x801CE0F8;
    D_8014F59C = (u8 *)0x801CE118;
    D_8014F5A0 = *(s16 *)0x801CE12C;
    D_8014F5A4 = (u8 *)0x801B0000;
    D_8014F5A8 = (u8 *)0x801B2000;
    D_8014F5AC = (u8 *)0x801B6000;
    D_8014F5B0 = (u8 *)0x801BA000;
    D_8014F5B4 = (u8 *)0x801BE000;
    D_8014F5B8 = (u8 *)0x801C2000;
    D_8014F5BC = (u8 *)0x801C6000;
}

void album_anim_48_tables_set(void) {
    album_anim_48_ptrs = (u8 *)0x801D2470;
    D_8014F5C4 = (u8 *)0x801D2478;
    D_8014F5C8 = (u8 *)0x801D24B8;
    D_8014F5CC = *(s16 *)0x801D24D0;
    D_8014F5D0 = (u8 *)0x801B0000;
    D_8014F5D4 = (u8 *)0x801B2000;
    D_8014F5D8 = (u8 *)0x801B6000;
    D_8014F5DC = (u8 *)0x801BA000;
    D_8014F5E0 = (u8 *)0x801BE000;
    D_8014F5E4 = (u8 *)0x801C2000;
    D_8014F5E8 = (u8 *)0x801C6000;
}

void album_anim_49_tables_set(void) {
    album_anim_49_ptrs = (u8 *)0x801D2600;
    D_8014F5F0 = (u8 *)0x801D2608;
    D_8014F5F4 = (u8 *)0x801D2674;
    D_8014F5F8 = *(s16 *)0x801D268C;
    D_8014F5FC = (u8 *)0x801B0000;
    D_8014F600 = (u8 *)0x801B2000;
    D_8014F604 = (u8 *)0x801B6000;
    D_8014F608 = (u8 *)0x801BA000;
    D_8014F60C = (u8 *)0x801BE000;
    D_8014F610 = (u8 *)0x801C2000;
    D_8014F614 = (u8 *)0x801C6000;
}

void album_anim_unused_a_tables_set(void) {
    album_anim_unused_a_ptrs = (u8 *)0x801B0000;
    D_8014F61C = (u8 *)0x801B2000;
    D_8014F620 = (u8 *)0x801B6000;
    D_8014F624 = (u8 *)0x801BA000;
    D_8014F628 = (u8 *)0x801BE000;
    D_8014F62C = (u8 *)0x801C2000;
    D_8014F630 = (u8 *)0x801C6000;
}

void album_anim_4d_tables_set(void) {
    album_anim_4d_ptrs = (u8 *)0x801CE150;
    D_8014F638 = (u8 *)0x801CE154;
    D_8014F63C = (u8 *)0x801CE1A4;
    D_8014F640 = *(s16 *)0x801CE1BC;
    D_8014F644 = (u8 *)0x801B0000;
    D_8014F648 = (u8 *)0x801B2000;
    D_8014F64C = (u8 *)0x801B6000;
    D_8014F650 = (u8 *)0x801BA000;
    D_8014F654 = (u8 *)0x801BE000;
    D_8014F658 = (u8 *)0x801C2000;
    D_8014F65C = (u8 *)0x801C6000;
}

void album_anim_unused_b_tables_set(void) {
    album_anim_unused_b_ptrs = (u8 *)0x801B93F0;
    D_8014F664 = (u8 *)0x801B9360;
    D_8014F668 = (u8 *)0x801B93D8;
    D_8014F66C = *(s16 *)0x801B93F8;
    D_8014F670 = (u8 *)0x80197000;
    D_8014F674 = (u8 *)0x80199000;
    D_8014F678 = (u8 *)0x8019D000;
    D_8014F67C = (u8 *)0x801A1000;
    D_8014F680 = (u8 *)0x801A5000;
    D_8014F684 = (u8 *)0x801A9000;
    D_8014F688 = (u8 *)0x801AD000;
}

void album_anim_unused_c_tables_set(void) {
    album_anim_unused_c_ptrs = (u8 *)0x801B523C;
    D_8014F690 = (u8 *)0x801B518C;
    D_8014F694 = (u8 *)0x801B51DC;
    D_8014F698 = *(s16 *)0x801B5240;
    D_8014F69C = (u8 *)0x80197000;
    D_8014F6A0 = (u8 *)0x80199000;
    D_8014F6A4 = (u8 *)0x8019D000;
    D_8014F6A8 = (u8 *)0x801A1000;
    D_8014F6AC = (u8 *)0x801A5000;
    D_8014F6B0 = (u8 *)0x801A9000;
    D_8014F6B4 = (u8 *)0x801AD000;
}

/* FAKE: s32 return view preserves the original epilogue allocation; callers ignore the result (T-24012). */
s32 album_anim_show_dispatch(void) {
    if (album_event_category == 2) {
        switch (album_event_scene) {
        case 0x0:
            album_anim_00_show();
            break;
        case 0x2:
            album_anim_02_show();
            break;
        case 0x3:
            album_anim_03_show();
            break;
        case 0x4:
            album_anim_04_show();
            break;
        case 0x5:
            album_anim_05_show();
            break;
        case 0x6:
            album_anim_06_show();
            break;
        case 0x7:
            album_anim_07_show();
            break;
        case 0x8:
            album_anim_08_show();
            break;
        case 0x9:
            album_anim_09_show();
            break;
        case 0xA:
            album_anim_0a_show();
            break;
        case 0xB:
            album_anim_0b_show();
            break;
        case 0xC:
            album_anim_0c_show();
            break;
        case 0xD:
            album_anim_0d_show();
            break;
        case 0xE:
            album_anim_0e_show();
            break;
        case 0xF:
            album_anim_0f_show();
            break;
        case 0x10:
            album_anim_10_show();
            break;
        case 0x11:
            album_anim_11_show();
            break;
        case 0x12:
            album_anim_12_show();
            break;
        case 0x13:
            album_anim_13_show();
            break;
        case 0x15:
            album_anim_15_show();
            break;
        case 0x16:
            album_anim_16_show();
            break;
        case 0x17:
            album_anim_17_show();
            break;
        case 0x18:
            album_anim_18_show();
            break;
        case 0x19:
            album_anim_19_show();
            break;
        case 0x1A:
            album_anim_1a_show();
            break;
        case 0x1B:
            album_anim_1b_show();
            break;
        case 0x1C:
            album_anim_1c_show();
            break;
        case 0x1D:
            album_anim_1d_show();
            break;
        case 0x1E:
            album_anim_1e_show();
            break;
        case 0x1F:
            album_anim_1f_show();
            break;
        case 0x20:
            album_anim_20_show();
            break;
        case 0x21:
            album_anim_21_show();
            break;
        case 0x23:
            album_anim_23_show();
            break;
        case 0x24:
            album_anim_24_show();
            break;
        case 0x25:
            album_anim_25_show();
            break;
        case 0x26:
            album_anim_26_show();
            break;
        case 0x27:
            album_anim_27_show();
            break;
        case 0x28:
            album_anim_28_show();
            break;
        case 0x29:
            album_anim_29_show();
            break;
        case 0x2A:
            album_anim_2a_show();
            break;
        case 0x2B:
            album_anim_2b_show();
            break;
        case 0x2C:
            album_anim_2c_show();
            break;
        case 0x2D:
            album_anim_2d_show();
            break;
        case 0x2E:
            album_anim_2e_show();
            break;
        case 0x2F:
            album_anim_2f_show();
            break;
        case 0x30:
            album_anim_30_show();
            break;
        case 0x31:
            album_anim_31_show();
            break;
        case 0x32:
            album_anim_32_show();
            break;
        case 0x34:
            album_anim_34_show();
            break;
        case 0x36:
            album_anim_36_show();
            break;
        case 0x37:
            album_anim_37_show();
            break;
        case 0x38:
            album_anim_38_show();
            break;
        case 0x39:
            album_anim_39_show();
            break;
        case 0x3A:
            album_anim_3a_show();
            break;
        case 0x3B:
            album_anim_3b_show();
            break;
        case 0x3C:
            album_anim_3c_show();
            break;
        case 0x3E:
            album_anim_3e_show();
            break;
        case 0x3F:
            album_anim_3f_show();
            break;
        case 0x40:
            album_anim_40_show();
            break;
        case 0x41:
            album_anim_41_show();
            break;
        case 0x42:
            album_anim_42_show();
            break;
        case 0x43:
            album_anim_43_show();
            break;
        case 0x45:
            album_anim_45_show();
            break;
        case 0x46:
            album_anim_46_show();
            break;
        case 0x47:
            album_anim_47_show();
            break;
        case 0x48:
            album_anim_48_show();
            break;
        case 0x49:
            album_anim_49_show();
            break;
        case 0x4B:
            album_anim_4b_show();
            break;
        case 0x4D:
            album_anim_4d_show();
            break;
        case 0x50:
            album_anim_50_show();
            break;
        }
    } else if (album_event_category == 8) {
        switch (album_event_scene) {
        case 0xE:
            album_special_anim_0e_show();
            break;
        case 0x46:
            album_special_anim_46_show();
            break;
        }
    }
}


void album_anim_images_setup(u8 *arg0, u8 *arg1, u8 *arg2, u8 *arg3, u8 *arg4, u8 *arg5) {
    u8 *buf[6];
    s32 pad[2]; /* FAKE: unused 8-byte local reproduces the original frame (buf at 0x48); real source unknown. T-2040 */

    buf[0] = arg0;
    buf[1] = arg1;
    buf[2] = arg2;
    buf[3] = arg3;
    buf[4] = arg4;
    buf[5] = arg5;
    prim_bg_slot_setup(buf, 0, 6, 3, 2, 0x1F1, -0xA0, -0x65, 0xC, 5);
}

void album_anim_00_show(void) {
    album_anim_00_tables_set();
    load_palette(D_8014EB88, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014EB90, D_8014EB94, D_8014EB98, D_8014EB9C, D_8014EBA0, D_8014EBA4);
    scene_anim_show(D_8014EB7C, D_8014EB80, album_anim_00_ptrs, D_8014EB84);
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->brightness = 0x80;
    ((Actor *)actor_pool_96)->y = -0x65;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].brightness = 0x80;
    ((Actor *)actor_pool_96)[1].y = -0x65;
}

void album_anim_02_show(void) {
    album_anim_02_tables_set();
    load_palette(D_8014EBBC, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014EBC0, D_8014EBC4, D_8014EBC8, D_8014EBCC, D_8014EBD0, D_8014EBD4);
    scene_anim_show(D_8014EBB0, D_8014EBB4, album_anim_02_ptrs, D_8014EBB8);
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->brightness = 0x80;
    ((Actor *)actor_pool_96)->y = -0x65;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].brightness = 0x80;
    ((Actor *)actor_pool_96)[1].y = -0x65;
}

void album_anim_03_show(void) {
    album_anim_03_tables_set();
    load_palette(D_8014EBE8, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014EBEC, D_8014EBF0, D_8014EBF4, D_8014EBF8, D_8014EBFC, D_8014EC00);
    scene_anim_show(D_8014EBDC, D_8014EBE0, album_anim_03_ptrs, D_8014EBE4);
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->brightness = 0x80;
    ((Actor *)actor_pool_96)->y = -0x65;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].brightness = 0x80;
    ((Actor *)actor_pool_96)[1].y = -0x65;
}

void album_anim_04_show(void) {
    album_anim_04_tables_set();
    load_palette(D_8014EC14, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014EC18, D_8014EC1C, D_8014EC20, D_8014EC24, D_8014EC28, D_8014EC2C);
    scene_anim_show(D_8014EC08, D_8014EC0C, album_anim_04_ptrs, D_8014EC10);
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->brightness = 0x80;
    ((Actor *)actor_pool_96)->y = -0x65;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].brightness = 0x80;
    ((Actor *)actor_pool_96)[1].y = -0x65;
}

void album_anim_05_show(void) {
    album_anim_05_tables_set();
    load_palette(album_anim_05_ptrs, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014EC34, D_8014EC38, D_8014EC3C, D_8014EC40, D_8014EC44, D_8014EC48);
    scene_anim_show(0, 0, 0, 0);
}

void album_anim_06_show(void) {
    album_anim_06_tables_set();
    load_palette(D_8014EC5C, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014EC64, D_8014EC68, D_8014EC6C, D_8014EC70, D_8014EC74, D_8014EC78);
    scene_anim_show(D_8014EC50, D_8014EC54, album_anim_06_ptrs, D_8014EC58);
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->brightness = 0x80;
    ((Actor *)actor_pool_96)->y = -0x65;
    ((Actor *)actor_pool_96)->unk_04 |= 8;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].brightness = 0x80;
    ((Actor *)actor_pool_96)[1].y = -0x65;
    ((Actor *)actor_pool_96)[1].unk_04 |= 8;
}

void album_anim_07_show(void) {
    album_anim_07_tables_set();
    load_palette(D_8014EC90, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014EC94, D_8014EC98, D_8014EC9C, D_8014ECA0, D_8014ECA4, D_8014ECA8);
    scene_anim_show(D_8014EC84, D_8014EC88, album_anim_07_ptrs, D_8014EC8C);
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->brightness = 0x80;
    ((Actor *)actor_pool_96)->y = -0x65;
    ((Actor *)actor_pool_96)->unk_04 |= 8;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].brightness = 0x80;
    ((Actor *)actor_pool_96)[1].y = -0x65;
    ((Actor *)actor_pool_96)[1].unk_04 |= 8;
}

void album_anim_08_show(void) {
    album_anim_08_tables_set();
    load_palette(D_8014F018, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014F01C, D_8014F020, D_8014F024, D_8014F028, D_8014F02C, D_8014F030);
    scene_anim_show(D_8014F00C, D_8014F010, album_anim_08_ptrs, D_8014F014);
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->brightness = 0x80;
    ((Actor *)actor_pool_96)->y = -0x65;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].brightness = 0x80;
    ((Actor *)actor_pool_96)[1].y = -0x65;
}

void album_anim_09_show(void) {
    album_anim_09_tables_set();
    load_palette(D_8014F044, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014F048, D_8014F04C, D_8014F050, D_8014F054, D_8014F058, D_8014F05C);
    scene_anim_show(D_8014F038, D_8014F03C, album_anim_09_ptrs, D_8014F040);
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->brightness = 0x80;
    ((Actor *)actor_pool_96)->y = -0x65;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].brightness = 0x80;
    ((Actor *)actor_pool_96)[1].y = -0x65;
}

void album_anim_0a_show(void) {
    album_anim_0a_tables_set();
    load_palette(D_8014F070, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014F074, D_8014F078, D_8014F07C, D_8014F080, D_8014F084, D_8014F088);
    scene_anim_show(D_8014F064, D_8014F068, album_anim_0a_ptrs, D_8014F06C);
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->brightness = 0x80;
    ((Actor *)actor_pool_96)->y = -0x65;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].brightness = 0x80;
    ((Actor *)actor_pool_96)[1].y = -0x65;
}

void album_anim_0b_show(void) {
    album_anim_0b_tables_set();
    load_palette(D_8014F09C, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014F0A0, D_8014F0A4, D_8014F0A8, D_8014F0AC, D_8014F0B0, D_8014F0B4);
    scene_anim_show(D_8014F090, D_8014F094, album_anim_0b_ptrs, D_8014F098);
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->brightness = 0x80;
    ((Actor *)actor_pool_96)->y = -0x65;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].brightness = 0x80;
    ((Actor *)actor_pool_96)[1].y = -0x65;
}

void album_anim_0c_show(void) {
    album_anim_0c_tables_set();
    load_palette(D_8014F0C8, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014F0CC, D_8014F0D0, D_8014F0D4, D_8014F0D8, D_8014F0DC, D_8014F0E0);
    scene_anim_show(D_8014F0BC, D_8014F0C0, album_anim_0c_ptrs, D_8014F0C4);
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->brightness = 0x80;
    ((Actor *)actor_pool_96)->y = -0x65;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].brightness = 0x80;
    ((Actor *)actor_pool_96)[1].y = -0x65;
}

void album_anim_0d_show(void) {
    album_anim_0c_tables_set();
    load_palette(D_8014F0C8, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014F0CC, D_8014F0D0, D_8014F0D4, D_8014F0D8, D_8014F0DC, D_8014F0E0);
    scene_anim_show(D_8014F0BC, D_8014F0C0, album_anim_0c_ptrs, D_8014F0C4);
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->brightness = 0x80;
    ((Actor *)actor_pool_96)->y = -0x65;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].brightness = 0x80;
    ((Actor *)actor_pool_96)[1].y = -0x65;
}

void album_anim_0e_show(void) {
    album_anim_0e_tables_set();
    load_palette(D_8014ECBC, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014ECC0, D_8014ECC4, D_8014ECC8, D_8014ECCC, D_8014ECD0, D_8014ECD4);
    scene_anim_show(0, 0, 0, 0);
}

void album_anim_0f_show(void) {
    album_anim_0f_tables_set();
    load_palette(album_anim_0f_ptrs, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014ECDC, D_8014ECE0, D_8014ECE4, D_8014ECE8, D_8014ECEC, D_8014ECF0);
    scene_anim_show(0, 0, 0, 0);
}

void album_anim_10_show(void) {
    album_anim_10_tables_set();
    load_palette(D_8014ED04, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014ED08, D_8014ED0C, D_8014ED10, D_8014ED14, D_8014ED18, D_8014ED1C);
    scene_anim_show(D_8014ECF8, D_8014ECFC, album_anim_10_ptrs, D_8014ED00);
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->brightness = 0x80;
    ((Actor *)actor_pool_96)->y = -0x65;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].brightness = 0x80;
    ((Actor *)actor_pool_96)[1].y = -0x65;
}

void album_anim_11_show(void) {
    album_anim_11_tables_set();
    load_palette(D_8014ED30, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014ED38, D_8014ED3C, D_8014ED40, D_8014ED44, D_8014ED48, D_8014ED4C);
    scene_anim_show(D_8014ED24, D_8014ED28, album_anim_11_ptrs, D_8014ED2C);
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->brightness = 0x80;
    ((Actor *)actor_pool_96)->y = -0x65;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].brightness = 0x80;
    ((Actor *)actor_pool_96)[1].y = -0x65;
}

void album_anim_12_show(void) {
    album_anim_12_tables_set();
    load_palette(D_8014ED64, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014ED68, D_8014ED6C, D_8014ED70, D_8014ED74, D_8014ED78, D_8014ED7C);
    scene_anim_show(D_8014ED58, D_8014ED5C, album_anim_12_ptrs, D_8014ED60);
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->brightness = 0x80;
    ((Actor *)actor_pool_96)->y = -0x65;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].brightness = 0x80;
    ((Actor *)actor_pool_96)[1].y = -0x65;
}


void album_anim_13_show(void) {
    album_anim_13_tables_set();
    load_palette(D_8014ED90, 0x11, 1, 2, 0);
    scene_anim_show(D_8014ED84, D_8014ED88, album_anim_13_ptrs, D_8014ED8C);
    ((Actor *)actor_pool_96)[1].unk_04 = 8;
    ((Actor *)actor_pool_96)->unk_04 = 8;
    ((Actor *)actor_pool_96)[1].unk_01 = 0xB;
    ((Actor *)actor_pool_96)->unk_01 = 0xB;
    ((Actor *)actor_pool_96)[2] = ((Actor *)actor_pool_96)[0];
    ((Actor *)actor_pool_96)[2].flags = 0x40;
    ((Actor *)actor_pool_96)[2].unk_18 = 0x14;
    ((Actor *)actor_pool_96)[2].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[2].brightness = 0x80;
    ((Actor *)actor_pool_96)[2].y = -0x65;
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->brightness = 0x80;
    ((Actor *)actor_pool_96)->y = -0x65;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].brightness = 0x80;
    ((Actor *)actor_pool_96)[1].y = -0x65;
}

void album_anim_15_show(void) {
    album_anim_15_tables_set();
    load_palette(D_8014EDC0, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014EDC4, D_8014EDC8, D_8014EDCC, D_8014EDD0, D_8014EDD4, D_8014EDD8);
    scene_anim_show(D_8014EDB4, D_8014EDB8, album_anim_15_ptrs, D_8014EDBC);
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->brightness = 0x80;
    ((Actor *)actor_pool_96)->y = -0x65;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].brightness = 0x80;
    ((Actor *)actor_pool_96)[1].y = -0x65;
}

void album_anim_16_show(void) {
    album_anim_16_tables_set();
    load_palette(D_8014EDEC, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014EDF0, D_8014EDF4, D_8014EDF8, D_8014EDFC, D_8014EE00, D_8014EE04);
    scene_anim_show(D_8014EDE0, D_8014EDE4, album_anim_16_ptrs, D_8014EDE8);
    ((Actor *)actor_pool_96)[1] = ((Actor *)actor_pool_96)[0];
    ((Actor *)actor_pool_96)[1].sequence_slot = 3;
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->brightness = 0x80;
    ((Actor *)actor_pool_96)->y = -0x65;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].brightness = 0x80;
    ((Actor *)actor_pool_96)[1].y = -0x65;
}

void album_anim_17_show(void) {
    album_anim_17_tables_set();
    load_palette(D_8014EE18, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014EE1C, D_8014EE20, D_8014EE24, D_8014EE28, D_8014EE2C, D_8014EE30);
    scene_anim_show(D_8014EE0C, D_8014EE10, album_anim_17_ptrs, D_8014EE14);
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->brightness = 0x80;
    ((Actor *)actor_pool_96)->y = -0x65;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].brightness = 0x80;
    ((Actor *)actor_pool_96)[1].y = -0x65;
    ((Actor *)actor_pool_96)[1].sequence_slot = 4;
}

void album_anim_18_show(void) {
    album_anim_18_tables_set();
    load_palette(D_8014EE44, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014EE48, D_8014EE4C, D_8014EE50, D_8014EE54, D_8014EE58, D_8014EE5C);
    scene_anim_show(D_8014EE38, D_8014EE3C, album_anim_18_ptrs, D_8014EE40);
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->brightness = 0x80;
    ((Actor *)actor_pool_96)->y = -0x65;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].brightness = 0x80;
    ((Actor *)actor_pool_96)[1].y = -0x65;
}

void album_anim_19_show(void) {
    album_anim_19_tables_set();
    load_palette(D_8014EE70, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014EE74, D_8014EE78, D_8014EE7C, D_8014EE80, D_8014EE84, D_8014EE88);
    scene_anim_show(D_8014EE64, D_8014EE68, album_anim_19_ptrs, D_8014EE6C);
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->brightness = 0x80;
    ((Actor *)actor_pool_96)->y = -0x65;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].brightness = 0x80;
    ((Actor *)actor_pool_96)[1].y = -0x65;
}

void album_anim_1a_show(void) {
    album_anim_1a_tables_set();
    load_palette(D_8014EE9C, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014EEA0, D_8014EEA4, D_8014EEA8, D_8014EEAC, D_8014EEB0, D_8014EEB4);
    scene_anim_show(D_8014EE90, D_8014EE94, album_anim_1a_ptrs, D_8014EE98);
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->brightness = 0x80;
    ((Actor *)actor_pool_96)->y = -0x65;
    ((Actor *)actor_pool_96)->unk_04 |= 8;
    ((Actor *)actor_pool_96)->sequence_slot = 4;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].brightness = 0x80;
    ((Actor *)actor_pool_96)[1].y = -0x65;
    ((Actor *)actor_pool_96)[1].unk_04 |= 8;
    ((Actor *)actor_pool_96)[2] = ((Actor *)actor_pool_96)[0];
    ((Actor *)actor_pool_96)[2].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[2].brightness = 0x80;
    ((Actor *)actor_pool_96)[2].y = -0x65;
    ((Actor *)actor_pool_96)[2].unk_04 |= 8;
    ((Actor *)actor_pool_96)[2].sequence_slot = 4;
}

void album_anim_1b_show(void) {
    album_anim_1b_tables_set();
    load_palette(D_8014F418, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014F41C, D_8014F420, D_8014F424, D_8014F428, D_8014F42C, D_8014F430);
    scene_anim_show(D_8014F40C, D_8014F410, album_anim_1b_ptrs, D_8014F414);
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->brightness = 0x80;
    ((Actor *)actor_pool_96)->y = -0x65;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].brightness = 0x80;
    ((Actor *)actor_pool_96)[1].y = -0x65;
}

void album_anim_1c_show(void) {
    album_anim_1c_tables_set();
    load_palette(D_8014F444, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014F448, D_8014F44C, D_8014F450, D_8014F454, D_8014F458, D_8014F45C);
    scene_anim_show(D_8014F438, D_8014F43C, album_anim_1c_ptrs, D_8014F440);
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->brightness = 0x80;
    ((Actor *)actor_pool_96)->y = -0x65;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].brightness = 0x80;
    ((Actor *)actor_pool_96)[1].y = -0x65;
}

void album_anim_1d_show(void) {
    album_anim_1d_tables_set();
    load_palette(D_8014F470, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014F474, D_8014F478, D_8014F47C, D_8014F480, D_8014F484, D_8014F488);
    scene_anim_show(D_8014F464, D_8014F468, album_anim_1d_ptrs, D_8014F46C);
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->brightness = 0x80;
    ((Actor *)actor_pool_96)->y = -0x65;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].brightness = 0x80;
    ((Actor *)actor_pool_96)[1].y = -0x65;
}

void album_anim_1e_show(void) {
    album_anim_1e_tables_set();
    load_palette(D_8014F49C, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014F4A0, D_8014F4A4, D_8014F4A8, D_8014F4AC, D_8014F4B0, D_8014F4B4);
    scene_anim_show(D_8014F490, D_8014F494, album_anim_1e_ptrs, D_8014F498);
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->brightness = 0x80;
    ((Actor *)actor_pool_96)->y = -0x65;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].brightness = 0x80;
    ((Actor *)actor_pool_96)[1].y = -0x65;
}

void album_anim_1f_show(void) {
    album_anim_1f_tables_set();
    load_palette(D_8014F4C8, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014F4CC, D_8014F4D0, D_8014F4D4, D_8014F4D8, D_8014F4DC, D_8014F4E0);
    scene_anim_show(D_8014F4BC, D_8014F4C0, album_anim_1f_ptrs, D_8014F4C4);
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->brightness = 0x80;
    ((Actor *)actor_pool_96)->y = -0x65;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].brightness = 0x80;
    ((Actor *)actor_pool_96)[1].y = -0x65;
}

void album_anim_20_show(void) {
    album_anim_20_tables_set();
    game_state.tpage_buf[21] = -1;
    game_state.tpage_buf[22] = -1;
    game_state.tpage_buf[23] = -1;
    game_state.tpage_buf[24] = -1;
    game_state.tpage_buf[25] = -1;
    game_state.tpage_buf[53] = -1;
    game_state.tpage_buf[54] = -1;
    game_state.tpage_buf[55] = -1;
    game_state.tpage_buf[56] = -1;
    game_state.tpage_buf[57] = -1;
    load_palette(D_8014F4F4, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014F4F8, D_8014F4FC, D_8014F500, D_8014F504, D_8014F508, D_8014F50C);
    scene_anim_show(D_8014F4E8, D_8014F4EC, album_anim_20_ptrs, D_8014F4F0);
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->brightness = 0x80;
    ((Actor *)actor_pool_96)->y = -0x65;
    ((Actor *)actor_pool_96)->unk_04 |= 8;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].brightness = 0x80;
    ((Actor *)actor_pool_96)[1].y = -0x65;
    ((Actor *)actor_pool_96)[1].unk_04 |= 8;
    ((Actor *)actor_pool_96)[2] = ((Actor *)actor_pool_96)[0];
    ((Actor *)actor_pool_96)[2].flags = 0x40;
    ((Actor *)actor_pool_96)[2].unk_18 = 0x28;
    ((Actor *)actor_pool_96)[2].unk_01 -= 1;
    ((Actor *)actor_pool_96)[2].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[2].brightness = 0x80;
    ((Actor *)actor_pool_96)[2].y = -0x65;
    ((Actor *)actor_pool_96)[2].unk_04 |= 8;
}

void album_anim_21_show(void) {
    album_anim_21_tables_set();
    load_palette(D_8014F13C, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014F140, D_8014F144, D_8014F148, D_8014F14C, D_8014F150, D_8014F154);
    scene_anim_show(D_8014F130, D_8014F134, album_anim_21_ptrs, D_8014F138);
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->brightness = 0x80;
    ((Actor *)actor_pool_96)->y = -0x65;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].brightness = 0x80;
    ((Actor *)actor_pool_96)[1].y = -0x65;
}

void album_anim_23_show(void) {
    album_anim_23_tables_set();
    load_palette(D_8014F168, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014F16C, D_8014F170, D_8014F174, D_8014F178, D_8014F17C, D_8014F180);
    scene_anim_show(D_8014F15C, D_8014F160, album_anim_23_ptrs, D_8014F164);
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->brightness = 0x80;
    ((Actor *)actor_pool_96)->y = -0x65;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].brightness = 0x80;
    ((Actor *)actor_pool_96)[1].y = -0x65;
}

void album_anim_24_show(void) {
    album_anim_24_tables_set();
    load_palette(D_8014F194, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014F198, D_8014F19C, D_8014F1A0, D_8014F1A4, D_8014F1A8, D_8014F1AC);
    scene_anim_show(D_8014F188, D_8014F18C, album_anim_24_ptrs, D_8014F190);
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->brightness = 0x80;
    ((Actor *)actor_pool_96)->y = -0x65;
    ((Actor *)actor_pool_96)->unk_04 |= 8;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].brightness = 0x80;
    ((Actor *)actor_pool_96)[1].y = -0x65;
    ((Actor *)actor_pool_96)[1].unk_04 |= 8;
}

void album_anim_25_show(void) {
    album_anim_25_tables_set();
    load_palette(D_8014F1C0, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014F1C4, D_8014F1C8, D_8014F1CC, D_8014F1D0, D_8014F1D4, D_8014F1D8);
    scene_anim_show(D_8014F1B4, D_8014F1B8, album_anim_25_ptrs, D_8014F1BC);
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->brightness = 0x80;
    ((Actor *)actor_pool_96)->y = -0x65;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].brightness = 0x80;
    ((Actor *)actor_pool_96)[1].y = -0x65;
}

void album_anim_26_show(void) {
    album_anim_26_tables_set();
    load_palette(album_anim_26_ptrs, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014F1E0, D_8014F1E4, D_8014F1E8, D_8014F1EC, D_8014F1F0, D_8014F1F4);
    scene_anim_show(0, 0, 0, 0);
}

void album_anim_27_show(void) {
    album_anim_27_tables_set();
    load_palette(D_8014F208, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014F20C, D_8014F210, D_8014F214, D_8014F218, D_8014F21C, D_8014F220);
    scene_anim_show(D_8014F1FC, D_8014F200, album_anim_27_ptrs, D_8014F204);
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->brightness = 0x80;
    ((Actor *)actor_pool_96)->y = -0x65;
    ((Actor *)actor_pool_96)->unk_04 |= 8;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].brightness = 0x80;
    ((Actor *)actor_pool_96)[1].y = -0x65;
    ((Actor *)actor_pool_96)[1].unk_04 |= 8;
}

void album_anim_28_show(void) {
    album_anim_28_tables_set();
    load_palette(D_8014F234, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014F238, D_8014F23C, D_8014F240, D_8014F244, D_8014F248, D_8014F24C);
    scene_anim_show(D_8014F228, D_8014F22C, album_anim_28_ptrs, D_8014F230);
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->brightness = 0x80;
    ((Actor *)actor_pool_96)->y = -0x65;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].brightness = 0x80;
    ((Actor *)actor_pool_96)[1].y = -0x65;
}

void album_anim_29_show(void) {
    album_anim_29_tables_set();
    load_palette(D_8014F260, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014F264, D_8014F268, D_8014F26C, D_8014F270, D_8014F274, D_8014F278);
    scene_anim_show(D_8014F254, D_8014F258, album_anim_29_ptrs, D_8014F25C);
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->brightness = 0x80;
    ((Actor *)actor_pool_96)->y = -0x65;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].brightness = 0x80;
    ((Actor *)actor_pool_96)[1].y = -0x65;
}

void album_anim_2a_show(void) {
    album_anim_2a_tables_set();
    load_palette(D_8014F28C, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014F290, D_8014F294, D_8014F298, D_8014F29C, D_8014F2A0, D_8014F2A4);
    scene_anim_show(D_8014F280, D_8014F284, album_anim_2a_ptrs, D_8014F288);
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->brightness = 0x80;
    ((Actor *)actor_pool_96)->y = -0x65;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].brightness = 0x80;
    ((Actor *)actor_pool_96)[1].y = -0x65;
}

void album_anim_2b_show(void) {
    album_anim_2b_tables_set();
    load_palette(D_8014F2B8, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014F2BC, D_8014F2C0, D_8014F2C4, D_8014F2C8, D_8014F2CC, D_8014F2D0);
    scene_anim_show(D_8014F2AC, D_8014F2B0, album_anim_2b_ptrs, D_8014F2B4);
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->brightness = 0x80;
    ((Actor *)actor_pool_96)->y = -0x65;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].brightness = 0x80;
    ((Actor *)actor_pool_96)[1].y = -0x65;
}

void album_anim_2c_show(void) {
    album_anim_2c_tables_set();
    load_palette(D_8014F2E4, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014F2E8, D_8014F2EC, D_8014F2F0, D_8014F2F4, D_8014F2F8, D_8014F2FC);
    scene_anim_show(D_8014F2D8, D_8014F2DC, album_anim_2c_ptrs, D_8014F2E0);
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->brightness = 0x80;
    ((Actor *)actor_pool_96)->y = -0x65;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].brightness = 0x80;
    ((Actor *)actor_pool_96)[1].y = -0x65;
}

void album_anim_2d_show(void) {
    album_anim_2c_tables_set();
    load_palette(D_8014F2E4, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014F2E8, D_8014F2EC, D_8014F2F0, D_8014F2F4, D_8014F2F8, D_8014F2FC);
    scene_anim_show(D_8014F2D8, D_8014F2DC, album_anim_2c_ptrs, D_8014F2E0);
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->brightness = 0x80;
    ((Actor *)actor_pool_96)->y = -0x65;
    ((Actor *)actor_pool_96)->unk_04 |= 8;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].brightness = 0x80;
    ((Actor *)actor_pool_96)[1].y = -0x65;
    ((Actor *)actor_pool_96)[1].unk_04 |= 8;
}

void album_anim_2e_show(void) {
    album_anim_2e_tables_set();
    load_palette(D_8014EEC8, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014EECC, D_8014EED0, D_8014EED4, D_8014EED8, D_8014EEDC, D_8014EEE0);
    scene_anim_show(D_8014EEBC, D_8014EEC0, album_anim_2e_ptrs, D_8014EEC4);
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->brightness = 0x80;
    ((Actor *)actor_pool_96)->y = -0x65;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].brightness = 0x80;
    ((Actor *)actor_pool_96)[1].y = -0x65;
}

void album_anim_2f_show(void) {
    album_anim_2f_tables_set();
    if (album_event_mood == 0) {
        load_palette(D_8014EEF4, 0x11, 1, 2, 0);
    } else if (album_event_mood == 1) {
        load_palette(D_8014EEF4, 0x11, 1, 2, 1);
    } else if (album_event_mood == 2) {
        load_palette(D_8014EEF4, 0x11, 1, 2, 2);
    }
    album_anim_images_setup(D_8014EEF8, D_8014EEFC, D_8014EF00, D_8014EF04, D_8014EF08, D_8014EF0C);
    scene_anim_show(D_8014EEE8, D_8014EEEC, album_anim_2f_ptrs, D_8014EEF0);
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->brightness = 0x80;
    ((Actor *)actor_pool_96)->y = -0x65;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].brightness = 0x80;
    ((Actor *)actor_pool_96)[1].y = -0x65;
}

void album_anim_30_show(void) {
    album_anim_30_tables_set();
    load_palette(D_8014EF20, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014EF24, D_8014EF28, D_8014EF2C, D_8014EF30, D_8014EF34, D_8014EF38);
    scene_anim_show(D_8014EF14, D_8014EF18, album_anim_30_ptrs, D_8014EF1C);
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->brightness = 0x80;
    ((Actor *)actor_pool_96)->y = -0x65;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].brightness = 0x80;
    ((Actor *)actor_pool_96)[1].y = -0x65;
}

void album_anim_31_show(void) {
    album_anim_31_tables_set();
    load_palette(D_8014EF4C, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014EF50, D_8014EF54, D_8014EF58, D_8014EF5C, D_8014EF60, D_8014EF64);
    scene_anim_show(D_8014EF40, D_8014EF44, album_anim_31_ptrs, D_8014EF48);
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->brightness = 0x80;
    ((Actor *)actor_pool_96)->y = -0x65;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].brightness = 0x80;
    ((Actor *)actor_pool_96)[1].y = -0x65;
}

void album_anim_32_show(void) {
    album_anim_32_tables_set();
    load_palette(D_8014EF78, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014EF7C, D_8014EF80, D_8014EF84, D_8014EF88, D_8014EF8C, D_8014EF90);
    scene_anim_show(D_8014EF6C, D_8014EF70, album_anim_32_ptrs, D_8014EF74);
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->brightness = 0x80;
    ((Actor *)actor_pool_96)->y = -0x65;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].brightness = 0x80;
    ((Actor *)actor_pool_96)[1].y = -0x65;
}

void album_anim_34_show(void) {
    album_anim_34_tables_set();
    load_palette(D_8014EFA4, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014EFA8, D_8014EFAC, D_8014EFB0, D_8014EFB4, D_8014EFB8, D_8014EFBC);
    scene_anim_show(D_8014EF98, D_8014EF9C, album_anim_34_ptrs, D_8014EFA0);
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->brightness = 0x80;
    ((Actor *)actor_pool_96)->y = -0x65;
    ((Actor *)actor_pool_96)->unk_04 |= 8;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].brightness = 0x80;
    ((Actor *)actor_pool_96)[1].y = -0x65;
    ((Actor *)actor_pool_96)[1].unk_04 |= 8;
}

void album_anim_36_show(void) {
    album_anim_36_tables_set();
    load_palette(D_8014EA80, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014EA84, D_8014EA88, D_8014EA8C, D_8014EA90, D_8014EA94, D_8014EA98);
    scene_anim_show(D_8014EA74, D_8014EA78, album_anim_36_ptrs, D_8014EA7C);
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->brightness = 0x80;
    ((Actor *)actor_pool_96)->y = -0x65;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].brightness = 0x80;
    ((Actor *)actor_pool_96)[1].y = -0x65;
}

void album_anim_37_show(void) {
    album_anim_37_tables_set();
    load_palette(D_8014EAAC, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014EAB0, D_8014EAB4, D_8014EAB8, D_8014EABC, D_8014EAC0, D_8014EAC4);
    scene_anim_show(D_8014EAA0, D_8014EAA4, album_anim_37_ptrs, D_8014EAA8);
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->brightness = 0x80;
    ((Actor *)actor_pool_96)->y = -0x65;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].brightness = 0x80;
    ((Actor *)actor_pool_96)[1].y = -0x65;
}

void album_anim_38_show(void) {
    album_anim_38_tables_set();
    load_palette(D_8014EAD8, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014EADC, D_8014EAE0, D_8014EAE4, D_8014EAE8, D_8014EAEC, D_8014EAF0);
    scene_anim_show(D_8014EACC, D_8014EAD0, album_anim_38_ptrs, D_8014EAD4);
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->brightness = 0x80;
    ((Actor *)actor_pool_96)->y = -0x65;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].brightness = 0x80;
    ((Actor *)actor_pool_96)[1].y = -0x65;
}

void album_anim_39_show(void) {
    album_anim_39_tables_set();
    load_palette(D_8014EB04, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014EB08, D_8014EB0C, D_8014EB10, D_8014EB14, D_8014EB18, D_8014EB1C);
    scene_anim_show(D_8014EAF8, D_8014EAFC, album_anim_39_ptrs, D_8014EB00);
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->brightness = 0x80;
    ((Actor *)actor_pool_96)->y = -0x65;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].brightness = 0x80;
    ((Actor *)actor_pool_96)[1].y = -0x65;
}

void album_anim_3a_show(void) {
    album_anim_3a_tables_set();
    load_palette(D_8014EB30, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014EB34, D_8014EB38, D_8014EB3C, D_8014EB40, D_8014EB44, D_8014EB48);
    scene_anim_show(D_8014EB24, D_8014EB28, album_anim_3a_ptrs, D_8014EB2C);
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->brightness = 0x80;
    ((Actor *)actor_pool_96)->y = -0x65;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].brightness = 0x80;
    ((Actor *)actor_pool_96)[1].y = -0x65;
}

void album_anim_3b_show(void) {
    album_anim_3b_tables_set();
    load_palette(D_8014EB5C, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014EB60, D_8014EB64, D_8014EB68, D_8014EB6C, D_8014EB70, D_8014EB74);
    scene_anim_show(D_8014EB50, D_8014EB54, album_anim_3b_ptrs, D_8014EB58);
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->brightness = 0x80;
    ((Actor *)actor_pool_96)->y = -0x65;
    ((Actor *)actor_pool_96)->unk_04 |= 8;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].brightness = 0x80;
    ((Actor *)actor_pool_96)[1].y = -0x65;
    ((Actor *)actor_pool_96)[1].unk_04 |= 8;
}

void album_anim_3c_show(void) {
    album_anim_3c_tables_set();
    load_palette(D_8014F310, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014F314, D_8014F318, D_8014F31C, D_8014F320, D_8014F324, D_8014F328);
    scene_anim_show(D_8014F304, D_8014F308, album_anim_3c_ptrs, D_8014F30C);
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->brightness = 0x80;
    ((Actor *)actor_pool_96)->y = -0x65;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].brightness = 0x80;
    ((Actor *)actor_pool_96)[1].y = -0x65;
}

void album_anim_3e_show(void) {
    album_anim_3e_tables_set();
    load_palette(D_8014F33C, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014F340, D_8014F344, D_8014F348, D_8014F34C, D_8014F350, D_8014F354);
    scene_anim_show(D_8014F330, D_8014F334, album_anim_3e_ptrs, D_8014F338);
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->brightness = 0x80;
    ((Actor *)actor_pool_96)->y = -0x65;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].brightness = 0x80;
    ((Actor *)actor_pool_96)[1].y = -0x65;
}

void album_anim_3f_show(void) {
    album_anim_3f_tables_set();
    load_palette(D_8014F368, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014F36C, D_8014F370, D_8014F374, D_8014F378, D_8014F37C, D_8014F380);
    scene_anim_show(D_8014F35C, D_8014F360, album_anim_3f_ptrs, D_8014F364);
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->brightness = 0x80;
    ((Actor *)actor_pool_96)->y = -0x65;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].brightness = 0x80;
    ((Actor *)actor_pool_96)[1].y = -0x65;
}

void album_anim_40_show(void) {
    album_anim_40_tables_set();
    load_palette(D_8014F394, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014F398, D_8014F39C, D_8014F3A0, D_8014F3A4, D_8014F3A8, D_8014F3AC);
    scene_anim_show(D_8014F388, D_8014F38C, album_anim_40_ptrs, D_8014F390);
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->brightness = 0x80;
    ((Actor *)actor_pool_96)->y = -0x65;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].brightness = 0x80;
    ((Actor *)actor_pool_96)[1].y = -0x65;
}

void album_anim_41_show(void) {
    album_anim_41_tables_set();
    load_palette(D_8014F3C0, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014F3C4, D_8014F3C8, D_8014F3CC, D_8014F3D0, D_8014F3D4, D_8014F3D8);
    scene_anim_show(D_8014F3B4, D_8014F3B8, album_anim_41_ptrs, D_8014F3BC);
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->brightness = 0x80;
    ((Actor *)actor_pool_96)->y = -0x65;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].brightness = 0x80;
    ((Actor *)actor_pool_96)[1].y = -0x65;
}

void album_anim_42_show(void) {
    album_anim_42_tables_set();
    load_palette(D_8014F3EC, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014F3F0, D_8014F3F4, D_8014F3F8, D_8014F3FC, D_8014F400, D_8014F404);
    scene_anim_show(D_8014F3E0, D_8014F3E4, album_anim_42_ptrs, D_8014F3E8);
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->brightness = 0x80;
    ((Actor *)actor_pool_96)->y = -0x65;
    ((Actor *)actor_pool_96)->unk_04 |= 8;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].brightness = 0x80;
    ((Actor *)actor_pool_96)[1].y = -0x65;
    ((Actor *)actor_pool_96)[1].unk_04 |= 8;
}

void album_anim_43_show(void) {
    album_anim_43_tables_set();
    load_palette(D_8014F520, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014F524, D_8014F528, D_8014F52C, D_8014F530, D_8014F534, D_8014F538);
    scene_anim_show(D_8014F514, D_8014F518, album_anim_43_ptrs, D_8014F51C);
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->brightness = 0x80;
    ((Actor *)actor_pool_96)->y = -0x65;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].brightness = 0x80;
    ((Actor *)actor_pool_96)[1].y = -0x65;
}

void album_anim_45_show(void) {
    album_anim_45_tables_set();
    load_palette(D_8014F54C, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014F550, D_8014F554, D_8014F558, D_8014F55C, D_8014F560, D_8014F564);
    scene_anim_show(D_8014F540, D_8014F544, album_anim_45_ptrs, D_8014F548);
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->brightness = 0x80;
    ((Actor *)actor_pool_96)->y = -0x65;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].brightness = 0x80;
    ((Actor *)actor_pool_96)[1].y = -0x65;
}

void album_anim_46_show(void) {
    album_anim_46_tables_set();
    load_palette(D_8014F578, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014F57C, D_8014F580, D_8014F584, D_8014F588, D_8014F58C, D_8014F590);
    scene_anim_show(D_8014F56C, D_8014F570, album_anim_46_ptrs, D_8014F574);
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->brightness = 0x80;
    ((Actor *)actor_pool_96)->y = -0x65;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].brightness = 0x80;
    ((Actor *)actor_pool_96)[1].y = -0x65;
}

void album_anim_47_show(void) {
    album_anim_47_tables_set();
    load_palette(D_8014F5A4, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014F5A8, D_8014F5AC, D_8014F5B0, D_8014F5B4, D_8014F5B8, D_8014F5BC);
    scene_anim_show(D_8014F598, D_8014F59C, album_anim_47_ptrs, D_8014F5A0);
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->brightness = 0x80;
    ((Actor *)actor_pool_96)->y = -0x65;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].brightness = 0x80;
    ((Actor *)actor_pool_96)[1].y = -0x65;
}

void album_anim_48_show(void) {
    album_anim_48_tables_set();
    load_palette(D_8014F5D0, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014F5D4, D_8014F5D8, D_8014F5DC, D_8014F5E0, D_8014F5E4, D_8014F5E8);
    scene_anim_show(D_8014F5C4, D_8014F5C8, album_anim_48_ptrs, D_8014F5CC);
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->brightness = 0x80;
    ((Actor *)actor_pool_96)->y = -0x65;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].brightness = 0x80;
    ((Actor *)actor_pool_96)[1].y = -0x65;
}

void album_anim_49_show(void) {
    album_anim_49_tables_set();
    load_palette(D_8014F5FC, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014F600, D_8014F604, D_8014F608, D_8014F60C, D_8014F610, D_8014F614);
    scene_anim_show(D_8014F5F0, D_8014F5F4, album_anim_49_ptrs, D_8014F5F8);
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->brightness = 0x80;
    ((Actor *)actor_pool_96)->y = -0x65;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].brightness = 0x80;
    ((Actor *)actor_pool_96)[1].y = -0x65;
}

void album_anim_4b_show(void) {
    load_palette(album_anim_unused_a_ptrs, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014F61C, D_8014F620, D_8014F624, D_8014F628, D_8014F62C, D_8014F630);
    scene_anim_show(0, 0, 0, 0);
}

void album_anim_4d_show(void) {
    album_anim_4d_tables_set();
    load_palette(D_8014F644, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014F648, D_8014F64C, D_8014F650, D_8014F654, D_8014F658, D_8014F65C);
    scene_anim_show(D_8014F638, D_8014F63C, album_anim_4d_ptrs, D_8014F640);
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->brightness = 0x80;
    ((Actor *)actor_pool_96)->y = -0x65;
    ((Actor *)actor_pool_96)->unk_04 |= 8;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].brightness = 0x80;
    ((Actor *)actor_pool_96)[1].y = -0x65;
    ((Actor *)actor_pool_96)[1].unk_04 |= 8;
}

void album_anim_50_show(void) {
    album_anim_50_tables_set();
    load_palette(album_anim_50_ptrs, 0x11, 1, 2, 0);
    album_anim_images_setup(D_8014ED98, D_8014ED9C, D_8014EDA0, D_8014EDA4, D_8014EDA8, D_8014EDAC);
    scene_anim_show(0, 0, 0, 0);
}

void album_special_anim_0e_show(void) {
    album_special_anim_0e_tables_set();
    if (album_event_month >= 6 && album_event_month < 0xA) {
        load_palette(D_8014F0F4, 0x11, 1, 2, 0);
        album_anim_images_setup(D_8014F0FC, D_8014F100, D_8014F104, D_8014F108, D_8014F10C, D_8014F110);
        scene_anim_show(D_8014F0E8, D_8014F0EC, album_special_anim_0e_ptrs, D_8014F0F0);
    } else {
        load_palette(D_8014F0F8, 0x11, 1, 2, 0);
        album_anim_images_setup(D_8014F114, D_8014F118, D_8014F11C, D_8014F120, D_8014F124, D_8014F128);
        scene_anim_show(D_8014F0E8, D_8014F0EC, album_special_anim_0e_ptrs, D_8014F0F0);
    }
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->brightness = 0x80;
    ((Actor *)actor_pool_96)->y = -0x65;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].brightness = 0x80;
    ((Actor *)actor_pool_96)[1].y = -0x65;
}

void album_special_anim_46_show(void) {
    album_special_anim_46_tables_set();
    if (album_event_month >= 6 && album_event_month < 0xA) {
        load_palette(D_8014EFD0, 0x11, 1, 2, 0);
        album_anim_images_setup(D_8014EFD8, D_8014EFDC, D_8014EFE0, D_8014EFE4, D_8014EFE8, D_8014EFEC);
    } else {
        load_palette(D_8014EFD4, 0x11, 1, 2, 0);
        album_anim_images_setup(D_8014EFF0, D_8014EFF4, D_8014EFF8, D_8014EFFC, D_8014F000, D_8014F004);
    }
    scene_anim_show(D_8014EFC4, D_8014EFC8, album_special_anim_46_ptrs, D_8014EFCC);
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->brightness = 0x80;
    ((Actor *)actor_pool_96)->y = -0x65;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].brightness = 0x80;
    ((Actor *)actor_pool_96)[1].y = -0x65;
}
