/* scene_saki.c: src/ovl/DATE/801586A0.c:7: mode 0x5E scenes of Saki (girl 4). Original object 0x801586A0-0x801594B0 (config/objects/DATE.txt). */
#define MAIN_API_OVERRIDE_talk_work /* switched as u32: selector in $v1 (main_api.h: s32) */
#include "common.h"
#include "ovl/DATE.h"

extern u32 talk_work;

void date_scene_saki_spring_data_init(void) {
    D_80160440 = 0x801CE124;
    D_80160444 = 0x801CE128;
    D_80160448 = 0x801CE148;
    D_8016044C = *(s16 *)0x801CE15C;
    D_80160450 = 0x801B0000;
    D_80160454 = 0x801B2000;
    D_80160458 = 0x801B6000;
    D_8016045C = 0x801BA000;
    D_80160460 = 0x801BE000;
    D_80160464 = 0x801C2000;
    D_80160468 = 0x801C6000;
}

void date_scene_saki_summer_data_init(void) {
    D_8016046C = 0x801D2240;
    D_80160470 = 0x801D2248;
    D_80160474 = 0x801D2288;
    D_80160478 = *(s16 *)0x801D22A0;
    D_8016047C = 0x801B0000;
    D_80160480 = 0x801B2000;
    D_80160484 = 0x801B6000;
    D_80160488 = 0x801BA000;
    D_8016048C = 0x801BE000;
    D_80160490 = 0x801C2000;
    D_80160494 = 0x801C6000;
}

void date_scene_saki_autumn_data_init(void) {
    D_80160498 = 0x801CE0F4;
    D_8016049C = 0x801CE0F8;
    D_801604A0 = 0x801CE118;
    D_801604A4 = *(s16 *)0x801CE12C;
    D_801604A8 = 0x801B0000;
    D_801604AC = 0x801B2000;
    D_801604B0 = 0x801B6000;
    D_801604B4 = 0x801BA000;
    D_801604B8 = 0x801BE000;
    D_801604BC = 0x801C2000;
    D_801604C0 = 0x801C6000;
}

void date_scene_saki_winter_data_init(void) {
    D_801604C4 = 0x801CE1A0;
    D_801604C8 = 0x801CE1A4;
    D_801604CC = 0x801CE1D4;
    D_801604D0 = *(s16 *)0x801CE1F0;
    D_801604D4 = 0x801B0000;
    D_801604D8 = 0x801B2000;
    D_801604DC = 0x801B6000;
    D_801604E0 = 0x801BA000;
    D_801604E4 = 0x801BE000;
    D_801604E8 = 0x801C2000;
    D_801604EC = 0x801C6000;
}

void date_scene_saki_dispatch(void) {
    switch (talk_work) {
    case 2:
        date_scene_saki_spring();
        return;
    case 3:
        date_scene_saki_summer();
        return;
    case 4:
        date_scene_saki_autumn();
        return;
    case 5:
        date_scene_saki_winter();
        return;
    default:
        game_mode_reset_to_title();
        return;
    }
}

void date_scene_saki_spring_setup(void) {
    date_scene_text = D_8015DF48;
    date_scene_command = D_8015E084;
    date_scene_voice = D_8015E1C0;
    date_scene_saki_spring_data_init();
    load_palette(D_80160450, 0x11, 1, 2, 0);
    scene_still_show(D_80160454, D_80160458, D_8016045C, D_80160460, D_80160464, D_80160468);
    scene_anim_show(D_80160444, D_80160448, D_80160440, (s32) D_8016044C);
    game_state.girls[4].tokimeki += 3;
    game_state.girls[4].affection += 2;
    game_state.girls[4].syoushin -= 0x14;
    game_state.param_condition.value += 0xA;
    check_para_limit();
    mode_step_next();
}

void date_scene_saki_summer_setup(void) {
    date_scene_text = D_8015DF4C;
    date_scene_command = D_8015E088;
    date_scene_voice = D_8015E1C4;
    date_scene_saki_summer_data_init();
    load_palette(D_8016047C, 0x11, 1, 2, 0);
    scene_still_show(D_80160480, D_80160484, D_80160488, D_8016048C, D_80160490, D_80160494);
    scene_anim_show(D_80160470, D_80160474, D_8016046C, (s32) D_80160478);
    scene_mouth_frames_reversed_saved = 1;
    D_80122D0C = 1;
    game_state.girls[4].tokimeki += 3;
    game_state.girls[4].affection += 2;
    game_state.girls[4].syoushin -= 0x14;
    check_para_limit();
    D_800CA224[0] = 3;
    D_800CA224[1] = 3;
    D_800CA224[2] = 3;
    D_800CA234[0] = 2;
    D_800CA234[1] = 2;
    D_800CA234[2] = 2;
    mode_step_next();
}

void date_scene_saki_autumn_setup(void) {
    date_scene_text = D_8015DF50;
    date_scene_command = D_8015E08C;
    date_scene_voice = D_8015E1C8;
    date_scene_saki_autumn_data_init();
    load_palette(D_801604A8, 0x11, 1, 2, 0);
    scene_still_show(D_801604AC, D_801604B0, D_801604B4, D_801604B8, D_801604BC, D_801604C0);
    scene_anim_show(D_8016049C, D_801604A0, D_80160498, (s32) D_801604A4);
    scene_mouth_frames_reversed_saved = 1;
    game_state.girls[4].affection += 2;
    game_state.girls[4].syoushin -= 0x14;
    check_para_limit();
    mode_step_next();
}

