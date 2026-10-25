/* yuko_scenes.c: src/ovl/GEKO/801393C0.c:18,39,69; Yuko selector1 callback table and initialization including her affection increment. Current GEKO folder retained; no narrative meaning is inferred from callback ordinals. Original object 0x801393C0-0x80139910 (config/objects/GEKO.txt). */
#include "common.h"
#include "ovl/GEKO.h"

void encounter_yuko_assets_bind_1(void) {
    encounter_yuko_pack_1_animation_input_2 = (u8 *)0x801CE0FC;
    encounter_yuko_pack_1_animation_input_0 = (u8 *)0x801CE100;
    encounter_yuko_pack_1_animation_input_1 = (u8 *)0x801CE120;
    encounter_yuko_pack_1_animation_input_3 = *(s16 *)0x801CE13C;
    encounter_yuko_pack_1_palette = (u8 *)0x801B0000;
    encounter_yuko_pack_1_still_image_0 = (u8 *)0x801B2000;
    encounter_yuko_pack_1_still_image_1 = (u8 *)0x801B6000;
    encounter_yuko_pack_1_still_image_2 = (u8 *)0x801BA000;
    encounter_yuko_pack_1_still_image_3 = (u8 *)0x801BE000;
    encounter_yuko_pack_1_still_image_4 = (u8 *)0x801C2000;
    encounter_yuko_pack_1_still_image_5 = (u8 *)0x801C6000;
}

void encounter_yuko_scene_select_1(void) {
    if (talk_work == 1) {
        encounter_yuko_scene_one_dispatch();
        return;
    }
    game_mode_reset_to_title();
}

void encounter_yuko_scene_one_dispatch(void) {
    if (game_state.mode_stage == 0) {
        encounter_yuko_scene_one_step_dispatch();
        return;
    }
    game_mode_reset_to_title();
}

typedef struct {
    void (*callbacks[53])(); /* HYPOTHESIS: GEKO grouping vocabulary remains unconfirmed; T-27051; src/ovl/GEKO/801393C0.c:36; func_801394EC at line39 copies this exact53-callback table and dispatches by mode_step; scene number is the actual talk_work selector, not a narrative identity. */
} EncounterYukoSceneOneSteps; /* size 0xD4 */
extern EncounterYukoSceneOneSteps encounter_yuko_scene_one_steps;

void encounter_yuko_scene_one_step_dispatch(void) {
    s32 mode_step_index; /* declared before mode_step_callbacks: its stack slot sits above mode_step_callbacks (T-3330) */
    EncounterYukoSceneOneSteps mode_step_callbacks;

    mode_step_callbacks = encounter_yuko_scene_one_steps;
    mode_step_index = game_state.mode_step;
    mode_step_callbacks.callbacks[mode_step_index](0x80);
    D_80120652 = 1;
}

void encounter_yuko_audio_command_1(void) {
    normal_date_move_place();
    if (game_state.substep++ == 0) {
        audio_command(0x501);
    }
}

void encounter_yuko_assets_load_1(void) {
    cd_read_sectors(0x3D, 0x801B0000, 0x7D96);
    encounter_yuko_assets_bind_1();
    mode_step_next();
}

void encounter_yuko_audio_command_2(void) {
    portrait_value_clear_if_high(get_g_zyotai_h(GIRL_YUKO));
    audio_command(0x501);
    talk_names_set(game_state.current_girl);
    mode_step_next();
}

void encounter_yuko_scene_init_1(void) {
    game_state.current_girl = GIRL_YUKO;
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
    strcpy(name_code_girl_short, "先生");
    talk_clear();
    encounter_scene_scripts_bind();
    encounter_scene_text_table = encounter_scene_text_section_0;
    encounter_scene_command_table = encounter_scene_command_section_0;
    encounter_scene_voice_table = encounter_scene_voice_section_0;
    strcpy(scene_place_title, "廊下");
    strcpy(scene_place_title_2, "職員室");
    game_state.girls[8].affection += 1;
    check_para_limit();
    D_800CA368 = 0x36;
    mode_step_next();
}

void encounter_yuko_still_show_1(void) {
    encounter_yuko_assets_bind_1();
    load_palette(encounter_yuko_pack_1_palette, 0x11, 1, 2, 0);
    scene_still_show((s32) encounter_yuko_pack_1_still_image_0, (s32) encounter_yuko_pack_1_still_image_1, (s32) encounter_yuko_pack_1_still_image_2, (s32) encounter_yuko_pack_1_still_image_3, (s32) encounter_yuko_pack_1_still_image_4, (s32) encounter_yuko_pack_1_still_image_5);
    scene_anim_show((s32) encounter_yuko_pack_1_animation_input_0, (s32) encounter_yuko_pack_1_animation_input_1, (s32) encounter_yuko_pack_1_animation_input_2, (s32) encounter_yuko_pack_1_animation_input_3);
    D_80120666 = 4;
    D_80122D10 = 1;
    mode_step_next();
}

void encounter_yuko_bg_load_1(void) {
    bg_read_sub2(0x4045);
    mode_step_next();
}

void encounter_yuko_bg_load_2(void) {
    bg_read_sub2(0x408D);
    mode_step_next();
}

void encounter_yuko_bg_load_3(void) {
    bg_read_sub2(0x46D6);
    mode_step_next();
}

void encounter_yuko_bg_load_4(void) {
    bg_read_sub2(0x4045);
    mode_step_next();
}
