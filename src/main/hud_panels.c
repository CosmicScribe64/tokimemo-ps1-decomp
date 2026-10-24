/* hud_panels.c: src/main/80062CD0.c; T-27059: original object [0x80062CD0,0x800674B0); portrait/bustup, parameter digits, calendar date, message window and command-icon panel implementations. Original object 0x80062CD0-0x800674B0 (config/objects/main.txt). */
#define MAIN_API_OVERRIDE_prim_gradient_rect /* T-10890: matched full-width coordinate calls; narrow canonical prototype adds conversions. */
#include "common.h"
#include "game.h"
#include "psx_types.h"

void prim_gradient_rect();

void portrait_read_start(s32 arg0) {
    cd_read_sectors(0x2D, 0x80180000, arg0);
    D_8011F50F = 0;
    D_8011F4CB = 0;
    D_800B5A60 = 0;
}

void portrait_hide(s32 arg0) {
    if (arg0 == 0) {
        ((Actor *)actor_pool)[31].draw_flags &= 0x7F;
        ((Actor *)actor_pool)[30].draw_flags &= 0x7F;
        sprite_quads[0].flags |= 0x80000000;
        sprite_quads[1].flags |= 0x80000000;
        sprite_quads[2].flags |= 0x80000000;
        sprite_quads[3].flags |= 0x80000000;
        sprite_quads[4].flags |= 0x80000000;
        sprite_quads[5].flags |= 0x80000000;
    }
}

void portrait_build(s32 arg0, s32 *arg1, u8 **arg2, u8 *arg3, s32 arg4) {
    s16 x0;
    s16 y0;
    s16 w0;
    s16 h0;
    s16 x1;
    s16 y1;
    s16 origin_x;
    s16 origin_y;
    s16 w1;
    s16 h1;
    SpriteFrame *temp_v0;

    load_palette(arg0, 0x10, 1, 2, 0);
    load_tpage_buf_lock(5, arg1[0], 1);
    load_tpage_buf_lock(6, arg1[1], 1);
    load_tpage_buf_lock(7, arg1[2], 1);
    load_tpage_buf_lock(8, arg1[3], 1);
    load_tpage_buf_lock(9, arg1[4], 1);
    actor_clear(0x1F);
    ((Actor *)actor_pool)[31].unk_01 = 0xC;
    ((Actor *)actor_pool)[31].flags = 0;
    ((Actor *)actor_pool)[31].quad_flags = 0xC1000000;
    ((Actor *)actor_pool)[31].draw_flags = 0x8C;
    ((Actor *)actor_pool)[31].frame_table = (u8 *)arg2;
    ((Actor *)actor_pool)[31].sequence_table = arg3;
    ((Actor *)actor_pool)[31].tpage_table = (u8 *)arg1;
    ((Actor *)actor_pool)[31].sequence_slot = 0;
    ((Actor *)actor_pool)[31].blink_period = 2;
    ((Actor *)actor_pool)[31].unk_43 = 0x10;
    ((Actor *)actor_pool)[31].y = -0x20;
    ((Actor *)actor_pool)[31].brightness = 0;
    actor_clear(0x1E);
    ((Actor *)actor_pool)[30].unk_01 = 0xC;
    ((Actor *)actor_pool)[30].flags = 0;
    ((Actor *)actor_pool)[30].quad_flags = 0xC1000000;
    ((Actor *)actor_pool)[30].draw_flags = 0x8C;
    ((Actor *)actor_pool)[30].frame_table = (u8 *)arg2;
    ((Actor *)actor_pool)[30].sequence_table = arg3;
    ((Actor *)actor_pool)[30].tpage_table = (u8 *)arg1;
    ((Actor *)actor_pool)[30].sequence_slot = 1;
    ((Actor *)actor_pool)[30].blink_period = 2;
    ((Actor *)actor_pool)[30].unk_43 = 0x10;
    ((Actor *)actor_pool)[30].y = -0x20;
    ((Actor *)actor_pool)[30].brightness = 0;
    temp_v0 = (SpriteFrame *)*arg2;
    if (temp_v0->count >= 6) {
        origin_x = temp_v0->origin_x;
        origin_y = temp_v0->origin_y;
        x0 = temp_v0->part[5].unk_00;
        y0 = temp_v0->part[5].unk_02;
        x1 = temp_v0->part[4].unk_00;
        y1 = temp_v0->part[4].unk_02;
        sprite_quads->flags = 0x81000000;
        sprite_quads[0].w = 0x10;
        sprite_quads[0].h = 0x10;
        sprite_quads[0].unk_0E = 0x30;
        sprite_quads[0].unk_0F = 0;
        sprite_quads[0].unk_10 = 0;
        sprite_quads[0].unk_12 = 0x1F0;
        sprite_quads[0].tint_r = 0x80;
        sprite_quads[0].tint_g = 0x80;
        sprite_quads[0].tint_b = 0x80;
        sprite_quads[0].anchor_x = 0;
        sprite_quads[0].anchor_y = 0;
        sprite_quads[0].unk_1C = 0x1000;
        sprite_quads[0].unk_1E = 0x1000;
        sprite_quads[0].unk_20 = 0;
        sprite_quads[0].x = (((Actor *)actor_pool)[31].x + x0) - origin_x;
        sprite_quads[0].y = (((Actor *)actor_pool)[31].y + y0) - origin_y;
        game_state.quad_layer[0] = 0xB;
        sprite_quads[1].flags = 0x81000000;
        sprite_quads[1].w = 0x30;
        sprite_quads[1].h = 0x20;
        sprite_quads[1].unk_0E = 0;
        sprite_quads[1].unk_0F = 0;
        sprite_quads[1].unk_10 = 0;
        sprite_quads[1].unk_12 = 0x1F0;
        sprite_quads[1].tint_r = 0x80;
        sprite_quads[1].tint_g = 0x80;
        sprite_quads[1].tint_b = 0x80;
        sprite_quads[1].anchor_x = 0;
        sprite_quads[1].anchor_y = 0;
        sprite_quads[1].unk_1C = 0x1000;
        sprite_quads[1].unk_1E = 0x1000;
        sprite_quads[1].unk_20 = 0;
        sprite_quads[1].x = (((Actor *)actor_pool)[31].x + x1) - origin_x;
        sprite_quads[1].y = (((Actor *)actor_pool)[31].y + y1) - origin_y;
        game_state.quad_layer[1] = 0xB;
    }
    if (temp_v0->count >= 4) {
        x0 = temp_v0->part[2].unk_00;
        y0 = temp_v0->part[2].unk_02;
        w0 = temp_v0->part[2].unk_0A;
        h0 = temp_v0->part[2].unk_0C;
        x1 = temp_v0->part[3].unk_00;
        y1 = temp_v0->part[3].unk_02;
        w1 = temp_v0->part[3].unk_0A;
        h1 = temp_v0->part[3].unk_0C;
        sprite_quads[2].flags = -0x3F000000;
        sprite_quads[2].w = 0x10;
        sprite_quads[2].h = w0;
        sprite_quads[2].unk_0E = (s8) h0;
        sprite_quads[2].unk_0F = 0;
        sprite_quads[2].unk_10 = 0;
        sprite_quads[2].unk_12 = 0x1F0;
        sprite_quads[2].tint_r = 0x80;
        sprite_quads[2].tint_g = 0x80;
        sprite_quads[2].tint_b = 0x80;
        sprite_quads[2].anchor_x = 0;
        sprite_quads[2].anchor_y = 0;
        sprite_quads[2].unk_1C = 0x1000;
        sprite_quads[2].unk_1E = 0x1000;
        sprite_quads[2].unk_20 = 0;
        sprite_quads[2].x = (((Actor *)actor_pool)[31].x + x0) - origin_x;
        sprite_quads[2].y = (((Actor *)actor_pool)[31].y + y0) - origin_y;
        game_state.quad_layer[2] = 0x8B;
        sprite_quads[3].flags = -0x3F000000;
        sprite_quads[3].w = w1;
        sprite_quads[3].h = h1;
        sprite_quads[3].unk_0E = 0;
        sprite_quads[3].unk_0F = 0;
        sprite_quads[3].unk_10 = 0;
        sprite_quads[3].unk_12 = 0x1F0;
        sprite_quads[3].tint_r = 0x80;
        sprite_quads[3].tint_g = 0x80;
        sprite_quads[3].tint_b = 0x80;
        sprite_quads[3].anchor_x = 0;
        sprite_quads[3].anchor_y = 0;
        sprite_quads[3].unk_1C = 0x1000;
        sprite_quads[3].unk_1E = 0x1000;
        sprite_quads[3].unk_20 = 0;
        sprite_quads[3].x = (((Actor *)actor_pool)[31].x + x1) - origin_x;
        sprite_quads[3].y = (((Actor *)actor_pool)[31].y + y1) - origin_y;
        game_state.quad_layer[3] = 0x8B;
    }
    if (temp_v0->count >= 7) {
        x0 = temp_v0->part[6].unk_00;
        y0 = temp_v0->part[6].unk_02;
        w0 = temp_v0->part[6].unk_0A;
        h0 = temp_v0->part[6].unk_0C;
        sprite_quads[4].flags = -0x3F000000;
        sprite_quads[4].w = 0x10;
        sprite_quads[4].h = w0;
        sprite_quads[4].unk_0E = (s8) h0;
        sprite_quads[4].unk_0F = 0;
        sprite_quads[4].unk_10 = 0;
        sprite_quads[4].unk_12 = 0x1F0;
        sprite_quads[4].tint_r = 0x80;
        sprite_quads[4].tint_g = 0x80;
        sprite_quads[4].tint_b = 0x80;
        sprite_quads[4].anchor_x = 0;
        sprite_quads[4].anchor_y = 0;
        sprite_quads[4].unk_1C = 0x1000;
        sprite_quads[4].unk_1E = 0x1000;
        sprite_quads[4].unk_20 = 0;
        sprite_quads[4].x = (((Actor *)actor_pool)[31].x + x0) - origin_x;
        sprite_quads[4].y = (((Actor *)actor_pool)[31].y + y0) - origin_y;
        game_state.quad_layer[4] = 0x8B;
    }
    if (temp_v0->count == 8) {
        x1 = temp_v0->part[7].unk_00;
        y1 = temp_v0->part[7].unk_02;
        w1 = temp_v0->part[7].unk_0A;
        h1 = temp_v0->part[7].unk_0C;
        sprite_quads[5].flags = -0x3F000000;
        sprite_quads[5].w = w1;
        sprite_quads[5].h = h1;
        sprite_quads[5].unk_0E = 0;
        sprite_quads[5].unk_0F = 0;
        sprite_quads[5].unk_10 = 0;
        sprite_quads[5].unk_12 = 0x1F0;
        sprite_quads[5].tint_r = 0x80;
        sprite_quads[5].tint_g = 0x80;
        sprite_quads[5].tint_b = 0x80;
        sprite_quads[5].anchor_x = 0;
        sprite_quads[5].anchor_y = 0;
        sprite_quads[5].unk_1C = 0x1000;
        sprite_quads[5].unk_1E = 0x1000;
        sprite_quads[5].unk_20 = 0;
        sprite_quads[5].x = (((Actor *)actor_pool)[31].x + x1) - origin_x;
        sprite_quads[5].y = (((Actor *)actor_pool)[31].y + y1) - origin_y;
        game_state.quad_layer[5] = 0x8B;
    }
    portrait_expression_select(arg4);
    portrait_value_clear_if_high(arg4);
}

