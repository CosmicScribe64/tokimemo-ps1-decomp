/* name_entry_and_starfield.c: src/ovl/TACO/80139170.c:4: score name entry plus glyph drawing and starfield update/drawing; original object kept intact; body 5: switch (game_state.mode_step) {; 7: shooter_name_entry_step_init();. Original object 0x80139170-0x8013B170 (config/objects/TACO.txt). */
#include "common.h"
#include "ovl/shooter_3d.h"

void shooter_stage_name_entry(void) {
    switch (game_state.mode_step) {
    case 0:
        shooter_name_entry_step_init();
        return;
    case 1:
        shooter_name_entry_step_intro();
        return;
    case 2:
        shooter_name_entry_step_input();
        return;
    case 3:
        shooter_name_entry_step_outro();
        return;
    case 4:
        shooter_name_entry_step_fade();
        return;
    case 5:
        shooter_name_entry_step_done();
        return;
    }
}

void shooter_name_entry_step_init(void) {
    s32 previous;

    shooter_name_entry_reset();
    shooter_init_stars();
    audio_bgm_play_short(0, 0x202);
    game_state.clear_color_r = 0;
    game_state.clear_color_g = 0;
    game_state.clear_color_b = 0;
    back_clear_switch(1);
    shooter_background_flags = 0;
    /* FAKE: the scalar address view and shared source line retain the two
     * score-load registers and their order. Original spelling unknown. T-9530 */
    view_model_slots_clear(); previous = *(s32 *)&shooter_high_score_1;
    if (previous < shooter_score) {
        shooter_name_entry_rank = 0;
        shooter_high_score_3 = shooter_high_score_2;
        shooter_high_score_1 = shooter_score;
        shooter_high_score_2 = previous;
        strncpy(shooter_high_score_name_3, shooter_high_score_name_2, 4);
        strncpy(shooter_high_score_name_2, shooter_high_score_name_1, 4);
        strncpy(shooter_high_score_name_1, "   ", 4);
    } else if (shooter_high_score_2 < shooter_score) {
        shooter_name_entry_rank = 1;
        shooter_high_score_3 = shooter_high_score_2;
        shooter_high_score_2 = shooter_score;
        strncpy(shooter_high_score_name_3, shooter_high_score_name_2, 4);
        strncpy(shooter_high_score_name_2, "   ", 4);
    } else {
        shooter_name_entry_rank = 2;
        shooter_high_score_3 = shooter_score;
        strncpy(shooter_high_score_name_3, "   ", 4);
    }
    mode_step_next();
}

void shooter_name_entry_step_fade(void) {
    game_state.substep += 1;
    shooter_update_stars(0, 0);
    if ((u32)(game_state.substep * 3) >= 0x81U) {
        mode_step_next();
    }
}

void shooter_name_entry_step_done(void) {
    mode_stage_set(5);
    mode_step_next();
    mode_step_next();
}

/* Draws glyph arg0 ('0'-'9', 'A'-'Z', ". - ! @ * #", other: a fallback cell) as a POLY_FT4 centred
 * on game_state.draw_origin_x/unk_018[draw_buffer] plus (arg4, arg5), half sizes from arg6/arg7, clut row arg1, ordering-table
 * slot arg2, grey level arg3; arg8 == 3 selects the wide cell, arg8 == 1 anchors the glyph at its
 * bottom edge. Called without a prototype (K&R definition, the u8 parameter is narrowed at entry).
 * Matching notes (T-25112): every vertex coordinate is its own full expression with the offset
 * grouped first, `centre + (arg4 - dx)`: uopt then shares the centre load but not the sum, and the
 * repeated vertices become its copies (x1 and x2 stay in their stack homes) as in the original.
 * FAKE (layout): T-25112, each UV setup is one comma statement on one line, the expansion of the
 * libgpu setUVWH(p, u, v, w, 12) macro (likely the original's): the special glyphs need it for the
 * original's constant registers and store order. */
