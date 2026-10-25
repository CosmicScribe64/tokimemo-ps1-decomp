/* miharu_scenes.c: src/ovl/GEKO/80140A00.c:69,115,136,166,217; Miharu selectors1/2/4, story-counter group selection and stress/girl-score effects. Current GEKO folder retained; no narrative meaning is inferred from callback ordinals. Original object 0x80140A00-0x80141AF0 (config/objects/GEKO.txt). */
#define MAIN_API_OVERRIDE_talk_work /* switched as u32 (main_api.h: s32), T-6030 */
#include "common.h"
#include "ovl/GEKO.h"

extern u32 talk_work;

void encounter_miharu_bg_load_1(void) {
    bg_read_sub2(0x4699);
    mode_step_next();
}

void encounter_miharu_bg_load_2(void) {
    bg_read_sub2(0x4084);
    mode_step_next();
}

void encounter_miharu_bg_load_3(void) {
    bg_read_sub2(0x4045);
    mode_step_next();
}

void encounter_miharu_bg_load_4(void) {
    bg_read_sub2(0x4144);
    mode_step_next();
}

void encounter_miharu_bg_load_5(void) {
    D_80122D38 = 0;
    parameter_disp_switch(0);
    icon_disp_switch(0);
    bg_read_sub2(0x4144);
    mode_step_next();
}

void encounter_miharu_bg_load_6(void) {
    bg_read_sub2(0x415E);
    mode_step_next();
}

void encounter_miharu_bg_load_7(void) {
    bg_read_sub2(0x4743);
    mode_step_next();
}

void encounter_miharu_assets_bind_1(void) {
    encounter_miharu_pack_1_palette = 0x801B0000;
    encounter_miharu_pack_1_still_image_0 = 0x801B2000;
    encounter_miharu_pack_1_still_image_1 = 0x801B6000;
    encounter_miharu_pack_1_still_image_2 = 0x801BA000;
    encounter_miharu_pack_1_still_image_3 = 0x801BE000;
    encounter_miharu_pack_1_still_image_4 = 0x801C2000;
    encounter_miharu_pack_1_still_image_5 = 0x801C6000;
}

void encounter_miharu_assets_bind_2(void) {
    encounter_miharu_pack_2_animation_input_2 = 0x801CE150;
    encounter_miharu_pack_2_animation_input_0 = 0x801CE154;
    encounter_miharu_pack_2_animation_input_1 = 0x801CE1A4;
    encounter_miharu_pack_2_animation_input_3 = (*(s16 *)0x801CE1BC);
    encounter_miharu_pack_2_palette = 0x801B0000;
    encounter_miharu_pack_2_still_image_0 = 0x801B2000;
    encounter_miharu_pack_2_still_image_1 = 0x801B6000;
    encounter_miharu_pack_2_still_image_2 = 0x801BA000;
    encounter_miharu_pack_2_still_image_3 = 0x801BE000;
    encounter_miharu_pack_2_still_image_4 = 0x801C2000;
    encounter_miharu_pack_2_still_image_5 = 0x801C6000;
}

void encounter_miharu_scene_select_1(void) {
    switch (talk_work) {
    case 1:
        encounter_miharu_scene_one_dispatch();
        return;
    case 2:
        encounter_miharu_scene_two_dispatch();
        return;
    case 4:
        encounter_miharu_scene_four_dispatch();
        return;
    default:
        game_mode_reset_to_title();
        return;
    }
}

void encounter_miharu_scene_one_dispatch(void) {
    if (game_state.mode_stage == 0) {
        encounter_miharu_scene_one_step_dispatch();
        return;
    }
    game_mode_reset_to_title();
}

void encounter_miharu_scene_two_dispatch(void) {
    if (game_state.mode_stage == 0) {
        encounter_miharu_scene_two_step_dispatch();
        return;
    }
    game_mode_reset_to_title();
}

