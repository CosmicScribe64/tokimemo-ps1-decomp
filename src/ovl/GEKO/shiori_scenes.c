/* shiori_scenes.c: src/ovl/GEKO/80139910.c:48,73,218,256,446; Shiori selectors1/2/8, callback/resources and birthday-score initialization. Current GEKO folder retained; no narrative meaning is inferred from callback ordinals. Original object 0x80139910-0x8013AF80 (config/objects/GEKO.txt). */
#define MAIN_API_OVERRIDE_talk_work /* switched as u32 (main_api.h: s32), T-6030 */
#include "common.h"
#include "ovl/GEKO.h"

extern u32 talk_work;


typedef struct {
    void (*f[55])();
} FnTbl55; /* size 0xDC */
extern FnTbl55 D_80146518;

typedef struct {
    void (*f[39])();
} FnTbl39; /* size 0x9C */
extern FnTbl39 D_80146420;

void func_80139910(void) {
    D_80146380 = 0x801D6394;
    D_80146384 = 0x801D639C;
    D_80146388 = 0x801D63BC;
    D_8014638C = *(s16 *)0x801D63D0;
    D_80146390 = 0x801B0000;
    D_80146394 = 0x801D6000;
    D_80146398 = 0x801B2000;
    D_8014639C = 0x801B6000;
    D_801463A0 = 0x801BA000;
    D_801463A4 = 0x801BE000;
    D_801463A8 = 0x801C2000;
    D_801463AC = 0x801C6000;
    D_801463B0 = 0x801D2000;
}

void func_801399DC(void) {
    D_801463B4 = 0x801D22D0;
    D_801463B8 = 0x801D22D8;
    D_801463BC = 0x801D233C;
    D_801463C0 = *(s16 *)0x801D2358;
    D_801463C4 = 0x801B0000;
    D_801463C8 = 0x801B2000;
    D_801463CC = 0x801B6000;
    D_801463D0 = 0x801BA000;
    D_801463D4 = 0x801BE000;
    D_801463D8 = 0x801C2000;
    D_801463DC = 0x801C6000;
}

void func_80139A8C(void) {
    switch (talk_work) {
    case 1:
        func_80139B08();
        return;
    case 2:
        func_8013A144();
        return;
    case 8:
        func_8013A230();
        return;
    default:
        game_mode_reset_to_title();
        return;
    }
}

void func_80139B08(void) {
    if (game_state.mode_stage == 0) {
        func_80139B44();
        return;
    }
    game_mode_reset_to_title();
}

void func_80139B44(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl39 tbl;

    tbl = D_80146420;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
    func_80139E1C();
}

void func_80139BC0(void) {
    audio_command(0x505);
    audio_command(0x506);
    mode_step_next();
}

void func_80139BF0(void) {
    LoadSquare(0x140, 0, 0x40, 0x80, D_801463B0);
    game_state.tpage_buf[5] = 1;
    mode_step_next();
}

void func_80139C38(void) {
    cd_read_sectors(0x4D, 0x801B0000, 0x7F14);
    func_80139910();
    mode_step_next();
}

void func_80139C70(void) {
    s16 i;
    u8 state;
    u8 colour;

    state = game_state.mode_step;
    func_800853FC();
    colour = (u8)dec_bg_brightness;

    for (i = 4; i < 19; i++) {
        if (colour < 0x60U) {
            sprite_quads[i].tint_r = sprite_quads[i].tint_g = sprite_quads[i].tint_b = colour;
        }
    }
    if (state != game_state.mode_step) {
        for (i = 4; i < 19; i++) {
            sprite_quads[i].flags |= 0x80000000;
            audio_command(0x1506);
        }
    }
}

void func_80139D54(void) {
    s16 i;
    u8 colour;

    if (game_state.mode_step_timer.w == 0) {
        for (i = 4; i < 19; i++) {
            sprite_quads[i].flags &= 0x7FFFFFFF;
        }
    }
    func_80085368();
    colour = (u8)dec_bg_brightness;
    for (i = 4; i < 19; i++) {
        if (colour < 0x61U) {
            sprite_quads[i].tint_r = sprite_quads[i].tint_g = sprite_quads[i].tint_b = colour;
        }
    }
}

