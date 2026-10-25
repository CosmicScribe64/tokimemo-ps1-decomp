/* yukari_scenes.c: src/ovl/GEKO/8013D4B0.c:36,66,86,95,136; Yukari selectors1/2 and their callback/score/script initialization. Current GEKO folder retained; no narrative meaning is inferred from callback ordinals. Original object 0x8013D4B0-0x8013DB80 (config/objects/GEKO.txt). */
#define MAIN_API_OVERRIDE_talk_work /* matched as u32 (main_api.h: s32), switch selector in $v1 (T-6010) */
#include "common.h"
#include "ovl/girl_encounters.h"

extern u32 talk_work;

typedef struct {
    void (*f[32])();
} FnTbl32; /* size 0x80 */
extern FnTbl32 encounter_yukari_scene_one_steps;

void encounter_yukari_bg_load_1(void) {
    bg_read_sub2(0x46E7);
    mode_step_next();
}

void encounter_yukari_bg_load_2(void) {
    bg_read_sub2(0x403B);
    mode_step_next();
}

void encounter_yukari_assets_bind_1(void) {
    encounter_yukari_pack_1_animation_input_2 = 0x801CE0F4;
    encounter_yukari_pack_1_animation_input_0 = 0x801CE0F8;
    encounter_yukari_pack_1_animation_input_1 = 0x801CE118;
    encounter_yukari_pack_1_animation_input_3 = (*(s16 *)0x801CE12C);
    encounter_yukari_pack_1_palette = 0x801B0000;
    encounter_yukari_pack_1_still_image_0 = 0x801B2000;
    encounter_yukari_pack_1_still_image_1 = 0x801B6000;
    encounter_yukari_pack_1_still_image_2 = 0x801BA000;
    encounter_yukari_pack_1_still_image_3 = 0x801BE000;
    encounter_yukari_pack_1_still_image_4 = 0x801C2000;
    encounter_yukari_pack_1_still_image_5 = 0x801C6000;
}

void encounter_yukari_scene_select_1(void) {
    switch (talk_work) {
    case 1:
        encounter_yukari_scene_one_dispatch();
        return;
    case 2:
        encounter_yukari_scene_two_dispatch();
        return;
    default:
        game_mode_reset_to_title();
        return;
    }
}

void encounter_yukari_scene_one_dispatch(void) {
    if (game_state.mode_stage == 0) {
        encounter_yukari_scene_one_step_dispatch();
        return;
    }
    game_mode_reset_to_title();
}

void encounter_yukari_scene_two_dispatch(void) {
    if (game_state.mode_stage == 0) {
        encounter_yukari_scene_two_step_dispatch();
        return;
    }
    game_mode_reset_to_title();
}

void encounter_yukari_scene_one_step_dispatch(void) {
    s32 mode_step_index; /* declared before mode_step_callbacks: its stack slot sits above mode_step_callbacks (T-3330) */
    FnTbl32 mode_step_callbacks;

    mode_step_callbacks = encounter_yukari_scene_one_steps;
    mode_step_index = game_state.mode_step;
    mode_step_callbacks.f[mode_step_index](0x80);
}

void encounter_yukari_assets_load_1(void) {
    cd_read_sectors(0x3D, 0x801B0000, 0x87D9);
    encounter_yukari_assets_bind_1();
    mode_step_next();
}

typedef struct {
    void (*f[23])();
} FnTbl23; /* size 0x5C */
extern FnTbl23 encounter_yukari_scene_two_steps;

void encounter_yukari_scene_two_step_dispatch(void) {
    s32 mode_step_index; /* declared before mode_step_callbacks: its stack slot sits above mode_step_callbacks (T-3330) */
    FnTbl23 mode_step_callbacks;

    mode_step_callbacks = encounter_yukari_scene_two_steps;
    mode_step_index = game_state.mode_step;
    mode_step_callbacks.f[mode_step_index]();
}

void encounter_yukari_scene_init_1(void) {
    game_state.current_girl = GIRL_YUKARI;
    draw2d3d(1, 0);
    actor_draw_enable(1);
    sprite_tables_clear();
    actor_clear_range(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_system_reset();
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
    encounter_scene_text_table = encounter_scene_text_section_42;
    encounter_scene_command_table = encounter_scene_command_section_42;
    encounter_scene_voice_table = encounter_scene_voice_section_42;
    strcpy(scene_place_title, "廊下");
    if (talk_work == 1) {
        game_state.girls[5].affection += 1;
        game_state.girls[5].syoushin -= 1;
        check_para_limit();
    }
    D_800CA368 = 0x21;
    mode_step_next();
}

void encounter_yukari_still_show_1(void) {
    encounter_yukari_assets_bind_1();
    load_palette(encounter_yukari_pack_1_palette, 0x11, 1, 2, 0);
    scene_still_show(encounter_yukari_pack_1_still_image_0, encounter_yukari_pack_1_still_image_1, encounter_yukari_pack_1_still_image_2, encounter_yukari_pack_1_still_image_3, encounter_yukari_pack_1_still_image_4, encounter_yukari_pack_1_still_image_5);
    scene_anim_show(encounter_yukari_pack_1_animation_input_0, encounter_yukari_pack_1_animation_input_1, encounter_yukari_pack_1_animation_input_2, encounter_yukari_pack_1_animation_input_3);
    mode_step_next();
}

void encounter_yukari_scene_init_2(void) {
    game_state.current_girl = GIRL_YUKARI;
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
    encounter_scene_text_table = encounter_scene_text_section_43;
    encounter_scene_command_table = encounter_scene_command_section_43;
    encounter_scene_voice_table = encounter_scene_voice_section_43;
    strcpy(scene_place_title, "廊下");
    game_state.girls[5].tokimeki += 1;
    check_para_limit();
    D_800CA368 = 0x22;
    mode_step_next();
}

void encounter_yukari_bg_load_3(void) {
    bg_read_sub2(0x4045);
    mode_step_next();
}

void encounter_yukari_bg_load_4(void) {
    bg_read_sub2(0x46CD);
    mode_step_next();
}

void encounter_yukari_bg_load_5(void) {
    bg_read_sub2(0x4045);
    mode_step_next();
}
