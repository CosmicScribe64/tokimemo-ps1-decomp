/* yuina_scenes.c: src/ovl/GEKO/8013AF80.c:26,56,102,111,222; Yuina selectors1/7, scene callbacks, RPG handoff and forced-calendar branch. Current GEKO folder retained; no narrative meaning is inferred from callback ordinals. Original object 0x8013AF80-0x8013B930 (config/objects/GEKO.txt). */
#define MAIN_API_OVERRIDE_talk_work /* matched as u32 (main_api.h: s32), switch selector in $v1 (T-6010) */
#include "common.h"
#include "ovl/GEKO.h"

extern u32 talk_work;

typedef struct {
    void (*callbacks[41])(); /* HYPOTHESIS: GEKO grouping vocabulary remains unconfirmed; T-27051; src/ovl/GEKO/8013AF80.c:9; func_8013B2C8 at line102 copies this exact41-callback table and dispatches by mode_step; scene number is the actual talk_work selector, not a narrative identity. */
} EncounterYuinaSceneSevenSteps; /* size 0xA4 */
extern EncounterYuinaSceneSevenSteps encounter_yuina_scene_seven_steps;

void encounter_yuina_assets_bind_1(void) {
    D_80146600 = 0x801D212C;
    D_80146604 = 0x801D2130;
    D_80146608 = 0x801D2150;
    D_8014660C = (*(s16 *)0x801D216C);
    encounter_yuina_pack_1_palette = 0x801B0000;
    encounter_yuina_pack_1_still_image_0 = 0x801B2000;
    encounter_yuina_pack_1_still_image_1 = 0x801B6000;
    encounter_yuina_pack_1_still_image_2 = 0x801BA000;
    encounter_yuina_pack_1_still_image_3 = 0x801BE000;
    encounter_yuina_pack_1_still_image_4 = 0x801C2000;
    encounter_yuina_pack_1_still_image_5 = 0x801C6000;
}

void encounter_yuina_scene_select_1(void) {
    switch (talk_work) {
    case 1:
        encounter_yuina_scene_one_dispatch();
        return;
    case 7:
        encounter_yuina_scene_seven_dispatch();
        return;
    default:
        game_mode_reset_to_title();
        return;
    }
}

void encounter_yuina_scene_one_dispatch(void) {
    if (game_state.mode_stage == 0) {
        encounter_yuina_scene_one_step_dispatch();
        return;
    }
    game_mode_reset_to_title();
}

void encounter_yuina_scene_seven_dispatch(void) {
    if (game_state.mode_stage == 0) {
        encounter_yuina_scene_seven_step_dispatch();
        return;
    }
    game_mode_reset_to_title();
}

s32 encounter_yuina_scene_one_step_dispatch(void) {
    s32 mode_step_index; /* declared before mode_step_callbacks: its stack slot sits above mode_step_callbacks (T-3330) */
    s32 level;
    EncounterYuinaSceneOneSteps mode_step_callbacks;

    mode_step_callbacks = encounter_yuina_scene_one_steps;
    mode_step_index = game_state.mode_step;
    mode_step_callbacks.callbacks[mode_step_index](0x80);
    if (D_80122D04 != 0) {
        /* FAKE: Actor view on the first two stores keeps as1 from hoisting the unk_03C load above sb D_80120723. T-24095 */
        ((Actor *)actor_pool_96)[3].brightness = dec_bg_brightness;
        ((Actor *)actor_pool_96)[3].draw_flags |= 0x80;
        if ((u8)game_state.voice_mode != VOICE_OFF) {
            if (!encounter_yuina_voice_trigger_latched) {
                D_8012071E = 3;
            }
            if (D_80120734 == 6 && !encounter_yuina_voice_trigger_latched) {
                D_8012071E = 0;
            }
            if (cd_audio_state() & 4) {
                level = getCDlevel();
            } else {
                level = 0;
            }
            if (level != 0) {
                if (!encounter_yuina_voice_trigger_latched) {
                    D_8012071E = 3;
                    encounter_yuina_voice_trigger_latched = 1;
                }
            }
        } else if (!encounter_yuina_voice_trigger_latched) {
            D_8012071E = 3;
            D_80120734 = 1;
            encounter_yuina_voice_trigger_latched = 1;
        }
    } else {
        D_8012071F &= 0xFF7F;
    }
}

void encounter_yuina_assets_load_1(void) {
    cd_read_sectors(0x45, 0x801B0000, 0x80FF);
    encounter_yuina_assets_bind_1();
    mode_step_next();
}

void encounter_yuina_scene_seven_step_dispatch(void) {
    s32 mode_step_index; /* declared before mode_step_callbacks: its stack slot sits above mode_step_callbacks (T-3330) */
    EncounterYuinaSceneSevenSteps mode_step_callbacks;

    mode_step_callbacks = encounter_yuina_scene_seven_steps;
    mode_step_index = game_state.mode_step;
    mode_step_callbacks.callbacks[mode_step_index](0x80);
}

void encounter_yuina_rpg_enter(void) {
    D_80122CE0 = 6;
    actor_clear_range(0);
    hud_cursor_show(0);
    game_state.saved_game_mode = game_state.game_mode;
    game_state.saved_mode_stage = game_state.mode_stage;
    game_state.saved_mode_step = game_state.mode_step + 1;
    game_mode_set(GAME_MODE_RPG_BATTLE);
}

