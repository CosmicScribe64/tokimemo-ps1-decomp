/* megumi_scenes.c: src/ovl/GEKO/8013DF70.c:33,63,73,134; Megumi selectors1/2 and affection-threshold-dependent score/flag changes. Current GEKO folder retained; no narrative meaning is inferred from callback ordinals. Original object 0x8013DF70-0x8013EE40 (config/objects/GEKO.txt). */
#define MAIN_API_OVERRIDE_talk_work /* matched as u32 (main_api.h: s32), switch selector in $v1 (T-6010) */
#include "common.h"
#include "ovl/GEKO.h"

extern u32 talk_work;

typedef struct {
    void (*f[22])();
} FnTbl22; /* size 0x58 */
extern FnTbl22 encounter_megumi_scene_two_steps;

typedef struct {
    void (*callbacks[46])(); /* HYPOTHESIS: GEKO grouping vocabulary remains unconfirmed; T-27051; src/ovl/GEKO/8013DF70.c:14; func_8013E11C at line63 copies this exact46-callback table and dispatches by mode_step; scene number is the actual talk_work selector, not a narrative identity. */
} EncounterMegumiSceneOneSteps; /* size 0xB8 */
extern EncounterMegumiSceneOneSteps encounter_megumi_scene_one_steps;

void encounter_megumi_assets_bind_1(void) {
    encounter_megumi_pack_1_animation_input_2 = 0x801D22F4;
    encounter_megumi_pack_1_animation_input_0 = 0x801D22F8;
    encounter_megumi_pack_1_animation_input_1 = 0x801D2318;
    encounter_megumi_pack_1_animation_input_3 = *(s16 *)0x801D232C;
    encounter_megumi_pack_1_palette = 0x801B0000;
    encounter_falling_quad_palette = 0x801D2000;
    encounter_megumi_pack_1_still_image_0 = 0x801B2000;
    encounter_megumi_pack_1_still_image_1 = 0x801B6000;
    encounter_megumi_pack_1_still_image_2 = 0x801BA000;
    encounter_megumi_pack_1_still_image_3 = 0x801BE000;
    encounter_megumi_pack_1_still_image_4 = 0x801C2000;
    encounter_megumi_pack_1_still_image_5 = 0x801C6000;
    D_801463B0 = 0x801CE000;
}

void encounter_megumi_scene_select_1(void) {
    switch (talk_work) {
    case 1:
        encounter_megumi_scene_one_dispatch();
        return;
    case 2:
        encounter_megumi_scene_two_dispatch();
        return;
    default:
        game_mode_reset_to_title();
        return;
    }
}

void encounter_megumi_scene_one_dispatch(void) {
    if (game_state.mode_stage == 0) {
        encounter_megumi_scene_one_step_dispatch();
        return;
    }
    game_mode_reset_to_title();
}

void encounter_megumi_scene_two_dispatch(void) {
    if (game_state.mode_stage == 0) {
        encounter_megumi_scene_two_step_dispatch();
        return;
    }
    game_mode_reset_to_title();
}

void encounter_megumi_scene_one_step_dispatch(void) {
    s32 mode_step_index; /* declared before mode_step_callbacks: its stack slot sits above mode_step_callbacks (T-3330) */
    EncounterMegumiSceneOneSteps mode_step_callbacks;

    mode_step_callbacks = encounter_megumi_scene_one_steps;
    mode_step_index = game_state.mode_step;
    mode_step_callbacks.callbacks[mode_step_index](0x80);
    encounter_megumi_falling_quads_update();
}

void encounter_megumi_scene_scores_apply(void) {
    if (get_h_yuukou(GIRL_MEGUMI) >= 0x50U) {
        game_state.girls[9].tokimeki += 1;
        game_state.girls[9].affection += 2;
    } else {
        game_state.girls[9].flags.f.b14 = 1;
        game_state.girls[9].affection += 1;
    }
    check_para_limit();
    mode_step_next();
}

void encounter_megumi_falling_quads_init(void) {
    s16 i;
    SpriteQuad *p;

    game_state.tpage_buf[5] = 1;
    LoadSquare(0x140, 0, 0x40, 0x80, D_801463B0);
    encounter_megumi_assets_bind_1();
    load_palette(encounter_falling_quad_palette, 0x12, 1, 3, 0);
    for (i = 4; i < 0xE; i++) {
        p = &sprite_quads[i];
        p->flags = 0xC1000000;
        p->x = rand() % 84 - 0x9F;
        p->y = rand() % 130 - 0x5F;
        p->w = 8;
        p->h = 0x18;
        p->unk_0C = 5;
        p->unk_0E = 0;
        p->unk_0F = 0;
        p->unk_10 = 0;
        p->unk_12 = 0x1F2;
        p->tint_r = 0;
        p->tint_g = 0;
        p->tint_b = 0;
        p->anchor_x = 0;
        p->anchor_y = 0;
        p->unk_1C = 0x1000;
        p->unk_1E = 0x1000;
        p->unk_20 = 0;
        game_state.quad_layer[i] = 0x8C;
        encounter_falling_quad_phases[i - 4] = 0;
        encounter_falling_quad_delays[i - 4] = 0;
    }
    mode_step_next();
}