void portrait_value_clear_if_high(void) {
    if ((u32)D_800B5A64 >= 0xA5) {
        D_800B5A64 = 0;
    }
}

void portrait_expression_select(u32 arg0) {
    switch (arg0) {
    case 0:
        sprite_quads[1].unk_0C = 7;
        sprite_quads[0].unk_0C = 7;
        return;
    case 1:
        sprite_quads[1].unk_0C = 7;
        sprite_quads[1].unk_0E += 0x40;
        sprite_quads[0].unk_0C = 7;
        sprite_quads[0].unk_0E += 0x40;
        return;
    case 2:
        sprite_quads[1].unk_0C = 8;
        sprite_quads[0].unk_0C = 8;
        return;
    case 3:
        sprite_quads[1].unk_0C = 8;
        sprite_quads[1].unk_0E += 0x40;
        sprite_quads[0].unk_0C = 8;
        sprite_quads[0].unk_0E += 0x40;
        return;
    case 4:
        sprite_quads[1].unk_0C = 9;
        sprite_quads[0].unk_0C = 9;
        return;
    case 5:
        sprite_quads[1].unk_0C = 9;
        sprite_quads[1].unk_0E += 0x40;
        sprite_quads[0].unk_0C = 9;
        sprite_quads[0].unk_0E += 0x40;
        return;
    }
}

/* FAKE: the u8 view of the s8 field unk_03C gives the original lbu (as in portrait_event_face_mouth_lipsync), and the three locals declared
 * before `saved` put three unused slots above its spill slot (frame size 0x38, as TACO func_801438F0). T-17011 */
void portrait_face_update(void) {
    s32 max;
    s32 level;
    s32 n;
    s32 saved;
    s32 i;
    u8 k;
    s32 face;

    ((Actor *)actor_pool)[31].brightness = 0x80;
    ((Actor *)actor_pool)[31].quad_flags &= 0x7FFFFFFF;
    sprite_quads[0].flags &= 0x7FFFFFFF;
    sprite_quads[1].flags &= 0x7FFFFFFF;
    saved = ((Actor *)actor_pool)[30].unk_18;
    if (*(u8 *)&game_state.voice_mode != 0) {
        if (cd_audio_state() & 4) {
            level = getCDlevel() + 1;
        } else {
            level = 0;
        }
        D_80125C80[game_state.frame_count & 3] = level;
        /* max is never read; the loop is in the original code. */
        max = 0;
        for (i = 0; i < 4; i++) {
            if (max < D_80125C80[i]) {
                max = D_80125C80[i];
            }
        }
        n = 0;
        k = game_state.current_girl;
        for (i = 0; i < 3; i++) {
            if (D_800B5A68[k][i] < level) {
                n = i + 1;
            }
        }
        if (n >= 4) {
            n = 3;
        }
        if (sprite_quads[0].unk_0F == 0 || !(game_state.frame_count & 1)) {
            if (level == 0) {
                sprite_quads[0].unk_0F = 0;
            } else if (n == 0) {
                sprite_quads[0].unk_0F = 0;
            } else if (n == 1) {
                sprite_quads[0].unk_0F = 0x10;
            } else if (n == 2) {
                sprite_quads[0].unk_0F = 0x20;
            } else if (n >= 3) {
                sprite_quads[0].unk_0F = 0x30;
            }
        }
        if (game_state.pad_held & PAD_SQUARE) {
            D_800B5A98 = 0;
        }
        if (D_800B5A98 < level) {
            D_800B5A98 = level;
        }
    } else {
        face = ((Actor *)actor_pool)[31].unk_18;
        sprite_quads[0].unk_0F = face * 0x10;
    }
    sprite_quads[1].unk_0F = saved << 5;
}

void bustup_speech(void) {
    /* GameState.voice_mode is s8; this function reads it as u8 (lbu) */
    if (*(u8 *)&game_state.voice_mode == 0 && !(D_8011F50E & 1)) {
        /* FAKE: the no-op "& 0xFF" shifts IDO's temp numbering (ori in $t9, as in the original); T-1010 */
        D_8011F50E = (D_8011F50E & 0xFF) | 0x11;
    }
}

void bustup_wink(void) {
    if (!(D_8011F4CA & 1)) {
        /* FAKE: the no-op "& 0xFF" shifts IDO's temp numbering (ori in $t8, as in the original); T-1010 */
        D_8011F4CA = (D_8011F4CA & 0xFF) | 0x11;
    }
}

/* nine +0xFC records, reached by byte offset: the word view (value) and the s16 at +2 (cap) */
void parameter_change(s32 arg0, s32 arg1) {
    u8 *p = (u8 *)&game_state + arg0 * 4;

    *(s32 *)(p + 0xFC) += arg1;
    if (*(s32 *)(p + 0xFC) < 0) {
        *(s32 *)(p + 0xFC) = 0;
    }
    if (*(s16 *)(p + 0xFE) >= 0x3E8) {
        *(s16 *)(p + 0xFE) = 0x3E7;
    }
}

void parameter_disp_switch(s32 arg0) {
    s32 i;

    if (arg0 == 1) {
        /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
        for (i = 0; i < 28; i++) ((Actor *)actor_pool)[i + 32].draw_flags |= 0x80;
    } else {
        /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
        for (i = 0; i < 28; i++) ((Actor *)actor_pool)[i + 32].draw_flags &= 0x7F;
    }
}

