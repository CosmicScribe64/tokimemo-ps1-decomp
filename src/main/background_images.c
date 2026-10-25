/* background_images.c: src/main/80057390.c; MDEC still-background image cache and decode/upload, brightness/draw selection and screen capture; current dec_bg_* helpers implement the picture pipeline. New filename uses English words rather than the unlisted dec abbreviation. Original object 0x80057390-0x80058D20 (config/objects/main.txt). */
#include "common.h"
#include "game.h"
#include "psx_types.h"
/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
s32 bg_lba_cache[4] = { 0, 0, 0, 0 };
s32 D_800B5930 = 0;
s32 D_800B5934 = 0;
u8 dec_bg_show[4] = { 0 };
u8 bg_brightness = 0;
u8 bg_fade_amount = 0;
s32 bg_wipe_count = 0;
s32 dec_bg_last_screen_mode = 0;
s32 bg_decode_result = 0;

/* Set the brightness tint of the decoded backgrounds */
void set_dec_bri(u8 arg0) {
    bg_brightness = arg0;
}

/* Return the last screen mode (selects the background draw variant) */
s32 get_last_gamen_mode(void) {
    return dec_bg_last_screen_mode;
}

/* Clear the background cache, show flags and brightness (0x80) */
void dec_bg_reset(void) {
    bg_lba_cache[3] = 0;
    bg_lba_cache[1] = 0;
    bg_lba_cache[2] = 0;
    bg_lba_cache[0] = 0;
    dec_bg_show[1] = 0;
    dec_bg_show[0] = 0;
    bg_brightness = 0x80;
    dec_bg_last_screen_mode = -1;
    D_800B5934 = 0;
    dec_bg_show[2] = 0;
}

/* Toggles which VRAM buffer of a background layer is shown. */
void dec_bg_show_switch(s32 arg0) {
    dec_bg_show[arg0] = 1 - dec_bg_show[arg0];
}

/* Selects which of a background layer's two VRAM buffers is shown (0 or 1). */
void dec_bg_show_set(s32 arg0, s32 arg1) {
    dec_bg_show[arg0] = arg1 & 1;
}

/* Start reading a compressed background file unless one of the two cached LBAs of the slot already holds it; returns
 * 0/1 for a cache hit slot, -1 for a read started */
s32 dec_bg_cd_read(s32 arg0, s32 arg1) {
    if (arg1 == 1) {
        if (arg0 == bg_lba_cache[2]) {
            return 0;
        }
        if (arg0 == bg_lba_cache[3]) {
            return 1;
        }
        cd_read_sectors_nowait(0xA, 0x80162000, arg0);
        *(s32 *)((u8 *)&bg_lba_cache[3] + -(D_800B5939 * 4)) = arg0;
        return -1;
    }
    if (arg0 == bg_lba_cache[0]) {
        return 0;
    }
    if (arg0 == bg_lba_cache[1]) {
        return 1;
    }
    cd_read_sectors_nowait(0xA, 0x80162000, arg0);
    *(s32 *)((u8 *)&bg_lba_cache[1] + -(dec_bg_show[0] * 4)) = arg0;
    return -1;
}

s32 dec_bg_decode_to_vram(s32 arg0) {
    bg_decode_result = 0;
    bg_still_buffers_set(0x80167000, 0x80162000, 0x80162000);
    if (arg0 == 0) {
        switch (1 - dec_bg_show[arg0]) {
        case 0:
            bg_decode_result = bg_still_decode_to_vram(0x2C0, 0xA0, 0x140, 0x58);
            return 0;
        case 1:
            bg_decode_result = bg_still_decode_to_vram(0x140, 0x1A0, 0x140, 0x58);
            return 1;
        }
    } else if (arg0 == 1) {
        switch (1 - dec_bg_show[arg0]) {
        case 0:
            bg_decode_result = bg_still_decode_to_vram(0x2C0, 0, 0x140, 0xA0);
            return 0;
        case 1:
            bg_decode_result = bg_still_decode_to_vram(0x140, 0x100, 0x140, 0xA0);
            return 1;
        }
    }
}

