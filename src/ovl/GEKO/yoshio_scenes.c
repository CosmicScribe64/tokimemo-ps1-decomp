/* yoshio_scenes.c: src/ovl/GEKO/80141AF0.c:18,35,67,142; Yoshio callback flow, selected-girl storage and descending six-slot girl ranking. Current GEKO folder retained; no narrative meaning is inferred from callback ordinals. Original object 0x80141AF0-0x801426D0 (config/objects/GEKO.txt). */
#include "common.h"
#include "ovl/GEKO.h"

void encounter_yoshio_assets_bind_1(void) {
    encounter_yoshio_pack_1_animation_input_2 = 0x801CE130;
    encounter_yoshio_pack_1_animation_input_0 = 0x801CE134;
    encounter_yoshio_pack_1_animation_input_1 = 0x801CE15C;
    encounter_yoshio_pack_1_animation_input_3 = (*(s16 *)0x801CE170);
    encounter_yoshio_pack_1_palette = 0x801B0000;
    encounter_yoshio_pack_1_still_image_0 = 0x801B2000;
    encounter_yoshio_pack_1_still_image_1 = 0x801B6000;
    encounter_yoshio_pack_1_still_image_2 = 0x801BA000;
    encounter_yoshio_pack_1_still_image_3 = 0x801BE000;
    encounter_yoshio_pack_1_still_image_4 = 0x801C2000;
    encounter_yoshio_pack_1_still_image_5 = 0x801C6000;
}

void encounter_yoshio_entry(void) {
    encounter_yoshio_scene_all_dispatch();
}

void encounter_yoshio_scene_all_dispatch(void) {
    if (game_state.mode_stage == 0) {
        encounter_yoshio_scene_all_step_dispatch();
        return;
    }
    game_mode_reset_to_title();
}

typedef struct {
    void (*f[43])();
} FnTbl43; /* size 0xAC */
extern FnTbl43 encounter_yoshio_scene_all_steps;

void encounter_yoshio_scene_all_step_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl43 tbl;

    tbl = encounter_yoshio_scene_all_steps;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
    if (D_8014755C != 0) {
        portrait_face_update();
    }
}

void encounter_yoshio_assets_load_1(void) {
    cd_read_sectors(0x3D, 0x801B0000, 0x8FB5);
    encounter_yoshio_assets_bind_1();
    mode_step_next();
}

void encounter_yoshio_talk_portrait_update(void) {
    D_800B5A60 = 0;
    encounter_scene_talk();
    D_80122CF4 = 0;
    /* records of 0x44 / 0x24 bytes: indexing from the first symbol keeps IDO from hoisting the loads above the stores (T-4070) */
    (&D_8011F4CB)[0x44] |= 0x80;
    D_8011F4CB |= 0x80;
    sprite_quads[0].flags &= 0x7FFFFFFF;
    sprite_quads[1].flags &= 0x7FFFFFFF;
    if ((u32)game_state.frame_count % 240U == 0) {
        bustup_wink();
    }
}

void encounter_yoshio_girl_result_store(void) {
    game_state.scene_flags[82] = game_state.date_girl;
    game_state.current_girl = game_state.date_girl;
    talk_names_set(game_state.date_girl);
    game_state.girls[game_state.current_girl].flags.b[0] = game_state.girls[game_state.current_girl].flags.b[0] & 0xFFFD;
    mode_step_next();
}

void encounter_yoshio_portrait_shift(void) {
    (&D_8011F4EE)[0x22] += 0x40;
    D_8011F4EE += 0x40;
    D_801217D4 += 0x40;
    (&D_801217D4)[0x12] += 0x40;
    mode_step_next();
}

void encounter_yoshio_speaker_update(void) {
    encounter_scene_talk();
    if ((u16)D_800CA154 == 2) {
        D_800B5A60 = 0;
        D_80122CF4 = 0;
        (&D_8011F4CB)[0x44] |= 0x80;
        D_8011F4CB |= 0x80;
        D_8011F524 = 0;
        sprite_quads[0].flags &= 0x7FFFFFFF;
        sprite_quads[1].flags &= 0x7FFFFFFF;
        if ((u32)game_state.frame_count % 240U == 0) {
            bustup_wink();
        }
    } else {
        portrait_value_clear_if_high(0);
        D_80122CF4 = 1;
        (&D_8011F4CB)[0x44] |= 0x80;
        D_8011F4CB |= 0x80;
        sprite_quads[0].flags &= 0x7FFFFFFF;
        sprite_quads[1].flags &= 0x7FFFFFFF;
        D_80120652 = 0;
        D_80120668 = 0;
        if ((u32)game_state.frame_count % 240U == 0x78) {
            portrait_event_face_eyes_animation_start();
        }
    }
}

void encounter_yoshio_girl_group_offset(void) {
    D_800CA150 = (u16) D_800CA150 + game_state.current_girl;
    mode_step_next();
}

void encounter_yoshio_closing_group(void) {
    D_800CA150 = 0xC;
    mode_step_next();
}

s32 encounter_yoshio_portrait_fade_out(void) {
    s32 mask;
    u8 value;

    func_800853FC();
    D_80122CF4 = 1;
    value = D_80120657;
    D_8011F513 = value;
    D_8011F4CF = value;
    func_80084C98(0, value);
    func_80084C98(1, D_80120657);
    if (D_80120657 < 8U) {
        mask = ~0x80;
        D_80122CF4 = 0;
        (&D_8011F4CB)[0x44] &= mask;
        D_8011F4CB &= mask;
        sprite_quads[0].flags |= 0x80000000;
        sprite_quads[1].flags |= 0x80000000;
    }
}