void shooter_draw_glyph(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)
u8 arg0;
s32 arg1;
s32 arg2;
s32 arg3;
s32 arg4;
s32 arg5;
s32 arg6;
s32 arg7;
s32 arg8;
{
    POLY_FT4 *p;
    s32 dx;
    s32 dy;
    s32 x0, y0, x1, y1, x2, y2, x3, y3;
    s32 w;
    u8 letter;
    u8 *uv;
    s32 pad[2]; /* FAKE: two unused words give the original's 0x68 frame and home slots; real source unknown. T-25112 */

    /* FAKE: dead store; mentioning x3 first gives it the lower uopt bit, so it takes t5 and x0 ra
     * at their equal priority, as in the original. Real source unknown. T-25112 */
    x3 = 0;
    if (arg8 == 3) {
        w = 12;
    } else {
        w = 11;
    }
    p = (POLY_FT4 *)GetWorkBase(sizeof(POLY_FT4), draw_buffer);
    func_8009F040(p);
    SetShadeTex(p, 0);
    SetSemiTrans(p, 0);
    p->b0 = p->g0 = p->r0 = arg3;
    dx = (w * arg6) / 8192;
    x0 = game_state.draw_origin_x[draw_buffer] + (arg4 - dx);
    x3 = game_state.draw_origin_x[draw_buffer] + (arg4 + dx);
    x1 = game_state.draw_origin_x[draw_buffer] + (arg4 + dx);
    x2 = game_state.draw_origin_x[draw_buffer] + (arg4 - dx);
    if (arg8 != 1) {
        dy = (arg7 * 12) / 8192;
        y0 = game_state.draw_origin_y[draw_buffer] + (arg5 - dy);
        y1 = game_state.draw_origin_y[draw_buffer] + (arg5 - dy);
        y2 = game_state.draw_origin_y[draw_buffer] + (arg5 + dy);
        y3 = game_state.draw_origin_y[draw_buffer] + (arg5 + dy);
    } else {
        y0 = game_state.draw_origin_y[draw_buffer] + (arg5 - (arg7 * 12) / 4096);
        y1 = game_state.draw_origin_y[draw_buffer] + (arg5 - (arg7 * 12) / 4096);
        y2 = game_state.draw_origin_y[draw_buffer] + arg5;
        y3 = game_state.draw_origin_y[draw_buffer] + arg5;
    }
    p->tpage = func_8009ECB0(0, 0, 0x340, 0x180);
    p->x0 = x0;
    p->y0 = y0;
    p->x1 = x1;
    p->y1 = y1;
    p->x2 = x2;
    p->y2 = y2;
    p->x3 = x3;
    p->y3 = y3;
    if (arg0 >= '0' && arg0 <= '9') {
        uv = &shooter_digit_uv[(u8)(arg0 - '0') * 2];
        p->u0 = uv[0] + 0x80, p->v0 = uv[1], p->u1 = uv[0] + 0x80 + w, p->v1 = uv[1], p->u2 = uv[0] + 0x80, p->v2 = uv[1] + 12, p->u3 = uv[0] + 0x80 + w, p->v3 = uv[1] + 12;
    } else if ((letter = arg0 - 'A') < 26) {
        uv = &shooter_letter_uv[letter * 2];
        p->u0 = uv[0] + 0x80, p->v0 = uv[1], p->u1 = uv[0] + 0x80 + w, p->v1 = uv[1], p->u2 = uv[0] + 0x80, p->v2 = uv[1] + 12, p->u3 = uv[0] + 0x80 + w, p->v3 = uv[1] + 12;
    } else if (arg0 == '.') {
        p->u0 = shooter_glyph_period_u + 0x80, p->v0 = shooter_glyph_period_v, p->u1 = shooter_glyph_period_u + 0x80 + w, p->v1 = shooter_glyph_period_v, p->u2 = shooter_glyph_period_u + 0x80, p->v2 = shooter_glyph_period_v + 12, p->u3 = shooter_glyph_period_u + 0x80 + w, p->v3 = shooter_glyph_period_v + 12;
    } else if (arg0 == '-') {
        p->u0 = shooter_glyph_hyphen_u + 0x80, p->v0 = shooter_glyph_hyphen_v, p->u1 = shooter_glyph_hyphen_u + 0x80 + w, p->v1 = shooter_glyph_hyphen_v, p->u2 = shooter_glyph_hyphen_u + 0x80, p->v2 = shooter_glyph_hyphen_v + 12, p->u3 = shooter_glyph_hyphen_u + 0x80 + w, p->v3 = shooter_glyph_hyphen_v + 12;
    } else if (arg0 == '!') {
        p->u0 = shooter_glyph_other_u + 0x80, p->v0 = shooter_glyph_other_v, p->u1 = shooter_glyph_other_u + 0x80 + w, p->v1 = shooter_glyph_other_v, p->u2 = shooter_glyph_other_u + 0x80, p->v2 = shooter_glyph_other_v + 12, p->u3 = shooter_glyph_other_u + 0x80 + w, p->v3 = shooter_glyph_other_v + 12;
    } else if (arg0 == '@') {
        p->u0 = 0xA6, p->v0 = 0xD3, p->u1 = 0xA6 + w, p->v1 = 0xD3, p->u2 = 0xA6, p->v2 = 0xD3 + 12, p->u3 = 0xA6 + w, p->v3 = 0xD3 + 12;
    } else if (arg0 == '*') {
        p->u0 = 0xBE, p->v0 = 0xD3, p->u1 = 0xBE + w, p->v1 = 0xD3, p->u2 = 0xBE, p->v2 = 0xD3 + 12, p->u3 = 0xBE + w, p->v3 = 0xD3 + 12;
    } else if (arg0 == '#') {
        p->u0 = 0xEE, p->v0 = 0xD3, p->u1 = 0xEE + w, p->v1 = 0xD3, p->u2 = 0xEE, p->v2 = 0xD3 + 12, p->u3 = 0xEE + w, p->v3 = 0xD3 + 12;
    } else {
        p->u0 = shooter_glyph_space_u + 0x80, p->v0 = shooter_glyph_space_v, p->u1 = shooter_glyph_space_u + 0x80 + w, p->v1 = shooter_glyph_space_v, p->u2 = shooter_glyph_space_u + 0x80, p->v2 = shooter_glyph_space_v + 12, p->u3 = shooter_glyph_space_u + 0x80 + w, p->v3 = shooter_glyph_space_v + 12;
    }
    p->clut = func_8009ED74(arg1 * 16, 0x1EF);
    AddPrim(draw_ot_tags + (draw_buffer << 10) + arg2 * 4, (s32)p);
    dtd_on_tpage(0x340, 0x180, arg2, 0, 0);
}