void bg_capture_screen(s32 arg0) {
    s32 v;
    RECT rect;

    rect.x = 0;
    rect.y = draw_buffer * 0xF0;
    rect.w = 0x140;
    rect.h = arg0;
    v = 1 - (s32)D_800B5939;
    (&bg_lba_cache[2])[v] = 0;
    switch (v) {
    case 0:
        func_8009C93C(&rect, 0x2C0, 0);
        v = 1 - (s32)D_800B5939;
        break;
    case 1:
        func_8009C93C(&rect, 0x140, 0x100);
        v = 1 - (s32)D_800B5939;
        break;
    }
    /* FAKE: the cast makes IDO recompute 1 - (s32)D_800B5939 for the argument instead of passing v (decomp-permuter, score 0); real source unknown. T-4090 */
    dec_bg_show_set(1, (u32) (1 - (s32)D_800B5939));
    bg_wipe_count = 0;
    func_8009C674(0);
}

s32 bg_wipe_step(void) {
    s16 width;
    s16 x;
    s16 band;
    s32 doubled;
    s32 offset;
    s32 edge;
    s32 point;

    bg_brightness -= 8;
    if (bg_brightness >= 0x81U) {
        bg_brightness = 0;
    }
    bg_wipe_count++;
    doubled = bg_wipe_count * 2;
    width = doubled;
    if ((s16)doubled > 0x20) {
        width = 0x20;
    }
    band = 0;
    do {
        offset = band << 5;
        x = 0;
        if (offset < 0xA0) {
            edge = offset + 0x1F;
            while (x < width) {
                point = edge - x;
                prim_gradient_line((s16)(point - 0xA0), -0x78,
                             -0xA0, (s16)(point - 0x78), 0, 0, 0xD);
                x++;
            }
        } else if (offset < 0x140) {
            edge = offset + 0x1F;
            while (x < width) {
                point = edge - x;
                prim_gradient_line((s16)(point - 0xA0), -0x78,
                             (s16)(point - 0x140), 0x28, 0, 0, 0xD);
                x++;
            }
        } else {
            edge = offset + 0x1F;
            while (x < width) {
                point = edge - x;
                prim_gradient_line(0xA0, (s16)(point - 0x1B8),
                             (s16)(point - 0x140), 0x28, 0, 0, 0xD);
                x++;
            }
        }
        band++;
    } while (band < 0xF);
}

