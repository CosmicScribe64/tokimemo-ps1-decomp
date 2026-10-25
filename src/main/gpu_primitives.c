/* gpu_primitives.c: src/main/800490C0.c; Original object 0x800490C0-0x80049FF0: primitive work-area allocation, FT4 sprite and tpage/environment primitives, pulsing light triangle and Gouraud gradient-box drawing. Original object 0x800490C0-0x80049FF0 (config/objects/main.txt). */
#include "common.h"
#include "common.h"
#include "game/draw_records.h"
#include "game/game_state.h"
#include "game_enums.h"
#include "libapi.h"
#include "libgpu.h"
#include "libgte.h"
#include "main/draw.h"
#include "main/import_views/entry_ui.h"
#include "main/prim.h"
#include "main/state.h"
#include "psx_types.h"

/* Resets a draw-buffer primitive arena to the supplied base address. */
void SetWorkBase(s32 arg0, s32 arg1) {
    game_state.prim_arena[arg1].base = arg0;
    game_state.prim_arena[arg1].size = 0x10C00;
    game_state.prim_arena[arg1].used = 0;
}

/* Allocates aligned bytes from a draw-buffer primitive arena. */
s32 GetWorkBase(s32 arg0, s32 arg1) {
    s32 ret;

    ret = game_state.prim_arena[arg1].base + (game_state.prim_arena[arg1].used * 4);
    game_state.prim_arena[arg1].used += ((arg0 + 3) / 4) * 4;
    return ret;
}

/* Emits a visible sprite quad as a textured GPU polygon. */
void sprite_set_gpu_poly_ft4(SpriteQuad *arg0, s32 arg1, s32 arg2) {
    u8 *p;

    if (!(arg0->flags & 0x80000000)) {
        p = (u8 *)GetWorkBase(0x28, arg2);
        func_8009F040(p);
        if (arg0->flags & 0x40000000) {
            SetSemiTrans(p, 1);
        } else {
            SetSemiTrans(p, 0);
        }
        SetShadeTex(p, 0);
        p[4] = arg0->tint_r;
        p[5] = arg0->tint_g;
        p[6] = arg0->tint_b;
        *(s16 *)(p + 8) = game_state.draw_origin_x[arg2] + arg0->x - arg0->anchor_x;
        *(s16 *)(p + 0xA) = game_state.draw_origin_y[arg2] + arg0->y - arg0->anchor_y;
        *(s16 *)(p + 0x10) = game_state.draw_origin_x[arg2] + arg0->x - arg0->anchor_x + (u16)arg0->w;
        *(s16 *)(p + 0x12) = game_state.draw_origin_y[arg2] + arg0->y - arg0->anchor_y;
        *(s16 *)(p + 0x18) = game_state.draw_origin_x[arg2] + arg0->x - arg0->anchor_x;
        *(s16 *)(p + 0x1A) = game_state.draw_origin_y[arg2] + arg0->y - arg0->anchor_y + arg0->h;
        *(s16 *)(p + 0x20) = game_state.draw_origin_x[arg2] + arg0->x - arg0->anchor_x + (u16)arg0->w;
        *(s16 *)(p + 0x22) = game_state.draw_origin_y[arg2] + arg0->y - arg0->anchor_y + arg0->h;
        p[0xC] = arg0->unk_0E;
        p[0xD] = arg0->unk_0F;
        p[0x14] = arg0->unk_0E + (u16)arg0->w;
        p[0x15] = arg0->unk_0F;
        p[0x1C] = arg0->unk_0E;
        p[0x1D] = arg0->unk_0F + arg0->h;
        p[0x24] = arg0->unk_0E + (u16)arg0->w;
        p[0x25] = arg0->unk_0F + arg0->h;
        *(s16 *)(p + 0x16) = (u16)arg0->unk_0C | 0x80;
        *(s16 *)(p + 0xE) = func_8009ED74(arg0->unk_10, arg0->unk_12);
        AddPrim(draw_ot_tags + (arg2 << 10) + arg1 * 4, p);
    }
}

/* Select a texture page slot */
void goto_tpage(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 work;
    u16 tpage;

    tpage = func_8009ECB0(arg4, arg3, arg0, arg1);
    work = GetWorkBase(0xC, draw_buffer);
    func_8009D294(work, 0, 0, tpage, 0);
    AddPrim(draw_ot_tags + (draw_buffer << 10) + arg2 * 4, work);
    safe_env(arg2);
}

/* Queues a one-pixel tile primitive at position zero in the draw ordering table. */
void safe_env(s32 arg0) {
    u8 *p;

    p = (u8 *)GetWorkBase(0xC, draw_buffer);
    func_8009F0B8(p);
    *(s16 *)(p + 8) = 0;
    *(s16 *)(p + 0xA) = 0;
    AddPrim(draw_ot_tags + (draw_buffer << 10) + arg0 * 4, (s32)p);
}

