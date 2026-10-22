/* T-25034 research draft, not a byte match. See the checkpoint README
 * for measured differences and the full-word extent correction. */
#include "psx_types.h"
void func_80046AC8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7) {
    GameState *gs;
    void *primitive;
    s32 quadrant;
    s32 order;
    s32 order_offset;
    s32 texture_u;
    s32 texture_v;
    s32 texture_row;
    s32 alpha;
    s32 green;
    s32 red;
    s32 radius;
    s32 extent;

    gs = &D_800E6280;
    order = arg7 & 0xF;
    texture_row = arg0 & 0x20;
    texture_u = (arg0 & 0xF) << 6;
    texture_v = (texture_row * 4) + ((arg0 & 0x10) * 0x10);
    order_offset = order * 4;
    alpha = (arg6 >> 0x1C) & 0xF;
    quadrant = 0;
    do {
        primitive = (void *)GetWorkBase(0x34, D_8011ECA0);
        func_8009F068(primitive);
        if (alpha != 0) {
            SetSemiTrans(primitive, 1);
        } else {
            SetSemiTrans(primitive, 0);
        }
        if (alpha != 0) {
            red = (s8)(arg6 >> 0x10) & 0xFF;
            green = arg6 >> 8;
            ((POLY_GT4 *)primitive)->r0 = red, ((POLY_GT4 *)primitive)->g0 = (s8) green, ((POLY_GT4 *)primitive)->b0 = (s8) arg6;
            ((POLY_GT4 *)primitive)->r1 = red, ((POLY_GT4 *)primitive)->g1 = (s8) green, ((POLY_GT4 *)primitive)->b1 = (s8) arg6;
            ((POLY_GT4 *)primitive)->r2 = red, ((POLY_GT4 *)primitive)->g2 = (s8) green, ((POLY_GT4 *)primitive)->b2 = (s8) arg6;
            ((POLY_GT4 *)primitive)->r3 = red, ((POLY_GT4 *)primitive)->g3 = (s8) green, ((POLY_GT4 *)primitive)->b3 = (s8) arg6;
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
            ((POLY_GT4 *)primitive)->x0 = (s16) (gs->unk_014[D_8011ECA0] + arg4);
            green = (texture_row * 4) + 0x7F;
            ((POLY_GT4 *)primitive)->y0 = (s16) (gs->unk_018[D_8011ECA0] + arg5);
            radius = (s32) (arg3 << 7) / 4096;
            ((POLY_GT4 *)primitive)->x1 = (s16) (gs->unk_014[D_8011ECA0] + arg4 + radius);
            ((POLY_GT4 *)primitive)->y1 = (s16) (gs->unk_018[D_8011ECA0] + arg5);
            ((POLY_GT4 *)primitive)->x2 = (s16) (gs->unk_014[D_8011ECA0] + arg4);
            ((POLY_GT4 *)primitive)->y2 = (s16) (gs->unk_018[D_8011ECA0] + arg5 + radius);
            ((POLY_GT4 *)primitive)->x3 = (s16) (gs->unk_014[D_8011ECA0] + arg4 + radius);
            ((POLY_GT4 *)primitive)->y3 = (s16) (gs->unk_018[D_8011ECA0] + arg5 + radius);
            ((POLY_GT4 *)primitive)->v3 = green;
            ((POLY_GT4 *)primitive)->u3 = 0x7F;
            ((POLY_GT4 *)primitive)->v2 = green;
            ((POLY_GT4 *)primitive)->u2 = 0;
            ((POLY_GT4 *)primitive)->v1 = (texture_row * 4);
            ((POLY_GT4 *)primitive)->u1 = 0x7F;
            ((POLY_GT4 *)primitive)->v0 = (texture_row * 4);
            goto block_16;
        case 1:
            ((POLY_GT4 *)primitive)->x0 = (s16) (gs->unk_014[D_8011ECA0] + arg4);
            green = (texture_row * 4) + 0x7F;
            ((POLY_GT4 *)primitive)->y0 = (s16) (gs->unk_018[D_8011ECA0] + arg5);
            radius = (s32) (arg3 << 7) / 4096;
            ((POLY_GT4 *)primitive)->x1 = (s16) (gs->unk_014[D_8011ECA0] + arg4 + radius);
            ((POLY_GT4 *)primitive)->y1 = (s16) (gs->unk_018[D_8011ECA0] + arg5);
            extent = -radius;
            ((POLY_GT4 *)primitive)->x2 = (s16) (gs->unk_014[D_8011ECA0] + arg4);
            ((POLY_GT4 *)primitive)->y2 = (s16) (gs->unk_018[D_8011ECA0] + arg5 + extent);
            ((POLY_GT4 *)primitive)->x3 = (s16) (gs->unk_014[D_8011ECA0] + arg4 + radius);
            ((POLY_GT4 *)primitive)->y3 = (s16) (gs->unk_018[D_8011ECA0] + arg5 + extent);
            ((POLY_GT4 *)primitive)->v3 = green;
            ((POLY_GT4 *)primitive)->u3 = 0x7F;
            ((POLY_GT4 *)primitive)->v2 = green;
            ((POLY_GT4 *)primitive)->u2 = 0;
            ((POLY_GT4 *)primitive)->v1 = (texture_row * 4);
            ((POLY_GT4 *)primitive)->u1 = 0x7F;
            ((POLY_GT4 *)primitive)->v0 = (texture_row * 4);
            goto block_16;
        case 2:
            ((POLY_GT4 *)primitive)->x0 = (s16) (gs->unk_014[D_8011ECA0] + arg4);
            green = (texture_row * 4) + 0x7F;
            ((POLY_GT4 *)primitive)->y0 = (s16) (gs->unk_018[D_8011ECA0] + arg5);
            radius = (s32) (arg3 << 7) / 4096;
            extent = -radius;
            ((POLY_GT4 *)primitive)->x1 = (s16) (gs->unk_014[D_8011ECA0] + arg4 + extent);
            ((POLY_GT4 *)primitive)->y1 = (s16) (gs->unk_018[D_8011ECA0] + arg5);
            ((POLY_GT4 *)primitive)->x2 = (s16) (gs->unk_014[D_8011ECA0] + arg4);
            ((POLY_GT4 *)primitive)->y2 = (s16) (gs->unk_018[D_8011ECA0] + arg5 + radius);
            ((POLY_GT4 *)primitive)->x3 = (s16) (gs->unk_014[D_8011ECA0] + arg4 + extent);
            ((POLY_GT4 *)primitive)->y3 = (s16) (gs->unk_018[D_8011ECA0] + arg5 + radius);
            ((POLY_GT4 *)primitive)->v3 = green;
            ((POLY_GT4 *)primitive)->u3 = 0x7F;
            ((POLY_GT4 *)primitive)->v2 = green;
            ((POLY_GT4 *)primitive)->u2 = 0;
            ((POLY_GT4 *)primitive)->v1 = (texture_row * 4);
            ((POLY_GT4 *)primitive)->u1 = 0x7F;
            ((POLY_GT4 *)primitive)->v0 = (texture_row * 4);
            goto block_16;
        case 3:
            ((POLY_GT4 *)primitive)->x0 = (s16) (gs->unk_014[D_8011ECA0] + arg4);
            green = (texture_row * 4) + 0x7F;
            ((POLY_GT4 *)primitive)->y0 = (s16) (gs->unk_018[D_8011ECA0] + arg5);
            extent = -((s32) (arg3 << 7) / 4096);
            ((POLY_GT4 *)primitive)->x1 = (s16) (gs->unk_014[D_8011ECA0] + arg4 + extent);
            ((POLY_GT4 *)primitive)->y1 = (s16) (gs->unk_018[D_8011ECA0] + arg5);
            ((POLY_GT4 *)primitive)->x2 = (s16) (gs->unk_014[D_8011ECA0] + arg4);
            ((POLY_GT4 *)primitive)->y2 = (s16) (gs->unk_018[D_8011ECA0] + arg5 + extent);
            ((POLY_GT4 *)primitive)->x3 = (s16) (gs->unk_014[D_8011ECA0] + arg4 + extent);
            ((POLY_GT4 *)primitive)->y3 = (s16) (gs->unk_018[D_8011ECA0] + arg5 + extent);
            ((POLY_GT4 *)primitive)->v3 = green;
            ((POLY_GT4 *)primitive)->u3 = 0x7F;
            ((POLY_GT4 *)primitive)->v2 = green;
            ((POLY_GT4 *)primitive)->u2 = 0;
            ((POLY_GT4 *)primitive)->v1 = (texture_row * 4);
            ((POLY_GT4 *)primitive)->u1 = 0x7F;
            ((POLY_GT4 *)primitive)->v0 = (texture_row * 4);
block_16:
            ((POLY_GT4 *)primitive)->u0 = 0;
            break;
        }
        ((POLY_GT4 *)primitive)->tpage = func_8009ECB0(0, 0, texture_u, texture_v);
        ((POLY_GT4 *)primitive)->clut = func_8009ED74(arg1, arg2);
        AddPrim(&D_800E8CA0[(D_8011ECA0 << 0xA) + order_offset], primitive);
        dtd_on_tpage(0, 0, order, 1, 0);
        quadrant += 1;
    } while (quadrant < 4);
    quadrant = 0;
    if ((arg6 >> 0x18) & 0xF) {
        do {
            primitive = (void *)GetWorkBase(0x10, D_8011ECA0);
            func_8009F0F4(primitive);
            SetSemiTrans(primitive, 0);
            SetShadeTex(primitive, 0);
            ((TILE *)primitive)->r0 = (s8)(arg6 >> 0x10) & 0xFF;
            ((TILE *)primitive)->g0 = (s8)(arg6 >> 8) & 0xFF;
            ((TILE *)primitive)->b0 = arg6 & 0xFF;
            switch (quadrant) {
            case 0:
                ((TILE *)primitive)->x0 = (s16) (gs->unk_014[D_8011ECA0] - 0xA0);
                ((TILE *)primitive)->w = 0x140;
                ((TILE *)primitive)->y0 = (s16) (gs->unk_018[D_8011ECA0] - 0x78);
                extent = (arg5 - ((s32) (arg3 << 7) / 4096)) + 0x78;
                ((TILE *)primitive)->h = extent;
                if (extent > 0) {
                    AddPrim(&D_800E8CA0[(D_8011ECA0 << 0xA) + order_offset], primitive);
                }
                break;
            case 1:
                ((TILE *)primitive)->x0 = (s16) (gs->unk_014[D_8011ECA0] - 0xA0);
                ((TILE *)primitive)->y0 = (s16) (gs->unk_018[D_8011ECA0] - 0x78);
                ((TILE *)primitive)->h = 0xF0;
                extent = (arg4 - ((s32) (arg3 << 7) / 4096)) + 0xA0;
                ((TILE *)primitive)->w = extent;
                if (extent > 0) {
                    AddPrim(&D_800E8CA0[(D_8011ECA0 << 0xA) + order_offset], primitive);
                }
                break;
            case 2:
                ((TILE *)primitive)->x0 = (s16) (gs->unk_014[D_8011ECA0] - 0xA0);
                radius = (s32) (arg3 << 7) / 4096;
                extent = (-arg5 - radius) + 0x78;
                ((TILE *)primitive)->y0 = (s16) (gs->unk_018[D_8011ECA0] + arg5 + radius);
                ((TILE *)primitive)->h = extent;
                ((TILE *)primitive)->w = 0x140;
                if (extent > 0) {
                    AddPrim(&D_800E8CA0[(D_8011ECA0 << 0xA) + order_offset], primitive);
                }
                break;
            case 3:
                radius = (s32) (arg3 << 7) / 4096;
                ((TILE *)primitive)->x0 = (s16) (gs->unk_014[D_8011ECA0] + arg4 + radius);
                extent = (-arg4 - radius) + 0xA0;
                ((TILE *)primitive)->y0 = (s16) (gs->unk_018[D_8011ECA0] - 0x78);
                ((TILE *)primitive)->w = extent;
                ((TILE *)primitive)->h = 0xF0;
                if (extent > 0) {
                    AddPrim(&D_800E8CA0[(D_8011ECA0 << 0xA) + order_offset], primitive);
                }
                break;
            }
            quadrant += 1;
        } while (quadrant != 4);
    }
}