void encounter_miharu_scene_four_dispatch(void) {
    if (game_state.mode_stage == 0) {
        encounter_miharu_scene_four_step_dispatch();
        return;
    }
    game_mode_reset_to_title();
}

typedef struct {
    void (*callbacks[27])(); /* HYPOTHESIS: GEKO grouping vocabulary remains unconfirmed; T-27051; src/ovl/GEKO/80140A00.c:112; func_80140D80 at line115 copies this exact27-callback table and dispatches by mode_step; scene number is the actual talk_work selector, not a narrative identity. */
} EncounterMiharuSceneOneSteps; /* size 0x6C */
extern EncounterMiharuSceneOneSteps encounter_miharu_scene_one_steps;

void encounter_miharu_scene_one_step_dispatch(void) {
    s32 mode_step_index; /* declared before mode_step_callbacks: its stack slot sits above mode_step_callbacks (T-3330) */
    EncounterMiharuSceneOneSteps mode_step_callbacks;

    mode_step_callbacks = encounter_miharu_scene_one_steps;
    mode_step_index = game_state.mode_step;
    mode_step_callbacks.callbacks[mode_step_index]();
}

void encounter_miharu_audio_command_1(void) {
    set_kanji_string(-0x80, 0x30, 1, "ドン", 0);
    audio_command(0x24);
    audio_command(0x500);
    mode_step_next();
}

typedef struct {
    void (*callbacks[28])(); /* HYPOTHESIS: GEKO grouping vocabulary remains unconfirmed; T-27051; src/ovl/GEKO/80140A00.c:133; func_80140E40 at line136 copies this exact28-callback table and dispatches by mode_step; scene number is the actual talk_work selector, not a narrative identity. */
} EncounterMiharuSceneTwoSteps; /* size 0x70 */
extern EncounterMiharuSceneTwoSteps encounter_miharu_scene_two_steps;

void encounter_miharu_scene_two_step_dispatch(void) {
    s32 mode_step_index; /* declared before mode_step_callbacks: its stack slot sits above mode_step_callbacks (T-3330) */
    EncounterMiharuSceneTwoSteps mode_step_callbacks;

    mode_step_callbacks = encounter_miharu_scene_two_steps;
    mode_step_index = game_state.mode_step;
    mode_step_callbacks.callbacks[mode_step_index](0x80);
}

s32 encounter_miharu_talk_audio_command(void) {
    encounter_scene_talk();
    if ((u16) D_800CA154 == 3) {
        if (game_state.mode_step_timer.w++ == 1) {
            audio_command(0x501);
        }
    }
}

void encounter_miharu_assets_load_1(void) {
    cd_read_sectors(0x35, 0x801B0000, 0x8F43);
    mode_step_next();
}

void encounter_miharu_still_show_1(void) {
    load_palette(encounter_miharu_pack_1_palette, 0x11, 1, 2, 0);
    scene_still_show(encounter_miharu_pack_1_still_image_0, encounter_miharu_pack_1_still_image_1, encounter_miharu_pack_1_still_image_2, encounter_miharu_pack_1_still_image_3, encounter_miharu_pack_1_still_image_4, encounter_miharu_pack_1_still_image_5);
    scene_anim_show(0, 0, 0, 0);
    mode_step_next();
}

void encounter_miharu_story_group_select(void) {
    u32 story_stage_value;
    GameState *state_data;

    state_data = &game_state;
    story_stage_value = state_data->girls[11].flags.f.story_stage;
    if (story_stage_value >= 5U) {
        state_data->girls[11].flags.f.story_stage = 4;
        story_stage_value = game_state.girls[11].flags.f.story_stage;
    }
    D_800CA150 = (u16)D_800CA150 + story_stage_value * 2;
    state_data->girls[11].flags.f.story_stage++;
    mode_step_next();
}

void encounter_miharu_title_hide(void) {
    message_window_title_show(0);
    mode_step_next();
}