/* FAKE: s32 return type without a return statement keeps $v0 live so as1 fills the beqz delay slot from the target block. T-24081 */
s32 func_80139E1C(void) {
    s16 i;

    for (i = 4; i < 0x13; i++) {
        sprite_quads[i].y += 0xA;
        if (((rand() & 3) == 0 && sprite_quads[i].y >= 0x11 && D_801463E0[i - 4] == 0 && D_80146400[i - 4] == 0) ||
            D_801463E0[i - 4] != 0 || D_80146400[i - 4] != 0) {
            sprite_quads[i].y -= 0xA;
            if (D_801463E0[i - 4] == 0) {
                if (D_80146400[i - 4] == 0) {
                    if (sprite_quads[i].x < -0x57) {
                        sprite_quads[i].y = rand() % 4 + 0x1C;
                    } else {
                        sprite_quads[i].y = rand() % 15 + 0x11;
                    }
                    sprite_quads[i].x -= 0xC;
                    /* FAKE: signed views of unk_0E/unk_0A share constants 8 and 0x18 with unk_08 in $s3/$s5. T-24081 */
                    *(s8 *)&sprite_quads[i].unk_0E = 8;
                    sprite_quads[i].w = 0x18;
                    *(s16 *)&sprite_quads[i].h = 8;
                    D_80146400[i - 4] += 1;
                } else {
                    D_80146400[i - 4] = 0;
                    D_801463E0[i - 4] += 1;
                }
            } else if (D_801463E0[i - 4] == 5) {
                sprite_quads[i].unk_0E = 0;
                sprite_quads[i].w = 8;
                *(s16 *)&sprite_quads[i].h = 0x18;
                sprite_quads[i].x = rand() % 167 - 0x9F;
                if (sprite_quads[i].x >= -0x57) {
                    sprite_quads[i].x += 0x99;
                }
                sprite_quads[i].y = -0x5F;
                D_801463E0[i - 4] = 0;
                D_80146400[i - 4] = 0;
            } else if (D_801463E0[i - 4] == 4) {
                if (D_80146400[i - 4] == 0) {
                    sprite_quads[i].unk_0E -= 0x18;
                    D_80146400[i - 4] += 1;
                } else {
                    D_80146400[i - 4] = 0;
                    D_801463E0[i - 4] += 1;
                }
            } else {
                if (D_80146400[i - 4] == 0) {
                    sprite_quads[i].unk_0E += 0x18;
                    D_80146400[i - 4] += 1;
                } else {
                    D_80146400[i - 4] = 0;
                    D_801463E0[i - 4] += 1;
                }
            }
        } else if (sprite_quads[i].y >= 0x11 && D_801463E0[i - 4] == 0 && D_80146400[i - 4] == 0) {
            sprite_quads[i].x = rand() % 167 - 0x9F;
            if (sprite_quads[i].x >= -0x57) {
                sprite_quads[i].x += 0x99;
            }
            sprite_quads[i].y = -0x5F;
        }
    }
}

void func_8013A144(void) {
    if (game_state.mode_stage == 0) {
        func_8013A180();
        return;
    }
    game_mode_reset_to_title();
}

typedef struct {
    void (*f[23])();
} FnTbl23; /* size 0x5C */
extern FnTbl23 D_801464BC;

void func_8013A180(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl23 tbl;

    tbl = D_801464BC;
    idx = game_state.mode_step;
    tbl.f[idx]();
    if (game_state.mode_step >= 0xD) {
        if (game_state.mode_step < 0x14) {
            sprite_light_effect_draw();
        }
    }
}

void func_8013A230(void) {
    if (game_state.mode_stage == 0) {
        func_8013A2EC();
        return;
    }
    game_mode_reset_to_title();
}

void func_8013A26C(void) {
    s32 temp_v0;

    D_800B3D60 = 0;
    temp_v0 = dec_bg_cd_read(0x3FBE, 0);
    if (temp_v0 == *dec_bg_show) {
        mode_step_next();
        mode_step_next();
        mode_step_next();
    } else if (temp_v0 == (1 - *dec_bg_show)) {
        mode_step_next();
        mode_step_next();
    }
    mode_step_next();
}

void func_8013A2EC(void) {
    void (**p)();
    FnTbl55 tbl;

    tbl = D_80146518;
    tbl.f[game_state.mode_step](0x80);
    p = &tbl.f[game_state.mode_step];
    if (tbl.f[game_state.mode_step] == func_80085368) {
        D_801206DB |= 0x80;
    }
    if (func_800853FC == *p) {
        /* FAKE: empty double test (decomp-permuter, score 20), same register-choice shift as SHOUGATU func_80136078 (T-4090); real source unknown. T-10820 */
        if (!game_state.mode_step && !game_state.mode_step) {
        }
        D_801206DB &= 0xFF7F;
    }
    func_8013A668();
}