void encounter_yuina_rpg_return_init(void) {
    s32 fake_stack_pad; /* FAKE: unused scalar reproduces RECT home at +0x2C; original local unknown. T-9770 */
    RECT r;

    draw2d3d(1, 0);
    actor_draw_enable(1);
    sprite_tables_clear();
    actor_clear_range(0);
    hud_cursor_show(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_system_reset();
    talk_clear();
    k_reset(1);
    r.x = 0x140;
    r.y = 0x100;
    r.w = 0x140;
    r.h = 0xF8;
    func_8009C7F8(&r, 8, 0, 0);
    r.x = 0x2C0;
    r.y = 0;
    func_8009C7F8(&r, 8, 0, 0);
    game_state.tile_layer_enabled = 0;
    game_state.quads_enabled = 1;
    /* FAKE: scalar byte store then member read preserves argument scheduling; sibling encounter_mira_speaker_restore uses the same idiom. T-9770 */
    *(u8 *)&game_state.current_girl = 2;
    talk_names_set(game_state.current_girl);
    addr_init_bustup();
    game_state.screen_brightness = 0x80;
    dec_bg_brightness = 0;
    hizuke_init();
    message_window_init();
    text_marker_hide();
    encounter_scene_scripts_bind();
    encounter_scene_text_table = encounter_scene_text_section_20;
    encounter_scene_command_table = encounter_scene_command_section_20;
    encounter_scene_voice_table = encounter_scene_voice_section_20;
    mode_step_next();
}

void encounter_yuina_scene_init_1(void) {
    game_state.current_girl = GIRL_YUINA;
    draw2d3d(1, 0);
    actor_draw_enable(1);
    sprite_tables_clear();
    actor_clear_range(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_system_reset();
    D_8011ED9F = 0;
    D_8011EDE3 = 0;
    D_800CA150 = 0;
    D_800CA154 = 0;
    addr_init_bustup();
    encounter_scene_scripts_bind();
    hizuke_init();
    message_window_init();
    text_marker_hide();
    talk_names_set(game_state.current_girl);
    talk_clear();
    encounter_scene_scripts_bind();
    encounter_scene_text_table = encounter_scene_text_section_14;
    encounter_scene_command_table = encounter_scene_command_section_14;
    encounter_scene_voice_table = encounter_scene_voice_section_14;
    strcpy(D_800CA19C, "屋上");
    encounter_yuina_voice_trigger_latched = 0;
    game_state.girls[2].tokimeki += 1;
    game_state.girls[2].affection += 2;
    check_para_limit();
    D_800CA368 = 0xE;
    mode_step_next();
}

void encounter_yuina_still_show_1(void) {
    encounter_yuina_assets_bind_1();
    load_palette(encounter_yuina_pack_1_palette, 0x11, 1, 2, 0);
    scene_still_show(encounter_yuina_pack_1_still_image_0, encounter_yuina_pack_1_still_image_1, encounter_yuina_pack_1_still_image_2, encounter_yuina_pack_1_still_image_3, encounter_yuina_pack_1_still_image_4, encounter_yuina_pack_1_still_image_5);
    scene_anim_show(0, 0, 0, 0);
    actor_clear(0x63);
    D_8012071D = 0xA;
    D_8012071E = 0;
    D_80120754 = 0x41000000;
    D_8012071F = 4;
    D_80120728 = D_80146604;
    D_8012072C = D_80146608;
    D_80120750 = D_80146600;
    D_80120730 = D_8014660C;
    D_80120732 = 4;
    D_80120734 = 0;
    D_80120724 = 0;
    D_80120722 = 1;
    D_8012075F = 0x11;
    D_80120742 = -0xA0;
    D_80120746 = -0x78;
    D_80120723 = 0;
    D_80120721 = 0;
    D_80120720 = 0;
    game_state.tpage_buf[26] = -1;
    mode_step_next();
}

void encounter_yuina_final_scene_init(void) {
    game_state.year = 0x62;
    game_state.month = 2;
    game_state.day = 0x1C;
    game_state.weekday = WEEKDAY_SATURDAY;
    game_state.current_girl = GIRL_YUINA;
    draw2d3d(1, 0);
    actor_draw_enable(1);
    sprite_tables_clear();
    actor_clear_range(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_system_reset();
    D_800CA150 = 0;
    D_800CA154 = 0;
    addr_init_bustup();
    encounter_scene_scripts_bind();
    hizuke_init();
    message_window_init();
    text_marker_hide();
    talk_names_set(game_state.current_girl);
    talk_clear();
    encounter_scene_scripts_bind();
    encounter_scene_text_table = encounter_scene_text_section_20;
    encounter_scene_command_table = encounter_scene_command_section_20;
    encounter_scene_voice_table = encounter_scene_voice_section_20;
    strcpy(D_800CA19C, "近所の公園");
    D_800CA368 = 0x14;
    mode_step_next();
}

void encounter_yuina_bg_load_1(void) {
    bg_read_sub2(0x412C);
    mode_step_next();
}
