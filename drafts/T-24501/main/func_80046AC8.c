/* T-24501 diagnostic, not production: resolved cost 738. The inherited signed
 * color/extent narrowings remain suspect; see companion notes before reuse. */
#include "psx_types.h"
void func_80046AC8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7) {
    GameState *gs;
    POLY_GT4 *quad;
    TILE *tile;
    s32 quadrant;
    s32 order;
    s32 order_offset;
    s32 texture_u;
    s32 texture_v;
    s32 texture_row;
    s32 alpha;
    s32 green;
    s32 blue;
    s8 red;
    s8 end_v;
    s32 radius;
    s32 reverse_radius;
    s16 extent;

    gs = &D_800E6280;
    order = arg7 & 0xF;
    texture_row = arg0 & 0x20;
    texture_u = (arg0 & 0xF) << 6;
    alpha = (arg6 >> 0x1C) & 0xF;
    quadrant = 0;
    texture_v = (texture_row * 4) + ((arg0 & 0x10) * 0x10);
    order_offset = order * 4;
    do {
        quad = (POLY_GT4 *)GetWorkBase(0x34, D_8011ECA0);
        func_8009F068(quad);
        if (alpha != 0) {
            SetSemiTrans(quad, 1);
        } else {
            SetSemiTrans(quad, 0);
        }
        if (alpha != 0) {
            red = (arg6 >> 0x10) & 0xFF;
            green = arg6 >> 8;
            quad->g0 = (s8) green;
            quad->r0 = red;
            quad->b0 = (s8) arg6;
            quad->g1 = (s8) green;
            quad->r1 = red;
            quad->b1 = (s8) arg6;
            quad->g2 = (s8) green;
            quad->r2 = red;
            quad->b2 = (s8) arg6;
            quad->g3 = (s8) green;
            quad->r3 = red;
            quad->b3 = (s8) arg6;
        } else {
            quad->r0 = 0x80;
            quad->g0 = 0x80;
            quad->b0 = 0x80;
            quad->r1 = 0x80;
            quad->g1 = 0x80;
            quad->b1 = 0x80;
            quad->r2 = 0x80;
            quad->g2 = 0x80;
            quad->b2 = 0x80;
            quad->r3 = 0x80;
            quad->g3 = 0x80;
            quad->b3 = 0x80;
        }
        switch (quadrant) {
        case 0:
            quad->x0 = (s16) (gs->unk_014[D_8011ECA0] + arg4);
            end_v = (texture_row * 4) + 0x7F;
            quad->y0 = (s16) (gs->unk_018[D_8011ECA0] + arg5);
            radius = (s32) (arg3 << 7) / 4096;
            quad->x1 = (s16) (gs->unk_014[D_8011ECA0] + arg4 + radius);
            quad->y1 = (s16) (gs->unk_018[D_8011ECA0] + arg5);
            quad->x2 = (s16) (gs->unk_014[D_8011ECA0] + arg4);
            quad->y2 = (s16) (gs->unk_018[D_8011ECA0] + arg5 + radius);
            quad->x3 = (s16) (gs->unk_014[D_8011ECA0] + arg4 + radius);
            quad->v3 = end_v;
            quad->y3 = (s16) (gs->unk_018[D_8011ECA0] + arg5 + radius);
            quad->u3 = 0x7F;
            quad->v2 = end_v;
            quad->u2 = 0;
            quad->v1 = (texture_row * 4);
            quad->u1 = 0x7F;
            quad->v0 = (texture_row * 4);
block_16:
            quad->u0 = 0;
            break;
        case 1:
            quad->x0 = (s16) (gs->unk_014[D_8011ECA0] + arg4);
            end_v = (texture_row * 4) + 0x7F;
            quad->y0 = (s16) (gs->unk_018[D_8011ECA0] + arg5);
            radius = (s32) (arg3 << 7) / 4096;
            quad->x1 = (s16) (gs->unk_014[D_8011ECA0] + arg4 + radius);
            quad->y1 = (s16) (gs->unk_018[D_8011ECA0] + arg5);
            reverse_radius = -radius;
            quad->x2 = (s16) (gs->unk_014[D_8011ECA0] + arg4);
            quad->y2 = (s16) (gs->unk_018[D_8011ECA0] + arg5 + reverse_radius);
            quad->x3 = (s16) (gs->unk_014[D_8011ECA0] + arg4 + radius);
            quad->v3 = end_v;
            quad->y3 = (s16) (gs->unk_018[D_8011ECA0] + arg5 + reverse_radius);
            quad->u3 = 0x7F;
            quad->v2 = end_v;
            quad->u2 = 0;
            quad->v1 = (texture_row * 4);
            quad->u1 = 0x7F;
            quad->v0 = (texture_row * 4);
            goto block_16;
        case 2:
            quad->x0 = (s16) (gs->unk_014[D_8011ECA0] + arg4);
            end_v = (texture_row * 4) + 0x7F;
            quad->y0 = (s16) (gs->unk_018[D_8011ECA0] + arg5);
            radius = (s32) (arg3 << 7) / 4096;
            reverse_radius = -radius;
            quad->x1 = (s16) (gs->unk_014[D_8011ECA0] + arg4 + reverse_radius);
            quad->y1 = (s16) (gs->unk_018[D_8011ECA0] + arg5);
            quad->x2 = (s16) (gs->unk_014[D_8011ECA0] + arg4);
            quad->y2 = (s16) (gs->unk_018[D_8011ECA0] + arg5 + radius);
            quad->x3 = (s16) (gs->unk_014[D_8011ECA0] + arg4 + reverse_radius);
            quad->v3 = end_v;
            quad->y3 = (s16) (gs->unk_018[D_8011ECA0] + arg5 + radius);
            quad->u3 = 0x7F;
            quad->v2 = end_v;
            quad->u2 = 0;
            quad->v1 = (texture_row * 4);
            quad->u1 = 0x7F;
            quad->v0 = (texture_row * 4);
            goto block_16;
        case 3:
            quad->x0 = (s16) (gs->unk_014[D_8011ECA0] + arg4);
            end_v = (texture_row * 4) + 0x7F;
            quad->y0 = (s16) (gs->unk_018[D_8011ECA0] + arg5);
            reverse_radius = -((s32) (arg3 << 7) / 4096);
            quad->x1 = (s16) (gs->unk_014[D_8011ECA0] + arg4 + reverse_radius);
            quad->y1 = (s16) (gs->unk_018[D_8011ECA0] + arg5);
            quad->x2 = (s16) (gs->unk_014[D_8011ECA0] + arg4);
            quad->y2 = (s16) (gs->unk_018[D_8011ECA0] + arg5 + reverse_radius);
            quad->x3 = (s16) (gs->unk_014[D_8011ECA0] + arg4 + reverse_radius);
            quad->v3 = end_v;
            quad->y3 = (s16) (gs->unk_018[D_8011ECA0] + arg5 + reverse_radius);
            quad->u3 = 0x7F;
            quad->v2 = end_v;
            quad->u2 = 0;
            quad->v1 = (texture_row * 4);
            quad->u1 = 0x7F;
            quad->v0 = (texture_row * 4);
            goto block_16;
        }
        quad->tpage = func_8009ECB0(0, 0, texture_u, texture_v);
        quad->clut = func_8009ED74(arg1, arg2);
        AddPrim(&D_800E8CA0[(D_8011ECA0 << 0xA) + order_offset], quad);
        dtd_on_tpage(0, 0, order, 1, 0);
        quadrant += 1;
    } while (quadrant < 4);
    quadrant = 0;
    if ((arg6 >> 0x18) & 0xF) {
        blue = arg6 & 0xFF;
        do {
            tile = (TILE *)GetWorkBase(0x10, D_8011ECA0);
            func_8009F0F4(tile);
            SetSemiTrans(tile, 0);
            SetShadeTex(tile, 0);
            tile->r0 = (s8) ((arg6 >> 0x10) & 0xFF);
            tile->g0 = (s8) ((arg6 >> 8) & 0xFF);
            tile->b0 = (s8) blue;
            switch (quadrant) {
            case 0:
                tile->x0 = (s16) (gs->unk_014[D_8011ECA0] - 0xA0);
                tile->w = 0x140;
                tile->y0 = (s16) (gs->unk_018[D_8011ECA0] - 0x78);
                extent = (arg5 - ((s32) (arg3 << 7) / 4096)) + 0x78;
                tile->h = extent;
                if (extent > 0) {
                    AddPrim(&D_800E8CA0[(D_8011ECA0 << 0xA) + order_offset], tile);
                }
                break;
            case 1:
                tile->x0 = (s16) (gs->unk_014[D_8011ECA0] - 0xA0);
                tile->y0 = (s16) (gs->unk_018[D_8011ECA0] - 0x78);
                tile->h = 0xF0;
                extent = (arg4 - ((s32) (arg3 << 7) / 4096)) + 0xA0;
                tile->w = extent;
                if (extent > 0) {
                    AddPrim(&D_800E8CA0[(D_8011ECA0 << 0xA) + order_offset], tile);
                }
                break;
            case 2:
                tile->x0 = (s16) (gs->unk_014[D_8011ECA0] - 0xA0);
                radius = (s32) (arg3 << 7) / 4096;
                extent = (-arg5 - radius) + 0x78;
                tile->y0 = (s16) (gs->unk_018[D_8011ECA0] + arg5 + radius);
                tile->h = extent;
                tile->w = 0x140;
                if (extent > 0) {
                    AddPrim(&D_800E8CA0[(D_8011ECA0 << 0xA) + order_offset], tile);
                }
                break;
            case 3:
                radius = (s32) (arg3 << 7) / 4096;
                tile->x0 = (s16) (gs->unk_014[D_8011ECA0] + arg4 + radius);
                extent = (-arg4 - radius) + 0xA0;
                tile->y0 = (s16) (gs->unk_018[D_8011ECA0] - 0x78);
                tile->w = extent;
                tile->h = 0xF0;
                if (extent > 0) {
                    AddPrim(&D_800E8CA0[(D_8011ECA0 << 0xA) + order_offset], tile);
                }
                break;
            }
            quadrant += 1;
        } while (quadrant != 4);
    }
}
