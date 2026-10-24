/* gpu_effects.c: src/main/80049FF0.c; Original object 0x80049FF0-0x8004AD60: gradient-line drawing, background-layer setup, radial/disc effects, textured/tile circle effects and full-screen gradient fades. Original object 0x80049FF0-0x8004AD60 (config/objects/main.txt). */
#include "common.h"
#include "game.h"
#include "psx_types.h"

s32 prim_gradient_line(x0, y0, x1, y1, c0, c1, flags)
s16 x0;
s16 y0;
s16 x1;
s16 y1;
u32 c0;
u32 c1;
u8 flags;
{
    GameState *g;
    LINE_G2 *line;

    g = &game_state;
    line = (LINE_G2 *)GetWorkBase(0x14, draw_buffer);
    func_8009F11C(line);
    if (flags & 0x80) {
        SetSemiTrans(line, 1);
    } else {
        SetSemiTrans(line, 0);
    }
    /* FAKE: explicit colour masks and comma grouping preserve renderer scheduling (T-20600, T-24041). */
    line->r0 = (c0 >> 16) & 0xFF, line->g0 = (c0 >> 8) & 0xFF, line->b0 = c0 & 0xFF;
    line->r1 = (c1 >> 16) & 0xFF, line->g1 = (c1 >> 8) & 0xFF, line->b1 = c1 & 0xFF;
    line->x0 = g->screen_center_x[draw_buffer] + x0;
    line->y0 = g->screen_center_y[draw_buffer] + y0;
    line->x1 = g->screen_center_x[draw_buffer] + x1;
    line->y1 = g->screen_center_y[draw_buffer] + y1;
    AddPrim(&D_800E8CA0[(draw_buffer << 10) + ((flags & 0x7F) * 4)], line);
}

void func_8004A14C(s32 arg0, RECT rect, s32 arg3) {
    prim_gradient_rect(rect.x, rect.y, rect.w, rect.h, 1, arg3, 1);
}

void func_8004A1A0(s32 *tex, s32 slot, s32 count, s32 w, s32 h, s32 clut, s32 x, s32 y, s32 flag, s32 tp) {
    BgLayer *bg;
    u8 cellh;
    s32 row;
    s32 col;

    cellh = 0x78;
    /* main_api.h declares bg_layers as the first word of slot 0; the slots are BgLayer records. */
    bg = &((BgLayer *)&bg_layers)[slot];
    bg->unk_00 = 0x1000000;
    bg->unk_04 = x;
    bg->unk_06 = y;
    bg->unk_08 = w << 7;
    bg->unk_0A = h * cellh;
    bg->unk_0C = 0;
    bg->unk_0E = 0;
    bg->unk_10 = 0x80;
    bg->unk_11 = 0x80;
    bg->unk_12 = 0x80;
    bg->unk_14 = (u32)&D_80122120[slot];
    bg->unk_18 = 0;
    bg->unk_1A = 0;
    bg->unk_1C = 0x1000;
    bg->unk_1E = 0x1000;
    /* FAKE: T-25302, empty test: a uopt block boundary (the branch is dropped after register allocation),
     * so 0x1000 and row share $v1 and the map address takes $a0 as in the original. Real source unknown. */
    if (slot) {
    }
    bg->unk_20 = 0;
    /* Indexed, not through a map local: with four locals the cell and index spills land at sp+0x58/0x54. */
    D_80122120[slot].cellw = 0x80;
    D_80122120[slot].cellh = cellh;
    D_80122120[slot].ncellw = w;
    D_80122120[slot].ncellh = h;
    D_80122120[slot].base = D_80122140[slot];
    D_80122120[slot].index = D_80122540[slot].v;
    for (row = 0; row < h; row++) {
        for (col = 0; col < w; col++) {
            load_tpage_buf_lock(tp + col + row * 32, tex[row * w + col], 1);
        }
    }
    for (row = 0; row < h; row++) {
        for (col = 0; col < w; col++) {
            D_80122540[slot].v[row * w + col] = row * w + col;
            D_80122140[slot][row * w + col].u = 0;
            D_80122140[slot][row * w + col].v = row * 0x78;
            D_80122140[slot][row * w + col].tpage = tp + col;
            D_80122140[slot][row * w + col].cba = func_8009ED74(0, clut);
            D_80122140[slot][row * w + col].flag = 0;
            if (row * w + col > 0x40) {
                printf("BG_ERROR:p_line.c \n");
            }
        }
    }
    game_state.bg_layer_order[slot] = flag;
}

