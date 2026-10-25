/* shiori_scenes.c: src/ovl/GEKO/80139910.c:48,73,218,256,446; Shiori selectors1/2/8, callback/resources and birthday-score initialization. Current GEKO folder retained; no narrative meaning is inferred from callback ordinals. Original object 0x80139910-0x8013AF80 (config/objects/GEKO.txt). */
#define MAIN_API_OVERRIDE_talk_work /* switched as u32 (main_api.h: s32), T-6030 */
#include "common.h"
#include "ovl/GEKO.h"

extern u32 talk_work;


typedef struct {
    void (*callbacks[55])(); /* HYPOTHESIS: GEKO grouping vocabulary remains unconfirmed; T-27051; src/ovl/GEKO/80139910.c:10; func_8013A2EC at line256 copies this exact55-callback table and dispatches by mode_step; scene number is the actual talk_work selector, not a narrative identity. */
} EncounterShioriSceneEightSteps; /* size 0xDC */
extern EncounterShioriSceneEightSteps encounter_shiori_scene_eight_steps;

typedef struct {
    void (*f[39])();
} FnTbl39; /* size 0x9C */
extern FnTbl39 encounter_shiori_scene_one_steps;

void encounter_shiori_assets_bind_1(void) {
    encounter_shiori_pack_1_animation_input_2 = 0x801D6394;
    encounter_shiori_pack_1_animation_input_0 = 0x801D639C;
    encounter_shiori_pack_1_animation_input_1 = 0x801D63BC;
    encounter_shiori_pack_1_animation_input_3 = *(s16 *)0x801D63D0;
    encounter_shiori_pack_1_palette = 0x801B0000;
    encounter_falling_quad_palette = 0x801D6000;
    encounter_shiori_pack_1_still_image_0 = 0x801B2000;
    encounter_shiori_pack_1_still_image_1 = 0x801B6000;
    encounter_shiori_pack_1_still_image_2 = 0x801BA000;
    encounter_shiori_pack_1_still_image_3 = 0x801BE000;
    encounter_shiori_pack_1_still_image_4 = 0x801C2000;
    encounter_shiori_pack_1_still_image_5 = 0x801C6000;
    D_801463B0 = 0x801D2000;
}

void encounter_shiori_assets_bind_2(void) {
    encounter_shiori_pack_2_animation_input_2 = 0x801D22D0;
    encounter_shiori_pack_2_animation_input_0 = 0x801D22D8;
    encounter_shiori_pack_2_animation_input_1 = 0x801D233C;
    encounter_shiori_pack_2_animation_input_3 = *(s16 *)0x801D2358;
    encounter_shiori_pack_2_palette = 0x801B0000;
    encounter_shiori_pack_2_still_image_0 = 0x801B2000;
    encounter_shiori_pack_2_still_image_1 = 0x801B6000;
    encounter_shiori_pack_2_still_image_2 = 0x801BA000;
    encounter_shiori_pack_2_still_image_3 = 0x801BE000;
    encounter_shiori_pack_2_still_image_4 = 0x801C2000;
    encounter_shiori_pack_2_still_image_5 = 0x801C6000;
}

void encounter_shiori_scene_select_1(void) {
    switch (talk_work) {
    case 1:
        encounter_shiori_scene_one_dispatch();
        return;
    case 2:
        encounter_shiori_scene_two_dispatch();
        return;
    case 8:
        encounter_shiori_scene_eight_dispatch();
        return;
    default:
        game_mode_reset_to_title();
        return;
    }
}

void encounter_shiori_scene_one_dispatch(void) {
    if (game_state.mode_stage == 0) {
        encounter_shiori_scene_one_step_dispatch();
        return;
    }
    game_mode_reset_to_title();
}

void encounter_shiori_scene_one_step_dispatch(void) {
    s32 mode_step_index; /* declared before mode_step_callbacks: its stack slot sits above mode_step_callbacks (T-3330) */
    FnTbl39 mode_step_callbacks;

    mode_step_callbacks = encounter_shiori_scene_one_steps;
    mode_step_index = game_state.mode_step;
    mode_step_callbacks.f[mode_step_index](0x80);
    encounter_shiori_falling_quads_update();
}

void encounter_shiori_audio_command_1(void) {
    audio_command(0x505);
    audio_command(0x506);
    mode_step_next();
}

void encounter_shiori_extra_image_load(void) {
    LoadSquare(0x140, 0, 0x40, 0x80, D_801463B0);
    game_state.tpage_buf[5] = 1;
    mode_step_next();
}

void encounter_shiori_assets_load_1(void) {
    cd_read_sectors(0x4D, 0x801B0000, 0x7F14);
    encounter_shiori_assets_bind_1();
    mode_step_next();
}

