/* debug_hud_palette.c: src/main/8004AD60.c; T-27059: object [0x8004AD60,0x8004C3F0); debug-number overlay and level control plus color-bar and sepia palette helpers; retain the mixed original object (prior h-graphics candidate). Original object 0x8004AD60-0x8004C3F0 (config/objects/main.txt). */
#include "common.h"
#include "game.h"
#include "psx_types.h"

void debug_hud_level_step(s32 arg0) {
    if ((arg0 & 0xFF) == 0) {
        debug_hud_level = 0;
    } else {
        debug_hud_level = debug_hud_level - 1;
    }
    if ((u32)debug_hud_level >= 9U) {
        debug_hud_level = 8;
    }
}

void debug_hud_level_set(u8 arg0) {
    debug_hud_level = arg0;
    if ((u32)arg0 >= 9) {
        debug_hud_level = 8;
    }
}

u8 debug_hud_level_get(void) {
    return debug_hud_level;
}

void debug_font_load(void) {
    if (debug_hud_enabled != 0) {
        LoadSquare(0x2C0, 0x1E0, 0x40, 0x20, (void *)0x801F0020);
    }
}

/* FAKE: debug_print_int_color takes four arguments and the original leaves a0-a2 untouched; passing the
 * (unused) incoming a0-a2 through reproduces that, real source unknown. T-0800 */
void draw_debug_number(s32 a0, s32 a1, s32 a2) {
    if (debug_hud_enabled != 0) {
        debug_print_int_color(a0, a1, a2, 0xF);
    }
}

/* tpx and tpy are declared locals so that the allocator spills the later one (tpy, sp+0x5C) and keeps tpx in
 * $fp, as the original does; as bare expressions it spills the other. T-17013 */
void debug_print_int_color(s32 a0, s32 a1, s32 a2, s32 a3) {
    SPRT_8 *p;
    s32 n;
    s32 v;
    s32 tpx;
    s32 tpy;

    n = 0;
    if (debug_hud_level >= 4 && debug_hud_enabled != 0) {
        v = abs(a0);
        /* Both v >= 0 tests (here and at the loop end) are always true, but the original has the
         * bltz/bgez branches, so they stay. T-17013 */
        if (v >= 0) {
            tpx = (a3 & 8) * 4 + 0x100;
            tpy = (a3 & 7) + 0x1F8;
            do {
                p = (SPRT_8 *)GetWorkBase(0x10, draw_buffer);
                func_8009F07C(p);
                SetSemiTrans(p, 0);
                p->r0 = 0x80;
                p->g0 = 0x80;
                p->b0 = 0x80;
                p->x0 = game_state.draw_origin_x[draw_buffer] + a1 - n * 5;
                p->y0 = game_state.draw_origin_y[draw_buffer] + a2;
                p->u0 = debug_font_uv[(v % 10) * 2];
                p->v0 = debug_font_uv[(v % 10) * 2 + 1];
                p->clut = func_8009ED74(tpx, tpy);
                AddPrim(draw_ot_tags + (draw_buffer << 10) + 4, p);
                if (v < 10) {
                    n++;
                    break;
                }
                v = v / 10;
                n++;
            } while (v >= 0);
        }
        if (a0 < 0) {
            tpx = (a3 & 8) * 4 + 0x100;
            tpy = (a3 & 7) + 0x1F8;
            p = (SPRT_8 *)GetWorkBase(0x10, draw_buffer);
            func_8009F07C(p);
            SetSemiTrans(p, 0);
            p->r0 = 0x80;
            p->g0 = 0x80;
            p->b0 = 0x80;
            p->x0 = game_state.draw_origin_x[draw_buffer] + a1 - n * 5;
            p->y0 = game_state.draw_origin_y[draw_buffer] + a2;
            p->u0 = debug_font_uv[0x12] + 8;
            p->v0 = debug_font_uv[0x13];
            p->clut = func_8009ED74(tpx, tpy);
            AddPrim(draw_ot_tags + (draw_buffer << 10) + 4, p);
        }
        dtd_on_tpage(0x2C0, 0x100, 1, 0, 0);
    }
}

void debug_print_hex(s32 arg0, s32 arg1, s32 arg2) {
    u8 *p;
    s32 n;
    u32 x;
    u8 *e;

    n = 0;
    if (debug_hud_level >= 4 && debug_hud_enabled != 0) {
        x = arg0;
        for (;;) {
            p = (u8 *)GetWorkBase(0x10, draw_buffer);
            func_8009F07C(p);
            SetSemiTrans(p, 0);
            p[4] = 0x80;
            p[5] = 0x80;
            p[6] = 0x80;
            *(s16 *)(p + 8) = game_state.draw_origin_x[draw_buffer] + arg1 - n * 6;
            e = debug_font_uv + (x & 0xF) * 2;
            *(s16 *)(p + 0xA) = game_state.draw_origin_y[draw_buffer] + arg2;
            p[0xC] = e[0];
            p[0xD] = e[1];
            *(s16 *)(p + 0xE) = func_8009ED74(0x100, 0x1FF);
            AddPrim(draw_ot_tags + (draw_buffer << 10) + 4, p);
            if (x < 0x10) {
                dtd_on_tpage(0x2C0, 0x100, 1, 0, 0);
                break;
            }
            x = x >> 4;
            n++;
        }
    }
}

