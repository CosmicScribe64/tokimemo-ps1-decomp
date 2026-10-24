#include "common.h"
#include "game.h"
#include "psx_types.h"

s32 func_80046500(void) {
    func_800789E0();
    func_800412E0(0);
    main_state_set(0x12);
    func_80041584();
    func_80048DD0(0);
    back_clear_switch(1);
    dec_bg_reset();
    func_8004482C();
    tpage_buf_clear();
    func_80044750(0x7F);
    func_8007BDE8();
    func_8007B844();
    func_800452C4();
    func_80041878();
    func_80042058();
    return -1;
}

s32 func_80046590(s16 arg0, u8 arg1) {
    s32 p;
    u32 i;

    p = 1;
    if ((u32)arg1 >= 6) {
        return 0;
    }
    i = 0;
    if (arg1 != 0) {
        do {
            i += 1;
            p *= 10;
        } while (i < (u32)arg1);
    }
    return (arg0 / (p / 10)) % 10;
}

s32 func_80046684(s32 arg0) {
    s32 result;
    s32 mul;
    s32 i;

    result = 0;
    mul = 1;
    for (i = 0; i < 8; i++) {
        result += (mul * (arg0 & (0xF << (i * 4)))) >> (i * 4);
        mul *= 10;
    }
    return result;
}

void func_80046754(void *arg0, s32 arg1, s32 arg2) {
    s32 x;
    s32 y;
    s32 shade;
    u8 color;
    u16 load_x;
    u16 load_y;
    u32 *column;
    u32 *out;

    /* Clear the 128 rows of sixteen packed 4-bit pixel words. */
    x = 0;
    column = arg0;
    for (; x < 16; x++) {
        y = 0;
        out = column;
        for (; y < 128; y++) {
            *out = 0;
            out += 16;
        }
        column++;
    }
    /* High mode selects radial shading; the low nibble selects a flat fill. */
    if ((arg2 / 16) & 0xFF) {
        for (x = 0; x < 128; x++) {
            for (y = 0; y < 128; y++) {
                for (shade = 15; shade != 0; shade--) {
                    if (x * x + y * y < ((15 - shade) * (15 - shade) << 6)) {
                        *((u32 *)arg0 + x / 8 + y * 16) |= shade << ((x % 8) * 4);
                        break;
                    }
                }
            }
        }
    } else {
        color = arg2 & 15;
        for (x = 0; x < 128; x++) {
            for (y = 0; y < 128; y++) {
                if (color) {
                    if (x * x + y * y < 0x3F01) {
                        *((u32 *)arg0 + x / 8 + y * 16) |= color << ((x % 8) * 4);
                    }
                } else if (x * x + y * y >= 0x3F02) {
                    *((u32 *)arg0 + x / 8 + y * 16) |= 1 << ((x % 8) * 4);
                }
            }
        }
    }
    load_x = (arg1 & 15) << 6;
    load_y = ((arg1 & 16) * 16) + ((arg1 & 32) * 4);
    LoadSquare(load_x, load_y, 32U, 128U, arg0);
}