void date_scene_saki_winter_setup(void) {
    date_scene_text = D_8015DF54;
    date_scene_command = D_8015E090;
    date_scene_voice = D_8015E1CC;
    date_scene_saki_winter_data_init();
    load_palette(D_801604D4, 0x11, 1, 2, 0);
    scene_still_show(D_801604D8, D_801604DC, D_801604E0, D_801604E4, D_801604E8, D_801604EC);
    scene_anim_show(D_801604C8, D_801604CC, D_801604C4, (s32) D_801604D0);
    scene_mouth_frames_reversed_saved = 1;
    game_state.girls[4].tokimeki += 1;
    game_state.girls[4].affection += 1;
    game_state.girls[4].syoushin -= 0x14;
    check_para_limit();
    D_800CA21C[0] = 2;
    D_800CA21C[2] = 2;
    D_800CA21C[1] = 2;
    D_800CA224[0] = 2;
    D_800CA224[2] = 2;
    D_800CA224[1] = 2;
    D_800CA22C[0] = 2;
    D_800CA22C[2] = 2;
    D_800CA22C[1] = 2;
    D_800CA234[0] = 1;
    D_800CA234[2] = 1;
    D_800CA234[1] = 1;
    mode_step_next();
}

void date_scene_saki_spring_background_1(void) {
    bg_read_sub2(0x47D6);
    mode_step_next();
}

void date_scene_saki_spring_background_2(void) {
    bg_read_sub2(0x42EB);
    mode_step_next();
}

void date_scene_saki_summer_background_1(void) {
    bg_read_sub2(0x48E0);
    mode_step_next();
}

void date_scene_saki_summer_background_2(void) {
    bg_read_sub2(0x45BF);
    mode_step_next();
}

void date_scene_saki_autumn_background_1(void) {
    bg_read_sub2(0x490A);
    mode_step_next();
}

void date_scene_saki_autumn_background_2(void) {
    bg_read_sub2(0x45F8);
    mode_step_next();
}

void date_scene_saki_winter_background_1(void) {
    bg_read_sub2(0x4868);
    mode_step_next();
}

void date_scene_saki_winter_audio_command_background_1(void) {
    audio_command(0x203);
    bg_read_sub2(0x4520);
    mode_step_next();
}

void date_scene_saki_spring(void) {
    s32 fake_padding; /* FAKE: never read; declared first so step_handlers lands at the original frame offset (T-3330) */
    DateYukariAndSakiSpringSteps step_handlers;

    step_handlers = date_scene_saki_spring_steps;
    if (game_state.mode_step < 8U && bg_brightness == 0x80) {
        hud_sprite_light_effect_draw();
    }
    step_handlers.callbacks[game_state.mode_step](0x80);
}

void date_scene_saki_spring_load_data(void) {
    cd_read_sectors(0x3D, 0x801B0000, 0x8C4B);
    date_scene_saki_spring_data_init();
    mode_step_next();
}

void date_scene_saki_spring_flag_group(void) {
    if (game_state.scene_flags[27] != 0) {
        D_800CA150 = (u16) D_800CA150 + 1;
        D_800CA154 = 0;
    }
    mode_step_next();
}

void date_scene_saki_summer(void) {
    s32 fake_padding; /* FAKE: never read; declared first so step_handlers lands at the original frame offset (T-3330) */
    FnTbl56 step_handlers;

    step_handlers = date_scene_saki_summer_steps;
    if (D_80122D04 == 0 && bg_brightness == 0x80) {
        hud_sprite_light_effect_draw();
    }
    step_handlers.callbacks[game_state.mode_step](0x80);
}

void date_scene_saki_summer_load_data(void) {
    cd_read_sectors(0x45, 0x801B0000, 0x8C88);
    date_scene_saki_summer_data_init();
    mode_step_next();
}

void date_scene_saki_autumn(void) {
    s32 step_index; /* declared before step_handlers: its stack slot sits above step_handlers (T-3330) */
    FnTbl43 step_handlers;

    step_handlers = date_scene_saki_autumn_steps;
    step_index = game_state.mode_step;
    step_handlers.callbacks[step_index](0x80);
}

void date_scene_saki_autumn_load_data(void) {
    cd_read_sectors(0x3D, 0x801B0000, 0x8CCD);
    date_scene_saki_autumn_data_init();
    mode_step_next();
}

void date_scene_saki_winter(void) {
    s32 step_index; /* declared before step_handlers: its stack slot sits above step_handlers (T-3330) */
    DateSakiWinterSteps step_handlers;

    step_handlers = date_scene_saki_winter_steps;
    step_index = game_state.mode_step;
    step_handlers.callbacks[step_index](0x80);
}

void date_scene_saki_winter_speak_as_saki(void) {
    game_state.current_girl = GIRL_SAKI;
    date_scene_speak();
}

void date_scene_saki_winter_speak_narration(void) {
    game_state.current_girl = GIRL_SOTOI;
    date_scene_speak();
}

void date_scene_saki_winter_load_data(void) {
    cd_read_sectors(0x3D, 0x801B0000, 0x8D0A);
    date_scene_saki_winter_data_init();
    mode_step_next();
}

void date_scene_saki_winter_narration_step(void) {
    game_state.current_girl = GIRL_SOTOI;
    D_80120666 = 4;
    mode_step_next();
}

void date_scene_saki_winter_back_to_saki(void) {
    game_state.current_girl = GIRL_SAKI;
    mode_step_next();
}