void encounter_shiori_still_fade_out(void) {
    s16 i;
    u8 mode_step_before;
    u8 colour;

    mode_step_before = game_state.mode_step;
    scene_still_fade_out();
    colour = (u8)bg_brightness;

    for (i = 4; i < 19; i++) {
        if (colour < 0x60U) {
            sprite_quads[i].tint_r = sprite_quads[i].tint_g = sprite_quads[i].tint_b = colour;
        }
    }
    if (mode_step_before != game_state.mode_step) {
        for (i = 4; i < 19; i++) {
            sprite_quads[i].flags |= 0x80000000;
            audio_command(0x1506);
        }
    }
}

void encounter_shiori_still_fade_in(void) {
    s16 i;
    u8 colour;

    if (game_state.mode_step_timer.w == 0) {
        for (i = 4; i < 19; i++) {
            sprite_quads[i].flags &= 0x7FFFFFFF;
        }
    }
    scene_still_fade_in();
    colour = (u8)bg_brightness;
    for (i = 4; i < 19; i++) {
        if (colour < 0x61U) {
            sprite_quads[i].tint_r = sprite_quads[i].tint_g = sprite_quads[i].tint_b = colour;
        }
    }
}

/* FAKE: s32 return type without a return statement keeps $v0 live so as1 fills the beqz delay slot from the target block. T-24081 */
s32 encounter_shiori_falling_quads_update(void) {
    s16 i;

    for (i = 4; i < 0x13; i++) {
        sprite_quads[i].y += 0xA;
        if (((rand() & 3) == 0 && sprite_quads[i].y >= 0x11 && encounter_falling_quad_phases[i - 4] == 0 && encounter_falling_quad_delays[i - 4] == 0) ||
            encounter_falling_quad_phases[i - 4] != 0 || encounter_falling_quad_delays[i - 4] != 0) {
            sprite_quads[i].y -= 0xA;
            if (encounter_falling_quad_phases[i - 4] == 0) {
                if (encounter_falling_quad_delays[i - 4] == 0) {
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
                    encounter_falling_quad_delays[i - 4] += 1;
                } else {
                    encounter_falling_quad_delays[i - 4] = 0;
                    encounter_falling_quad_phases[i - 4] += 1;
                }
            } else if (encounter_falling_quad_phases[i - 4] == 5) {
                sprite_quads[i].unk_0E = 0;
                sprite_quads[i].w = 8;
                *(s16 *)&sprite_quads[i].h = 0x18;
                sprite_quads[i].x = rand() % 167 - 0x9F;
                if (sprite_quads[i].x >= -0x57) {
                    sprite_quads[i].x += 0x99;
                }
                sprite_quads[i].y = -0x5F;
                encounter_falling_quad_phases[i - 4] = 0;
                encounter_falling_quad_delays[i - 4] = 0;
            } else if (encounter_falling_quad_phases[i - 4] == 4) {
                if (encounter_falling_quad_delays[i - 4] == 0) {
                    sprite_quads[i].unk_0E -= 0x18;
                    encounter_falling_quad_delays[i - 4] += 1;
                } else {
                    encounter_falling_quad_delays[i - 4] = 0;
                    encounter_falling_quad_phases[i - 4] += 1;
                }
            } else {
                if (encounter_falling_quad_delays[i - 4] == 0) {
                    sprite_quads[i].unk_0E += 0x18;
                    encounter_falling_quad_delays[i - 4] += 1;
                } else {
                    encounter_falling_quad_delays[i - 4] = 0;
                    encounter_falling_quad_phases[i - 4] += 1;
                }
            }
        } else if (sprite_quads[i].y >= 0x11 && encounter_falling_quad_phases[i - 4] == 0 && encounter_falling_quad_delays[i - 4] == 0) {
            sprite_quads[i].x = rand() % 167 - 0x9F;
            if (sprite_quads[i].x >= -0x57) {
                sprite_quads[i].x += 0x99;
            }
            sprite_quads[i].y = -0x5F;
        }
    }
}

void encounter_shiori_scene_two_dispatch(void) {
    if (game_state.mode_stage == 0) {
        encounter_shiori_scene_two_step_dispatch();
        return;
    }
    game_mode_reset_to_title();
}

typedef struct {
    void (*f[23])();
} FnTbl23; /* size 0x5C */
extern FnTbl23 encounter_shiori_scene_two_steps;

