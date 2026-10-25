/* rei_scenes.c: src/ovl/GEKO/8013B930.c:37,101,138,171,209; Rei selectors1/2/3 and the choice outcome that writes scene_flags79. Current GEKO folder retained; no narrative meaning is inferred from callback ordinals. Original object 0x8013B930-0x8013C4C0 (config/objects/GEKO.txt). */
#define MAIN_API_OVERRIDE_talk_work /* switched as u32 (main_api.h: s32), T-6030 */
#include "common.h"
#include "ovl/GEKO.h"

typedef struct {
    void (*callbacks[50])(); /* HYPOTHESIS: GEKO grouping vocabulary remains unconfirmed; T-27051; src/ovl/GEKO/8013B930.c:7; func_8013BE58 at line171 copies this exact50-callback table and dispatches by mode_step; scene number is the actual talk_work selector, not a narrative identity. */
} EncounterReiSceneThreeSteps; /* size 0xC8 */
extern EncounterReiSceneThreeSteps encounter_rei_scene_three_steps;

typedef struct {
    void (*f[39])();
} FnTbl39; /* size 0x9C */
extern FnTbl39 encounter_rei_scene_two_steps;

extern u32 talk_work;

void encounter_rei_bg_load_1(void) {
    bg_read_sub2(0x46A3);
    mode_step_next();
}

void encounter_rei_bg_load_2(void) {
    bg_read_sub2(0x42D8);
    mode_step_next();
}

void encounter_rei_assets_bind_1(void) {
    encounter_rei_pack_1_palette = 0x801B0000;
    encounter_rei_pack_1_still_image_0 = 0x801B2000;
    encounter_rei_pack_1_still_image_1 = 0x801B6000;
    encounter_rei_pack_1_still_image_2 = 0x801BA000;
    encounter_rei_pack_1_still_image_3 = 0x801BE000;
    encounter_rei_pack_1_still_image_4 = 0x801C2000;
    encounter_rei_pack_1_still_image_5 = 0x801C6000;
}

void encounter_rei_scene_select_1(void) {
    switch (talk_work) {
    case 1:
        encounter_rei_scene_one_dispatch();
        return;
    case 2:
        encounter_rei_scene_two_dispatch();
        return;
    case 3:
        encounter_rei_scene_three_dispatch();
        return;
    default:
        game_mode_reset_to_title();
        return;
    }
}

void encounter_rei_scene_one_dispatch(void) {
    if (game_state.mode_stage == 0) {
        encounter_rei_scene_one_step_dispatch();
        return;
    }
    game_mode_reset_to_title();
}

void encounter_rei_scene_two_dispatch(void) {
    if (game_state.mode_stage == 0) {
        encounter_rei_scene_two_step_dispatch();
        return;
    }
    game_mode_reset_to_title();
}

void encounter_rei_scene_three_dispatch(void) {
    if (game_state.mode_stage == 0) {
        encounter_rei_scene_three_step_dispatch();
        return;
    }
    game_mode_reset_to_title();
}

void encounter_rei_audio_start(void) {
    audio_bank_start(0, 0x200);
    mode_step_next();
}

s32 encounter_rei_scene_audio_load(void) {
    audio_command(0xCF);
    if ((u32)game_state.mode_step_timer.w++ < 0x40U) {
        return 0;
    }
    D_800B3D60 = 0;
    audio_bank_load_start(0, 0, 0, 0xD989, 0xD949, 0xD941);
    if (audio_bank_load_poll() == 1) {
        mode_step_next();
    }
    mode_step_next();
}

typedef struct {
    void (*callbacks[58])(); /* HYPOTHESIS: GEKO grouping vocabulary remains unconfirmed; T-27051; src/ovl/GEKO/8013B930.c:98; func_8013BBE0 at line101 copies this exact58-callback table and dispatches by mode_step; scene number is the actual talk_work selector, not a narrative identity. */
} EncounterReiSceneOneSteps; /* size 0xE8 */
extern EncounterReiSceneOneSteps encounter_rei_scene_one_steps;

void encounter_rei_scene_one_step_dispatch(void) {
    s32 mode_step_index; /* unused: its stack slot sits above mode_step_callbacks (T-3330) */
    EncounterReiSceneOneSteps mode_step_callbacks;

    mode_step_callbacks = encounter_rei_scene_one_steps;
    if (game_state.mode_step == 0x1B) {
        audio_command(0xCF);
    }
    mode_step_callbacks.callbacks[game_state.mode_step](0x80);
}

void encounter_rei_portrait_show(void) {
    s32 mode_step_after; /* declared before mode_step_before: its stack slot sits above mode_step_before (T-3330) */
    s32 mode_step_before;

    mode_step_before = game_state.mode_step;
    func_8007C8A4();
    mode_step_after = game_state.mode_step;
    if (mode_step_before != mode_step_after) {
        game_state.mode_step = mode_step_after - 1;
        place_init();
    }
}

void encounter_rei_background_fade_in(void) {
    s32 mode_step_after; /* declared before mode_step_before: its stack slot sits above mode_step_before (T-3330) */
    s32 mode_step_before;

    mode_step_before = game_state.mode_step;
    normal_date_bg_fadein();
    mode_step_after = game_state.mode_step;
    if (mode_step_before != mode_step_after) {
        game_state.mode_step = mode_step_after - 1;
        place_init2();
    }
}

