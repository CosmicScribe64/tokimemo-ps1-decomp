/* miharu_yumi_scenes.c: src/ovl/DATE/8015A190.c:7: Miharu introductory scene and shared date-girl continuation handlers, including the continuation used for Yumi. Original object 0x8015A190-0x8015AB20 (config/objects/DATE.txt). */
#define MAIN_API_OVERRIDE_talk_work /* matched as u32 (main_api.h: s32), switch selector in $v1 (T-6010) */
#include "common.h"
#include "ovl/date.h"

extern u32 talk_work;

void date_scene_yumi_summer_load_data(void) {
    cd_read_sectors(0x3D, 0x801B0000, 0x8E3F);
    date_scene_yumi_summer_data_init();
    mode_step_next();
}

void date_scene_yumi_autumn(void) {
    s32 step_index; /* declared before step_handlers: its stack slot sits above step_handlers (T-3330) */
    FnTbl44 step_handlers;

    step_handlers = date_scene_yumi_autumn_steps;
    step_index = game_state.mode_step;
    step_handlers.callbacks[step_index](0x80);
}

void date_scene_yumi_autumn_load_data(void) {
    cd_read_sectors(0x3D, 0x801B0000, 0x8E7C);
    date_scene_yumi_autumn_data_init();
    mode_step_next();
}

void date_scene_yumi_winter(void) {
    s32 step_index;
    DateYumiWinterSteps step_handlers;
    step_handlers = date_scene_yumi_winter_steps;
    if ((game_state.mode_step >= 0x1D) && (game_state.mode_step < 0x26)) {
        hud_sprite_light_effect_draw();
    }
    step_index = game_state.mode_step;
    step_handlers.callbacks[step_index](0x3C);
}

void date_scene_yumi_winter_audio_command_1(void) {
    audio_command(0x501);
    mode_step_next();
}

void date_scene_yumi_winter_audio_command_2(void) {
    audio_command(0x500);
    mode_step_next();
}

void date_scene_yumi_winter_load_data(void) {
    cd_read_sectors(0x45, 0x801B0000, 0x8EB9);
    date_scene_yumi_winter_data_init();
    mode_step_next();
}

void date_scene_miharu_dispatch(void) {
    switch (talk_work) {
    case 3:
        date_scene_miharu_spring();
        return;
    case 5:
        date_scene_miharu_summer();
        return;
    default:
        game_mode_reset_to_title();
        return;
    }
}

void date_scene_miharu_start(void) {
    message_window_title_show(1);
    game_state.current_girl = GIRL_MIHARU;
    strcpy(name_code_girl_short, "謎の女");
    event_log_add(0xB, talk_date_spot);
    date_scene_text = D_8015DF80;
    date_scene_command = D_8015E0BC;
    date_scene_voice = D_8015E1F8;
    talk_secondary_group = 0;
    talk_secondary_line = 0;
    mode_step_next();
}

void date_scene_miharu_spring(void) {
    s32 step_index; /* declared before step_handlers: its stack slot sits above step_handlers (T-3330) */
    DateMiharuSceneSteps step_handlers;

    step_handlers = date_scene_miharu_spring_steps;
    step_index = game_state.mode_step;
    step_handlers.callbacks[step_index](0x80);
}

void date_scene_miharu_restore_girl(void) {
    game_state.current_girl = game_state.date_girl;
    talk_names_set(game_state.date_girl);
    mode_step_next();
}

void date_scene_miharu_spring_read_portrait(void) {
    portrait_read_start(0x6BAB);
    mode_step_next();
}

void date_scene_miharu_summer(void) {
    s32 step_index; /* declared before step_handlers: its stack slot sits above step_handlers (T-3330) */
    DateMiharuSceneSteps step_handlers;

    step_handlers = date_scene_miharu_summer_steps;
    hud_sprite_light_effect_draw();
    step_index = game_state.mode_step;
    step_handlers.callbacks[step_index](0x80);
}

void date_scene_miharu_summer_read_portrait(void) {
    portrait_read_start(0x6BD8);
    mode_step_next();
}