void encounter_megumi_assets_load_1(void) {
    cd_read_sectors(0x45, 0x801B0000, 0x8A88);
    encounter_megumi_assets_bind_1();
    mode_step_next();
}

void encounter_megumi_still_show_1(void) {
    encounter_megumi_assets_bind_1();
    load_palette(encounter_megumi_pack_1_palette, 0x11, 1, 2, 0);
    scene_still_show(encounter_megumi_pack_1_still_image_0, encounter_megumi_pack_1_still_image_1, encounter_megumi_pack_1_still_image_2, encounter_megumi_pack_1_still_image_3, encounter_megumi_pack_1_still_image_4, encounter_megumi_pack_1_still_image_5);
    scene_anim_show(encounter_megumi_pack_1_animation_input_0, encounter_megumi_pack_1_animation_input_1, encounter_megumi_pack_1_animation_input_2, encounter_megumi_pack_1_animation_input_3);
    mode_step_next();
}

void encounter_megumi_scene_two_step_dispatch(void) {
    s32 mode_step_index; /* declared before mode_step_callbacks: its stack slot sits above mode_step_callbacks (T-3330) */
    FnTbl22 mode_step_callbacks;

    mode_step_callbacks = encounter_megumi_scene_two_steps;
    mode_step_index = game_state.mode_step;
    mode_step_callbacks.f[mode_step_index](0x80);
}

s32 encounter_megumi_talk_portrait_update(void) {
    encounter_scene_talk();
    if (game_state.mode_step_timer.w++ == 1) {
        if ((u16) D_800CA154 == 9) {
            portrait_layout_set(0);
        }
    }
}

void encounter_megumi_low_affection_group_offset(void) {
    if (get_h_yuukou(GIRL_MEGUMI) < 0x50U) {
        D_800CA150 = (u16) D_800CA150 + 2;
    }
    mode_step_next();
}

void encounter_megumi_low_affection_skip(void) {
    if (get_h_yuukou(GIRL_MEGUMI) < 0x50U) {
        game_state.mode_step += 0xE;
    }
    mode_step_next();
}

void encounter_megumi_high_affection_skip(void) {
    if (get_h_yuukou(GIRL_MEGUMI) >= 0x50U) {
        mode_step_next();
        mode_step_next();
        mode_step_next();
    }
    mode_step_next();
}

void encounter_megumi_still_fade_out(void) {
    s16 i;
    SpriteQuad *records;
    s32 mode_step_before;
    s32 mode_step_after;

    mode_step_before = game_state.mode_step;
    scene_still_fade_out();
    records = (SpriteQuad *)sprite_quads;
    for (i = 4; i < 19; i++) {
        if (bg_brightness < 112) {
            records[i].tint_r = records[i].tint_g = records[i].tint_b = bg_brightness;
        }
    }
    mode_step_after = game_state.mode_step;
    if (mode_step_before != mode_step_after) {
        for (i = 4; i < 19; i++) {
            records[i].flags |= 0x80000000;
        }
    }
}

void encounter_megumi_still_fade_in(void) {
    s16 i;
    SpriteQuad *records;

    if (game_state.mode_step_timer.w == 0) {
        records = (SpriteQuad *)sprite_quads;
        for (i = 4; i < 19; i++) {
            records[i].flags &= 0x7FFFFFFF;
        }
    }
    scene_still_fade_in();
    records = (SpriteQuad *)sprite_quads;
    for (i = 4; i < 19; i++) {
        if (bg_brightness < 113) {
            records[i].tint_r = records[i].tint_g = records[i].tint_b = bg_brightness;
        }
    }
}

void encounter_megumi_scene_init_1(void) {
    game_state.current_girl = GIRL_MEGUMI;
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
    encounter_scene_scripts_bind();
    hizuke_init();
    message_window_init();
    text_marker_hide();
    bg_brightness = 0;
    talk_names_set(game_state.current_girl);
    talk_clear();
    encounter_scene_scripts_bind();
    encounter_scene_text_table = encounter_scene_text_section_54;
    encounter_scene_command_table = encounter_scene_command_section_54;
    encounter_scene_voice_table = encounter_scene_voice_section_54;
    strcpy(scene_place_title, "教室");
    D_800CA368 = 0x3C;
    mode_step_next();
}

