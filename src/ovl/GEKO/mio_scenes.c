/* mio_scenes.c: src/ovl/GEKO/8013D0F0.c:23,39,54; Mio selector1, script/talk/resource callbacks and score initialization. Current GEKO folder retained; no narrative meaning is inferred from callback ordinals. Original object 0x8013D0F0-0x8013D4B0 (config/objects/GEKO.txt). */
#include "common.h"
#include "ovl/GEKO.h"

typedef struct {
    void (*f[39])();
} FnTbl39; /* size 0x9C */
extern FnTbl39 encounter_mio_scene_one_steps;

void encounter_mio_assets_bind_1(void) {
    encounter_mio_pack_1_animation_input_2 = 0x801CE0F4;
    encounter_mio_pack_1_animation_input_0 = 0x801CE0F8;
    encounter_mio_pack_1_animation_input_1 = 0x801CE118;
    encounter_mio_pack_1_animation_input_3 = *(s16 *)0x801CE12C;
    encounter_mio_pack_1_palette = 0x801B0000;
    encounter_mio_pack_1_still_image_0 = 0x801B2000;
    encounter_mio_pack_1_still_image_1 = 0x801B6000;
    encounter_mio_pack_1_still_image_2 = 0x801BA000;
    encounter_mio_pack_1_still_image_3 = 0x801BE000;
    encounter_mio_pack_1_still_image_4 = 0x801C2000;
    encounter_mio_pack_1_still_image_5 = 0x801C6000;
}

void encounter_mio_scene_select_1(void) {
    if (talk_work == 1) {
        encounter_mio_scene_one_dispatch();
        return;
    }
    game_mode_reset_to_title();
}

void encounter_mio_scene_one_dispatch(void) {
    if (game_state.mode_stage == 0) {
        encounter_mio_scene_one_step_dispatch();
        return;
    }
    game_mode_reset_to_title();
}

void encounter_mio_scene_one_step_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl39 tbl;

    tbl = encounter_mio_scene_one_steps;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
}

void encounter_mio_assets_load_1(void) {
    cd_read_sectors(0x3D, 0x801B0000, 0x8627);
    encounter_mio_assets_bind_1();
    mode_step_next();
}

void encounter_mio_scene_init_1(void) {
    game_state.current_girl = GIRL_MIO;
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
    talk_names_set(1);
    talk_clear();
    encounter_scene_scripts_bind();
    encounter_scene_text_table = encounter_scene_text_section_37;
    encounter_scene_command_table = encounter_scene_command_section_37;
    encounter_scene_voice_table = encounter_scene_voice_section_37;
    strcpy(D_800CA19C, "廊下");
    game_state.girls[1].tokimeki += 1;
    game_state.girls[1].affection += 2;
    check_para_limit();
    D_800CA368 = 8;
    mode_step_next();
}

void encounter_mio_still_show_1(void) {
    encounter_mio_assets_bind_1();
    load_palette(encounter_mio_pack_1_palette, 0x11, 1, 2, 0);
    scene_still_show(encounter_mio_pack_1_still_image_0, encounter_mio_pack_1_still_image_1, encounter_mio_pack_1_still_image_2, encounter_mio_pack_1_still_image_3, encounter_mio_pack_1_still_image_4, encounter_mio_pack_1_still_image_5);
    scene_anim_show(encounter_mio_pack_1_animation_input_0, encounter_mio_pack_1_animation_input_1, encounter_mio_pack_1_animation_input_2, (s32) encounter_mio_pack_1_animation_input_3);
    mode_step_next();
}