/* Create the digit actors of the parameter panel and snapshot the profile */
void parameter_show_init(void) {
    s32 i;
    s32 value;
    u8 *snapshot;
    s32 digit;

    /* FAKE: digit and x stores share a source line for as1 scheduling. T-10890 */
    LoadSquare(0x120U, 0x1E0U, 0x10U, 5U, &D_800C6220);
    for (i = 0; i != 28; i++) {
        actor_clear(i + 32);
        ((Actor *)actor_pool)[i + 32].unk_01 = 11;
        ((Actor *)actor_pool)[i + 32].flags = 0x40;
        ((Actor *)actor_pool)[i + 32].quad_flags = 0x40000000;
        ((Actor *)actor_pool)[i + 32].draw_flags = 0x84;
        ((Actor *)actor_pool)[i + 32].frame_table = sprite_frame_table;
        ((Actor *)actor_pool)[i + 32].sequence_table = D_800C9A14;
        ((Actor *)actor_pool)[i + 32].tpage_table = D_800C9730;
        ((Actor *)actor_pool)[i + 32].unk_14 = D_800C9A54;
        ((Actor *)actor_pool)[i + 32].unk_04 = 8;
        ((Actor *)actor_pool)[i + 32].unk_05 = 1;
    }
    value = game_state.param_condition.value;
    digit = func_80046590((s16)value, 3); ((Actor *)actor_pool)[32].unk_18 = digit + 0x44; ((Actor *)actor_pool)[32].x = -0x49;
    ((Actor *)actor_pool)[32].y = -0x58;
    value = game_state.param_condition.value;
    digit = func_80046590((s16)value, 2); ((Actor *)actor_pool)[33].unk_18 = digit + 0x44; ((Actor *)actor_pool)[33].x = -0x41;
    ((Actor *)actor_pool)[33].y = -0x58;
    value = game_state.param_condition.value;
    digit = func_80046590((s16)value, 1); ((Actor *)actor_pool)[34].unk_18 = digit + 0x44; ((Actor *)actor_pool)[34].x = -0x39;
    ((Actor *)actor_pool)[34].y = -0x58;
    if ((*(s16 *)&game_state.progress_state[0x5A]) < game_state.param_condition.value) {
        ((Actor *)actor_pool)[32].unk_43 = ((Actor *)actor_pool)[33].unk_43 = ((Actor *)actor_pool)[34].unk_43 = 0x43;
        ((Actor *)actor_pool)[32].dx = ((Actor *)actor_pool)[33].dx = ((Actor *)actor_pool)[34].dx = 1;
    } else if (game_state.param_condition.value < (*(s16 *)&game_state.progress_state[0x5A])) {
        ((Actor *)actor_pool)[32].unk_43 = ((Actor *)actor_pool)[33].unk_43 = ((Actor *)actor_pool)[34].unk_43 = 0x44;
        ((Actor *)actor_pool)[32].dx = ((Actor *)actor_pool)[33].dx = ((Actor *)actor_pool)[34].dx = 1;
    }
    value = game_state.param_liberal_arts.value;
    digit = func_80046590((s16)value, 3); ((Actor *)actor_pool)[35].unk_18 = digit + 0x44; ((Actor *)actor_pool)[35].x = 0xF;
    ((Actor *)actor_pool)[35].y = -0x58;
    value = game_state.param_liberal_arts.value;
    digit = func_80046590((s16)value, 2); ((Actor *)actor_pool)[36].unk_18 = digit + 0x44; ((Actor *)actor_pool)[36].x = 0x17;
    ((Actor *)actor_pool)[36].y = -0x58;
    value = game_state.param_liberal_arts.value;
    digit = func_80046590((s16)value, 1); ((Actor *)actor_pool)[37].unk_18 = digit + 0x44; ((Actor *)actor_pool)[37].x = 0x1F;
    ((Actor *)actor_pool)[37].y = -0x58;
    if ((*(s16 *)&game_state.progress_state[0x5E]) < game_state.param_liberal_arts.value) {
        ((Actor *)actor_pool)[35].unk_43 = ((Actor *)actor_pool)[36].unk_43 = ((Actor *)actor_pool)[37].unk_43 = 0x43;
        ((Actor *)actor_pool)[35].dx = ((Actor *)actor_pool)[36].dx = ((Actor *)actor_pool)[37].dx = 1;
    } else if (game_state.param_liberal_arts.value < (*(s16 *)&game_state.progress_state[0x5E])) {
        ((Actor *)actor_pool)[35].unk_43 = ((Actor *)actor_pool)[36].unk_43 = ((Actor *)actor_pool)[37].unk_43 = 0x44;
        ((Actor *)actor_pool)[35].dx = ((Actor *)actor_pool)[36].dx = ((Actor *)actor_pool)[37].dx = 1;
    }
    value = game_state.param_science.value;
    digit = func_80046590((s16)value, 3); ((Actor *)actor_pool)[38].unk_18 = digit + 0x44; ((Actor *)actor_pool)[38].x = 0x67;
    ((Actor *)actor_pool)[38].y = -0x58;
    value = game_state.param_science.value;
    digit = func_80046590((s16)value, 2); ((Actor *)actor_pool)[39].unk_18 = digit + 0x44; ((Actor *)actor_pool)[39].x = 0x6F;
    ((Actor *)actor_pool)[39].y = -0x58;
    value = game_state.param_science.value;
    digit = func_80046590((s16)value, 1); ((Actor *)actor_pool)[40].unk_18 = digit + 0x44; ((Actor *)actor_pool)[40].x = 0x77;
    ((Actor *)actor_pool)[40].y = -0x58;
    if ((*(s16 *)&game_state.progress_state[0x62]) < game_state.param_science.value) {
        ((Actor *)actor_pool)[38].unk_43 = ((Actor *)actor_pool)[39].unk_43 = ((Actor *)actor_pool)[40].unk_43 = 0x43;
        ((Actor *)actor_pool)[38].dx = ((Actor *)actor_pool)[39].dx = ((Actor *)actor_pool)[40].dx = 1;
    } else if (game_state.param_science.value < (*(s16 *)&game_state.progress_state[0x62])) {
        ((Actor *)actor_pool)[38].unk_43 = ((Actor *)actor_pool)[39].unk_43 = ((Actor *)actor_pool)[40].unk_43 = 0x44;
        ((Actor *)actor_pool)[38].dx = ((Actor *)actor_pool)[39].dx = ((Actor *)actor_pool)[40].dx = 1;
    }
    value = game_state.param_art.value;
    digit = func_80046590((s16)value, 3); ((Actor *)actor_pool)[41].unk_18 = digit + 0x44; ((Actor *)actor_pool)[41].x = -0x49;
    ((Actor *)actor_pool)[41].y = -0x4A;
    value = game_state.param_art.value;
    digit = func_80046590((s16)value, 2); ((Actor *)actor_pool)[42].unk_18 = digit + 0x44; ((Actor *)actor_pool)[42].x = -0x41;
    ((Actor *)actor_pool)[42].y = -0x4A;
    value = game_state.param_art.value;
    digit = func_80046590((s16)value, 1); ((Actor *)actor_pool)[43].unk_18 = digit + 0x44; ((Actor *)actor_pool)[43].x = -0x39;
    ((Actor *)actor_pool)[43].y = -0x4A;
    if ((*(s16 *)&game_state.progress_state[0x66]) < game_state.param_art.value) {
        ((Actor *)actor_pool)[41].unk_43 = ((Actor *)actor_pool)[42].unk_43 = ((Actor *)actor_pool)[43].unk_43 = 0x43;
        ((Actor *)actor_pool)[41].dx = ((Actor *)actor_pool)[42].dx = ((Actor *)actor_pool)[43].dx = 1;
    } else if (game_state.param_art.value < (*(s16 *)&game_state.progress_state[0x66])) {
        ((Actor *)actor_pool)[41].unk_43 = ((Actor *)actor_pool)[42].unk_43 = ((Actor *)actor_pool)[43].unk_43 = 0x44;
        ((Actor *)actor_pool)[41].dx = ((Actor *)actor_pool)[42].dx = ((Actor *)actor_pool)[43].dx = 1;
    }
    value = game_state.param_athletics.value;
    digit = func_80046590((s16)value, 3); ((Actor *)actor_pool)[44].unk_18 = digit + 0x44; ((Actor *)actor_pool)[44].x = 0xF;
    ((Actor *)actor_pool)[44].y = -0x4A;
    value = game_state.param_athletics.value;
    digit = func_80046590((s16)value, 2); ((Actor *)actor_pool)[45].unk_18 = digit + 0x44; ((Actor *)actor_pool)[45].x = 0x17;
    ((Actor *)actor_pool)[45].y = -0x4A;
    value = game_state.param_athletics.value;
    digit = func_80046590((s16)value, 1); ((Actor *)actor_pool)[46].unk_18 = digit + 0x44; ((Actor *)actor_pool)[46].x = 0x1F;
    ((Actor *)actor_pool)[46].y = -0x4A;
    if ((*(s16 *)&game_state.progress_state[0x6A]) < game_state.param_athletics.value) {
        ((Actor *)actor_pool)[44].unk_43 = ((Actor *)actor_pool)[45].unk_43 = ((Actor *)actor_pool)[46].unk_43 = 0x43;
        ((Actor *)actor_pool)[44].dx = ((Actor *)actor_pool)[45].dx = ((Actor *)actor_pool)[46].dx = 1;
    } else if (game_state.param_athletics.value < (*(s16 *)&game_state.progress_state[0x6A])) {
        ((Actor *)actor_pool)[44].unk_43 = ((Actor *)actor_pool)[45].unk_43 = ((Actor *)actor_pool)[46].unk_43 = 0x44;
        ((Actor *)actor_pool)[44].dx = ((Actor *)actor_pool)[45].dx = ((Actor *)actor_pool)[46].dx = 1;
    }
    value = game_state.param_trivia.value;
    digit = func_80046590((s16)value, 3); ((Actor *)actor_pool)[47].unk_18 = digit + 0x44; ((Actor *)actor_pool)[47].x = 0x67;
    ((Actor *)actor_pool)[47].y = -0x4A;
    value = game_state.param_trivia.value;
    digit = func_80046590((s16)value, 2); ((Actor *)actor_pool)[48].unk_18 = digit + 0x44; ((Actor *)actor_pool)[48].x = 0x6F;
    ((Actor *)actor_pool)[48].y = -0x4A;
    value = game_state.param_trivia.value;
    digit = func_80046590((s16)value, 1); ((Actor *)actor_pool)[49].unk_18 = digit + 0x44; ((Actor *)actor_pool)[49].x = 0x77;
    ((Actor *)actor_pool)[49].y = -0x4A;
    if ((*(s16 *)&game_state.progress_state[0x6E]) < game_state.param_trivia.value) {
        ((Actor *)actor_pool)[47].unk_43 = ((Actor *)actor_pool)[48].unk_43 = ((Actor *)actor_pool)[49].unk_43 = 0x43;
        ((Actor *)actor_pool)[47].dx = ((Actor *)actor_pool)[48].dx = ((Actor *)actor_pool)[49].dx = 1;
    } else if (game_state.param_trivia.value < (*(s16 *)&game_state.progress_state[0x6E])) {
        ((Actor *)actor_pool)[47].unk_43 = ((Actor *)actor_pool)[48].unk_43 = ((Actor *)actor_pool)[49].unk_43 = 0x44;
        ((Actor *)actor_pool)[47].dx = ((Actor *)actor_pool)[48].dx = ((Actor *)actor_pool)[49].dx = 1;
    }
    value = game_state.param_looks.value;
    digit = func_80046590((s16)value, 3); ((Actor *)actor_pool)[50].unk_18 = digit + 0x44; ((Actor *)actor_pool)[50].x = -0x49;
    ((Actor *)actor_pool)[50].y = -0x3C;
    value = game_state.param_looks.value;
    digit = func_80046590((s16)value, 2); ((Actor *)actor_pool)[51].unk_18 = digit + 0x44; ((Actor *)actor_pool)[51].x = -0x41;
    ((Actor *)actor_pool)[51].y = -0x3C;
    value = game_state.param_looks.value;
    digit = func_80046590((s16)value, 1); ((Actor *)actor_pool)[52].unk_18 = digit + 0x44; ((Actor *)actor_pool)[52].x = -0x39;
    ((Actor *)actor_pool)[52].y = -0x3C;
    if ((*(s16 *)&game_state.progress_state[0x72]) < game_state.param_looks.value) {
        ((Actor *)actor_pool)[50].unk_43 = ((Actor *)actor_pool)[51].unk_43 = ((Actor *)actor_pool)[52].unk_43 = 0x43;
        ((Actor *)actor_pool)[50].dx = ((Actor *)actor_pool)[51].dx = ((Actor *)actor_pool)[52].dx = 1;
    } else if (game_state.param_looks.value < (*(s16 *)&game_state.progress_state[0x72])) {
        ((Actor *)actor_pool)[50].unk_43 = ((Actor *)actor_pool)[51].unk_43 = ((Actor *)actor_pool)[52].unk_43 = 0x44;
        ((Actor *)actor_pool)[50].dx = ((Actor *)actor_pool)[51].dx = ((Actor *)actor_pool)[52].dx = 1;
    }
    value = game_state.param_guts.value;
    digit = func_80046590((s16)value, 3); ((Actor *)actor_pool)[53].unk_18 = digit + 0x44; ((Actor *)actor_pool)[53].x = 0xF;
    ((Actor *)actor_pool)[53].y = -0x3C;
    value = game_state.param_guts.value;
    digit = func_80046590((s16)value, 2); ((Actor *)actor_pool)[54].unk_18 = digit + 0x44; ((Actor *)actor_pool)[54].x = 0x17;
    ((Actor *)actor_pool)[54].y = -0x3C;
    value = game_state.param_guts.value;
    digit = func_80046590((s16)value, 1); ((Actor *)actor_pool)[55].unk_18 = digit + 0x44; ((Actor *)actor_pool)[55].x = 0x1F;
    ((Actor *)actor_pool)[55].y = -0x3C;
    if ((*(s16 *)&game_state.progress_state[0x76]) < game_state.param_guts.value) {
        ((Actor *)actor_pool)[53].unk_43 = ((Actor *)actor_pool)[54].unk_43 = ((Actor *)actor_pool)[55].unk_43 = 0x43;
        ((Actor *)actor_pool)[53].dx = ((Actor *)actor_pool)[54].dx = ((Actor *)actor_pool)[55].dx = 1;
    } else if (game_state.param_guts.value < (*(s16 *)&game_state.progress_state[0x76])) {
        ((Actor *)actor_pool)[53].unk_43 = ((Actor *)actor_pool)[54].unk_43 = ((Actor *)actor_pool)[55].unk_43 = 0x44;
        ((Actor *)actor_pool)[53].dx = ((Actor *)actor_pool)[54].dx = ((Actor *)actor_pool)[55].dx = 1;
    }
    value = game_state.param_stress.value;
    digit = func_80046590((s16)value, 3); ((Actor *)actor_pool)[56].unk_18 = digit + 0x44; ((Actor *)actor_pool)[56].x = 0x67;
    ((Actor *)actor_pool)[56].y = -0x3C;
    value = game_state.param_stress.value;
    digit = func_80046590((s16)value, 2); ((Actor *)actor_pool)[57].unk_18 = digit + 0x44; ((Actor *)actor_pool)[57].x = 0x6F;
    ((Actor *)actor_pool)[57].y = -0x3C;
    value = game_state.param_stress.value;
    digit = func_80046590((s16)value, 1); ((Actor *)actor_pool)[58].unk_18 = digit + 0x44; ((Actor *)actor_pool)[58].x = 0x77;
    ((Actor *)actor_pool)[58].y = -0x3C;
    if ((*(s16 *)&game_state.progress_state[0x7A]) < game_state.param_stress.value) {
        ((Actor *)actor_pool)[56].unk_43 = ((Actor *)actor_pool)[57].unk_43 = ((Actor *)actor_pool)[58].unk_43 = 0x43;
        ((Actor *)actor_pool)[56].dx = ((Actor *)actor_pool)[57].dx = ((Actor *)actor_pool)[58].dx = 1;
    } else if (game_state.param_stress.value < (*(s16 *)&game_state.progress_state[0x7A])) {
        ((Actor *)actor_pool)[56].unk_43 = ((Actor *)actor_pool)[57].unk_43 = ((Actor *)actor_pool)[58].unk_43 = 0x44;
        ((Actor *)actor_pool)[56].dx = ((Actor *)actor_pool)[57].dx = ((Actor *)actor_pool)[58].dx = 1;
    }
    ((Actor *)actor_pool)[59].unk_18 = 0x3D; ((Actor *)actor_pool)[59].x = 0;
    ((Actor *)actor_pool)[59].y = -0x48;
    snapshot = &game_state.progress_state[0x20];
    memcpy(snapshot, game_state.player_full_name, 0x7C);
}