void debug_print_string(s32 arg0, s32 arg1, s32 arg2) {
    debug_print_string_color(arg0, arg1, arg2, 0xF);
}

s32 debug_print_string_color(s32 a0, s32 a1, s32 a2, s32 a3) {
    SPRT *p;
    s32 x;
    u8 c;
    s32 flag;
    u8 *ptr;
    s32 tpx;
    s32 tpy;
    /* FAKE: unused local pads stack frame to 0x88 and places spilled tpy at sp+0x5C. T-24123 */
    s32 pad;

    if (debug_hud_level < 4) {
        return 0;
    }
    if (debug_hud_enabled == 0) {
        return 0;
    }
    ptr = (u8 *)a0;
    x = a1;
    while (*ptr != 0) {
        c = *ptr & 0x1F;
        flag = *ptr & 0x20;
        tpx = (a3 & 8) * 2 + 0x100;
        tpy = (a3 & 7) + 0x1F8;
        p = (SPRT *)GetWorkBase(0x14, draw_buffer);
        func_8009F0A4(p);
        SetSemiTrans(p, 0);
        p->r0 = 0x80;
        p->g0 = 0x80;
        p->b0 = 0x80;
        p->x0 = game_state.draw_origin_x[draw_buffer] + x;
        p->y0 = game_state.draw_origin_y[draw_buffer] + a2;
        if (flag != 0) {
            x += 6;
        } else {
            x += 8;
        }
        if (flag != 0) {
            p->u0 = debug_font_uv[0x14] + c * 8 - 8;
            p->v0 = debug_font_uv[0x15];
        } else {
            p->u0 = debug_font_uv[0x14] + c * 8 - 8;
            p->v0 = debug_font_uv[0x15] - 0x10;
        }
        if (flag != 0) {
            setWH(p, 6, 0xC);
        } else {
            setWH(p, 8, 8);
        }
        p->clut = func_8009ED74(tpx, tpy);
        AddPrim(draw_ot_tags + (draw_buffer << 10) + 4, p);
        ptr++;
    }
    dtd_on_tpage(0x2C0, 0x100, 1, 0, 0);
    return 0;
}

/* Two 5-5-5 colours per word (CLUT entries): read as u32 fields, written as u16 fields. */
typedef struct {
    u32 r0 : 5;
    u32 g0 : 5;
    u32 b0 : 5;
    u32 x0 : 1;
    u32 r1 : 5;
    u32 g1 : 5;
    u32 b1 : 5;
    u32 x1 : 1;
} PixelPairIn;

typedef struct {
    u16 r0 : 5;
    u16 g0 : 5;
    u16 b0 : 5;
    u16 x0 : 1;
    u16 r1 : 5;
    u16 g1 : 5;
    u16 b1 : 5;
    u16 x1 : 1;
} PixelPairOut;

void color_palette_to_gray(arg0, arg1)
PixelPairIn *arg0;
PixelPairOut *arg1;
{
    PixelPairIn *src;
    PixelPairOut *dst;
    s32 i;
    s32 lum;
    s32 t;

    src = arg0;
    dst = arg1;
    for (i = 0; i != 0x80; i++) {
        lum = src->r0 * 0x1E + src->g0 * 0x3B + src->b0 * 0xB;
        if (lum != 0 && lum < 0x64) {
            lum = 0x64;
        }
        dst->r0 = t = lum / 100;
        dst->g0 = t;
        dst->b0 = t;
        lum = src->r1 * 0x1E + src->g1 * 0x3B + src->b1 * 0xB;
        if (lum != 0 && lum < 0x64) {
            lum = 0x64;
        }
        dst->r1 = t = lum / 100;
        dst->g1 = t;
        dst->b1 = t;
        src++;
        dst++;
    }
}

