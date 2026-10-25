/* yoshio_scenes.c: src/ovl/GEKO/80141AF0.c:18,35,67,142; Yoshio callback flow, selected-girl storage and descending six-slot girl ranking. Current GEKO folder retained; no narrative meaning is inferred from callback ordinals. Original object 0x80141AF0-0x801426D0 (config/objects/GEKO.txt). */
#include "common.h"
#include "ovl/girl_encounters.h"
#include "main/sprite.h"

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
    void (*callbacks[43])(); /* HYPOTHESIS: GEKO grouping vocabulary remains unconfirmed; T-27051; src/ovl/GEKO/80141AF0.c:32; func_80141BFC at line35 copies this exact43-callback table and dispatches by mode_step; scene number is the actual talk_work selector, not a narrative identity. */
} EncounterYoshioSceneSteps; /* size 0xAC */
extern EncounterYoshioSceneSteps encounter_yoshio_scene_all_steps;

void encounter_yoshio_scene_all_step_dispatch(void) {
    s32 mode_step_index; /* declared before mode_step_callbacks: its stack slot sits above mode_step_callbacks (T-3330) */
    EncounterYoshioSceneSteps mode_step_callbacks;

    mode_step_callbacks = encounter_yoshio_scene_all_steps;
    mode_step_index = game_state.mode_step;
    mode_step_callbacks.callbacks[mode_step_index](0x80);
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
    if ((u16)talk_secondary_line == 2) {
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
    talk_secondary_group = (u16) talk_secondary_group + game_state.current_girl;
    mode_step_next();
}

void encounter_yoshio_closing_group(void) {
    talk_secondary_group = 0xC;
    mode_step_next();
}

s32 encounter_yoshio_portrait_fade_out(void) {
    s32 mask;
    u8 value;

    scene_still_fade_out();
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
    u8 fake_frame_pad[0x420];
    GameState *state_data;
    s16 candidate_girl;
    EncounterGirlRankingValues ranked_scores;
    EncounterGirlRankingValues ranked_girls;

    state_data = &game_state;
    ranked_scores = encounter_yoshio_rank_scores_init;
    ranked_girls = encounter_yoshio_rank_girls_init;
    game_state.date_girl = GIRL_MIO;
    /* FAKE: break the equal-priority tie between ranked_girls[2] and ranked_girls[5],
     * preserving their t3/t4 allocation without emitted code. T-24605. */
    if (ranked_girls.entries[2]) {}
    /* FAKE: the shared line schedules the record base/stride before
     * the initial candidate_girl index. T-24605. */
    candidate_girl = 9; do {
        if (state_data->girls[candidate_girl].flags.f.met) {
            if ((state_data->girls[candidate_girl].tokimeki + state_data->girls[candidate_girl].affection) >= ranked_scores.entries[0]) {
                ranked_scores.entries[5] = ranked_scores.entries[4];
                ranked_girls.entries[5] = ranked_girls.entries[4];
                ranked_scores.entries[4] = ranked_scores.entries[3];
                ranked_girls.entries[4] = ranked_girls.entries[3];
                ranked_scores.entries[3] = ranked_scores.entries[2];
                ranked_girls.entries[3] = ranked_girls.entries[2];
                ranked_scores.entries[2] = ranked_scores.entries[1];
                ranked_girls.entries[2] = ranked_girls.entries[1];
                ranked_scores.entries[1] = ranked_scores.entries[0];
                ranked_girls.entries[1] = ranked_girls.entries[0];
                ranked_scores.entries[0] = state_data->girls[candidate_girl].tokimeki + state_data->girls[candidate_girl].affection;
                ranked_girls.entries[0] = candidate_girl;
            } else if ((state_data->girls[candidate_girl].tokimeki + state_data->girls[candidate_girl].affection) >= ranked_scores.entries[1]) {
                ranked_scores.entries[5] = ranked_scores.entries[4];
                ranked_girls.entries[5] = ranked_girls.entries[4];
                ranked_scores.entries[4] = ranked_scores.entries[3];
                ranked_girls.entries[4] = ranked_girls.entries[3];
                ranked_scores.entries[3] = ranked_scores.entries[2];
                ranked_girls.entries[3] = ranked_girls.entries[2];
                ranked_scores.entries[2] = ranked_scores.entries[1];
                ranked_girls.entries[2] = ranked_girls.entries[1];
                ranked_scores.entries[1] = state_data->girls[candidate_girl].tokimeki + state_data->girls[candidate_girl].affection;
                ranked_girls.entries[1] = candidate_girl;
            } else if ((state_data->girls[candidate_girl].tokimeki + state_data->girls[candidate_girl].affection) >= ranked_scores.entries[2]) {
                ranked_scores.entries[5] = ranked_scores.entries[4];
                ranked_girls.entries[5] = ranked_girls.entries[4];
                ranked_scores.entries[4] = ranked_scores.entries[3];
                ranked_girls.entries[4] = ranked_girls.entries[3];
                ranked_scores.entries[3] = ranked_scores.entries[2];
                ranked_girls.entries[3] = ranked_girls.entries[2];
                ranked_scores.entries[2] = state_data->girls[candidate_girl].tokimeki + state_data->girls[candidate_girl].affection;
                ranked_girls.entries[2] = candidate_girl;
            } else if ((state_data->girls[candidate_girl].tokimeki + state_data->girls[candidate_girl].affection) >= ranked_scores.entries[3]) {
                ranked_scores.entries[5] = ranked_scores.entries[4];
                ranked_girls.entries[5] = ranked_girls.entries[4];
                ranked_scores.entries[4] = ranked_scores.entries[3];
                ranked_girls.entries[4] = ranked_girls.entries[3];
                ranked_scores.entries[3] = state_data->girls[candidate_girl].tokimeki + state_data->girls[candidate_girl].affection;
                ranked_girls.entries[3] = candidate_girl;
            } else if ((state_data->girls[candidate_girl].tokimeki + state_data->girls[candidate_girl].affection) >= ranked_scores.entries[4]) {
                ranked_scores.entries[5] = ranked_scores.entries[4];
                ranked_girls.entries[5] = ranked_girls.entries[4];
                ranked_scores.entries[4] = state_data->girls[candidate_girl].tokimeki + state_data->girls[candidate_girl].affection;
                ranked_girls.entries[4] = candidate_girl;
            } else if ((state_data->girls[candidate_girl].tokimeki + state_data->girls[candidate_girl].affection) >= ranked_scores.entries[5]) {
                ranked_scores.entries[5] = state_data->girls[candidate_girl].tokimeki + state_data->girls[candidate_girl].affection;
                ranked_girls.entries[5] = candidate_girl;
            }
        }
        candidate_girl--;
    } while (candidate_girl > 0);
    game_state.date_girl = (u8)ranked_girls.entries[5];
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
    talk_secondary_group = 0;
    talk_secondary_line = 0;
    addr_init_bustup();
    encounter_scene_scripts_bind();
    hizuke_init();
    message_window_init();
    text_marker_hide();
    bg_brightness = 0;
    talk_names_set(game_state.current_girl);
    talk_clear();
    encounter_scene_scripts_bind();
    encounter_scene_text_table = encounter_scene_text_section_78;
    encounter_scene_command_table = encounter_scene_command_section_78;
    encounter_scene_voice_table = encounter_scene_voice_section_78;
    strcpy(scene_place_title, "教室");
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