/* Draw a sprite from a texture page slot */
void dtd_on_tpage(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 work;

    work = GetWorkBase(0xC, draw_buffer);
    func_8009D294(work, 0, 1, func_8009ECB0(arg4, arg3, arg0, arg1), 0);
    AddPrim(draw_ot_tags + (draw_buffer << 10) + arg2 * 4, work);
    safe_env(arg2);
}

/* Queues a dither-enabled draw mode and a one-pixel tile in the draw ordering table. */
void dtd_on(s32 arg0) {
    s32 work;

    work = GetWorkBase(0xC, draw_buffer);
    func_8009D294(work, 0, 1, 0, 0);
    AddPrim(draw_ot_tags + (draw_buffer << 10) + arg0 * 4, work);
    safe_env(arg0);
}

/* Pulsing light triangle: r/g/b are the arg7 colour bytes plus a sine pulse of the frame counter.
 * Vertex 0 gets 3/5 of the colour, vertices 1 and 2 the black edge (x * 0 / 5, which IDO keeps as a
 * division). With arg4 == 0 the base edge is x1/x2 = arg2 -/+ a sine width at height arg3; otherwise
 * arg4/arg5 give the third vertex.
 * Matching notes (T-25115): r, g and b each have a second use in the edge colours, so uopt reloads
 * all three into registers after the calls; that takes t0-t3 out of ugen's ring for the whole function.
 * w is declared first: it owns the unused top slot 0x3C, the others take 0x38..0x28. */
void _sprite_set_light_effect1(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8) {
    s32 w;
    POLY_G3 *p;
    s32 r;
    s32 g;
    s32 b;
    s32 s;

    r = ((rsin(game_state.frame_count * 0xFF0) << 5) / 4096) + (((u32)arg7 >> 16) & 0xFF);
    g = ((rsin(game_state.frame_count * 0xFF0) << 5) / 4096) + (((u32)arg7 >> 8) & 0xFF);
    b = ((rsin(game_state.frame_count * 0xFF0) << 5) / 4096) + (arg7 & 0xFF);
    /* FAKE: T-25115, signed sum; with the u32 field uopt evaluates the frame counter before arg8. */
    s = abs(rsin(((s32)game_state.frame_count + arg8) * 0xFF0));
    p = (POLY_G3 *)GetWorkBase(0x1C, draw_buffer);
    func_8009F004(p);
    SetSemiTrans(p, 1);
    p->r0 = r * 3 / 5;
    p->g0 = g * 3 / 5;
    p->b0 = b * 3 / 5;
    /* FAKE (layout): T-25115, one comma statement per vertex in setRGB1/setRGB2 field order: as1 then
     * stores b, g, r while r keeps v1. One statement per field gives r, g, b stores (4 rows). */
    p->r1 = r * 0 / 5, p->g1 = g * 0 / 5, p->b1 = b * 0 / 5;
    p->r2 = r * 0 / 5, p->g2 = g * 0 / 5, p->b2 = b * 0 / 5;
    if (arg4 == 0) {
        p->x0 = game_state.draw_origin_x[draw_buffer] + arg0;
        p->y0 = game_state.draw_origin_y[draw_buffer] + arg1;
        w = s * 8 / 4096;
        p->x1 = game_state.draw_origin_x[draw_buffer] + arg2 - w;
        p->y1 = game_state.draw_origin_y[draw_buffer] + arg3;
        p->x2 = game_state.draw_origin_x[draw_buffer] + arg2 + w;
        p->y2 = game_state.draw_origin_y[draw_buffer] + arg3;
    } else {
        p->x0 = game_state.draw_origin_x[draw_buffer] + arg0;
        p->y0 = game_state.draw_origin_y[draw_buffer] + arg1;
        p->x1 = game_state.draw_origin_x[draw_buffer] + arg2;
        p->y1 = game_state.draw_origin_y[draw_buffer] + arg3;
        p->x2 = game_state.draw_origin_x[draw_buffer] + arg4;
        p->y2 = game_state.draw_origin_y[draw_buffer] + arg5;
    }
    AddPrim(&draw_ot_tags[(draw_buffer << 10) + arg6 * 4], (s32)p);
}

/* Gouraud box (POLY_G4) at arg0/arg1, size arg2 x arg3, in OT slot arg4. arg5 is the 0xRRGGBB colour; bit 7 of
 * arg6 makes it semi-transparent and its low bits pick the vertex shading (black vertices, the plain colour, or
 * scaled colours: case 3 stores g * 8 in the blue channel, the default case has no / 10).
 * Matching notes (T-25115): the definition takes full words (lw from the argument homes), so main_api.h declares
 * it with s32 parameters. gs is set before the calls: its reads in the coordinate block then count toward uopt's
 * 20-reference block limit and the block ends before y2, where the original reloads arg3. The black vertices are
 * written as channel * 0 (as the black edge of _sprite_set_light_effect1): with these reads cases 6, 5 and 4 also
 * pass the limit, and the three extra blocks raise the cost of a first saved register above that of the stack
 * saves around the calls, so p stays in a2 as in the original. */