void encounter_shiori_scene_two_step_dispatch(void) {
    s32 mode_step_index; /* declared before mode_step_callbacks: its stack slot sits above mode_step_callbacks (T-3330) */
    FnTbl23 mode_step_callbacks;

    mode_step_callbacks = encounter_shiori_scene_two_steps;
    mode_step_index = game_state.mode_step;
    mode_step_callbacks.f[mode_step_index]();
    if (game_state.mode_step >= 0xD) {
        if (game_state.mode_step < 0x14) {
            hud_sprite_light_effect_draw();
        }
    }
}

void encounter_shiori_scene_eight_dispatch(void) {
    if (game_state.mode_stage == 0) {
        encounter_shiori_scene_eight_step_dispatch();
        return;
    }
    game_mode_reset_to_title();
}

void encounter_shiori_birthday_bg_request(void) {
    s32 background_read_result;

    cd_read_checked = 0;
    background_read_result = dec_bg_cd_read(0x3FBE, 0);
    if (background_read_result == *dec_bg_show) {
        mode_step_next();
        mode_step_next();
        mode_step_next();
    } else if (background_read_result == (1 - *dec_bg_show)) {
        mode_step_next();
        mode_step_next();
    }
    mode_step_next();
}

void encounter_shiori_scene_eight_step_dispatch(void) {
    void (**current_callback)();
    EncounterShioriSceneEightSteps mode_step_callbacks;

    mode_step_callbacks = encounter_shiori_scene_eight_steps;
    mode_step_callbacks.callbacks[game_state.mode_step](0x80);
    current_callback = &mode_step_callbacks.callbacks[game_state.mode_step];
    if (mode_step_callbacks.callbacks[game_state.mode_step] == scene_still_fade_in) {
        D_801206DB |= 0x80;
    }
    if (scene_still_fade_out == *current_callback) {
        /* FAKE: empty double test (decomp-permuter, score 20), same register-choice shift as SHOUGATU func_80136078 (T-4090); real source unknown. T-10820 */
        if (!game_state.mode_step && !game_state.mode_step) {
        }
        D_801206DB &= 0xFF7F;
    }
    encounter_shiori_birthday_portrait_update();
}

s32 encounter_shiori_birthday_phone_talk(void) {
    u16 line_copy;

    /* FAKE: u16 temp on the first D_800CA154 test keeps uopt from CSE-promoting D_800CA154 to $v0. T-24095 */
    if ((game_state.pad_pressed & 0x400040) && (line_copy = D_800CA154) != 0 && D_800CA154 < 4U && game_state.mode_step_timer.u >= 2U) {
        audio_command(0x1508);
        D_80122D00 = 0;
        k_reset(1);
        talk_voice_wait_frames = 0;
        D_800CA154 = 3;
    } else if (check_end_k() != 0 && D_800CA154 < 4U) {
        if (game_state.substep++ < 0x40U) {
            return 0;
        }
        game_state.substep = 0;
        k_reset(1);
    }
    if (D_800CA154 < 3U) {
        encounter_scene_phone_talk();
    } else {
        encounter_scene_talk();
    }
    /* FAKE: u16 cast on D_800CA150 emits lhu instead of lh. T-24095 */
    if (game_state.mode_step_timer.u++ == 0 && D_800CA154 < 4U && (u16)D_800CA150 == 0) {
        audio_command(0x609);
    }
    if (game_state.mode_step_timer.u++ == 1 && D_800CA154 == 5) {
        audio_command(0x60A);
    }
}

void encounter_shiori_assets_load_2(void) {
    cd_read_sectors(0x45, 0x801B0000, 0x80BA);
    encounter_shiori_assets_bind_2();
    mode_step_next();
}

void encounter_shiori_audio_load_1(void) {
    cd_read_checked = 0;
    audio_bank_load_start(1, 0xC007, 0xBFF7, 0xCBA3, 0xCB53, 0xCB4E);
    mode_step_next();
}

s32 encounter_shiori_audio_command_wait(void) {
    if (game_state.mode_step_timer.w == 0) {
        audio_command(0x500);
    }
    if (game_state.mode_step_timer.w++ == 0x40) {
        mode_step_next();
    }
}

void encounter_shiori_portrait_read(void) {
    portrait_read_start(0x57CE);
    mode_step_next();
}

void encounter_shiori_birthday_portrait_update(void) {
    if ((u8) game_state.mode_step >= 0x23U) {
        D_801206DF = D_80120657;
    }
    if (((u8) game_state.mode_step >= 0x23U) && (((u32) game_state.frame_count % 300U) == 0x14)) {
        D_801206DA |= 3;
    }
}

void encounter_shiori_scene_init_1(void) {
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
    encounter_scene_scripts_bind();
    encounter_scene_text_table = encounter_scene_text_section_6;
    encounter_scene_command_table = encounter_scene_command_section_6;
    encounter_scene_voice_table = encounter_scene_voice_section_6;
    strcpy(scene_place_title, "教室");
    if (talk_work == 1) {
        game_state.girls[0].tokimeki += 1;
        game_state.girls[0].affection += 2;
        check_para_limit();
    }
    D_800CA368 = 0;
    mode_step_next();
}