void encounter_rei_scene_two_step_dispatch(void) {
    s32 mode_step_index; /* declared before mode_step_callbacks: its stack slot sits above mode_step_callbacks (T-3330) */
    FnTbl39 mode_step_callbacks;

    mode_step_callbacks = encounter_rei_scene_two_steps;
    mode_step_index = game_state.mode_step;
    mode_step_callbacks.f[mode_step_index](0x80);
}

void encounter_rei_cd_audio_request(void) {
    cd_audio_request_set(0, 0, 0xC);
    audio_command(0x300);
    mode_step_next();
}

s32 encounter_rei_text_hold(void) {
    if ((u32)game_state.mode_step_timer.w++ >= 0x11U) {
        k_reset(1);
        mode_step_next();
    }
}

void encounter_rei_audio_command_1(void) {
    audio_command(0x500);
    mode_step_next();
}

void encounter_rei_assets_load_1(void) {
    cd_read_sectors(0x35, 0x801B0000, 0x8265);
    encounter_rei_assets_bind_1();
    mode_step_next();
}

void encounter_rei_scene_three_step_dispatch(void) {
    s32 mode_step_index; /* declared before mode_step_callbacks: its stack slot sits above mode_step_callbacks (T-3330) */
    EncounterReiSceneThreeSteps mode_step_callbacks;

    mode_step_callbacks = encounter_rei_scene_three_steps;
    mode_step_index = game_state.mode_step;
    mode_step_callbacks.callbacks[mode_step_index](0x80);
}

s32 encounter_rei_scene_three_talk(void) {
    encounter_scene_talk();
    if ((u16) D_800CA154 == 9 && check_end_k() != 0) {
        if (*(u8 *) &game_state.voice_mode == 0 || (*(u8 *) &game_state.voice_mode != 0 && func_80046094() == 9)) {
            D_80122CE4 = 0;
            k_reset(1);
            D_80122CF0 = 0;
            D_8011ED5B &= 0xFF7F;
            D_800CA2AC = 0;
            D_800CA150 = (u16) D_800CA150 + 1;
            D_800CA154 = 0;
            mode_step_next();
        }
    }
}

void encounter_rei_audio_command_2(void) {
    audio_command(0x24);
    audio_command(0x503);
    mode_step_next();
}

void encounter_rei_choice_group_offset(void) {
    if (D_80122CDC != 0) {
        D_800CA150 = (u16) D_800CA150 + 5;
    }
    mode_step_next();
}

void encounter_rei_choice_outcome_apply(void) {
    mode_step_next();
    if (D_80122CDC != 0) {
        game_state.mode_step += 0x14;
        game_state.scene_flags[0x4F] = 1;
        return;
    }
    game_state.scene_flags[0x4F] = 2;
}

void encounter_rei_scene_step_next(void) {
    mode_step_next();
}

void encounter_rei_scene_init_1(void) {
    /* Chain over two GameState bytes, written as element stores: IDO then keeps the one
     * li v0,12 for both, as the original (T-9210). */
    (&game_state.current_girl)[0] = (&game_state.date_girl)[0] = 0xC;
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
    encounter_scene_text_table = encounter_scene_text_section_21;
    encounter_scene_command_table = encounter_scene_command_section_21;
    encounter_scene_voice_table = encounter_scene_voice_section_21;
    strcpy(D_800CA19C, "廊下");
    strcpy(D_800CA1DC, "廃工場前");
    if (talk_work == 1) {
        game_state.param_stress.value += 5;
    }
    check_para_limit();
    D_800CA368 = 0x4F;
    mode_step_next();
}

void encounter_rei_scene_init_2(void) {
    /* Same chain as encounter_rei_scene_init_1 (T-9210). */
    (&game_state.current_girl)[0] = (&game_state.date_girl)[0] = 0xC;
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
    encounter_scene_text_table = encounter_scene_text_section_22;
    encounter_scene_command_table = encounter_scene_command_section_22;
    encounter_scene_voice_table = encounter_scene_voice_section_22;
    strcpy(D_800CA19C, "廊下");
    D_800CA368 = 0x50;
    mode_step_next();
}

void encounter_rei_still_show_1(void) {
    encounter_rei_assets_bind_1();
    load_palette(encounter_rei_pack_1_palette, 0x11, 1, 2, 0);
    scene_still_show(encounter_rei_pack_1_still_image_0, encounter_rei_pack_1_still_image_1, encounter_rei_pack_1_still_image_2, encounter_rei_pack_1_still_image_3, encounter_rei_pack_1_still_image_4, encounter_rei_pack_1_still_image_5);
    scene_anim_show(0, 0, 0, 0);
    mode_step_next();
}

void encounter_rei_scene_three_script_select(void) {
    encounter_scene_text_table = encounter_scene_text_section_23;
    encounter_scene_command_table = encounter_scene_command_section_23;
    encounter_scene_voice_table = encounter_scene_voice_section_23;
    D_800CA368 = 0x51;
    mode_step_next();
}

void encounter_rei_bg_load_3(void) {
    bg_read_sub2(0x4045);
    mode_step_next();
}

void encounter_rei_bg_load_4(void) {
    bg_read_sub2(0x4943);
    mode_step_next();
}

void encounter_rei_bg_load_5(void) {
    bg_read_sub2(0x403B);
    mode_step_next();
}

void encounter_rei_bg_load_6(void) {
    bg_read_sub2(0x403B);
    mode_step_next();
}