void dec_bg_draw(s32 a) {
    u8 *mode_flags;
    SPRT *sprite;
    u8 *flags;

    if (a != -1) {
        dec_bg_last_screen_mode = a;
        sprite = (SPRT *)GetWorkBase(0x14, draw_buffer);
        func_8009F0A4(sprite);
        SetSemiTrans(sprite, 0);
        SetShadeTex(sprite, 0);
        sprite->r0 = (u8) bg_brightness;
        sprite->g0 = (u8) bg_brightness;
        sprite->b0 = (u8) bg_brightness;
        if (a == 1) {
            sprite->x0 = (s16) (game_state.draw_origin_x[draw_buffer] - 0xA0);
            sprite->y0 = (s16) (game_state.draw_origin_y[draw_buffer] - 0x78);
            sprite->v0 = 0;
            sprite->u0 = 0;
            setWH(sprite, 0xC0, 0xA0);
        } else if (a == 2) {
            sprite->x0 = (s16) (game_state.draw_origin_x[draw_buffer] - 0xA0);
            sprite->y0 = (s16) (game_state.draw_origin_y[draw_buffer] - 0x78);
            sprite->v0 = 0;
            sprite->u0 = 0;
            setWH(sprite, 0xC0, 0xF0);
        } else {
            sprite->x0 = (s16) (game_state.draw_origin_x[draw_buffer] - 0xA0);
            sprite->y0 = (s16) (game_state.draw_origin_y[draw_buffer] - 0x30);
            sprite->v0 = 0xA0;
            sprite->u0 = 0;
            setWH(sprite, 0xC0, 0x58);
        }
        AddPrim((draw_buffer << 0xA) + 0x34 + draw_ot_tags, sprite);
        flags = &dec_bg_show[a];
        mode_flags = flags;
        if (*flags != 0) {
            goto_tpage(0x140, 0x100, 0xD, 0, 2);
        } else {
            goto_tpage(0x2C0, 0, 0xD, 0, 2);
        }
        sprite = (SPRT *)GetWorkBase(0x14, draw_buffer);
        func_8009F0A4(sprite);
        SetSemiTrans(sprite, 0);
        SetShadeTex(sprite, 0);
        sprite->r0 = (u8) bg_brightness;
        sprite->g0 = (u8) bg_brightness;
        sprite->b0 = (u8) bg_brightness;
        if (a == 1) {
            sprite->x0 = (s16) (game_state.draw_origin_x[draw_buffer] + 0x20);
            sprite->y0 = (s16) (game_state.draw_origin_y[draw_buffer] - 0x78);
            sprite->v0 = 0;
            sprite->u0 = 0;
            setWH(sprite, 0x80, 0xA0);
        } else if (a == 2) {
            sprite->x0 = (s16) (game_state.draw_origin_x[draw_buffer] + 0x20);
            sprite->y0 = (s16) (game_state.draw_origin_y[draw_buffer] - 0x78);
            sprite->v0 = 0;
            sprite->u0 = 0;
            setWH(sprite, 0x80, 0xF0);
        } else {
            sprite->x0 = (s16) (game_state.draw_origin_x[draw_buffer] + 0x20);
            sprite->y0 = (s16) (game_state.draw_origin_y[draw_buffer] - 0x30);
            sprite->v0 = 0xA0;
            sprite->u0 = 0;
            setWH(sprite, 0x80, 0x58);
        }
        AddPrim((draw_buffer << 0xA) + 0x34 + draw_ot_tags, sprite);
        if (*mode_flags != 0) {
            goto_tpage(0x200, 0x100, 0xD, 0, 2);
            return;
        }
        goto_tpage(0x380, 0, 0xD, 0, 2);
    }
}

void bg_fade_set(u8 arg0) {
    bg_fade_amount = arg0;
}