void make_color_bar(void *arg0, s32 flag) {
    /* T-25140: every gradient value comes from the pair index (k * 2, k * 2 - 1). uopt
     * strength-reduces them and the row pointer itself; separate i/j counters or a walked
     * pointer give the wrong register order. */
    s32 k;
    PixelPairOut *p;

    p = arg0;
    for (k = 0; k < 16; k++) {
        p[k].r1 = k * 2;
        p[k + 16].r1 = k * 2;
        p[k + 32].r1 = 0;
        p[k + 48].r1 = 0;
        p[k + 64].r1 = k * 2;
        p[k + 80].r1 = k * 2;
        p[k + 96].r1 = 0;
        p[k + 112].r1 = 0;
        p[k].r0 = k * 2 - 1;
        p[k + 16].r0 = k * 2 - 1;
        p[k + 32].r0 = 0;
        p[k + 48].r0 = 0;
        p[k + 64].r0 = k * 2 - 1;
        p[k + 80].r0 = k * 2 - 1;
        p[k + 96].r0 = 0;
        p[k + 112].r0 = 0;
    }
    for (k = 0; k < 16; k++) {
        p[k].g1 = k * 2;
        p[k + 16].g1 = k * 2;
        p[k + 32].g1 = k * 2;
        p[k + 48].g1 = k * 2;
        p[k + 64].g1 = 0;
        p[k + 80].g1 = 0;
        p[k + 96].g1 = 0;
        p[k + 112].g1 = 0;
        p[k].g0 = k * 2 - 1;
        p[k + 16].g0 = k * 2 - 1;
        p[k + 32].g0 = k * 2 - 1;
        p[k + 48].g0 = k * 2 - 1;
        p[k + 64].g0 = 0;
        p[k + 80].g0 = 0;
        p[k + 96].g0 = 0;
        p[k + 112].g0 = 0;
    }
    for (k = 0; k < 16; k++) {
        p[k].b1 = k * 2;
        p[k + 16].b1 = 0;
        p[k + 32].b1 = k * 2;
        p[k + 48].b1 = 0;
        p[k + 64].b1 = k * 2;
        p[k + 80].b1 = 0;
        p[k + 96].b1 = k * 2;
        p[k + 112].b1 = 0;
        p[k].b0 = k * 2 - 1;
        p[k + 16].b0 = 0;
        p[k + 32].b0 = k * 2 - 1;
        p[k + 48].b0 = 0;
        p[k + 64].b0 = k * 2 - 1;
        p[k + 80].b0 = 0;
        p[k + 96].b0 = k * 2 - 1;
        p[k + 112].b0 = 0;
    }
    if (flag) {
        for (k = 0; k != 128; k++) {
            p[k].x1 = 1;
            p[k].x0 = 1;
        }
    } else {
        for (k = 0; k != 128; k++) {
            p[k].x1 = 0;
            p[k].x0 = 0;
        }
    }
}

void make_color_bar16(void *arg0, u32 start, u32 end, s32 count, s32 flag) {
    /* T-24501: declaration order preserves the interpolation spill homes. */
    s32 i;
    PixelPairOut *p;
    s32 sr;
    s32 er;
    s32 sg;
    s32 eg;
    s32 sb;
    s32 eb;

    sr = (start >> 16) & 255;
    sg = (start >> 8) & 255;
    sb = start & 255;
    er = (end >> 16) & 255;
    eg = (end >> 8) & 255;
    eb = end & 255;
    p = arg0;
    i = 0;
    if ((count / 2) > 0) {
        do {
            p[i].r1 = ((s32)((count - (i * 2 + 1)) * er + (i * 2 + 1) * sr) / count) / 8;
            p[i].r0 = ((s32)((count - i * 2) * er + i * 2 * sr) / count) / 8;
            p[i].g1 = ((s32)((count - (i * 2 + 1)) * eg + (i * 2 + 1) * sg) / count) / 8;
            p[i].g0 = ((s32)((count - i * 2) * eg + i * 2 * sg) / count) / 8;
            p[i].b1 = ((s32)((count - (i * 2 + 1)) * eb + (i * 2 + 1) * sb) / count) / 8;
            p[i].b0 = ((s32)((count - i * 2) * eb + i * 2 * sb) / count) / 8;
            i++;
        } while (i != (count / 2));
    }
    if (flag) {
        for (i = 1; i < (count / 2); i++) {
            p[i].x1 = 1;
            p[i].x0 = 1;
        }
        p[0].x1 = 1;
    } else {
        for (i = 0; i < (count / 2); i++) {
            p[i].x1 = 0;
            p[i].x0 = 0;
        }
    }
    p[0].x0 = 0;
    p[0].r0 = 0;
    p[0].g0 = 0;
    p[0].b0 = 0;
}

void col2sepia(arg0, arg1, dr, dg, db)
PixelPairIn *arg0;
PixelPairIn *arg1;
s32 dr;
s32 dg;
s32 db;
{
    PixelPairIn *p;
    s32 i;
    u32 v;
    u32 s;

    color_palette_to_gray(arg0, arg1);
    p = arg1;
    for (i = 0; i != 0x80; i++) {
        s = p->r0 + p->g0 + p->b0;
        if (s != 0) {
            v = p->r0 + dr;
            p->r0 = (v >= 0x20) ? 0x1F : v;
            v = p->g0 + dg;
            p->g0 = (v >= 0x20) ? 0x1F : v;
            s = p->b0 + db;
            p->b0 = (s >= 0x20) ? 0x1F : s;
        }
        s = p->r1 + p->g1 + p->b1;
        if (s != 0) {
            v = p->r1 + dr;
            p->r1 = (v >= 0x20) ? 0x1F : v;
            v = p->g1 + dg;
            p->g1 = (v >= 0x20) ? 0x1F : v;
            v = p->b1 + db;
            p->b1 = (v >= 0x20) ? 0x1F : v;
        }
        p++;
    }
}