void parameter_show(void) {
    s32 i;
    s32 d;
    s32 step;
    Actor *tab;

    /* GameState.sd_anim_fast is s8; this function reads it as u8 (lbu) */
    if ((u8)game_state.sd_anim_fast != 0) {
        step = 4;
    } else {
        step = 8;
    }
    tab = (Actor *)actor_pool;
    if (tab[32].draw_flags & 0x80) {
        prim_gradient_rect(-0x84, -0x61, 0x108, 0x2A, 0xC, 0x30E00, 0);
        dtd_on(0xC);
        for (i = 32; i < 56; i++) {
            if (tab[i].dx == 1) {
                d = 0x1000 / step;
                tab[i].unk_08 += d;
                tab[i].scale_x -= d;
                if (d >= tab[i].scale_x) {
                    tab[i].dx = 2;
                }
            } else if (tab[i].dx == 2) {
                d = 0x1000 / step;
                tab[i].unk_08 -= d;
                tab[i].scale_x += d;
                if (tab[i].scale_x >= 0x1000) {
                    tab[i].scale_x = 0x1000;
                    tab[i].dx = 0;
                    tab[i].unk_08 = 0;
                }
            }
        }
        /* Records 56..58 keep an s16 at 0x1C (unk_1C; the TAIIKU overlay reads it the same way). T-17011 */
        for (i = 56; i < 59; i++) {
            if (tab[i].dx == 1) {
                d = 0x1000 / step;
                tab[i].scale_y -= d;
                if (d >= tab[i].scale_y) {
                    tab[i].dx = 2;
                }
            } else if (tab[i].dx == 2) {
                tab[i].scale_y += 0x1000 / step;
                if (tab[i].scale_y >= 0x1000) {
                    tab[i].scale_y = 0x1000;
                    tab[i].dx = 0;
                }
            }
        }
    }
}

/* Show or hide the calendar date HUD */
void hizuke_disp_switch(s32 arg0) {
    s32 i;

    if (arg0 == 1) {
        /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
        for (i = 0; i < 8; i++) ((Actor *)actor_pool)[i + 16].draw_flags |= 0x80;
    } else {
        /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
        for (i = 0; i < 8; i++) ((Actor *)actor_pool)[i + 16].draw_flags &= 0x7F;
    }
}