void bg_fade_draw(s32 arg0) {
    /* FAKE: unused word retains the original spill home and frame. T-14200. */
    s32 pad;
    u8 *mode_flags;
    SPRT *sprite;
    u8 *flags;

    sprite = (SPRT *)GetWorkBase(0x14, draw_buffer);
    func_8009F0A4(sprite);
    SetSemiTrans(sprite, 1);
    SetShadeTex(sprite, 0);
    sprite->r0 = (0x80 - bg_fade_amount);
    sprite->g0 = (0x80 - bg_fade_amount);
    sprite->b0 = (0x80 - bg_fade_amount);
    if (arg0 == 1) {
        sprite->x0 = (s16) (game_state.draw_origin_x[draw_buffer] - 0xA0), sprite->y0 = (s16) (game_state.draw_origin_y[draw_buffer] - 0x78);
        sprite->u0 = 0, sprite->v0 = 0;
        setWH(sprite, 0xC0, 0xA0);
    } else {
        sprite->x0 = (s16) (game_state.draw_origin_x[draw_buffer] - 0xA0), sprite->y0 = (s16) (game_state.draw_origin_y[draw_buffer] - 0x30);
        sprite->u0 = 0, sprite->v0 = 0xA0;
        setWH(sprite, 0xC0, 0x58);
    }
    AddPrim((draw_buffer << 0xA) + 0x28 + draw_ot_tags, sprite);
    flags = &dec_bg_show[arg0];
    mode_flags = flags;
    if (*flags != 0) {
        goto_tpage(0x140, 0x100, 0xA, 1, 2);
    } else {
        goto_tpage(0x2C0, 0, 0xA, 1, 2);
    }
    sprite = (SPRT *)GetWorkBase(0x14, draw_buffer);
    func_8009F0A4(sprite);
    SetSemiTrans(sprite, 1);
    SetShadeTex(sprite, 0);
    sprite->r0 = (0x80 - bg_fade_amount);
    sprite->g0 = (0x80 - bg_fade_amount);
    sprite->b0 = (0x80 - bg_fade_amount);
    if (arg0 == 1) {
        sprite->x0 = (s16) (game_state.draw_origin_x[draw_buffer] + 0x20), sprite->y0 = (s16) (game_state.draw_origin_y[draw_buffer] - 0x78);
        sprite->u0 = 0, sprite->v0 = 0;
        setWH(sprite, 0x80, 0xA0);
    } else {
        sprite->x0 = (s16) (game_state.draw_origin_x[draw_buffer] + 0x20), sprite->y0 = (s16) (game_state.draw_origin_y[draw_buffer] - 0x30);
        sprite->u0 = 0, sprite->v0 = 0xA0;
        setWH(sprite, 0x80, 0x58);
    }
    AddPrim((draw_buffer << 0xA) + 0x28 + draw_ot_tags, sprite);
    if (*mode_flags != 0) {
        goto_tpage(0x200, 0x100, 0xA, 1, 2);
    } else {
        goto_tpage(0x380, 0, 0xA, 1, 2);
    }
    sprite = (SPRT *)GetWorkBase(0x14, draw_buffer);
    func_8009F0A4(sprite);
    SetSemiTrans(sprite, 0);
    SetShadeTex(sprite, 0);
    sprite->r0 = (u8) bg_fade_amount;
    sprite->g0 = (u8) bg_fade_amount;
    sprite->b0 = (u8) bg_fade_amount;
    if (arg0 == 1) {
        sprite->x0 = (s16) (game_state.draw_origin_x[draw_buffer] - 0xA0), sprite->y0 = (s16) (game_state.draw_origin_y[draw_buffer] - 0x78);
        sprite->u0 = 0, sprite->v0 = 0;
        setWH(sprite, 0xC0, 0xA0);
    } else {
        sprite->x0 = (s16) (game_state.draw_origin_x[draw_buffer] - 0xA0), sprite->y0 = (s16) (game_state.draw_origin_y[draw_buffer] - 0x30);
        sprite->u0 = 0, sprite->v0 = 0xA0;
        setWH(sprite, 0xC0, 0x58);
    }
    AddPrim((draw_buffer << 0xA) + 0x34 + draw_ot_tags, sprite);
    if (*mode_flags != 0) {
        goto_tpage(0x140, 0x100, 0xD, 0, 2);
    } else {
        goto_tpage(0x2C0, 0, 0xD, 0, 2);
    }
    sprite = (SPRT *)GetWorkBase(0x14, draw_buffer);
    func_8009F0A4(sprite);
    SetSemiTrans(sprite, 0);
    SetShadeTex(sprite, 0);
    sprite->r0 = (u8) bg_fade_amount;
    sprite->g0 = (u8) bg_fade_amount;
    sprite->b0 = (u8) bg_fade_amount;
    if (arg0 == 1) {
        sprite->x0 = (s16) (game_state.draw_origin_x[draw_buffer] + 0x20), sprite->y0 = (s16) (game_state.draw_origin_y[draw_buffer] - 0x78);
        sprite->u0 = 0, sprite->v0 = 0;
        setWH(sprite, 0x80, 0xA0);
    } else {
        sprite->x0 = (s16) (game_state.draw_origin_x[draw_buffer] + 0x20), sprite->y0 = (s16) (game_state.draw_origin_y[draw_buffer] - 0x30);
        sprite->u0 = 0, sprite->v0 = 0xA0;
        setWH(sprite, 0x80, 0x58);
    }
    AddPrim((draw_buffer << 0xA) + 0x34 + draw_ot_tags, sprite);
    if (*mode_flags != 0) {
        goto_tpage(0x200, 0x100, 0xD, 0, 2);
        return;
    }
    goto_tpage(0x380, 0, 0xD, 0, 2);
}

