/* saki_scenes.c: src/ovl/GEKO/8013EE40.c:23,39,54,65; Saki selector1, script/resource callbacks and scene-counter/condition changes. Current GEKO folder retained; no narrative meaning is inferred from callback ordinals. Original object 0x8013EE40-0x8013F360 (config/objects/GEKO.txt). */
#include "common.h"
#include "ovl/girl_encounters.h"

typedef struct {
    void (*callbacks[35])(); /* HYPOTHESIS: GEKO grouping vocabulary remains unconfirmed; T-27051; src/ovl/GEKO/8013EE40.c:6; func_8013EF6C at line39 copies this exact35-callback table and dispatches by mode_step; scene number is the actual talk_work selector, not a narrative identity. */
} EncounterSakiSceneOneSteps; /* size 0x8C */
extern EncounterSakiSceneOneSteps encounter_saki_scene_one_steps;

void encounter_saki_assets_bind_1(void) {
    encounter_saki_pack_1_animation_input_2 = 0x801CE124;
    encounter_saki_pack_1_animation_input_0 = 0x801CE128;
    encounter_saki_pack_1_animation_input_1 = 0x801CE148;
    encounter_saki_pack_1_animation_input_3 = (*(s16 *)0x801CE15C);
    encounter_saki_pack_1_palette = 0x801B0000;
    encounter_saki_pack_1_still_image_0 = 0x801B2000;
    encounter_saki_pack_1_still_image_1 = 0x801B6000;
    encounter_saki_pack_1_still_image_2 = 0x801BA000;
    encounter_saki_pack_1_still_image_3 = 0x801BE000;
    encounter_saki_pack_1_still_image_4 = 0x801C2000;
    encounter_saki_pack_1_still_image_5 = 0x801C6000;
}

void encounter_saki_scene_select_1(void) {
    if (talk_work == 1) {
        encounter_saki_scene_one_dispatch();
        return;
    }
    game_mode_reset_to_title();
}

void encounter_saki_scene_one_dispatch(void) {
    if (game_state.mode_stage == 0) {
        encounter_saki_scene_one_step_dispatch();
        return;
    }
    game_mode_reset_to_title();
}

void encounter_saki_scene_one_step_dispatch(void) {
    s32 mode_step_index; /* declared before mode_step_callbacks: its stack slot sits above mode_step_callbacks (T-3330) */
    EncounterSakiSceneOneSteps mode_step_callbacks;

    mode_step_callbacks = encounter_saki_scene_one_steps;
    mode_step_index = game_state.mode_step;
    mode_step_callbacks.callbacks[mode_step_index]();
}

void encounter_saki_assets_load_1(void) {
    cd_read_sectors(0x3D, 0x801B0000, 0x8C0E);
    encounter_saki_assets_bind_1();
    mode_step_next();
}

void encounter_saki_scene_group_select(void) {
    if (game_state.scene_flags[28] == 1 && game_state.scene_flags[27] == 1) {
        talk_secondary_group = (u16) talk_secondary_group + 4;
        talk_secondary_line = 0;
    } else if (game_state.scene_flags[27] >= 2U) {
        talk_secondary_group = (u16) talk_secondary_group + 2;
        talk_secondary_line = 0;
    }
    mode_step_next();
}

void encounter_saki_scene_init_1(void) {
    game_state.current_girl = GIRL_SAKI;
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
    encounter_scene_text_table = encounter_scene_text_section_61;
    encounter_scene_command_table = encounter_scene_command_section_61;
    encounter_scene_voice_table = encounter_scene_voice_section_61;
    strcpy(scene_place_title, "廊下");
    if (game_state.scene_flags[27] >= 3U) {
        game_state.scene_flags[27] = 2;
    }
    if (game_state.scene_flags[28] == 0 && game_state.scene_flags[27] == 1) {
        game_state.girls[4].tokimeki += 1;
    }
    game_state.girls[4].affection += 1;
    game_state.param_condition.value += 0xA;
    check_para_limit();
    D_800CA368 = 0x1B;
    mode_step_next();
}

void encounter_saki_still_show_1(void) {
    encounter_saki_assets_bind_1();
    load_palette(encounter_saki_pack_1_palette, 0x11, 1, 2, 0);
    scene_still_show(encounter_saki_pack_1_still_image_0, encounter_saki_pack_1_still_image_1, encounter_saki_pack_1_still_image_2, encounter_saki_pack_1_still_image_3, encounter_saki_pack_1_still_image_4, encounter_saki_pack_1_still_image_5);
    scene_anim_show(encounter_saki_pack_1_animation_input_0, encounter_saki_pack_1_animation_input_1, encounter_saki_pack_1_animation_input_2, encounter_saki_pack_1_animation_input_3);
    scene_mouth_frames_reversed_saved = 1;
    mode_step_next();
}

void encounter_saki_bg_load_1(void) {
    bg_read_sub2(0x4045);
    mode_step_next();
}

void encounter_saki_bg_load_2(void) {
    bg_read_sub2(0x46E0);
    mode_step_next();
}
