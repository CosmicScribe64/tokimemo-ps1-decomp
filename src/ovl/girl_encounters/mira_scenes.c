/* mira_scenes.c: src/ovl/GEKO/8013C980.c:31,53,99,108; Mira selectors1/8, scene callbacks, script and affection initialization. Current GEKO folder retained; no narrative meaning is inferred from callback ordinals. Original object 0x8013C980-0x8013D0F0 (config/objects/GEKO.txt). */
#define MAIN_API_OVERRIDE_talk_work /* matched as u32 (main_api.h: s32), switch selector in $v1 (T-6010) */
#include "common.h"
#include "ovl/girl_encounters.h"

extern u32 talk_work;

typedef struct {
    void (*callbacks[38])(); /* HYPOTHESIS: GEKO grouping vocabulary remains unconfirmed; T-27051; src/ovl/GEKO/8013C980.c:9; func_8013CAD0 at line53 copies this exact38-callback table and dispatches by mode_step; scene number is the actual talk_work selector, not a narrative identity. */
} EncounterMiraSceneOneSteps; /* size 0x98 */
extern EncounterMiraSceneOneSteps encounter_mira_scene_one_steps;

typedef struct {
    void (*f[22])();
} FnTbl22; /* size 0x58 */
extern FnTbl22 encounter_mira_scene_eight_steps;

void encounter_mira_assets_bind_1(void) {
    encounter_mira_pack_1_animation_input_2 = 0x801CE218;
    encounter_mira_pack_1_animation_input_0 = 0x801CE21C;
    encounter_mira_pack_1_animation_input_1 = 0x801CE25C;
    encounter_mira_pack_1_animation_input_3 = *(s16 *)0x801CE27C;
    encounter_mira_pack_1_palette = 0x801B0000;
    encounter_mira_pack_1_still_image_0 = 0x801B2000;
    encounter_mira_pack_1_still_image_1 = 0x801B6000;
    encounter_mira_pack_1_still_image_2 = 0x801BA000;
    encounter_mira_pack_1_still_image_3 = 0x801BE000;
    encounter_mira_pack_1_still_image_4 = 0x801C2000;
    encounter_mira_pack_1_still_image_5 = 0x801C6000;
}

void encounter_mira_scene_select_1(void) {
    switch (talk_work) {
    case 1:
        encounter_mira_scene_one_dispatch();
        return;
    case 8:
        encounter_mira_scene_eight_dispatch();
        return;
    default:
        game_mode_reset_to_title();
        return;
    }
}

void encounter_mira_scene_one_dispatch(void) {
    if (game_state.mode_stage == 0) {
        encounter_mira_scene_one_step_dispatch();
        return;
    }
    game_mode_reset_to_title();
}

void encounter_mira_scene_one_step_dispatch(void) {
    s32 mode_step_index; /* declared before mode_step_callbacks: its stack slot sits above mode_step_callbacks (T-3330) */
    EncounterMiraSceneOneSteps mode_step_callbacks;

    mode_step_callbacks = encounter_mira_scene_one_steps;
    mode_step_index = game_state.mode_step;
    mode_step_callbacks.callbacks[mode_step_index](0x80);
}

void encounter_mira_assets_load_1(void) {
    cd_read_sectors(0x3D, 0x801B0000, 0x8440);
    encounter_mira_assets_bind_1();
    mode_step_next();
}

void encounter_mira_speaker_one_select(void) {
    D_800B5A60 = 1;
    game_state.current_girl = GIRL_SOTOI;
    strcpy(name_code_girl_short, "男子１");
    D_80120666 = 4;
    mode_step_next();
}

void encounter_mira_speaker_two_select(void) {
    strcpy(name_code_girl_short, "男子２");
    D_80120666 = 5;
    mode_step_next();
}

/* FAKE: scalar byte store followed by member read preserves argument/constant scheduling. T-9550 */
void encounter_mira_speaker_restore(void) {
    D_800B5A60 = 0;
    *(u8 *)&game_state.current_girl = 7;
    talk_names_set(game_state.current_girl);
    D_80120666 = 2;
    mode_step_next();
}

void encounter_mira_scene_eight_dispatch(void) {
    if (game_state.mode_stage == 0) {
        encounter_mira_scene_eight_step_dispatch();
        return;
    }
    game_mode_reset_to_title();
}

void encounter_mira_scene_eight_step_dispatch(void) {
    s32 mode_step_index; /* declared before mode_step_callbacks: its stack slot sits above mode_step_callbacks (T-3330) */
    FnTbl22 mode_step_callbacks;

    mode_step_callbacks = encounter_mira_scene_eight_steps;
    mode_step_index = game_state.mode_step;
    mode_step_callbacks.f[mode_step_index](0x80);
}

void encounter_mira_scene_init_1(void) {
    s16 i;

    game_state.current_girl = GIRL_MIRA;
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
    for (i = 0; i < 5; i++) {
        game_state.tpage_buf[i + 11] = 1;
        game_state.tpage_buf[i + 43] = 1;
        game_state.tpage_buf[i + 21] = 1;
        game_state.tpage_buf[i + 53] = 1;
    }
    addr_init_bustup(i, &game_state);
    encounter_scene_scripts_bind();
    hizuke_init();
    message_window_init();
    text_marker_hide();
    bg_brightness = 0;
    talk_names_set(game_state.current_girl);
    talk_clear();
    encounter_scene_scripts_bind();
    encounter_scene_text_table = encounter_scene_text_section_30;
    encounter_scene_command_table = encounter_scene_command_section_30;
    encounter_scene_voice_table = encounter_scene_voice_section_30;
    strcpy(scene_place_title, "廊下");
    game_state.girls[7].affection += 1;
    check_para_limit();
    D_800CA368 = 0x2E;
    mode_step_next();
}

void encounter_mira_still_show_1(void) {
    encounter_mira_assets_bind_1();
    load_palette(encounter_mira_pack_1_palette, 0x11, 1, 2, 0);
    scene_still_show(encounter_mira_pack_1_still_image_0, encounter_mira_pack_1_still_image_1, encounter_mira_pack_1_still_image_2, encounter_mira_pack_1_still_image_3, encounter_mira_pack_1_still_image_4, encounter_mira_pack_1_still_image_5);
    scene_anim_show(encounter_mira_pack_1_animation_input_0, encounter_mira_pack_1_animation_input_1, encounter_mira_pack_1_animation_input_2, encounter_mira_pack_1_animation_input_3);
    mode_step_next();
}

void encounter_mira_scene_init_2(void) {
    game_state.current_girl = GIRL_MIRA;
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
    talk_secondary_group = 8;
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
    encounter_scene_text_table = encounter_scene_text_section_36;
    encounter_scene_command_table = encounter_scene_command_section_36;
    encounter_scene_voice_table = encounter_scene_voice_section_36;
    strcpy(scene_place_title, "廊下");
    D_800CA368 = 0x35;
    mode_step_next();
}

void encounter_mira_bg_load_1(void) {
    bg_read_sub2(0x403B);
    mode_step_next();
}

void encounter_mira_bg_load_2(void) {
    bg_read_sub2(0x403B);
    mode_step_next();
}

void encounter_mira_bg_load_3(void) {
    bg_read_sub2(0x403B);
    mode_step_next();
}