void bg_crossfade_draw(s32 arg0) {
    /* FAKE: unused word retains the original spill home and frame. T-14200. */
    s32 pad;
    u8 *mode_flags;
    SPRT *sprite;
    u8 *flags;

    sprite = (SPRT *)GetWorkBase(0x14, draw_buffer);
    func_8009F0A4(sprite);
    SetSemiTrans(sprite, 1);
    SetShadeTex(sprite, 0);
    sprite->r0 = (0x80 - bg_fade_amount);
    sprite->g0 = (0x80 - bg_fade_amount);
    sprite->b0 = (0x80 - bg_fade_amount);
    if (arg0 == 1) {
        sprite->x0 = (s16) (game_state.draw_origin_x[draw_buffer] - 0xA0), sprite->y0 = (s16) (game_state.draw_origin_y[draw_buffer] - 0x78);
        sprite->u0 = 0, sprite->v0 = 0;
        setWH(sprite, 0xC0, 0xA0);
    } else {
        sprite->x0 = (s16) (game_state.draw_origin_x[draw_buffer] - 0xA0), sprite->y0 = (s16) (game_state.draw_origin_y[draw_buffer] - 0x30);
        sprite->u0 = 0, sprite->v0 = 0xA0;
        setWH(sprite, 0xC0, 0x58);
    }
    AddPrim((draw_buffer << 0xA) + 0x28 + draw_ot_tags, sprite);
    flags = &dec_bg_show[arg0];
    mode_flags = flags;
    if (*flags == 0) {
        goto_tpage(0x140, 0x100, 0xA, 1, 2);
    } else {
        goto_tpage(0x2C0, 0, 0xA, 1, 2);
    }
    sprite = (SPRT *)GetWorkBase(0x14, draw_buffer);
    func_8009F0A4(sprite);
    SetSemiTrans(sprite, 1);
    SetShadeTex(sprite, 0);
    sprite->r0 = (0x80 - bg_fade_amount);
    sprite->g0 = (0x80 - bg_fade_amount);
    sprite->b0 = (0x80 - bg_fade_amount);
    if (arg0 == 1) {
        sprite->x0 = (s16) (game_state.draw_origin_x[draw_buffer] + 0x20), sprite->y0 = (s16) (game_state.draw_origin_y[draw_buffer] - 0x78);
        sprite->u0 = 0, sprite->v0 = 0;
        setWH(sprite, 0x80, 0xA0);
    } else {
        sprite->x0 = (s16) (game_state.draw_origin_x[draw_buffer] + 0x20), sprite->y0 = (s16) (game_state.draw_origin_y[draw_buffer] - 0x30);
        sprite->u0 = 0, sprite->v0 = 0xA0;
        setWH(sprite, 0x80, 0x58);
    }
    AddPrim((draw_buffer << 0xA) + 0x28 + draw_ot_tags, sprite);
    if (*mode_flags == 0) {
        goto_tpage(0x200, 0x100, 0xA, 1, 2);
    } else {
        goto_tpage(0x380, 0, 0xA, 1, 2);
    }
    sprite = (SPRT *)GetWorkBase(0x14, draw_buffer);
    func_8009F0A4(sprite);
    SetSemiTrans(sprite, 0);
    SetShadeTex(sprite, 0);
    sprite->r0 = (u8) bg_fade_amount;
    sprite->g0 = (u8) bg_fade_amount;
    sprite->b0 = (u8) bg_fade_amount;
    if (arg0 == 1) {
        sprite->x0 = (s16) (game_state.draw_origin_x[draw_buffer] - 0xA0), sprite->y0 = (s16) (game_state.draw_origin_y[draw_buffer] - 0x78);
        sprite->u0 = 0, sprite->v0 = 0;
        setWH(sprite, 0xC0, 0xA0);
    } else {
        sprite->x0 = (s16) (game_state.draw_origin_x[draw_buffer] - 0xA0), sprite->y0 = (s16) (game_state.draw_origin_y[draw_buffer] - 0x30);
        sprite->u0 = 0, sprite->v0 = 0xA0;
        setWH(sprite, 0xC0, 0x58);
    }
    AddPrim((draw_buffer << 0xA) + 0x30 + draw_ot_tags, sprite);
    if (*mode_flags != 0) {
        goto_tpage(0x140, 0x100, 0xC, 0, 2);
    } else {
        goto_tpage(0x2C0, 0, 0xC, 0, 2);
    }
    sprite = (SPRT *)GetWorkBase(0x14, draw_buffer);
    func_8009F0A4(sprite);
    SetSemiTrans(sprite, 0);
    SetShadeTex(sprite, 0);
    sprite->r0 = (u8) bg_fade_amount;
    sprite->g0 = (u8) bg_fade_amount;
    sprite->b0 = (u8) bg_fade_amount;
    if (arg0 == 1) {
        sprite->x0 = (s16) (game_state.draw_origin_x[draw_buffer] + 0x20), sprite->y0 = (s16) (game_state.draw_origin_y[draw_buffer] - 0x78);
        sprite->u0 = 0, sprite->v0 = 0;
        setWH(sprite, 0x80, 0xA0);
    } else {
        sprite->x0 = (s16) (game_state.draw_origin_x[draw_buffer] + 0x20), sprite->y0 = (s16) (game_state.draw_origin_y[draw_buffer] - 0x30);
        sprite->u0 = 0, sprite->v0 = 0xA0;
        setWH(sprite, 0x80, 0x58);
    }
    AddPrim((draw_buffer << 0xA) + 0x30 + draw_ot_tags, sprite);
    if (*mode_flags != 0) {
        goto_tpage(0x200, 0x100, 0xC, 0, 2);
    } else {
        goto_tpage(0x380, 0, 0xC, 0, 2);
    }
    sprite = (SPRT *)GetWorkBase(0x14, draw_buffer);
    func_8009F0A4(sprite);
    SetSemiTrans(sprite, 0);
    SetShadeTex(sprite, 0);
    sprite->r0 = (u8) bg_fade_amount;
    sprite->g0 = (u8) bg_fade_amount;
    sprite->b0 = (u8) bg_fade_amount;
    if (arg0 == 1) {
        sprite->x0 = (s16) (game_state.draw_origin_x[draw_buffer] - 0xA0), sprite->y0 = (s16) (game_state.draw_origin_y[draw_buffer] - 0x78);
        sprite->u0 = 0, sprite->v0 = 0;
        setWH(sprite, 0xC0, 0xA0);
    } else {
        sprite->x0 = (s16) (game_state.draw_origin_x[draw_buffer] - 0xA0), sprite->y0 = (s16) (game_state.draw_origin_y[draw_buffer] - 0x30);
        sprite->u0 = 0, sprite->v0 = 0xA0;
        setWH(sprite, 0xC0, 0x58);
    }
    AddPrim((draw_buffer << 0xA) + 0x34 + draw_ot_tags, sprite);
    if (*mode_flags == 0) {
        goto_tpage(0x140, 0x100, 0xD, 0, 2);
    } else {
        goto_tpage(0x2C0, 0, 0xD, 0, 2);
    }
    sprite = (SPRT *)GetWorkBase(0x14, draw_buffer);
    func_8009F0A4(sprite);
    SetSemiTrans(sprite, 0);
    SetShadeTex(sprite, 0);
    sprite->r0 = (u8) bg_fade_amount;
    sprite->g0 = (u8) bg_fade_amount;
    sprite->b0 = (u8) bg_fade_amount;
    if (arg0 == 1) {
        sprite->x0 = (s16) (game_state.draw_origin_x[draw_buffer] + 0x20), sprite->y0 = (s16) (game_state.draw_origin_y[draw_buffer] - 0x78);
        sprite->u0 = 0, sprite->v0 = 0;
        setWH(sprite, 0x80, 0xA0);
    } else {
        sprite->x0 = (s16) (game_state.draw_origin_x[draw_buffer] + 0x20), sprite->y0 = (s16) (game_state.draw_origin_y[draw_buffer] - 0x30);
        sprite->u0 = 0, sprite->v0 = 0xA0;
        setWH(sprite, 0x80, 0x58);
    }
    AddPrim((draw_buffer << 0xA) + 0x34 + draw_ot_tags, sprite);
    if (*mode_flags == 0) {
        goto_tpage(0x200, 0x100, 0xD, 0, 2);
        return;
    }
    goto_tpage(0x380, 0, 0xD, 0, 2);
}