s32 func_8013A3C4(void) {
    u16 ca154;

    /* FAKE: u16 temp on the first D_800CA154 test keeps uopt from CSE-promoting D_800CA154 to $v0. T-24095 */
    if ((game_state.pad_pressed & 0x400040) && (ca154 = D_800CA154) != 0 && D_800CA154 < 4U && game_state.mode_step_timer.u >= 2U) {
        audio_command(0x1508);
        D_80122D00 = 0;
        k_reset(1);
        D_800CA2AC = 0;
        D_800CA154 = 3;
    } else if (check_end_k() != 0 && D_800CA154 < 4U) {
        if (game_state.substep++ < 0x40U) {
            return 0;
        }
        game_state.substep = 0;
        k_reset(1);
    }
    if (D_800CA154 < 3U) {
        func_80138B74();
    } else {
        func_80138AF8();
    }
    /* FAKE: u16 cast on D_800CA150 emits lhu instead of lh. T-24095 */
    if (game_state.mode_step_timer.u++ == 0 && D_800CA154 < 4U && (u16)D_800CA150 == 0) {
        audio_command(0x609);
    }
    if (game_state.mode_step_timer.u++ == 1 && D_800CA154 == 5) {
        audio_command(0x60A);
    }
}

void func_8013A564(void) {
    cd_read_sectors(0x45, 0x801B0000, 0x80BA);
    func_801399DC();
    mode_step_next();
}

void func_8013A59C(void) {
    D_800B3D60 = 0;
    audio_bank_load_start(1, 0xC007, 0xBFF7, 0xCBA3, 0xCB53, 0xCB4E);
    mode_step_next();
}

s32 func_8013A5E8(void) {
    if (game_state.mode_step_timer.w == 0) {
        audio_command(0x500);
    }
    if (game_state.mode_step_timer.w++ == 0x40) {
        mode_step_next();
    }
}

void func_8013A640(void) {
    portrait_read_start(0x57CE);
    mode_step_next();
}

void func_8013A668(void) {
    if ((u8) game_state.mode_step >= 0x23U) {
        D_801206DF = D_80120657;
    }
    if (((u8) game_state.mode_step >= 0x23U) && (((u32) game_state.frame_count % 300U) == 0x14)) {
        D_801206DA |= 3;
    }
}