/* Create the calendar date actors (month, day, weekday) for the HUD */
void hizuke_init(void) {
    s32 i;
    s32 digit;
    u8 event;

    /* FAKE: entry reads preserve unsigned month/day promotion across digit calls
     * under the conversion pass; these assignments are discarded. T-24604. */
    i = game_state.month;
    i = game_state.day;
    for (i = 0; i != 8; i++) {
        actor_clear(i + 16);
        ((Actor *)actor_pool)[i + 16].unk_01 = 4;
        ((Actor *)actor_pool)[i + 16].flags = 0x40;
        ((Actor *)actor_pool)[i + 16].quad_flags = 0x40000000;
        ((Actor *)actor_pool)[i + 16].draw_flags = 0x84;
        ((Actor *)actor_pool)[i + 16].frame_table = sprite_frame_table;
        ((Actor *)actor_pool)[i + 16].sequence_table = D_800C9A14;
        ((Actor *)actor_pool)[i + 16].tpage_table = D_800C9730;
        ((Actor *)actor_pool)[i + 16].unk_14 = D_800C9A54;
        ((Actor *)actor_pool)[i + 16].unk_04 = 8;
        ((Actor *)actor_pool)[i + 16].unk_05 = 1;
    }
    ((Actor *)actor_pool)[17].unk_18 = func_80046590(game_state.year, 2) + 0x44;
    ((Actor *)actor_pool)[17].x = 0x8C;
    ((Actor *)actor_pool)[17].y = -0x41;
    /* FAKE: joining the digit and x stores keeps the following month address
     * load after the x store. T-24604. */
    ((Actor *)actor_pool)[18].unk_18 = func_80046590(game_state.year, 1) + 0x44; ((Actor *)actor_pool)[18].x = 0x94;
    ((Actor *)actor_pool)[18].y = -0x41;

    if (game_state.month >= 10) {
        digit = func_80046590((s16)game_state.month, 2);
        ((Actor *)actor_pool)[19].unk_18 = digit + 0x44;
        ((Actor *)actor_pool)[19].x = 0x8C;
        ((Actor *)actor_pool)[19].y = -0x22;

        digit = func_80046590((s16)game_state.month, 1);
        ((Actor *)actor_pool)[20].unk_18 = digit + 0x44;
        ((Actor *)actor_pool)[20].x = 0x94;
        ((Actor *)actor_pool)[20].y = -0x22;
    } else {
        ((Actor *)actor_pool)[19].draw_flags = 0;
        digit = func_80046590((s16)game_state.month, 2);
        ((Actor *)actor_pool)[19].unk_18 = digit + 0x44;
        ((Actor *)actor_pool)[19].x = 0xFF;
        ((Actor *)actor_pool)[19].y = 0xFF;

        digit = func_80046590((s16)game_state.month, 1);
        ((Actor *)actor_pool)[20].unk_18 = digit + 0x44;
        ((Actor *)actor_pool)[20].x = 0x90;
        ((Actor *)actor_pool)[20].y = -0x22;
    }

    if (game_state.day >= 10) {
        digit = func_80046590((s16)game_state.day, 2);
        ((Actor *)actor_pool)[21].unk_18 = digit + 0x44;
        ((Actor *)actor_pool)[21].x = 0x8C;
        ((Actor *)actor_pool)[21].y = -1;

        digit = func_80046590((s16)game_state.day, 1);
        ((Actor *)actor_pool)[22].unk_18 = digit + 0x44;
        ((Actor *)actor_pool)[22].x = 0x94;
        ((Actor *)actor_pool)[22].y = -1;
    } else {
        ((Actor *)actor_pool)[21].draw_flags = 0;
        digit = func_80046590((s16)game_state.day, 2);
        ((Actor *)actor_pool)[21].unk_18 = digit + 0x44;
        ((Actor *)actor_pool)[21].x = 0xFF;
        ((Actor *)actor_pool)[21].y = 0xFF;

        digit = func_80046590((s16)game_state.day, 1);
        ((Actor *)actor_pool)[22].unk_18 = digit + 0x44;
        ((Actor *)actor_pool)[22].x = 0x90;
        ((Actor *)actor_pool)[22].y = -1;
    }
    ((Actor *)actor_pool)[23].unk_18 = game_state.weekday + 0x52;
    ((Actor *)actor_pool)[23].x = 0x92;
    ((Actor *)actor_pool)[23].y = 0x1F;
    if (game_state.weekday == WEEKDAY_SATURDAY) {
        ((Actor *)actor_pool)[23].unk_43 = 0x43;
    } else if (game_state.weekday == WEEKDAY_SUNDAY) {
        ((Actor *)actor_pool)[23].unk_43 = 0x44;
    } else {
        ((Actor *)actor_pool)[23].unk_43 = 0;
    }
    event = game_state.month_calendar[0].day_kind[game_state.day];
    if (event == 1 || event == 2) {
        ((Actor *)actor_pool)[23].unk_43 = 0x44;
    }
    ((Actor *)actor_pool)[16].unk_18 = 0x3C;
    ((Actor *)actor_pool)[16].x = 0x90;
    ((Actor *)actor_pool)[16].y = -0x12;
}

/* Per-frame update of the calendar date HUD */
void hizuke_show(void) {
    if (D_8011F113 & 0x80) {
        prim_gradient_rect(0x86, -0x4B, 0x14, 0x72, 5, 0x30E00, 0);
        dtd_on(5);
    }
}

void message_disp_switch(s32 arg0) {
    if (arg0 == 1) {
        D_8011ED17 |= 0x80;
    } else {
        D_8011ED17 &= 0x7F;
    }
}

/* Create the message window actor */
void message_window_init(void) {
    actor_clear(1);
    D_8011ED15 = 7;
    D_8011ED16 = 0x40;
    D_8011ED4C = 0x40000000;
    D_8011ED17 = 0x84;
    D_8011ED20 = sprite_frame_table;
    D_8011ED24 = D_800C9A14;
    D_8011ED48 = D_800C9730;
    D_8011ED28 = D_800C9A54;
    D_8011ED18 = 8;
    D_8011ED19 = 1;
    D_8011ED2C = 0x3E;
    D_8011ED3A = 0;
    D_8011ED3E = 0x4B;
}

/* Per-frame update of the message window */
void message_window_show(void) {
    if (D_8011ED17 & 0x80) {
        prim_gradient_rect(-0x85, 0x2E, 0x10A, 0x34, 8, 0x30E00, 0);
        dtd_on(8);
    }
}

/* Show or hide the command icons */
void icon_disp_switch(s32 arg0) {
    u32 i;
    u8 *p;
    GameState *g;

    g = &game_state; /* keeps the base address in a register before the branch (T-9360) */
    if (arg0 != 0) {
        i = 0;
        if ((u8)game_state.icon_count != 0) {
            p = actor_pool;
            do {
                p[0x1543] |= 0x80;
                p[0x1103] |= 0x80;
                i++;
                p += 0x44;
            } while (i < (u8)g->icon_count);
        }
    } else {
        i = 0;
        if ((u8)game_state.icon_count != 0) {
            p = actor_pool;
            do {
                p[0x1543] &= 0x7F;
                p[0x1103] &= 0x7F;
                i++;
                p += 0x44;
            } while (i < (u8)g->icon_count);
        }
    }
}

/* Enable or disable one of the 14 command icons */
void icon_can_use_set(s32 idx, s32 on) {
    if (on) {
        D_80125C90[idx] = 1;
    } else {
        D_80125C90[idx] = 0;
    }
}

s32 get_icon_can_use(s32 arg0) {
    if (D_80125C90[arg0] == 1) {
        return 1;
    }
    return 0;
}