void prim_gradient_rect(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    GameState *gs;
    POLY_G4 *p;
    s32 r;
    s32 g;
    s32 b;

    /* FAKE: T-25115, gs alias of game_state assigned before the calls: it ends a uopt block before y2. */
    gs = &game_state;
    p = (POLY_G4 *)GetWorkBase(0x24, draw_buffer);
    func_8009F054(p);
    if (arg6 & 0x80) {
        SetSemiTrans(p, 1);
    } else {
        SetSemiTrans(p, 0);
    }
    r = ((u32)arg5 >> 16) & 0xFF;
    g = ((u32)arg5 >> 8) & 0xFF;
    b = arg5 & 0xFF;
    switch (arg6 & 0x7F) {
    case 6:
        /* FAKE: T-25115, black vertices as channel * 0 (cases 6, 5, 4): the extra uopt blocks keep p in a2. */
        p->r0 = r * 0, p->g0 = g * 0, p->b0 = b * 0;
        p->r1 = r, p->g1 = g, p->b1 = b;
        p->r2 = r * 0, p->g2 = g * 0, p->b2 = b * 0;
        p->r3 = r, p->g3 = g, p->b3 = b;
        break;
    case 5:
        p->r0 = r, p->g0 = g, p->b0 = b;
        p->r1 = r * 0, p->g1 = g * 0, p->b1 = b * 0;
        p->r2 = r, p->g2 = g, p->b2 = b;
        p->r3 = r * 0, p->g3 = g * 0, p->b3 = b * 0;
        break;
    case 4:
        p->r0 = r * 0, p->g0 = g * 0, p->b0 = b * 0;
        p->r1 = r * 0, p->g1 = g * 0, p->b1 = b * 0;
        p->r2 = r, p->g2 = g, p->b2 = b;
        p->r3 = r, p->g3 = g, p->b3 = b;
        break;
    case 3:
        /* FAKE: T-25115, (u8) of a channel that is already 0..255. The narrowing costs one ugen temporary
         * that as1 removes again, as the original spends one before each shift; plain r * 8 is two ring slots
         * short and shifts every temporary after this case (272 rows). */
        p->r3 = (u8)r * 8, p->g3 = (u8)g * 8, p->b3 = (u8)g * 8;
        p->r2 = (u8)r * 8, p->g2 = (u8)g * 8, p->b2 = (u8)g * 8;
        p->r1 = (u8)r * 8, p->g1 = (u8)g * 8, p->b1 = (u8)g * 8;
        p->r0 = (u8)r * 8, p->g0 = (u8)g * 8, p->b0 = (u8)g * 8;
        break;
    case 1:
        p->r3 = r, p->g3 = g, p->b3 = b;
        p->r2 = r, p->g2 = g, p->b2 = b;
        p->r1 = r, p->g1 = g, p->b1 = b;
        p->r0 = r, p->g0 = g, p->b0 = b;
        break;
    case 2:
        p->r3 = r * 8 / 10, p->g3 = g * 8 / 10, p->b3 = b * 8 / 10;
        p->r0 = p->r2 = p->r1 = r * 10 / 10;
        p->g0 = p->g2 = p->g1 = g * 10 / 10;
        p->b0 = p->b2 = p->b1 = b * 10 / 10;
        break;
    case 0:
    default:
        p->r3 = r * 4, p->g3 = g * 4, p->b3 = b * 4;
        p->r2 = r * 6, p->g2 = g * 6, p->b2 = b * 6;
        p->r1 = r * 8, p->g1 = g * 8, p->b1 = b * 8;
        p->r0 = r * 10, p->g0 = g * 10, p->b0 = b * 10;
        break;
    }
    p->x0 = gs->draw_origin_x[draw_buffer] + arg0;
    p->y0 = gs->draw_origin_y[draw_buffer] + arg1;
    p->x1 = gs->draw_origin_x[draw_buffer] + arg0 + arg2;
    p->y1 = gs->draw_origin_y[draw_buffer] + arg1;
    p->x2 = gs->draw_origin_x[draw_buffer] + arg0;
    p->y2 = gs->draw_origin_y[draw_buffer] + arg1 + arg3;
    p->x3 = gs->draw_origin_x[draw_buffer] + arg0 + arg2;
    p->y3 = gs->draw_origin_y[draw_buffer] + arg1 + arg3;
    AddPrim(draw_ot_tags + (draw_buffer << 10) + arg4 * 4, (s32)p);
}