void shooter_name_entry_step_intro(void) {
    if (shooter_name_entry_intro < 0x100) {
        shooter_star_brightness = shooter_name_entry_intro / 2;
    } else {
        shooter_star_brightness = 0x80;
    }
    shooter_update_stars(0, 0);
    shooter_draw_glyph_ring(0x41, 0, 0x280 - shooter_name_entry_intro, 0);
    shooter_name_entry_intro += 4;
    if (shooter_name_entry_intro >= 0x1E1) {
        mode_step_next();
    }
}

void shooter_name_entry_step_outro(void) {
    shooter_update_stars(0, 0);
    shooter_draw_entered_name(shooter_name_entry_intro);
    shooter_draw_glyph_ring(shooter_name_ring_index, 0, 0x280 - shooter_name_entry_intro, 0);
    shooter_name_entry_intro -= 8;
    if (shooter_name_entry_intro < 0) {
        mode_step_next();
    }
}

void shooter_name_entry_step_input(void) {
    s32 temp_lo;
    s32 scale;
    s32 right;
    s32 left;
    s32 pad;

    if (shooter_name_ring_offset < 0) {
        scale = shooter_name_ring_offset + 0x50;
    } else {
        scale = 0x50 - shooter_name_ring_offset;
    }
    shooter_draw_glyph_ring(shooter_name_ring_index, shooter_name_ring_offset, 0xA0, 0);
    temp_lo = scale * 0x7000 / 80;
    shooter_draw_glyph(shooter_name_glyph_table[shooter_name_ring_index % 40], 6, 4, 0x80, 0, 0x3C, temp_lo, temp_lo, 1);
    shooter_draw_entered_name(0x1E0);
    shooter_update_stars(0, 0);
    /* pad is tested again in the zero-speed block (five uses, so it is coloured before the masks),
       and left is assigned before right; both are needed for the original's register split (T-19021) */
    pad = game_state.pad_held;
    left = pad & 0x8000;
    right = pad & 0x2000;
    if (right != 0 && shooter_name_ring_speed < 0) {
        shooter_name_ring_speed = 1;
    }
    if (left != 0 && shooter_name_ring_speed > 0) {
        shooter_name_ring_speed = -1;
    }
    switch (shooter_name_ring_speed) {
    case -2:
        shooter_name_ring_offset += 0xA;
        break;
    case -1:
        shooter_name_ring_offset += 5;
        break;
    case 1:
        shooter_name_ring_offset -= 5;
        break;
    case 2:
        shooter_name_ring_offset -= 0xA;
        break;
    }
    if (shooter_name_ring_offset == 0) {
        if (!(pad & 0xA000)) {
            shooter_name_ring_speed = 0;
        }
        if (pad & 0x2000) {
            shooter_name_ring_speed += 1;
        }
        if (pad & 0x8000) {
            shooter_name_ring_speed -= 1;
        }
        if (shooter_name_ring_speed >= 3) {
            shooter_name_ring_speed = 2;
        }
        if (shooter_name_ring_speed < -2) {
            shooter_name_ring_speed = -2;
        }
    }
    if (shooter_name_ring_offset >= 0x33) {
        shooter_name_ring_index += 1;
        shooter_name_ring_offset = -0x32;
    }
    if (shooter_name_ring_offset < -0x32) {
        shooter_name_ring_offset = 0x32;
        shooter_name_ring_index -= 1;
    }
    if (shooter_name_ring_index >= 0x28) {
        shooter_name_ring_index = 0;
    }
    if (shooter_name_ring_index < 0) {
        shooter_name_ring_index = 0x27;
    }
    if ((game_state.pad_pressed & PAD_CIRCLE) && !(game_state.pad_pressed & PAD_CROSS)) {
        if (shooter_name_ring_index == 0x27) {
            /* the original passes the byte itself as the destination (a lost &); T-19021 */
            strncpy((void *)shooter_high_score_name_1[shooter_name_entry_rank * 4 + shooter_name_entry_position], "", 1);
            mode_step_next();
        } else if (shooter_name_entry_position != 3) {
            strncpy(&shooter_high_score_name_1[shooter_name_entry_rank * 4 + shooter_name_entry_position], &shooter_name_glyph_table[shooter_name_ring_index], 1);
            shooter_name_entry_position += 1;
        }
    }
    if ((game_state.pad_pressed & PAD_START) && !(game_state.pad_pressed & PAD_CROSS)) {
        strncpy((void *)shooter_high_score_name_1[shooter_name_entry_rank * 4 + shooter_name_entry_position], "", 1);
        mode_step_next();
    }
    if ((game_state.pad_pressed & PAD_CROSS) && shooter_name_entry_position != 0) {
        strncpy(&shooter_high_score_name_1[shooter_name_entry_rank * 4 + shooter_name_entry_position - 1], " ", 1);
        shooter_name_entry_position -= 1;
    }
}