void hud_icon_bar_init(void) {
    GameState *g = &game_state;
    s32 i;
    s32 n;
    s32 j;

    if (g->icon_mode == ICON_MODE_FREE) {
        g->icon_count = 0xF;
    }
    for (i = 0; i < g->icon_count; i++) {
        D_80125C90[i] = 1;
    }
    for (i = 0; i < g->icon_count; i++) {
        actor_clear(i + 0x50);
        ((Actor *)actor_pool)[i + 80].unk_01 = 3;
        ((Actor *)actor_pool)[i + 80].flags = 1;
        ((Actor *)actor_pool)[i + 80].quad_flags = 0x40000000;
        ((Actor *)actor_pool)[i + 80].draw_flags = 0x84;
        ((Actor *)actor_pool)[i + 80].frame_table = sprite_frame_table;
        ((Actor *)actor_pool)[i + 80].sequence_table = D_800C9A14;
        ((Actor *)actor_pool)[i + 80].tpage_table = D_800C9730;
        ((Actor *)actor_pool)[i + 80].unk_14 = D_800C9A54;
        ((Actor *)actor_pool)[i + 80].unk_04 = 8;
        ((Actor *)actor_pool)[i + 80].unk_05 = 1;
        ((Actor *)actor_pool)[i + 80].sequence_slot = i;
        ((Actor *)actor_pool)[i + 80].blink_period = 1;
        actor_clear(i + 0x40);
        ((Actor *)actor_pool)[i + 64].unk_01 = 4;
        ((Actor *)actor_pool)[i + 64].flags = 0x40;
        ((Actor *)actor_pool)[i + 64].quad_flags = 0x40000000;
        ((Actor *)actor_pool)[i + 64].draw_flags = 0x84;
        ((Actor *)actor_pool)[i + 64].frame_table = sprite_frame_table;
        ((Actor *)actor_pool)[i + 64].sequence_table = D_800C9A14;
        ((Actor *)actor_pool)[i + 64].tpage_table = D_800C9730;
        ((Actor *)actor_pool)[i + 64].unk_14 = D_800C9A54;
        ((Actor *)actor_pool)[i + 64].unk_04 = 8;
        ((Actor *)actor_pool)[i + 64].unk_05 = 1;
        ((Actor *)actor_pool)[i + 64].unk_18 = 2;
    }
    ((Actor *)actor_pool)[94].unk_01 = 3;
    ((Actor *)actor_pool)[78].unk_01 = 4;
    ((Actor *)actor_pool)[94].sequence_slot = 0xF;
    ((Actor *)actor_pool)[94].x = 0;
    ((Actor *)actor_pool)[94].y = 0;
    ((Actor *)actor_pool)[78].sequence_slot = 0xF;
    ((Actor *)actor_pool)[78].x = 0;
    ((Actor *)actor_pool)[78].y = 0;
    switch (g->icon_mode) {
    case ICON_MODE_TYPE_A:
        for (i = 0; i < g->icon_count; i++) {
            ((s16 *)D_80125C10)[i * 2] = D_800B5AE4[i][0];
            ((s16 *)D_80125C10)[i * 2 + 1] = D_800B5AE4[i][1];
        }
        break;
    case ICON_MODE_TYPE_B:
        for (i = 0; i < g->icon_count; i++) {
            ((s16 *)D_80125C10)[i * 2] = D_800B5AA4[i][0];
            ((s16 *)D_80125C10)[i * 2 + 1] = D_800B5AA4[i][1];
        }
        break;
    case ICON_MODE_FREE:
        /* Two or more (0, 0) pairs: restore the default table. The copy reuses the counter n (T-25108). */
        n = 0;
        for (j = 0; j < 15; j++) {
            if (((s16 *)D_80125C10)[j * 2] == 0 && ((s16 *)D_80125C10)[j * 2 + 1] == 0) {
                n++;
            }
            if (n >= 2) {
                for (n = 0; n < 15; n++) {
                    ((s16 *)D_80125C10)[n * 2] = D_800B5AE4[n][0];
                    ((s16 *)D_80125C10)[n * 2 + 1] = D_800B5AE4[n][1];
                }
                break;
            }
        }
        break;
    }
    hud_icon_layout_apply(g->icon_count);
    LoadSquare(0x120, 0x1F1, 0x10, 1, &D_800C6200);
    LoadSquare(0x120, 0x1F2, 0x10, 1, &D_800C6220);
    LoadSquare(0x120, 0x1F3, 0x10, 1, &D_800C6220[0x20]);
}

void hud_icon_layout_apply(s32 n) {
    s32 pad; /* FAKE: preserves original stack homes and frame size. T-24501 */
    s32 i;
    s16 x[16];
    s16 y[16];
    s16 w[16];
    s16 h[16];

    if (n != 15) {
        n = 14;
        game_state.icon_count = 14;
    }
    for (i = 0; i < n; i++) {
        x[i] = ((s16 (*)[2])D_80125C10)[i][0] - 10;
        y[i] = ((s16 (*)[2])D_80125C10)[i][1] - 10;
        w[i] = 20;
        h[i] = 16;
    }
    i = 0; /* FAKE: keeps the saved zero index across menu_set. T-24501 */
    menu_set(0, n & 0xFF, x, y, w, h);
    for (i = 0; i < n; i++) {
        ((Actor *)actor_pool)[i + 80].sequence_slot = i;
        ((Actor *)actor_pool)[i + 80].x = ((s16 (*)[2])D_80125C10)[i][0];
        ((Actor *)actor_pool)[i + 80].y = ((s16 (*)[2])D_80125C10)[i][1];
    }
    for (i = 0; i < n; i++) {
        ((Actor *)actor_pool)[i + 64].x = ((s16 (*)[2])D_80125C10)[i][0];
        ((Actor *)actor_pool)[i + 64].y = ((s16 (*)[2])D_80125C10)[i][1];
        ((Actor *)actor_pool)[i + 64].unk_18 = 2;
    }
    if (n == 15) {
        ((Actor *)actor_pool)[94].sequence_slot = 15;
        ((Actor *)actor_pool)[94].x = ((s16 *)D_80125C10)[28];
        ((Actor *)actor_pool)[94].y = ((s16 *)D_80125C10)[29];
        ((Actor *)actor_pool)[78].unk_18 = 2;
        ((Actor *)actor_pool)[78].x = ((s16 *)D_80125C10)[28];
        ((Actor *)actor_pool)[78].y = ((s16 *)D_80125C10)[29];
    } else {
        ((Actor *)actor_pool)[78].draw_flags = 0;
        ((Actor *)actor_pool)[94].draw_flags = 0;
    }
}

void hud_icon_memory_card_animation_set(u8 arg0) {
    /* D_8012059A[1] is D_8012059C; the array form keeps IDO from hoisting the load above the first store */
    if (arg0) {
        D_8012059A[0] = 0xE;
        if (D_8012059A[1] >= 4) {
            D_8012059A[1] = 0;
        }
    } else {
        D_8012059A[0] = 0xD;
        if (D_8012059A[1] >= 6) {
            D_8012059A[1] = 0;
        }
    }
}

/* Returns 0 when record idx of D_80125C10 moved by (dx, dy) leaves the screen (x0 < -160, x0 + 19 > 160,
 * y0 outside -120..120) or when one of its four corners lies in the 20 x 16 box of another of the 15
 * records, else 1. The corners are two local arrays filled from repeated expressions and tested in a
 * 4-trip loop: IDO unrolls that loop, keeps the eight corner values in registers (one pair per test,
 * so the outer loop runs out of caller-saved registers and spills the shared subexpressions to
 * sp+0x20..0x28) and leaves the 15-trip loop rolled. T-25108. */
s32 hud_icon_move_allowed(s32 idx, s32 dx, s32 dy) {
    s32 cx[4];
    s32 cy[4];
    s16 *p;
    s32 i;
    s32 j;
    s32 r;
    s16 *q; /* FAKE: T-25108, never used; the original's 15th local word is unknown. It keeps the frame at
             * 0x68 and the spill slots at sp+0x20..0x28. */
    s32 tx;
    s32 ty;

    p = (s16 *)&D_80125C10[idx * 4];
    r = 1;
    cx[0] = p[0] + dx; cy[0] = p[1] + dy;
    cx[1] = p[0] + dx + 19; cy[1] = p[1] + dy;
    cx[2] = p[0] + dx; cy[2] = p[1] + dy + 15;
    cx[3] = p[0] + dx + 19; cy[3] = p[1] + dy + 15;
    if (cx[0] < -160) {
        return 0;
    }
    if (cx[1] > 160) {
        return 0;
    }
    if (cy[0] < -120) {
        return 0;
    }
    if (cy[1] > 120) {
        return 0;
    }
    for (i = 0; i < 15; i++) {
        if (i != idx) {
            tx = ((s16 *)&D_80125C10[i * 4])[0];
            ty = ((s16 *)&D_80125C10[i * 4])[1];
            for (j = 0; j < 4; j++) {
                if (cx[j] >= tx && cx[j] < tx + 20 && cy[j] >= ty && cy[j] < ty + 16) {
                    r = 0;
                }
            }
        }
    }
    return r;
}

/* Cursor step of the 15-icon menu: every third frame of the pad repeat (`unk_F6C % 3 == 2`) moves the
 * selected icon's position in D_80125C10 and the cursor record by the pad direction (`unk_F6F` pixels), unless
 * hud_icon_move_allowed rejects the move, then clamps the icon to x -0x8A..0x80, y -0x5C..0x5C. Without pad input
 * it runs the menu cursor check instead. A selection change restarts the 0x20-frame blink of
 * D_800B5AA0, which scales the selected icons with rsin while `a` is 0; the loop sets the display modes of
 * the icon records 64 + i and 80 + i, and the tail sets the page counter limits as hud_icon_memory_card_animation_set does.
 * The selection index is read through `g`: the stores to D_80125C10 then reload the index expression (the
 * original recomputes the address for the y store), and uopt keeps the index in $s3, re-read after each call. */