void encounter_yoshio_ranked_girl_init(void) {
    /* FAKE: preserve the original 0x480 frame; the unused region
     * above the ranking arrays has no recovered source type. T-24605. */
    u8 frame_pad[0x420];
    GameState *g;
    s16 girl;
    GekoShorts6 scores;
    GekoShorts6 girls;

    g = &game_state;
    scores = encounter_yoshio_rank_scores_init;
    girls = encounter_yoshio_rank_girls_init;
    game_state.date_girl = GIRL_MIO;
    /* FAKE: break the equal-priority tie between girls[2] and girls[5],
     * preserving their t3/t4 allocation without emitted code. T-24605. */
    if (girls.v[2]) {}
    /* FAKE: the shared line schedules the record base/stride before
     * the initial girl index. T-24605. */
    girl = 9; do {
        if (g->girls[girl].flags.f.met) {
            if ((g->girls[girl].tokimeki + g->girls[girl].affection) >= scores.v[0]) {
                scores.v[5] = scores.v[4];
                girls.v[5] = girls.v[4];
                scores.v[4] = scores.v[3];
                girls.v[4] = girls.v[3];
                scores.v[3] = scores.v[2];
                girls.v[3] = girls.v[2];
                scores.v[2] = scores.v[1];
                girls.v[2] = girls.v[1];
                scores.v[1] = scores.v[0];
                girls.v[1] = girls.v[0];
                scores.v[0] = g->girls[girl].tokimeki + g->girls[girl].affection;
                girls.v[0] = girl;
            } else if ((g->girls[girl].tokimeki + g->girls[girl].affection) >= scores.v[1]) {
                scores.v[5] = scores.v[4];
                girls.v[5] = girls.v[4];
                scores.v[4] = scores.v[3];
                girls.v[4] = girls.v[3];
                scores.v[3] = scores.v[2];
                girls.v[3] = girls.v[2];
                scores.v[2] = scores.v[1];
                girls.v[2] = girls.v[1];
                scores.v[1] = g->girls[girl].tokimeki + g->girls[girl].affection;
                girls.v[1] = girl;
            } else if ((g->girls[girl].tokimeki + g->girls[girl].affection) >= scores.v[2]) {
                scores.v[5] = scores.v[4];
                girls.v[5] = girls.v[4];
                scores.v[4] = scores.v[3];
                girls.v[4] = girls.v[3];
                scores.v[3] = scores.v[2];
                girls.v[3] = girls.v[2];
                scores.v[2] = g->girls[girl].tokimeki + g->girls[girl].affection;
                girls.v[2] = girl;
            } else if ((g->girls[girl].tokimeki + g->girls[girl].affection) >= scores.v[3]) {
                scores.v[5] = scores.v[4];
                girls.v[5] = girls.v[4];
                scores.v[4] = scores.v[3];
                girls.v[4] = girls.v[3];
                scores.v[3] = g->girls[girl].tokimeki + g->girls[girl].affection;
                girls.v[3] = girl;
            } else if ((g->girls[girl].tokimeki + g->girls[girl].affection) >= scores.v[4]) {
                scores.v[5] = scores.v[4];
                girls.v[5] = girls.v[4];
                scores.v[4] = g->girls[girl].tokimeki + g->girls[girl].affection;
                girls.v[4] = girl;
            } else if ((g->girls[girl].tokimeki + g->girls[girl].affection) >= scores.v[5]) {
                scores.v[5] = g->girls[girl].tokimeki + g->girls[girl].affection;
                girls.v[5] = girl;
            }
        }
        girl--;
    } while (girl > 0);
    game_state.date_girl = (u8)girls.v[5];
    game_state.current_girl = GIRL_YOSHIO;
    draw2d3d(1, 0);
    actor_draw_enable(1);
    sprite_tables_clear();
    actor_clear_range(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_system_reset();
    message_window_set_title((s32) "");
    ((Actor *)actor_pool)[3].draw_flags = 0;
    ((Actor *)actor_pool)[4].draw_flags = 0;
    D_800CA150 = 0;
    D_800CA154 = 0;
    addr_init_bustup();
    encounter_scene_scripts_bind();
    hizuke_init();
    message_window_init();
    text_marker_hide();
    dec_bg_brightness = 0;
    talk_names_set(game_state.current_girl);
    talk_clear();
    encounter_scene_scripts_bind();
    encounter_scene_text_table = encounter_scene_text_section_78;
    encounter_scene_command_table = encounter_scene_command_section_78;
    encounter_scene_voice_table = encounter_scene_voice_section_78;
    strcpy(D_800CA19C, "教室");
    D_8014755C = 0;
    D_800CA368 = 0xFF;
    mode_step_next();
    game_state.scene_flags[0x52] = game_state.scene_flags[0x52] + 1;
}

void encounter_yoshio_still_show_1(void) {
    message_window_title_show(0);
    encounter_yoshio_assets_bind_1();
    load_palette(encounter_yoshio_pack_1_palette, 0x11, 1, 2, 0);
    scene_still_show(encounter_yoshio_pack_1_still_image_0, encounter_yoshio_pack_1_still_image_1, encounter_yoshio_pack_1_still_image_2, encounter_yoshio_pack_1_still_image_3, encounter_yoshio_pack_1_still_image_4, encounter_yoshio_pack_1_still_image_5);
    scene_anim_show(encounter_yoshio_pack_1_animation_input_0, encounter_yoshio_pack_1_animation_input_1, encounter_yoshio_pack_1_animation_input_2, encounter_yoshio_pack_1_animation_input_3);
    mode_step_next();
}

void encounter_yoshio_bg_load_1(void) {
    bg_read_sub2(0x4055);
    mode_step_next();
}

void encounter_yoshio_bg_load_2(void) {
    bg_read_sub2(0x4690);
    mode_step_next();
}