void func_8004A414(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    GameState *g;
    POLY_GT4 *poly;
    s32 x;
    s32 y;

    arg3 &= 0x7F;
    arg4 &= 0x7F;
    if (arg3 < 8) {
        arg3 = 8;
    }
    if (arg4 < 8) {
        arg4 = 8;
    }
    for (x = arg0 - arg2; x < arg0 + arg2; x += arg3) {
        g = &game_state;
        y = arg1 - arg2;
        if (y < arg1 + arg2) {
            do {
                if ((arg0 - x) * (arg0 - x) + (arg1 - y) * (arg1 - y) < arg2 * arg2) {
                    poly = (POLY_GT4 *)GetWorkBase(sizeof(POLY_GT4), draw_buffer);
                    func_8009F068(poly);
                    SetSemiTrans(poly, 0);
                    /* FAKE: T-25122, hand expansions of the PsyQ macros setXYWH, setUVWH(poly, 0xC0, 0x50, 0x10, 0x10)
                     * and setRGB0..setRGB3, one comma statement per macro as they expand. One statement per field
                     * gives other saved-register colours, constant registers and store order (142 diff lines). */
                    poly->x0 = g->screen_center_x[draw_buffer] + (s16)x - arg3 / 2,
                        poly->y0 = g->screen_center_y[draw_buffer] + (s16)y - arg4 / 2,
                        poly->x1 = g->screen_center_x[draw_buffer] + (s16)x - arg3 / 2 + (s16)arg3,
                        poly->y1 = g->screen_center_y[draw_buffer] + (s16)y - arg4 / 2,
                        poly->x2 = g->screen_center_x[draw_buffer] + (s16)x - arg3 / 2,
                        poly->y2 = g->screen_center_y[draw_buffer] + (s16)y - arg4 / 2 + (s16)arg4,
                        poly->x3 = g->screen_center_x[draw_buffer] + (s16)x - arg3 / 2 + (s16)arg3,
                        poly->y3 = g->screen_center_y[draw_buffer] + (s16)y - arg4 / 2 + (s16)arg4;
                    poly->u0 = 0xC0, poly->v0 = 0x50,
                        poly->u1 = 0xC0 + 0x10, poly->v1 = 0x50,
                        poly->u2 = 0xC0, poly->v2 = 0x50 + 0x10,
                        poly->u3 = 0xC0 + 0x10, poly->v3 = 0x50 + 0x10;
                    poly->r0 = 0x80, poly->g0 = 0x80, poly->b0 = 0x80;
                    poly->r1 = 0x80, poly->g1 = 0x80, poly->b1 = 0x80;
                    poly->r2 = 0x80, poly->g2 = 0x80, poly->b2 = 0x80;
                    poly->r3 = 0x80, poly->g3 = 0x80, poly->b3 = 0x80;
                    poly->tpage = func_8009ECB0(0, 0, 0x3C0, 0x100);
                    poly->clut = func_8009ED74(g->screen_center_x[draw_buffer] + (s16)x + (rand() & 8),
                                               (s16)y + g->screen_center_y[draw_buffer] + (rand() & 8));
                    AddPrim(&D_800E8CA0[(draw_buffer << 10) + arg5 * 4], poly);
                }
                y += arg4;
            } while (y < arg1 + arg2);
        }
    }
}

void func_8004A814(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7) {
    GameState *state;
    TILE *tile;
    s32 x;
    s32 y;
    s32 abs_x;
    u32 red;
    u32 green;
    s32 blue;
    s32 r;
    s32 g;
    s32 b;
    s32 abs_y;

    red = (u32)(arg5 & 0xFF0000) >> 16;
    green = (u32)(arg5 & 0xFF00) >> 8;
    blue = arg5 & 0xFF;
    arg3 &= 0x7F;
    arg4 &= 0x7F;
    if (arg3 < 2) {
        arg3 = 2;
    }
    if (arg4 < 2) {
        arg4 = 2;
    }
    for (x = arg0 - arg2; x < arg0 + arg2; x += arg3) {
        y = arg1 - arg2;
        if (y < arg1 + arg2) {
            do {
                if ((arg0 - x) * (arg0 - x) + (arg1 - y) * (arg1 - y) < arg2 * arg2) {
                    tile = (TILE *)GetWorkBase(sizeof(TILE), draw_buffer);
                    func_8009F0F4(tile);
                    SetSemiTrans(tile, 1);
                    state = &game_state;
                    tile->x0 = state->screen_center_x[draw_buffer] + (s16)x - arg3 / 2;
                    /* FAKE: T-24501, narrow only the stored 16-bit coordinate.
                     * The full-width y loop/distance values are unchanged. The
                     * erased conversion preserves original temporary allocation. */
                    tile->y0 = state->screen_center_y[draw_buffer] + (s16)y - arg4 / 2;
                    /* The target uses the x step for both dimensions. */
                    setWH(tile, arg3, arg3);
                    abs_x = abs(arg0 - x);
                    abs_y = abs(arg1 - y);
                    r = (s32)((arg2 * 2 - abs_y - abs_x) * red) / arg2;
                    abs_x = abs(arg0 - x);
                    abs_y = abs(arg1 - y);
                    g = (s32)((arg2 * 2 - abs_y - abs_x) * green) / arg2;
                    abs_x = abs(arg0 - x);
                    abs_y = abs(arg1 - y);
                    b = ((arg2 * 2 - abs_y - abs_x) * blue) / arg2;
                    if (r >= 256) {
                        r = 255;
                    }
                    if (g >= 256) {
                        g = 255;
                    }
                    if (b >= 256) {
                        b = 255;
                    }
                    tile->r0 = r;
                    tile->g0 = g;
                    tile->b0 = b;
                    AddPrim(&D_800E8CA0[(draw_buffer << 10) + arg7 * 4], tile);
                }
                y += arg4;
            } while (y < arg1 + arg2);
        }
    }
    dtd_on_tpage(0, 0, arg7, arg6 & 3, 0);
}

void func_8004AC18(s32 arg0) {
    s32 level;
    s32 color;

    level = arg0;
    if (level < 0x100) {
        color = level * 0x10101;
    } else {
        color = 0xFFFFFF;
    }
    prim_gradient_rect(-0xA0, -0x78, 0xA0, 0xF0, 1, color, 0x81);
    prim_gradient_rect(0, -0x78, 0xA0, 0xF0, 1, color, 0x81);
    dtd_on_tpage(0, 0, 1, 1, 1);
}

void func_8004ACC8(s32 arg0) {
    prim_gradient_rect(-0x100, -0x78, 0x100, 0xF0, 1, arg0, 0x81);
    prim_gradient_rect(0, -0x78, 0x100, 0xF0, 1, arg0, 0x81);
    dtd_on_tpage(0, 0, 1, 1, 1);
}