void encounter_miharu_scene_init_1(void) {
    game_state.current_girl = GIRL_MIHARU;
    draw2d3d(1, 0);
    actor_draw_enable(1);
    sprite_tables_clear();
    actor_clear_range(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_system_reset();
    strcpy(D_800CA19C, "廊下");
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
    strcpy(name_code_family, game_state.player_family_name);
    strcpy(name_code_girl_short, "謎の女");
    talk_clear();
    encounter_scene_scripts_bind();
    encounter_scene_text_table = encounter_scene_text_section_74;
    encounter_scene_command_table = encounter_scene_command_section_74;
    encounter_scene_voice_table = encounter_scene_voice_section_74;
    D_800CA368 = 0x4A;
    mode_step_next();
}

void encounter_miharu_scene_init_2(void) {
    s16 i;

    game_state.current_girl = GIRL_MIHARU;
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
    strcpy(name_code_family, game_state.player_family_name);
    talk_clear();
    encounter_scene_text_table = encounter_scene_text_section_75;
    encounter_scene_command_table = encounter_scene_command_section_75;
    encounter_scene_voice_table = encounter_scene_voice_section_75;
    if (talk_work == 1) {
        game_state.param_stress.value -= 5;
        for (i = 0; i < 0xB; i++) {
            if (game_state.girls[i].flags.f.met) {
                game_state.girls[i].tokimeki -= 1;
            }
        }
        check_para_limit();
    }
    D_800CA368 = 0xFF;
    mode_step_next();
}

void encounter_miharu_scene_init_3(void) {
    s16 icon_index;

    game_state.current_girl = GIRL_MIHARU;
    draw2d3d(1, 0);
    actor_draw_enable(1);
    sprite_tables_clear();
    actor_clear_range(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_system_reset();
    k_reset(1);
    D_800CA150 = 0;
    D_800CA154 = 0;
    addr_init_bustup();
    hizuke_init();
    message_window_init();
    text_marker_hide();
    hud_icon_bar_init();
    icon_index = 0;
    do {
        icon_can_use_set(icon_index, 0);
        icon_index += 1;
    } while (icon_index < 0xF);
    hud_icon_bar_update(1);
    parameter_show_init();
    parameter_show_init();
    dec_bg_brightness = 0x80;
    strcpy(name_code_family, game_state.player_family_name);
    strcpy(name_code_girl_short, "館林");
    strcpy(girl_name_buf, "館林さん");
    talk_clear();
    D_80122D38 = 1;
    encounter_scene_scripts_bind();
    encounter_scene_text_table = encounter_scene_text_section_77;
    encounter_scene_command_table = encounter_scene_command_section_77;
    encounter_scene_voice_table = encounter_scene_voice_section_77;
    strcpy(D_800CA19C, "きらめき中央公園");
    strcpy(D_800CA1DC, "並木道");
    mode_step_next();
}


void encounter_miharu_still_show_2(void) {
    encounter_miharu_assets_bind_2();
    load_palette(encounter_miharu_pack_2_palette, 0x11, 1, 2, 0);
    scene_still_show(encounter_miharu_pack_2_still_image_0, encounter_miharu_pack_2_still_image_1, encounter_miharu_pack_2_still_image_2, encounter_miharu_pack_2_still_image_3, encounter_miharu_pack_2_still_image_4, encounter_miharu_pack_2_still_image_5);
    scene_anim_show(encounter_miharu_pack_2_animation_input_0, encounter_miharu_pack_2_animation_input_1, encounter_miharu_pack_2_animation_input_2, encounter_miharu_pack_2_animation_input_3);
    ((Actor *)actor_pool_96)->unk_04 = ((Actor *)actor_pool_96)[1].unk_04 = 8;
    D_800CA368 = 0x4D;
    mode_step_next();
}

void encounter_miharu_bg_load_8(void) {
    bg_read_sub2(0x4045);
    mode_step_next();
}

void encounter_miharu_bg_load_9(void) {
    bg_read_sub2(0x403B);
    mode_step_next();
}

void encounter_miharu_bg_load_10(void) {
    bg_read_sub2(0x4708);
    mode_step_next();
}

void encounter_miharu_bg_load_11(void) {
    D_80122D38 = 0;
    parameter_disp_switch(0);
    icon_disp_switch(0);
    bg_read_sub2(0x42EB);
    mode_step_next();
}

void encounter_miharu_bg_load_12(void) {
    bg_read_sub2(0x430F);
    mode_step_next();
}

void encounter_miharu_bg_load_13(void) {
    bg_read_sub2(0x47F7);
    mode_step_next();
}

void encounter_miharu_closing_bg_request(void) {
    s32 background_read_result;

    D_800B3D60 = 0;
    background_read_result = dec_bg_cd_read(0x3FB6, 0);
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

void encounter_miharu_scene_four_step_dispatch(void) {
    u8 mode_step_index;
    EncounterMiharuClosingSteps mode_step_callbacks;

    mode_step_callbacks = encounter_miharu_scene_four_steps;
    mode_step_index = game_state.mode_step;
    mode_step_callbacks.callback[mode_step_index](0x20);
    mode_step_index = game_state.mode_step;
    if (mode_step_index >= 36 && mode_step_index < 43) {
        hud_sprite_light_effect_draw();
    } else if (mode_step_index == 49) {
        encounter_scene_light_effect_init();
    }
}

void encounter_miharu_closing_girl_in(void) {
    if (game_state.substep == 0) {
        k_reset(0);
        message_window_set_title(D_800CA1DC);
        game_state.substep += 1;
    }
    normal_date_girl_in();
}

s32 encounter_miharu_closing_phone_talk(void) {
    /* FAKE: dead entry-block read puts D_800CA154 in cvt_pass _entry_locations so uopt promotes it to $a0 (T-24115) */
    if (D_800CA154) {}
    if (check_end_k() != 0 && D_800CA154 == 1) {
        if (game_state.substep++ < 0x40U) {
            return 0;
        }
        game_state.substep = 0;
        k_reset(1);
    }
    if (D_800CA154 == 0 || D_800CA154 == 8) {
        encounter_scene_phone_talk();
    } else {
        encounter_scene_talk();
    }
    /* FAKE: signed .w in the first test and unsigned .u++ in the second stops uopt from CSE-merging the == 1 boolean while still CSE-ing the load (T-24115) */
    if (game_state.mode_step_timer.w == 1 && D_800CA154 == 1) {
        audio_command(0x609);
    }
    if (game_state.mode_step_timer.u++ == 1 && D_800CA154 == 9) {
        audio_command(0x60B);
    }
}

void encounter_miharu_audio_command_2(void) {
    audio_command(0x24);
    audio_command(0x500);
    mode_step_next();
}

void encounter_miharu_assets_load_2(void) {
    cd_read_sectors(0x3D, 0x801B0000, 0x8F78);
    encounter_miharu_assets_bind_2();
    mode_step_next();
}

void encounter_miharu_audio_load_1(void) {
    audio_bank_load_start(1, 0xC043, 0xC01D, 0xCEFD, 0xCEB4, 0xCEB1);
    if (audio_bank_load_poll() == 1) {
        mode_step_next();
    }
    mode_step_next();
}

void encounter_miharu_talk_group_next(void) {
    encounter_scene_talk();
    D_800CA150 = (u16) D_800CA150 + 1;
    D_800CA154 = 0;
    mode_step_next();
}

void encounter_miharu_text_clear(void) {
    k_reset(1);
    D_80122CE4 = 0;
    D_80122CF0 = 0;
    D_800CA2AC = 0;
    mode_step_next();
}

void encounter_miharu_portrait_read(void) {
    portrait_read_start(0x6C05);
    mode_step_next();
}