s32 func_80046AC8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7) {
    GameState *gs;
    void *primitive;
    s32 quadrant;
    s32 order;
    u8 alpha;
    s32 red;
    s32 radius;
    s32 extent;
    s32 row;

    gs = &D_800E6280;
    order = arg7 & 0xF;
    /* FAKE: T-25124, the v2/v3 endpoint reads row so uopt numbers its item after the
       texture page arguments (spill temporaries); v0/v1 use the expression itself. */
    row = (arg0 & 0x20) * 4;
    alpha = arg6 >> 0x1C;
    for (quadrant = 0; quadrant < 4; quadrant++) {
        primitive = (void *)GetWorkBase(0x34, D_8011ECA0);
        func_8009F068(primitive);
        if (alpha & 0xF) {
            SetSemiTrans(primitive, 1);
        } else {
            SetSemiTrans(primitive, 0);
        }
        if (alpha & 0xF) {
            /* FAKE: T-25124, the (u16) conversion is deleted by as1 but spends the ugen
               temporary that numbers the colour shifts; one RGB line per vertex. */
            red = (u16)(arg6 >> 0x10) & 0xFF;
            ((POLY_GT4 *)primitive)->r0 = red, ((POLY_GT4 *)primitive)->g0 = (s8) (arg6 >> 8), ((POLY_GT4 *)primitive)->b0 = (s8) arg6;
            ((POLY_GT4 *)primitive)->r1 = red, ((POLY_GT4 *)primitive)->g1 = (s8) (arg6 >> 8), ((POLY_GT4 *)primitive)->b1 = (s8) arg6;
            ((POLY_GT4 *)primitive)->r2 = red, ((POLY_GT4 *)primitive)->g2 = (s8) (arg6 >> 8), ((POLY_GT4 *)primitive)->b2 = (s8) arg6;
            ((POLY_GT4 *)primitive)->r3 = red, ((POLY_GT4 *)primitive)->g3 = (s8) (arg6 >> 8), ((POLY_GT4 *)primitive)->b3 = (s8) arg6;
        } else {
            ((POLY_GT4 *)primitive)->r0 = 0x80;
            ((POLY_GT4 *)primitive)->g0 = 0x80;
            ((POLY_GT4 *)primitive)->b0 = 0x80;
            ((POLY_GT4 *)primitive)->r1 = 0x80;
            ((POLY_GT4 *)primitive)->g1 = 0x80;
            ((POLY_GT4 *)primitive)->b1 = 0x80;
            ((POLY_GT4 *)primitive)->r2 = 0x80;
            ((POLY_GT4 *)primitive)->g2 = 0x80;
            ((POLY_GT4 *)primitive)->b2 = 0x80;
            ((POLY_GT4 *)primitive)->r3 = 0x80;
            ((POLY_GT4 *)primitive)->g3 = 0x80;
            ((POLY_GT4 *)primitive)->b3 = 0x80;
        }
        switch (quadrant) {
        case 0:
            ((POLY_GT4 *)primitive)->x0 = (s16) (gs->screen_center_x[D_8011ECA0] + arg4);
            ((POLY_GT4 *)primitive)->y0 = (s16) (gs->screen_center_y[D_8011ECA0] + arg5);
            radius = (s32) (arg3 << 7) / 4096;
            ((POLY_GT4 *)primitive)->x1 = (s16) (gs->screen_center_x[D_8011ECA0] + arg4 + radius);
            ((POLY_GT4 *)primitive)->y1 = (s16) (gs->screen_center_y[D_8011ECA0] + arg5);
            ((POLY_GT4 *)primitive)->x2 = (s16) (gs->screen_center_x[D_8011ECA0] + arg4);
            ((POLY_GT4 *)primitive)->y2 = (s16) (gs->screen_center_y[D_8011ECA0] + arg5 + radius);
            ((POLY_GT4 *)primitive)->x3 = (s16) (gs->screen_center_x[D_8011ECA0] + arg4 + radius);
            ((POLY_GT4 *)primitive)->y3 = (s16) (gs->screen_center_y[D_8011ECA0] + arg5 + radius);
            ((POLY_GT4 *)primitive)->u0 = 0, ((POLY_GT4 *)primitive)->v0 = ((arg0 & 0x20) * 4), ((POLY_GT4 *)primitive)->u1 = 0x7F, ((POLY_GT4 *)primitive)->v1 = ((arg0 & 0x20) * 4), ((POLY_GT4 *)primitive)->u2 = 0, ((POLY_GT4 *)primitive)->v2 = row + 0x7F, ((POLY_GT4 *)primitive)->u3 = 0x7F, ((POLY_GT4 *)primitive)->v3 = row + 0x7F;
            /* FAKE: T-25124 (from T-25034), the one-line comma groups of UV stores and the goto
               to the shared label after case 3 reproduce the original UV store and branch
               layout; the original source is unknown. */
            goto block_16;
        case 1:
            ((POLY_GT4 *)primitive)->x0 = (s16) (gs->screen_center_x[D_8011ECA0] + arg4);
            ((POLY_GT4 *)primitive)->y0 = (s16) (gs->screen_center_y[D_8011ECA0] + arg5);
            radius = (s32) (arg3 << 7) / 4096;
            ((POLY_GT4 *)primitive)->x1 = (s16) (gs->screen_center_x[D_8011ECA0] + arg4 + radius);
            ((POLY_GT4 *)primitive)->y1 = (s16) (gs->screen_center_y[D_8011ECA0] + arg5);
            extent = -radius;
            ((POLY_GT4 *)primitive)->x2 = (s16) (gs->screen_center_x[D_8011ECA0] + arg4);
            ((POLY_GT4 *)primitive)->y2 = (s16) (gs->screen_center_y[D_8011ECA0] + arg5 + extent);
            ((POLY_GT4 *)primitive)->x3 = (s16) (gs->screen_center_x[D_8011ECA0] + arg4 + radius);
            ((POLY_GT4 *)primitive)->y3 = (s16) (gs->screen_center_y[D_8011ECA0] + arg5 + extent);
            ((POLY_GT4 *)primitive)->u0 = 0, ((POLY_GT4 *)primitive)->v0 = ((arg0 & 0x20) * 4), ((POLY_GT4 *)primitive)->u1 = 0x7F, ((POLY_GT4 *)primitive)->v1 = ((arg0 & 0x20) * 4), ((POLY_GT4 *)primitive)->u2 = 0, ((POLY_GT4 *)primitive)->v2 = row + 0x7F, ((POLY_GT4 *)primitive)->u3 = 0x7F, ((POLY_GT4 *)primitive)->v3 = row + 0x7F;
            goto block_16;
        case 2:
            ((POLY_GT4 *)primitive)->x0 = (s16) (gs->screen_center_x[D_8011ECA0] + arg4);
            ((POLY_GT4 *)primitive)->y0 = (s16) (gs->screen_center_y[D_8011ECA0] + arg5);
            radius = (s32) (arg3 << 7) / 4096;
            extent = -radius;
            ((POLY_GT4 *)primitive)->x1 = (s16) (gs->screen_center_x[D_8011ECA0] + arg4 + extent);
            ((POLY_GT4 *)primitive)->y1 = (s16) (gs->screen_center_y[D_8011ECA0] + arg5);
            ((POLY_GT4 *)primitive)->x2 = (s16) (gs->screen_center_x[D_8011ECA0] + arg4);
            ((POLY_GT4 *)primitive)->y2 = (s16) (gs->screen_center_y[D_8011ECA0] + arg5 + radius);
            ((POLY_GT4 *)primitive)->x3 = (s16) (gs->screen_center_x[D_8011ECA0] + arg4 + extent);
            ((POLY_GT4 *)primitive)->y3 = (s16) (gs->screen_center_y[D_8011ECA0] + arg5 + radius);
            ((POLY_GT4 *)primitive)->u0 = 0, ((POLY_GT4 *)primitive)->v0 = ((arg0 & 0x20) * 4), ((POLY_GT4 *)primitive)->u1 = 0x7F, ((POLY_GT4 *)primitive)->v1 = ((arg0 & 0x20) * 4), ((POLY_GT4 *)primitive)->u2 = 0, ((POLY_GT4 *)primitive)->v2 = row + 0x7F, ((POLY_GT4 *)primitive)->u3 = 0x7F, ((POLY_GT4 *)primitive)->v3 = row + 0x7F;
            goto block_16;
        case 3:
            ((POLY_GT4 *)primitive)->x0 = (s16) (gs->screen_center_x[D_8011ECA0] + arg4);
            ((POLY_GT4 *)primitive)->y0 = (s16) (gs->screen_center_y[D_8011ECA0] + arg5);
            extent = -((s32) (arg3 << 7) / 4096);
            ((POLY_GT4 *)primitive)->x1 = (s16) (gs->screen_center_x[D_8011ECA0] + arg4 + extent);
            ((POLY_GT4 *)primitive)->y1 = (s16) (gs->screen_center_y[D_8011ECA0] + arg5);
            ((POLY_GT4 *)primitive)->x2 = (s16) (gs->screen_center_x[D_8011ECA0] + arg4);
            ((POLY_GT4 *)primitive)->y2 = (s16) (gs->screen_center_y[D_8011ECA0] + arg5 + extent);
            ((POLY_GT4 *)primitive)->x3 = (s16) (gs->screen_center_x[D_8011ECA0] + arg4 + extent);
            ((POLY_GT4 *)primitive)->y3 = (s16) (gs->screen_center_y[D_8011ECA0] + arg5 + extent);
            ((POLY_GT4 *)primitive)->u0 = 0, ((POLY_GT4 *)primitive)->v0 = ((arg0 & 0x20) * 4), ((POLY_GT4 *)primitive)->u1 = 0x7F, ((POLY_GT4 *)primitive)->v1 = ((arg0 & 0x20) * 4), ((POLY_GT4 *)primitive)->u2 = 0, ((POLY_GT4 *)primitive)->v2 = row + 0x7F, ((POLY_GT4 *)primitive)->u3 = 0x7F, ((POLY_GT4 *)primitive)->v3 = row + 0x7F;
block_16:
            break;
        }
        ((POLY_GT4 *)primitive)->tpage = func_8009ECB0(0, 0, (arg0 & 0xF) << 6, ((arg0 & 0x20) * 4) + ((arg0 & 0x10) * 0x10));
        ((POLY_GT4 *)primitive)->clut = func_8009ED74(arg1, arg2);
        AddPrim(&D_800E8CA0[(D_8011ECA0 << 0xA) + order * 4], primitive);
        dtd_on_tpage(0, 0, order, 1, 0);
    }
    quadrant = 0;
    /* FAKE: T-25124, (u8) and the (u16) colour conversions below spend ugen temporaries
       that as1 deletes; they set the register numbering of the tile setup. */
    if ((u8)(arg6 >> 0x18) & 0xF) {
        do {
            primitive = (void *)GetWorkBase(0x10, D_8011ECA0);
            func_8009F0F4(primitive);
            SetSemiTrans(primitive, 0);
            SetShadeTex(primitive, 0);
            red = (u16)(arg6 >> 0x10) & 0xFF;
            ((TILE *)primitive)->r0 = red;
            ((TILE *)primitive)->g0 = (u16)(arg6 >> 8) & 0xFF;
            ((TILE *)primitive)->b0 = (u16)(arg6 & 0xFF);
            switch (quadrant) {
            case 0:
                ((TILE *)primitive)->x0 = (s16) (gs->screen_center_x[D_8011ECA0] - 0xA0);
                ((TILE *)primitive)->y0 = (s16) (gs->screen_center_y[D_8011ECA0] - 0x78);
                ((TILE *)primitive)->w = 0x140;
                extent = (arg5 - ((s32) (arg3 << 7) / 4096)) + 0x78;
                ((TILE *)primitive)->h = extent;
                if (extent > 0) {
                    AddPrim(&D_800E8CA0[(D_8011ECA0 << 0xA) + order * 4], primitive);
                }
                break;
            case 1:
                ((TILE *)primitive)->x0 = (s16) (gs->screen_center_x[D_8011ECA0] - 0xA0);
                ((TILE *)primitive)->y0 = (s16) (gs->screen_center_y[D_8011ECA0] - 0x78);
                extent = (arg4 - ((s32) (arg3 << 7) / 4096)) + 0xA0;
                ((TILE *)primitive)->w = extent;
                ((TILE *)primitive)->h = 0xF0;
                if (extent > 0) {
                    AddPrim(&D_800E8CA0[(D_8011ECA0 << 0xA) + order * 4], primitive);
                }
                break;
            case 2:
                ((TILE *)primitive)->x0 = (s16) (gs->screen_center_x[D_8011ECA0] - 0xA0);
                radius = (s32) (arg3 << 7) / 4096;
                extent = (-arg5 - radius) + 0x78;
                ((TILE *)primitive)->y0 = (s16) (gs->screen_center_y[D_8011ECA0] + arg5 + radius);
                ((TILE *)primitive)->h = extent;
                ((TILE *)primitive)->w = 0x140;
                if (extent > 0) {
                    AddPrim(&D_800E8CA0[(D_8011ECA0 << 0xA) + order * 4], primitive);
                }
                break;
            case 3:
                radius = (s32) (arg3 << 7) / 4096;
                ((TILE *)primitive)->x0 = (s16) (gs->screen_center_x[D_8011ECA0] + arg4 + radius);
                extent = (-arg4 - radius) + 0xA0;
                ((TILE *)primitive)->y0 = (s16) (gs->screen_center_y[D_8011ECA0] - 0x78);
                ((TILE *)primitive)->w = extent;
                ((TILE *)primitive)->h = 0xF0;
                if (extent > 0) {
                    AddPrim(&D_800E8CA0[(D_8011ECA0 << 0xA) + order * 4], primitive);
                }
                break;
            }
            quadrant += 1;
        } while (quadrant != 4);
    }
    /* FAKE: T-25124, empty test keeps the order item live through the tile loop; uopt then
       gives the hoisted tile blue the spill temporary at 0x70 instead of 0x74. */
    if (order) {
    }
}