void hud_icon_bar_update(s32 a) {
    s32 dx;
    s32 dy;
    s32 n;
    u8 old;
    s32 i;
    GameState *g = &game_state;

    old = g->menu_cursor_0;
    dx = 0;
    dy = 0;
    if (g->pad_held & PAD_L2) {
        if (g->icon_mode % 3U == 2) {
            switch (g->pad_held & 0xA000) {
            case 0x8000:
                dx = -g->cursor_speed;
                break;
            case 0x2000:
                dx = g->cursor_speed;
                break;
            }
            /* T-25108: an unsigned selector; with a signed one the case constant 0x1000 shares the register
             * of the loop's halfword 0x1000 stores. */
            switch ((u32)g->pad_held & 0x5000) {
            case 0x1000:
                dy = -g->cursor_speed;
                break;
            case 0x4000:
                dy = g->cursor_speed;
                break;
            }
            if (hud_icon_move_allowed(g->menu_cursor_0, dx, dy) == 0) {
                dx = 0;
                dy = 0;
            }
            ((s16 (*)[2])D_80125C10)[g->menu_cursor_0][0] += dx;
            ((s16 (*)[2])D_80125C10)[g->menu_cursor_0][1] += dy;
            /* record 0 fields, not cursor_x/cursor_y: as1 keeps the y load behind the x store */
            ((Actor *)actor_pool)[0].x += dx;
            ((Actor *)actor_pool)[0].y += dy;
            hud_icon_layout_apply(g->icon_count);
            if (((s16 (*)[2])D_80125C10)[g->menu_cursor_0][0] > 0x80) {
                ((s16 (*)[2])D_80125C10)[g->menu_cursor_0][0] = 0x80;
            }
            if (((s16 (*)[2])D_80125C10)[g->menu_cursor_0][0] < -0x8A) {
                ((s16 (*)[2])D_80125C10)[g->menu_cursor_0][0] = -0x8A;
            }
            if (((s16 (*)[2])D_80125C10)[g->menu_cursor_0][1] < -0x5C) {
                ((s16 (*)[2])D_80125C10)[g->menu_cursor_0][1] = -0x5C;
            }
            if (((s16 (*)[2])D_80125C10)[g->menu_cursor_0][1] > 0x5C) {
                ((s16 (*)[2])D_80125C10)[g->menu_cursor_0][1] = 0x5C;
            }
        }
    } else {
        /* FAKE: T-25201, a never-true arm with an indirect store of 3. uopt counts it as a use of the shared
         * constant 3 in this block, so the $s5 range covers the else arm and is loaded before menu_check (as in
         * the original) instead of after the index reload; ugen then removes the arm. */
        if ((u32)a < 0) {
            ((Actor *)actor_pool)[a + 64].unk_01 = 3;
        }
        menu_check(0, ((Actor *)actor_pool)[0].x, ((Actor *)actor_pool)[0].y);
        menu_cursor_update(0);
    }
    if (old != D_800B5A9C) {
        D_800B5A9C = g->menu_cursor_0;
        D_800B5AA0 = 0x20;
    } else if (D_800B5AA0 != 0) {
        D_800B5AA0--;
    }
    n = 14;
    if (g->icon_count == 15) {
        n = 15;
    }
    if (g->icon_mode == ICON_MODE_FREE) {
        n = 15;
    }
    for (i = 0; i < n; i++) {
        if (D_80125C90[i] == 0) {
            ((Actor *)actor_pool)[i + 80].flags = 0;
            ((Actor *)actor_pool)[i + 64].unk_43 = 0x51;
            ((Actor *)actor_pool)[i + 64].unk_01 = 3;
        } else {
            ((Actor *)actor_pool)[i + 80].flags = 1;
            ((Actor *)actor_pool)[i + 64].unk_01 = 4;
            ((Actor *)actor_pool)[i + 80].unk_01 = 3;
            if (((MenuItem (*)[2])g->menu_items)[i][0].hover != 0) {
                ((Actor *)actor_pool)[i + 64].unk_43 = 0x52;
                ((Actor *)actor_pool)[i + 80].unk_01 = 2;
                ((Actor *)actor_pool)[i + 64].unk_01 = 3;
                if (a == 0) {
                    ((Actor *)actor_pool)[i + 80].scale_x = rsin((0x20 - D_800B5AA0) << 6) + 0x1000;
                    ((Actor *)actor_pool)[i + 80].scale_y = rsin((0x20 - D_800B5AA0) << 6) + 0x1000;
                } else {
                    ((Actor *)actor_pool)[i + 80].scale_x = 0x1000;
                    ((Actor *)actor_pool)[i + 80].scale_y = 0x1000;
                }
            } else {
                ((Actor *)actor_pool)[i + 64].unk_43 = 0x53;
                if (a == 0) {
                    ((Actor *)actor_pool)[i + 80].scale_x = 0x1000;
                    ((Actor *)actor_pool)[i + 80].scale_y = 0x1000;
                    ((Actor *)actor_pool)[i + 64].scale_x = 0x1000;
                    ((Actor *)actor_pool)[i + 64].scale_y = 0x1000;
                } else {
                    ((Actor *)actor_pool)[i + 80].scale_x = 0x1000;
                    ((Actor *)actor_pool)[i + 80].scale_y = 0x1000;
                }
            }
        }
    }
    if (g->memory_card_busy) {
        D_8012059A[0] = 0xE;
        if (D_8012059A[1] >= 4) {
            D_8012059A[1] = 0;
        }
    } else {
        D_8012059A[0] = 0xD;
        if (D_8012059A[1] >= 6) {
            D_8012059A[1] = 0;
        }
    }
}

/* Shows or hides records 3..D_800B5B24 + 4. The entry test is always false in the original too.
 * FAKE: T-24301, each loop on one source line: as1 then keeps the first unrolled load first. */
void message_window_title_show(s32 show) {
    s32 i;

    if (D_800B5B24 == 0 && D_800B5B24 >= 11) {
        return;
    }
    if (show == 1) {
        for (i = 0; i < D_800B5B24 + 2; i++) ((Actor *)actor_pool)[i + 3].draw_flags |= 0x80;
        k_disp_switch(0, 1);
    } else {
        if (((Actor *)actor_pool)[3].draw_flags & 0x80) {
            k_disp_switch(0, 0);
        }
        for (i = 0; i < D_800B5B24 + 2; i++) ((Actor *)actor_pool)[i + 3].draw_flags &= 0x7F;
        D_80125CA0 = 0xB8;
    }
}

void message_window_set_title_hidden(void) {
    message_window_set_title();
    D_80125CA0 = 0xB8;
}

void message_window_set_title(u8 *arg0) {
    u8 *p;
    s32 i;

    D_80125CA0 = -0x40;
    for (i = 0; i < D_800B5B24 + 2; i++) {
        actor_clear(i + 3);
    }
    set_kanji_string(-0xCE, -0x98, 0U, arg0, 0);
    D_800B5B24 = strlen(arg0) / 2;
    for (i = 0; i < D_800B5B24 + 2; i++) {
        actor_clear(i + 3);
        ((Actor *)actor_pool)[i + 3].unk_01 = 2;
        ((Actor *)actor_pool)[i + 3].flags = 0x40;
        ((Actor *)actor_pool)[i + 3].quad_flags = 0x40000000;
        ((Actor *)actor_pool)[i + 3].draw_flags = 0xA4;
        ((Actor *)actor_pool)[i + 3].frame_table = sprite_frame_table;
        ((Actor *)actor_pool)[i + 3].sequence_table = D_800C9A14;
        ((Actor *)actor_pool)[i + 3].tpage_table = D_800C9730;
        ((Actor *)actor_pool)[i + 3].unk_14 = D_800C9A54;
        ((Actor *)actor_pool)[i + 3].unk_04 = 8;
        ((Actor *)actor_pool)[i + 3].unk_05 = 1;
        ((Actor *)actor_pool)[i + 3].unk_18 = 0x72;
        ((Actor *)actor_pool)[i + 3].x = (D_800B5B24 - i) * 0xE + D_80125CA0 - 0x87;
        ((Actor *)actor_pool)[i + 3].y = D_80125CA0 - 0x51;
    }
    /* Byte pointer on purpose: &tab[n + 4] compiles to multu, this form to sll/addu as in the original. T-17011 */
    p = actor_pool + D_800B5B24 * 0x44;
    ((Actor *)(p + 0x110))->unk_18 = 0x71;
    ((Actor *)(p + 0x110))->x += 2;
    ((Actor *)actor_pool)[3].unk_18 = 0x73;
    ((Actor *)actor_pool)[3].x -= 2;
}