void encounter_shiori_still_show_1(void) {
    s16 i;
    SpriteQuad *p;
    u16 clut;

    encounter_shiori_assets_bind_1();
    load_palette(encounter_shiori_pack_1_palette, 0x11, 1, 2, 0);
    scene_still_show(encounter_shiori_pack_1_still_image_0, encounter_shiori_pack_1_still_image_1, encounter_shiori_pack_1_still_image_2, encounter_shiori_pack_1_still_image_3, encounter_shiori_pack_1_still_image_4, encounter_shiori_pack_1_still_image_5);
    scene_anim_show(encounter_shiori_pack_1_animation_input_0, encounter_shiori_pack_1_animation_input_1, encounter_shiori_pack_1_animation_input_2, encounter_shiori_pack_1_animation_input_3);
    load_palette(encounter_falling_quad_palette, 0x12, 1, 3, 0);
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
        encounter_falling_quad_phases[i - 4] = 0;
        encounter_falling_quad_delays[i - 4] = 0;
    }
    mode_step_next();
}

void encounter_shiori_scene_init_2(void) {
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
    encounter_scene_scripts_bind();
    encounter_scene_text_table = encounter_scene_text_section_7;
    encounter_scene_command_table = encounter_scene_command_section_7;
    encounter_scene_voice_table = encounter_scene_voice_section_7;
    strcpy(scene_place_title, "自宅前");
    game_state.param_stress.value -= 1;
    check_para_limit();
    D_800CA368 = 1;
    mode_step_next();
}

void encounter_shiori_birthday_init(void) {
    s16 i;

    hud_icon_bar_init();
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
    bg_brightness = 0x80;
    bg_fade_amount = 0;
    srand(game_state.frame_count);
    game_state.current_girl = GIRL_SHIORI;
    strcpy(scene_place_title, "詩織の部屋");
    strcpy(scene_place_title_2, "自宅前");
    D_800CA150 = 0;
    D_800CA154 = 0;
    addr_init_bustup();
    talk_names_set(game_state.current_girl);
    encounter_scene_scripts_bind();
    encounter_scene_text_table = encounter_scene_text_section_13;
    encounter_scene_command_table = encounter_scene_command_section_13;
    encounter_scene_voice_table = encounter_scene_voice_section_13;
    game_state.girls[0].tokimeki += 2;
    game_state.girls[0].affection += 2;
    check_para_limit();
    mode_step_next();
}

void encounter_shiori_birthday_scene_init(void) {
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
    bg_brightness = 0;
    D_800CA368 = 7;
    mode_step_next();
}

void encounter_shiori_still_show_2(void) {
    encounter_shiori_assets_bind_2();
    load_palette(encounter_shiori_pack_2_palette, 0x11, 1, 2, 0);
    scene_still_show(encounter_shiori_pack_2_still_image_0, encounter_shiori_pack_2_still_image_1, encounter_shiori_pack_2_still_image_2, encounter_shiori_pack_2_still_image_3, encounter_shiori_pack_2_still_image_4, encounter_shiori_pack_2_still_image_5);
    scene_anim_show(encounter_shiori_pack_2_animation_input_0, encounter_shiori_pack_2_animation_input_1, encounter_shiori_pack_2_animation_input_2, encounter_shiori_pack_2_animation_input_3);
    ((Actor *)actor_pool_96)[1].unk_04 = 8;
    ((Actor *)actor_pool_96)->unk_04 = 8;
    ((Actor *)actor_pool_96)[2] = ((Actor *)actor_pool_96)[0];
    ((Actor *)actor_pool_96)[2].sequence_slot = 4;
    mode_step_next();
}

void encounter_shiori_bg_load_1(void) {
    bg_read_sub2(0x4068);
    mode_step_next();
}

void encounter_shiori_bg_load_2(void) {
    bg_read_sub2(0x46AA);
    mode_step_next();
}

void encounter_shiori_bg_load_3(void) {
    bg_read_sub2(0x414C);
    mode_step_next();
}

void encounter_shiori_bg_load_4(void) {
    bg_read_sub2(0x4171);
    mode_step_next();
}

void encounter_shiori_bg_load_5(void) {
    bg_read_sub2(0x473A);
    mode_step_next();
}

void encounter_shiori_bg_load_6(void) {
    portrait_value_clear_if_high(get_g_zyotai_h(GIRL_SHIORI));
    bg_read_sub2(0x4156);
    mode_step_next();
}
