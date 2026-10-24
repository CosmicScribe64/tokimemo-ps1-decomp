/* ayako_scenes.c: src/ovl/GEKO/8013C4C0.c:18,39,54,71; Ayako selector1 dispatch, speaker/resource binding and affection update. Current GEKO folder retained; no narrative meaning is inferred from callback ordinals. Original object 0x8013C4C0-0x8013C980 (config/objects/GEKO.txt). */
#include "common.h"
#include "ovl/GEKO.h"

void encounter_ayako_assets_bind_1(void) {
    encounter_ayako_pack_1_animation_input_2 = 0x801CE120;
    encounter_ayako_pack_1_animation_input_0 = 0x801CE124;
    encounter_ayako_pack_1_animation_input_1 = 0x801CE144;
    encounter_ayako_pack_1_animation_input_3 = (*(s16 *)0x801CE158);
    encounter_ayako_pack_1_palette = 0x801B0000;
    encounter_ayako_pack_1_still_image_0 = 0x801B2000;
    encounter_ayako_pack_1_still_image_1 = 0x801B6000;
    encounter_ayako_pack_1_still_image_2 = 0x801BA000;
    encounter_ayako_pack_1_still_image_3 = 0x801BE000;
    encounter_ayako_pack_1_still_image_4 = 0x801C2000;
    encounter_ayako_pack_1_still_image_5 = 0x801C6000;
}

void encounter_ayako_scene_select_1(void) {
    if (talk_work == 1) {
        encounter_ayako_scene_one_dispatch();
        return;
    }
    game_mode_reset_to_title();
}

void encounter_ayako_scene_one_dispatch(void) {
    if (game_state.mode_stage == 0) {
        encounter_ayako_scene_one_step_dispatch();
        return;
    }
    game_mode_reset_to_title();
}

typedef struct {
    void (*f[32])();
} FnTbl32; /* size 0x80 */
extern FnTbl32 encounter_ayako_scene_one_steps;

void encounter_ayako_scene_one_step_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl32 tbl;

    tbl = encounter_ayako_scene_one_steps;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
}

void encounter_ayako_portrait_disable(void) {
    D_80122D10 = 2;
    D_80120652 = 0;
    mode_step_next();
}

void encounter_ayako_speaker_update(void) {
    if ((u16) D_800CA154 == 0) {
        game_state.current_girl = GIRL_SOTOI;
    } else if ((u16) D_800CA154 == 1) {
        game_state.current_girl = GIRL_YOSHIO;
    } else {
        game_state.current_girl = GIRL_AYAKO;
    }
    encounter_scene_talk();
}

void encounter_ayako_assets_load_1(void) {
    cd_read_sectors(0x3D, 0x801B0000, 0x829A);
    encounter_ayako_assets_bind_1();
    mode_step_next();
}

void encounter_ayako_scene_init_1(void) {
    game_state.current_girl = GIRL_AYAKO;
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
    dec_bg_brightness = 0;
    talk_names_set(game_state.current_girl);
    talk_clear();
    encounter_scene_scripts_bind();
    encounter_scene_text_table = encounter_scene_text_section_24;
    encounter_scene_command_table = encounter_scene_command_section_24;
    encounter_scene_voice_table = encounter_scene_voice_section_24;
    strcpy(D_800CA19C, "校舎裏");
    game_state.girls[3].affection += 2;
    check_para_limit();
    D_800CA368 = 0x15;
    mode_step_next();
}

void encounter_ayako_still_show_1(void) {
    encounter_ayako_assets_bind_1();
    load_palette(encounter_ayako_pack_1_palette, 0x11, 1, 2, 0);
    scene_still_show(encounter_ayako_pack_1_still_image_0, encounter_ayako_pack_1_still_image_1, encounter_ayako_pack_1_still_image_2, encounter_ayako_pack_1_still_image_3, encounter_ayako_pack_1_still_image_4, encounter_ayako_pack_1_still_image_5);
    scene_anim_show(encounter_ayako_pack_1_animation_input_0, encounter_ayako_pack_1_animation_input_1, encounter_ayako_pack_1_animation_input_2, encounter_ayako_pack_1_animation_input_3);
    D_800CA360 = 1;
    mode_step_next();
}

void encounter_ayako_bg_load_1(void) {
    bg_read_sub2(0x407A);
    mode_step_next();
}

void encounter_ayako_bg_load_2(void) {
    bg_read_sub2(0x46BC);
    mode_step_next();
}