/* Draws the 40-entry ring around shooter_name_ring_index: the front entry, 19 pairs on both sides, then the back
   entry. Three source details set the original's registers (T-25112): one x for the front, the +i and
   the back entries (sine in s5), scale reused for the mirrored angle before it is reflected (v0/v1 split),
   and a for loop (a do/while puts 3 and 40 in the wrong s registers and 0x100000 in fp instead of
   the table address). */
void shooter_draw_glyph_ring(s32 arg0, s32 arg1, s32 radius, s32 arg3) {
    s32 pos;
    s32 d;
    s32 angle;
    s32 x;
    s32 x2;
    s32 z;
    s32 scale;
    s32 i;

    angle = (-shooter_name_ring_offset << 10) / 512;
    x = (rsin(angle) * radius) >> 12;
    z = (func_800A0140(angle) * radius) >> 12;
    if (z < -0x100) {
        scale = 0;
    } else {
        scale = ((z << 12) + 0x100000) / 96;
    }
    shooter_draw_glyph(shooter_name_glyph_table[shooter_name_ring_index % 40], 2, 5, ((z << 5) + 0x2800) / 160, x, (z >> 2) + 50, scale, scale, 0);
    for (i = 1; i < 20; i++) {
        pos = i * 0x1000 / 40;
        d = pos - shooter_name_ring_offset;
        if (d < 0x200) {
            angle = (d << 10) / 512;
        } else {
            angle = (d * 2 + 0x800) / 3;
        }
        x = (rsin(angle) * radius) >> 12;
        z = (func_800A0140(angle) * radius) >> 12;
        if (z < -0x100) {
            scale = 0;
        } else {
            scale = ((z << 12) + 0x100000) / 96;
        }
        shooter_draw_glyph(shooter_name_glyph_table[(shooter_name_ring_index + i) % 40], 0, 5, ((z << 5) + 0x2800) / 160, x, (z >> 2) + 50, scale, scale, 0);
        d = pos + shooter_name_ring_offset;
        if (d < 0x200) {
            scale = (d << 10) / 512;
        } else {
            scale = (d * 2 + 0x800) / 3;
        }
        angle = 0x1000 - scale;
        x2 = (rsin(angle) * radius) >> 12;
        z = (func_800A0140(angle) * radius) >> 12;
        if (z < -0x100) {
            scale = 0;
        } else {
            scale = ((z << 12) + 0x100000) / 96;
        }
        shooter_draw_glyph(shooter_name_glyph_table[(shooter_name_ring_index - i + 40) % 40], 0, 5, ((z << 5) + 0x2800) / 160, x2, (z >> 2) + 50, scale, scale, 0);
    }
    d = 0x800 - shooter_name_ring_offset;
    if (d < 0x200) {
        angle = (d << 10) / 512;
    } else {
        angle = (d * 2 + 0x800) / 3;
    }
    x = (rsin(angle) * radius) >> 12;
    z = (func_800A0140(angle) * radius) >> 12;
    if (z < -0x100) {
        scale = 0;
    } else {
        scale = ((z << 12) + 0x100000) / 96;
    }
    shooter_draw_glyph(shooter_name_glyph_table[(shooter_name_ring_index + i) % 40], 0, 5, ((z << 5) + 0x2800) / 160, x, (z >> 2) + 50, scale, scale, 0);
}