void func_8013A6D0(void) {
    game_state.current_girl = GIRL_SHIORI;
    draw2d3d(1, 0);
    actor_draw_enable(1);
    sprite_tables_clear();
    actor_clear_range(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_system_reset();
    message_window_set_title("");
    D_8011ED9F = 0;
    D_8011EDE3 = 0;
    D_800CA150 = 0;
    D_800CA154 = 0;
    talk_clear();
    addr_init_bustup();
    hizuke_init();
    message_window_init();
    text_marker_hide();
    talk_names_set(game_state.current_girl);
    func_80137990();
    D_80146274 = D_80145ED8;
    D_80146278 = D_80146014;
    D_8014627C = D_80146150;
    strcpy(D_800CA19C, "教室");
    if (talk_work == 1) {
        game_state.girls[0].tokimeki += 1;
        game_state.girls[0].affection += 2;
        check_para_limit();
    }
    D_800CA368 = 0;
    mode_step_next();
}

void func_8013A820(void) {
    s16 i;
    SpriteQuad *p;
    u16 clut;

    func_80139910();
    load_palette(D_80146390, 0x11, 1, 2, 0);
    scene_still_show(D_80146398, D_8014639C, D_801463A0, D_801463A4, D_801463A8, D_801463AC);
    scene_anim_show(D_80146384, D_80146388, D_80146380, D_8014638C);
    load_palette(D_80146394, 0x12, 1, 3, 0);
    for (i = 4; i < 0x13; i++) {
        p = &sprite_quads[i];
        /* FAKE: u16 clut local before unk_04 call shifts the i * 2 CSE temporary to $v0. T-24081 */
        clut = 0x1F2;
        p->flags = 0xC1000000;
        p->x = rand() % 320 - 0x9F;
        p->y = -0x78;
        p->w = 8;
        /* FAKE: s16 view of the u16 member shares constant 8 with unk_08 in $s2. T-24081 */
        *(s16 *)&p->h = 8;
        p->unk_0C = 5;
        p->unk_0E = 0;
        p->unk_0F = 0;
        p->unk_10 = 0;
        p->unk_12 = clut;
        p->tint_r = 0;
        p->tint_g = 0;
        p->tint_b = 0;
        p->anchor_x = 0;
        p->anchor_y = 0;
        p->unk_1C = 0x1000;
        p->unk_1E = 0x1000;
        p->unk_20 = 0;
        game_state.quad_layer[i] = 0xC;
        D_801463E0[i - 4] = 0;
        D_80146400[i - 4] = 0;
    }
    mode_step_next();
}

void func_8013AA1C(void) {
    game_state.current_girl = GIRL_SHIORI;
    draw2d3d(1, 0);
    actor_draw_enable(1);
    sprite_tables_clear();
    actor_clear_range(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_system_reset();
    message_window_set_title("");
    D_8011ED9F = 0;
    D_8011EDE3 = 0;
    D_800CA150 = 0;
    D_800CA154 = 0;
    addr_init_bustup();
    hizuke_init();
    message_window_init();
    text_marker_hide();
    talk_names_set(game_state.current_girl);
    talk_clear();
    func_80137990();
    D_80146274 = D_80145EDC;
    D_80146278 = D_80146018;
    D_8014627C = D_80146154;
    strcpy(D_800CA19C, "自宅前");
    game_state.param_stress.value -= 1;
    check_para_limit();
    D_800CA368 = 1;
    mode_step_next();
}

void func_8013AB4C(void) {
    s16 i;

    func_8006509C();
    for (i = 0; i < 0xF; i++) {
        icon_can_use_set(i, 0);
    }
    hud_icon_bar_update(1);
    parameter_show_init();
    parameter_show_init();
    talk_clear();
    message_window_init();
    text_marker_hide();
    hizuke_init();
    talk_clear();
    /* FAKE: u8 pointer cast on unk_71E emits lbu for the bitwise OR. T-24095 */
    *(u8 *)&game_state.day_event_done_bits |= 5;
    D_80122D38 = 1;
    game_state.screen_brightness = 0x80;
    dec_bg_brightness = 0x80;
    D_800B5940 = 0;
    srand(game_state.frame_count);
    game_state.current_girl = GIRL_SHIORI;
    strcpy(D_800CA19C, "詩織の部屋");
    strcpy(D_800CA1DC, "自宅前");
    D_800CA150 = 0;
    D_800CA154 = 0;
    addr_init_bustup();
    talk_names_set(game_state.current_girl);
    func_80137990();
    D_80146274 = D_80145EF4;
    D_80146278 = D_80146030;
    D_8014627C = D_8014616C;
    game_state.girls[0].tokimeki += 2;
    game_state.girls[0].affection += 2;
    check_para_limit();
    mode_step_next();
}

void func_8013ACE0(void) {
    draw2d3d(1, 0);
    actor_draw_enable(1);
    sprite_tables_clear();
    actor_clear_range(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_system_reset();
    talk_clear();
    text_marker_hide();
    hizuke_init();
    message_window_init();
    dec_bg_brightness = 0;
    D_800CA368 = 7;
    mode_step_next();
}

void func_8013AD6C(void) {
    func_801399DC();
    load_palette(D_801463C4, 0x11, 1, 2, 0);
    scene_still_show(D_801463C8, D_801463CC, D_801463D0, D_801463D4, D_801463D8, D_801463DC);
    scene_anim_show(D_801463B8, D_801463BC, D_801463B4, D_801463C0);
    ((Actor *)actor_pool_96)[1].unk_04 = 8;
    ((Actor *)actor_pool_96)->unk_04 = 8;
    ((Actor *)actor_pool_96)[2] = ((Actor *)actor_pool_96)[0];
    ((Actor *)actor_pool_96)[2].sequence_slot = 4;
    mode_step_next();
}

void func_8013AE7C(void) {
    bg_read_sub2(0x4068);
    mode_step_next();
}

void func_8013AEA4(void) {
    bg_read_sub2(0x46AA);
    mode_step_next();
}

void func_8013AECC(void) {
    bg_read_sub2(0x414C);
    mode_step_next();
}

void func_8013AEF4(void) {
    bg_read_sub2(0x4171);
    mode_step_next();
}

void func_8013AF1C(void) {
    bg_read_sub2(0x473A);
    mode_step_next();
}

void func_8013AF44(void) {
    func_800634FC(get_g_zyotai_h(GIRL_SHIORI));
    bg_read_sub2(0x4156);
    mode_step_next();
}