void message_window_title_update(void) {
    s32 x;
    s32 y;
    /* FAKE: paired work-field writes share a line for as1 scheduling. T-10890 */
    TextEntry *work;
    s32 i;

    if (((Actor *)actor_pool)[3].draw_flags & 0x80) {
        work = (TextEntry *)get_k_work(0);
        if (D_80125CA0 < 0) {
            D_80125CA0++;
            for (i = 0; i < D_800B5B24 + 2; i++) {
                ((Actor *)actor_pool)[i + 3].x = (D_800B5B24 - i) * 14 + D_80125CA0 - 135;
                ((Actor *)actor_pool)[i + 3].y = D_80125CA0 - 81;
            }
            ((Actor *)actor_pool)[3].x -= 2;
            ((Actor *)actor_pool)[D_800B5B24 + 4].x += 2;
            x = D_80125CA0 - 144;
            y = D_80125CA0 - 90;
            prim_gradient_rect(x, y, D_800B5B24 * 14 + 8, 20, 2, 0x30E00, 0);
            work->x++; work->y++;
            return;
        }
        if (D_80125CA0 < 120) {
            D_80125CA0++;
            x = -144;
            y = -90;
            prim_gradient_rect(x, y, D_800B5B24 * 14 + 8, 20, 2, 0x30E00, 0);
            work->x = -142; work->y = -88;
            return;
        }
        if (D_80125CA0 < 184) {
            D_80125CA0++;
            for (i = 0; i < D_800B5B24 + 2; i++) {
                ((Actor *)actor_pool)[i + 3].x = (D_800B5B24 - i) * 14 + (120 - D_80125CA0) - 135;
                ((Actor *)actor_pool)[i + 3].y = (120 - D_80125CA0) - 81;
            }
            ((Actor *)actor_pool)[3].x -= 2;
            ((Actor *)actor_pool)[D_800B5B24 + 4].x += 2;
            x = 120 - D_80125CA0 - 144;
            y = 120 - D_80125CA0 - 90;
            prim_gradient_rect(x, y, D_800B5B24 * 14 + 8, 20, 2, 0x30E00, 0);
            work->x--; work->y--;
            return;
        }
        if (game_state.pad_held & PAD_TRIANGLE) {
            for (i = 0; i < D_800B5B24 + 2; i++) {
                ((Actor *)actor_pool)[i + 3].x = (D_800B5B24 - i) * 14 - 135;
                ((Actor *)actor_pool)[i + 3].y = -81;
            }
            ((Actor *)actor_pool)[3].x -= 2;
            ((Actor *)actor_pool)[D_800B5B24 + 4].x += 2;
            x = -144;
            y = -90;
            prim_gradient_rect(x, y, D_800B5B24 * 14 + 8, 20, 2, 0x30E00, 0);
            work->x = -142; work->y = -88;
            return;
        }
        for (i = 0; i < D_800B5B24 + 2; i++) {
            ((Actor *)actor_pool)[i + 3].x = (D_800B5B24 - i) * 14 - 199;
            ((Actor *)actor_pool)[i + 3].y = -145;
        }
        ((Actor *)actor_pool)[3].x -= 2;
        ((Actor *)actor_pool)[D_800B5B24 + 4].x += 2;
        x = -208;
        y = -154;
        prim_gradient_rect(x, y, D_800B5B24 * 14 + 8, 20, 2, 0x30E00, 0);
        work->x = -206; work->y = -152;
    }
}


s32 hud_season_index(void) {
    switch (game_state.month) {
    case 3:
    case 4:
    case 5:
    default:
        return 0;
    case 6:
    case 7:
    case 8:
        return 1;
    case 9:
    case 10:
    case 11:
        return 2;
    case 1:
    case 2:
    case 12:
        return 3;
    }
}

s32 hud_in_summer_months(void) {
    switch (game_state.month) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 10:
    case 11:
    case 12:
    default:
        return 0;
    case 6:
    case 7:
    case 8:
    case 9:
        return 1;
    }
}

s32 hud_summer_phase(void) {
    s32 ret;

    ret = hud_in_summer_months();
    if (game_state.month == 9) {
        if (game_state.day >= 0x18U) {
            ret = -1;
        }
        /* FAKE: empty test of ret makes ret the first register-allocated local ($v1), the global gets $a0. T-4100 */
        if (!ret) {
        }
    }
    if (game_state.month == 5) {
        if (game_state.day >= 0x19U) {
            ret = -1;
        }
    }
    return ret;
}

s32 hud_vacation_for_date(s32 arg0, s32 arg1) {
    if (arg0 == 8) {
        return 1;
    }
    if (arg0 == 7 && arg1 >= 0x18) {
        return 1;
    }
    if (arg0 == 0xC && arg1 >= 0x19) {
        return 2;
    }
    if (arg0 == 0xD && arg1 < 8) {
        return 2;
    }
    if (arg0 == 1 && arg1 < 8) {
        return 2;
    }
    if (arg0 == 3 && arg1 >= 0x18) {
        return 3;
    }
    if (arg0 == 4 && arg1 < 4) {
        return 3;
    }
    return 0;
}

void hud_screen_frame_draw(s32 a) {
    s32 x;
    s32 y;
    s32 old_index;
    s32 next_index;
    u32 blend;
    u8 fade;
    s32 row;
    s32 col;
    SPRT *spr;
    s32 current;
    ColorPair555 *old;
    ColorPair555 *next;
    ColorPair555 *out;
    s32 old_weight;
    s32 next_weight;

    u8 enabled[10][8];
    current = hud_season_index();
    switch (game_state.player_flags.b[1] & 15) {
    case 1:
        if ((game_state.frame_count & 3) == 3) D_800B5BD0--;
        if ((game_state.frame_count & 1) == 1) D_800B5BCC++;
        break;
    case 2:
        if ((game_state.frame_count & 7) == 7) D_800B5BD0--;
        if ((game_state.frame_count & 3) == 3) D_800B5BCC++;
        break;
    case 3:
        if ((game_state.frame_count & 0x15) == 0x15) D_800B5BD0--;
        if ((game_state.frame_count & 7) == 7) D_800B5BCC++;
        break;
    case 4:
        D_800B5BD0 += 16 - (rand() & 31);
        D_800B5BCC += 16 - (rand() & 31);
        break;
    case 5:
        D_800B5BD0 += rsin(game_state.frame_count) / 32;
        D_800B5BCC += func_800A0140(game_state.frame_count) / 32;
        break;
    case 0:
    default:
        if ((game_state.frame_count & 1) == 1) D_800B5BD0--;
        D_800B5BCC++;
        break;
    }
    for (row = 0; row < 8; row++) {
        for (col = 0; col < 10; col++) {
                enabled[col][row] = 1;
            }
    }
    switch (a) {
    case 0:
        break;
    case 1:
        for (row = 1; row < 6; row++) {
            for (col = 1; col < 9; col++) {
                enabled[col][row] = 0;
            }
        }
        break;
    case 2:
        for (row = 0; row < 6; row++) {
            for (col = 0; col < 10; col++) {
                enabled[col][row] = 0;
            }
        }
        enabled[0][6] = 1;
        enabled[9][6] = 1;
        enabled[0][5] = 1;
        enabled[9][5] = 1;
        break;
    }
    for (row = 0; row < 8; row++) {
        for (col = 0; col < 10; col++) {
            if (enabled[col][row] != 0) {
                spr = (SPRT *)GetWorkBase(0x14, draw_buffer);
                func_8009F0A4(spr);
                SetSemiTrans(spr, 0);
                x = col * 32;
                y = (s32)((u32)row * 32 + 240);
                spr->x0 = x;
                spr->y0 = y - draw_buffer * 240;
                spr->u0 = (D_800B5BCC & 31) + 128;
                spr->v0 = (D_800B5BD0 & 31) + 128;
                spr->h = 32;
                spr->w = 32;
                spr->r0 = game_state.screen_brightness;
                spr->g0 = game_state.screen_brightness;
                spr->b0 = game_state.screen_brightness;
                spr->clut = func_8009ED74(0xA0, 0x1E1);
                AddPrim(D_800E8CA0 + (draw_buffer << 10) + (u8)D_800B5BD4 * 4, (s32)spr);
            }
        }
    }
    goto_tpage(0x3C0, 0x100, (u8)D_800B5BD4, 0, 0);
    if ((game_state.frame_count & 3) == 3 && (u8)D_800B5BC8 >= 0xEF) {
        fade = ++D_800B5BC8;
        if (fade == 255) {
            current++;
            LoadSquare(0xA0, 0x1E1, 16, 1, D_800B5B28[(current + 3) & 3]);
            return;
        }
        if (fade >= 240) {
            next_weight = fade - 239;
            old_weight = 255 - fade;
            old_index = (current + 3) & 3;
            next_index = (current + 4) & 3;
            old = (ColorPair555 *)D_800B5B28[old_index];
            next = (ColorPair555 *)D_800B5B28[next_index];
            out = (ColorPair555 *)D_800B5BA8;
            for (col = 0; col < 8; col++) {
                blend = old_weight * old->r0 + next_weight * next->r0;
                out->r0 = blend >> 4;
                blend = old_weight * old->g0 + next_weight * next->g0;
                out->g0 = blend >> 4;
                blend = old_weight * old->b0 + next_weight * next->b0;
                out->b0 = blend >> 4;
                blend = old_weight * old->r1 + next_weight * next->r1;
                out->r1 = blend >> 4;
                blend = old_weight * old->g1 + next_weight * next->g1;
                out->g1 = blend >> 4;
                blend = old_weight * old->b1 + next_weight * next->b1;
                out->b1 = blend >> 4;
                old++;
                next++;
                out++;
            }
            LoadSquare(0xA0, 0x1E1, 16, 1, D_800B5BA8);
        }
    }
}

void hud_vram_init(void) {
    s32 i;
    s32 j;

    LoadSquare(0x3E0, 0x180, 0x20, 0x80, D_800B374C);
    for (i = 0; i < 32; i++) {
        /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
        for (j = 0; j < 4; j++) game_state.scratch_words[i * 4 + j] = 0;
    }
    LoadSquare(0x3E0, 0x1C0, 4, 0x10, game_state.scratch_words);
}

void hud_season_bgm_load(void) {
    s32 v;

    audio_command(5);
    audio_command(0x71);
    v = hud_season_index() & 3;
    audio_bank_load_start(0, 0xBF98, 0xBF79, D_800B5BE8[v], D_800B5BF8[v], D_800B5BD8[v]);
}