/* The y offset stays an expression: a fourth named temporary adds a frame word that moves the four
   spilled call values down by 4 (T-19021). */
void shooter_draw_entered_name(s32 arg0) {
    s32 k;
    s32 a;
    s32 b;
    s32 c;

    switch (shooter_name_entry_rank) {
    case 0:
        k = 1;
        break;
    case 1:
        k = 4;
        break;
    case 2:
        k = 3;
        break;
    }
    a = (arg0 << 7) / 480;
    b = (arg0 * 0x7000) / 480;
    c = (b * 12) / 4096;
    shooter_draw_glyph(shooter_high_score_name_1[shooter_name_entry_rank * 4], k, 3, a, -c, -0x28 - arg0 / 16, b, b, 0);
    shooter_draw_glyph(shooter_high_score_name_1[shooter_name_entry_rank * 4 + 1], k, 3, a, 0, -0x28 - arg0 / 16, b, b, 0);
    shooter_draw_glyph(shooter_high_score_name_1[shooter_name_entry_rank * 4 + 2], k, 3, a, c, -0x28 - arg0 / 16, b, b, 0);
}

void shooter_name_entry_reset(void) {
    shooter_name_entry_position = 0;
    shooter_name_ring_index = 0;
    shooter_name_ring_offset = 0;
    shooter_name_ring_speed = 0;
    shooter_name_entry_intro = 0;
}