void encounter_megumi_falling_quads_update(void) {
    s16 i;

    for (i = 4; i < 0xE; i++) {
        sprite_quads[i].y += 0xA;
        if (((rand() & 3) == 0 &&
             ((sprite_quads[i].y >= -0x52 && sprite_quads[i].y < -0x39 && sprite_quads[i].x < -0x57) ||
              (sprite_quads[i].y >= -0x39 && sprite_quads[i].y < -0x1A && sprite_quads[i].x < -0x77) ||
              (sprite_quads[i].y >= -0xB && sprite_quads[i].y < 3 && sprite_quads[i].x >= -0x8D && sprite_quads[i].x < -0x6F) ||
              (sprite_quads[i].y >= 5 && sprite_quads[i].x >= -0x7F && sprite_quads[i].x < -0x5C)) &&
             encounter_falling_quad_phases[i - 4] == 0 && encounter_falling_quad_delays[i - 4] == 0) ||
            encounter_falling_quad_phases[i - 4] != 0 || encounter_falling_quad_delays[i - 4] != 0) {
            sprite_quads[i].y -= 0xA;
            if (encounter_falling_quad_phases[i - 4] == 0) {
                if (encounter_falling_quad_delays[i - 4] == 0) {
                    if (sprite_quads[i].y < -0x44) {
                        sprite_quads[i].unk_0F += 8;
                    }
                    sprite_quads[i].x -= 0xC;
                    sprite_quads[i].y += 0x10;
                    /* FAKE: signed byte/halfword views on SpriteQuad unk_0E/unk_0A share constant registers $s3 (8) and $s5 (0x18) with unk_08, matching encounter_shiori_falling_quads_update. T-24085 */
                    *(s8 *)&sprite_quads[i].unk_0E = 8;
                    sprite_quads[i].w = 0x18;
                    *(s16 *)&sprite_quads[i].h = 8;
                    encounter_falling_quad_delays[i - 4] += 1;
                } else {
                    encounter_falling_quad_delays[i - 4] = 0;
                    encounter_falling_quad_phases[i - 4] += 1;
                }
            } else if (encounter_falling_quad_phases[i - 4] == 4) {
                sprite_quads[i].unk_0E = 0;
                sprite_quads[i].unk_0F = 0;
                sprite_quads[i].w = 8;
                *(s16 *)&sprite_quads[i].h = 0x18;
                /* FAKE: same-line stores place mfhi after the zero stores as in the original. T-24085 */
                sprite_quads[i].x = rand() % 84 - 0x9F; sprite_quads[i].y = -0x78; encounter_falling_quad_delays[i - 4] = 0; encounter_falling_quad_phases[i - 4] = 0;
            } else if (encounter_falling_quad_phases[i - 4] == 5) {
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
        } else if (sprite_quads[i].y >= 0x19 && encounter_falling_quad_phases[i - 4] == 0 && encounter_falling_quad_delays[i - 4] == 0) {
            /* FAKE: same-line store places mfhi after unk_06 = -0x78 as in the original. T-24085 */
            sprite_quads[i].x = rand() % 84 - 0x9F; sprite_quads[i].y = -0x78;
        }
    }
}

void encounter_megumi_scene_init_2(void) {
    game_state.current_girl = GIRL_MEGUMI;
    draw2d3d(1, 0);
    actor_draw_enable(1);
    sprite_tables_clear();
    actor_clear_range(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_system_reset();
    strcpy(scene_place_title, "廊下");
    D_8011ED9F = 0;
    D_8011EDE3 = 0;
    D_800CA150 = 0;
    D_800CA154 = 0;
    addr_init_bustup();
    encounter_scene_scripts_bind();
    hizuke_init();
    message_window_init();
    text_marker_hide();
    bg_brightness = 0;
    talk_names_set(game_state.current_girl);
    talk_clear();
    encounter_scene_scripts_bind();
    encounter_scene_text_table = encounter_scene_text_section_55;
    encounter_scene_command_table = encounter_scene_command_section_55;
    encounter_scene_voice_table = encounter_scene_voice_section_55;
    D_800CA368 = 0x3D;
    mode_step_next();
}

void encounter_megumi_bg_load_1(void) {
    bg_read_sub2(0x4068);
    mode_step_next();
}

void encounter_megumi_bg_load_2(void) {
    bg_read_sub2(0x46B2);
    mode_step_next();
}

void encounter_megumi_bg_load_3(void) {
    bg_read_sub2(0x403B);
    mode_step_next();
}
