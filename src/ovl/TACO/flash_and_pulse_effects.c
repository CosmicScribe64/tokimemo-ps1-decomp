/* flash_and_pulse_effects.c: src/ovl/TACO/80153AA0.c:4: circle flash, full-screen flash, pulse bomb visuals; body 9: shooter_build_circle_fan(buf, &x, arg2, arg0); 10: p = GetWorkBase(0xC, draw_buffer);. Original object 0x80153AA0-0x80154230 (config/objects/TACO.txt). */
#include "common.h"
#include "ovl/TACO.h"

void shooter_draw_circle_flash(s32 arg0, s32 arg1, s32 arg2) {
    POLY_G3 *buf[0x28]; /* shooter_build_circle_fan fills one POLY_G3 pointer per fan triangle (0xA0 bytes, the original frame). T-19018 */
    s32 x;
    s32 p;

    shooter_build_circle_fan(buf, &x, arg2, arg0);
    p = GetWorkBase(0xC, draw_buffer);
    func_8009D294(p, 0, 1, func_8009ECB0(1, arg1, 0, 0), 0);
    AddPrim(draw_buffer * 0x400 + 0x10 + D_800E8CA0, p);
    safe_env(4);
}

/* FAKE: integer return type with fallthrough preserves the original loop/result view.
 * T-24603. */
s32 shooter_build_circle_fan(buf, arg1, col, r)
POLY_G3 **buf;
s32 *arg1;
u32 col;
s32 r;
{
    SVECTOR v[42];
    s32 i;
    u8 cr;
    u8 cg;
    u8 cb;
    u8 dr;
    u8 dg;
    u8 db;
    POLY_G3 **p;

    for (i = 0; i < 42; i++) {
        if (i == 0) {
            v[0].vx = 0x100;
            v[0].vy = 0x78;
            v[0].vz = 0;
        } else {
            v[i].vx = func_800A0140(i * 0x9000 / 0x168) * r / 4096 + 0x100;
            v[i].vy = rsin(i * 0x9000 / 0x168) * r / 4096 + 0x78;
            v[i].vz = 0;
        }
    }
    cr = (col >> 16) & 0xFF;
    cg = (col >> 8) & 0xFF;
    cb = col & 0xFF;
    /* FAKE: this empty combined test preserves the blue-byte spill at sp+0x5C;
     * without it the compiler uses sp+0x54. T-24603. */
    if (cr | cg | cb) {}
    p = buf;
    for (i = 0; i != 40; i++) {
        *p = (POLY_G3 *)GetWorkBase(sizeof(POLY_G3), draw_buffer);
        func_8009F004(*p);
        SetSemiTrans(*p, 1);
        SetShadeTex(*p, 1);
        if (draw_buffer == 0) {
            (*p)->x0 = v[i + 1].vx;
            (*p)->y0 = v[i + 1].vy + 0xF0;
            (*p)->x1 = v[0].vx;
            (*p)->y1 = v[0].vy + 0xF0;
            (*p)->x2 = v[i + 2].vx;
            (*p)->y2 = v[i + 2].vy + 0xF0;
        }
        if (draw_buffer == 1) {
            (*p)->x0 = v[i + 1].vx;
            (*p)->y0 = v[i + 1].vy;
            (*p)->x1 = v[0].vx;
            (*p)->y1 = v[0].vy;
            (*p)->x2 = v[i + 2].vx;
            (*p)->y2 = v[i + 2].vy;
        }
        dr = (cr & 0xFF) - 0x25;
        dg = (cg & 0xFF) - 0x25;
        db = (cb & 0xFF) - 0x25;
        (*p)->r0 = dr;
        (*p)->g0 = dg;
        (*p)->b0 = db;
        (*p)->r1 = cr;
        (*p)->g1 = cg;
        (*p)->b1 = cb;
        (*p)->r2 = dr;
        (*p)->g2 = dg;
        (*p)->b2 = db;
        AddPrim(draw_buffer * 0x400 + 0x10 + D_800E8CA0, (s32)*p);
        p++;
    }
}

void shooter_draw_flash_overlay(s32 arg0, u8 arg1, u8 arg2, u8 arg3) {
    POLY_F4 *poly;
    s32 sprite;

    poly = GetWorkBase(0x18, draw_buffer);
    func_8009F02C(poly);
    SetSemiTrans(poly, 1);
    SetShadeTex(poly, 1);
    poly->x0 = 0;
    poly->y0 = game_state.screen_center_y[draw_buffer] - 0x78;
    poly->x1 = 0x200;
    poly->y1 = game_state.screen_center_y[draw_buffer] - 0x78;
    poly->x2 = 0;
    poly->y2 = game_state.screen_center_y[draw_buffer] + 0x78;
    poly->x3 = 0x200;
    poly->y3 = game_state.screen_center_y[draw_buffer] + 0x78;
    poly->r0 = arg1;
    poly->g0 = arg2;
    poly->b0 = arg3;
    AddPrim(draw_buffer * 0x400 + 0x14 + D_800E8CA0, poly);
    sprite = GetWorkBase(0xC, draw_buffer);
    func_8009D294(sprite, 0, 0, func_8009ECB0(1, arg0, 0, 0), 0);
    AddPrim(draw_buffer * 0x400 + 0x14 + D_800E8CA0, sprite);
    safe_env(5);
}

s32 shooter_update_pulse_bomb(void) {
    s32 t;
    s32 v;

    shooter_star_brightness = 0x80;
    t = shooter_pulse_bomb_timer;
    if (t < 0x32) {
        shooter_update_stars(0, 2);
    } else if (t >= 0x32 && t < 0xDC) {
        shooter_update_stars(0, 3);
    }
    if (t >= 0x32 && t < 0x5A) {
        shooter_draw_circle_flash(t * 3 - 0x96, 2, 0xA0A0A0);
    }
    if (t >= 0x5A && t < 0x96) {
        shooter_draw_circle_flash(0x78, 2, 0xA0A0A0);
    }
    if (t >= 0x96 && t < 0xB4) {
        shooter_draw_circle_flash(0x10E - t, 2, 0xA0A0A0);
    }
    if (t >= 0xB4 && t < 0xD2) {
        shooter_draw_circle_flash(0x276 - t * 3, 2, 0xA0A0A0);
    }
    if (t < 0x32) {
        v = t * 2;
        shooter_draw_flash_overlay(2, v, v, v);
    } else if (t >= 0x32 && t < 0xD2) {
        if ((t - 0x32) % 3 == 0) {
            shooter_draw_flash_overlay(1, 0xA0, 0xA0, 0xC0);
        } else {
            shooter_draw_flash_overlay(2, 0x64, 0x64, 0x64);
        }
    } else if (t >= 0xD2 && t < 0x104) {
        v = 0x208 - t * 2;
        shooter_draw_flash_overlay(2, v, v, v);
    }
    if (t == 0x104) {
        shooter_pulse_bomb_timer = 0;
        shooter_pulse_bomb_active = 0;
    } else {
        shooter_pulse_bomb_timer += 1;
    }
    return shooter_pulse_bomb_active;
}