void shooter_update_stars(s32 starfield_direction, s32 starfield_mode) {
    /* FAKE: preserve the original 0x68 frame while keeping next at sp+0x44. T-24502 */
    s32 pad[4];
    s32 divisor;
    s32 scroll;
    s32 count;
    s32 i;
    s32 next;

    divisor = 30000;
    scroll = 0;
    if (starfield_mode == 0) {
        if (game_state.pad_held & PAD_L2) divisor = 10000;
        else if (game_state.pad_held & PAD_R2) divisor = 50000;
        if (game_state.pad_held & PAD_L1) scroll = 40;
        if (game_state.pad_held & PAD_R1) scroll = -40;
    } else if (starfield_mode == 1) {
        if ((shooter_actors->mode & 1) == 1) scroll = 40;
        if ((shooter_actors->mode & 2) == 2) scroll = -40;
    } else if (starfield_mode == 2) {
        scroll = 40;
        divisor = 20000;
    } else if (starfield_mode == 3) {
        scroll = 40;
        divisor = 10000;
    }
    count = 0;
    if (scroll > 0) {
        for (i = 0; i < 100; i++) {
            shooter_stars[i].angle += scroll;
            if (shooter_stars[i].angle >= 4097) shooter_stars[i].angle -= 4096;
        }
    }
    if (scroll < 0) {
        for (i = 0; i < 100; i++) {
            shooter_stars[i].angle += scroll;
            if (shooter_stars[i].angle < 0) shooter_stars[i].angle += 4096;
        }
    }
    for (i = 0; i != 100; i++) {
        next = count + 1;
        if (shooter_stars[i].radius == 0) {
            /* FAKE: | 0 keeps the divisor/count value in one spill web. T-24502 */
            if (rand() % (next | 0) == 0) {
                count = next;
                shooter_stars[i].angle = rand() % 4096;
                if (starfield_direction == 0) shooter_stars[i].radius = 240000;
                else shooter_stars[i].radius = 1000;
            }
        } else count = next;
        if (starfield_direction == 0) {
            shooter_stars[i].radius -= shooter_stars[i].radius * 1000 / divisor;
            if (shooter_stars[i].radius < 3001) shooter_stars[i].radius = 0;
        } else {
            shooter_stars[i].radius += shooter_stars[i].radius * 1000 / divisor;
            /* FAKE: uopt counts this use, then ugen discards * 0; count gets s2 before the bound gets s3. T-24502 */
            if (shooter_stars[i].radius >= (count * 0) + 240000) shooter_stars[i].radius = 0;
        }
        if (shooter_stars[i].radius != 0) shooter_draw_star(shooter_stars[i].angle, shooter_stars[i].radius / 1000, -1, starfield_mode);
    }
}


/* r_sh and g_sh are single-use named shifts: they reserve the two frame words above the four spilled values
   that the original has (T-19021). */
void shooter_draw_star(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 r_sh;
    s32 g_sh;
    s32 r;
    s32 g;
    s32 b;
    s32 x;
    s32 y;
    u8 *p;

    r_sh = arg2 >> 16;
    g_sh = arg2 >> 8;
    r = r_sh & 0xFF;
    g = g_sh & 0xFF;
    b = arg2 & 0xFF;
    if (arg3 < 2) {
        if (arg1 < 0x80) {
            r = r * arg1 / 128;
            g = g * arg1 / 128;
            b = b * arg1 / 128;
        }
        r = r * shooter_star_brightness / 128;
        g = g * shooter_star_brightness / 128;
        b = b * shooter_star_brightness / 128;
    }
    p = GetWorkBase(0xC, draw_buffer);
    func_8009F0B8(p);
    SetSemiTrans(p, 0);
    p[4] = r;
    p[5] = g;
    p[6] = b;
    x = (arg1 * rsin(arg0) >> 12) + game_state.draw_origin_x[draw_buffer];
    if (arg3 == 0) {
        y = (arg1 * func_800A0140(arg0) >> 12) + game_state.draw_origin_y[draw_buffer] - 0x28;
    } else {
        y = (arg1 * func_800A0140(arg0) >> 12) + game_state.draw_origin_y[draw_buffer];
    }
    *(s16 *)(p + 8) = x;
    *(s16 *)(p + 0xA) = y;
    if (arg3 < 2) {
        AddPrim((draw_buffer << 10) + 0x3F8 + draw_ot_tags, p);
        return;
    }
    AddPrim((draw_buffer << 10) + 0x40 + draw_ot_tags, p);
}

void shooter_init_stars(void) {
    s32 i;

    srand(game_state.timer_ticks);
    shooter_stars = (ShooterStar *)game_state.scratch_words;
    shooter_star_brightness = 0;
    shooter_scores_fade = 0;
    for (i = 0; i < 0x64; i++) {
        if (i < 0x50) {
            if (rand() % 5 < 4) {
                shooter_stars[i].angle = rand() % 4096;
                shooter_stars[i].radius = i * 1000 * i / 30;
            } else {
                shooter_stars[i].radius = 0;
            }
        } else {
            shooter_stars[i].radius = 0;
        }
    }
}
