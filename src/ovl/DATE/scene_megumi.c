/* scene_megumi.c: src/ovl/DATE/80157880.c:7: mode 0x5E scenes of Megumi (girl 9). Original object 0x80157880-0x801586A0 (config/objects/DATE.txt). */
#define MAIN_API_OVERRIDE_talk_work /* switched as u32: selector in $v1 (main_api.h: s32) */
#include "common.h"
#include "ovl/DATE.h"

extern u32 talk_work;

void date_scene_megumi_spring_data_init(void) {
    D_801600A0 = 0x801CE090;
    D_801600A4 = 0x801CE094;
    D_801600A8 = 0x801CE0AC;
    D_801600AC = *(s16 *)0x801CE0C0;
    D_801600B0 = 0x801B0000;
    D_801600B4 = 0x801B2000;
    D_801600B8 = 0x801B6000;
    D_801600BC = 0x801BA000;
    D_801600C0 = 0x801BE000;
    D_801600C4 = 0x801C2000;
    D_801600C8 = 0x801C6000;
}

void date_scene_megumi_summer_data_init(void) {
    D_801600CC = 0x801D2240;
    D_801600D0 = 0x801D2248;
    D_801600D4 = 0x801D2288;
    D_801600D8 = *(s16 *)0x801D22A0;
    D_801600DC = 0x801B0000;
    D_801600E0 = 0x801B2000;
    D_801600E4 = 0x801B6000;
    D_801600E8 = 0x801BA000;
    D_801600EC = 0x801BE000;
    D_801600F0 = 0x801C2000;
    D_801600F4 = 0x801C6000;
}

void date_scene_megumi_autumn_data_init(void) {
    D_801600F8 = 0x801CE078;
    D_801600FC = 0x801CE07C;
    D_80160100 = 0x801CE08C;
    D_80160104 = *(s16 *)0x801CE098;
    D_80160108 = 0x801B0000;
    D_8016010C = 0x801B2000;
    D_80160110 = 0x801B6000;
    D_80160114 = 0x801BA000;
    D_80160118 = 0x801BE000;
    D_8016011C = 0x801C2000;
    D_80160120 = 0x801C6000;
}

void date_scene_megumi_winter_data_init(void) {
    D_80160124 = 0x801CE0F4;
    D_80160128 = 0x801CE0F8;
    D_8016012C = 0x801CE118;
    D_80160130 = *(s16 *)0x801CE12C;
    D_80160134 = 0x801B0000;
    D_80160138 = 0x801B2000;
    D_8016013C = 0x801B6000;
    D_80160140 = 0x801BA000;
    D_80160144 = 0x801BE000;
    D_80160148 = 0x801C2000;
    D_8016014C = 0x801C6000;
}

void date_scene_megumi_dispatch(void) {
    switch (talk_work) {
    case 3:
        date_scene_megumi_spring();
        return;
    case 4:
        date_scene_megumi_summer();
        return;
    case 5:
        date_scene_megumi_autumn();
        return;
    case 6:
        date_scene_megumi_winter();
        return;
    default:
        game_mode_reset_to_title();
        return;
    }
}

void date_scene_megumi_spring_setup(void) {
    date_scene_text = D_8015DF30;
    date_scene_command = D_8015E06C;
    date_scene_voice = D_8015E1A8;
    strcpy(D_800CA19C, "遊園地");
    game_state.girls[9].tokimeki += 1;
    game_state.girls[9].affection += 1;
    game_state.girls[9].syoushin -= 0x14;
    check_para_limit();
    date_scene_megumi_spring_data_init();
    D_80122D20 = 1;
    game_state.current_girl = GIRL_SOTOI;
    load_palette(D_801600B0, 0x11, 1, 2, 0);
    scene_still_show(D_801600B4, D_801600B8, D_801600BC, D_801600C0, D_801600C4, D_801600C8);
    scene_anim_show(D_801600A4, D_801600A8, D_801600A0, (s32) D_801600AC);
    D_800CA360 = 1;
    D_800CA224[0] = 3;
    D_800CA224[1] = 3;
    D_800CA224[2] = 3;
    D_800CA234[0] = 2;
    D_800CA234[1] = 2;
    D_800CA234[2] = 2;
    mode_step_next();
}

void date_scene_megumi_summer_setup(void) {
    date_scene_text = D_8015DF34;
    date_scene_command = D_8015E070;
    date_scene_voice = D_8015E1AC;
    game_state.girls[9].tokimeki += 2;
    game_state.girls[9].affection += 1;
    game_state.girls[9].syoushin -= 0x14;
    check_para_limit();
    date_scene_megumi_summer_data_init();
    load_palette(D_801600DC, 0x11, 1, 2, 0);
    scene_still_show(D_801600E0, D_801600E4, D_801600E8, D_801600EC, D_801600F0, D_801600F4);
    scene_anim_show(D_801600D0, D_801600D4, D_801600CC, (s32) D_801600D8);
    D_800CA360 = 1;
    mode_step_next();
}

void date_scene_megumi_autumn_setup(void) {
    date_scene_text = D_8015DF38;
    date_scene_command = D_8015E074;
    date_scene_voice = D_8015E1B0;
    game_state.girls[9].tokimeki += 1;
    game_state.girls[9].affection += 1;
    game_state.girls[9].syoushin -= 0x14;
    check_para_limit();
    date_scene_megumi_autumn_data_init();
    load_palette(D_80160108, 0x11, 1, 2, 0);
    scene_still_show(D_8016010C, D_80160110, D_80160114, D_80160118, D_8016011C, D_80160120);
    scene_anim_show(D_801600FC, D_80160100, D_801600F8, (s32) D_80160104);
    D_800CA224[0] = 2;
    D_800CA224[1] = 2;
    D_800CA224[2] = 2;
    D_800CA234[0] = 1;
    D_800CA234[1] = 1;
    D_800CA234[2] = 1;
    mode_step_next();
}

void date_scene_megumi_winter_setup(void) {
    date_scene_text = D_8015DF3C;
    date_scene_command = D_8015E078;
    date_scene_voice = D_8015E1B4;
    game_state.girls[9].tokimeki += 3;
    game_state.girls[9].affection += 2;
    game_state.girls[9].syoushin -= 0x14;
    check_para_limit();
    date_scene_megumi_winter_data_init();
    load_palette(D_80160134, 0x11, 1, 2, 0);
    scene_still_show(D_80160138, D_8016013C, D_80160140, D_80160144, D_80160148, D_8016014C);
    scene_anim_show(D_80160128, D_8016012C, D_80160124, D_80160130);
    mode_step_next();
}

void date_scene_megumi_spring_background_current(void) {
    game_state.current_girl = GIRL_MEGUMI;
    bg_read_sub2(0x4818);
    mode_step_next();
}

void date_scene_megumi_spring_audio_command_background_1(void) {
    audio_command(0x200);
    bg_read_sub2(0x432B);
    mode_step_next();
}

void date_scene_megumi_summer_background_1(void) {
    bg_read_sub2(0x47F1);
    mode_step_next();
}

void date_scene_megumi_autumn_background_1(void) {
    bg_read_sub2(0x48AC);
    mode_step_next();
}

void date_scene_megumi_autumn_background_2(void) {
    bg_read_sub2(0x457D);
    mode_step_next();
}

void date_scene_megumi_winter_background_1(void) {
    bg_read_sub2(0x456C);
    mode_step_next();
}

void date_scene_megumi_spring(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl47 tbl;

    tbl = date_scene_megumi_spring_steps;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
}

void date_scene_megumi_spring_speak_unhidden(void) {
    game_state.current_girl = GIRL_MEGUMI;
    D_80122D20 = 0;
    date_scene_speak();
}

void date_scene_megumi_spring_speak_narration(void) {
    if ((u16) D_800CA154 >= 4U) {
        game_state.current_girl = GIRL_SOTOI;
        D_80122D10 = 2;
    }
    date_scene_speak();
}

void date_scene_megumi_spring_load_data(void) {
    cd_read_sectors(0x3D, 0x801B0000, 0x8ACD);
    date_scene_megumi_spring_data_init();
    mode_step_next();
}

void date_scene_megumi_summer(void) {
    GameState *g = &game_state; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl63 tbl;

    tbl = date_scene_megumi_summer_steps;
    if (((g->year == 0x5F && g->month == 7 && g->day == 0x17) ||
         (g->year == 0x60 && g->month == 7 && g->day == 0x1C) ||
         (g->year == 0x61 && g->month == 7 && g->day == 0x1B)) &&
        dec_bg_brightness == 0x80 && D_80122D04 == 0) {
        hud_sprite_light_effect_draw();
    }
    tbl.f[g->mode_step](0x80);
}

void date_scene_megumi_summer_load_data(void) {
    cd_read_sectors(0x45, 0x801B0000, 0x8B0A);
    date_scene_megumi_summer_data_init();
    mode_step_next();
}

void date_scene_megumi_summer_audio_command_1(void) {
    audio_command(0x501);
    mode_step_next();
}

void date_scene_megumi_summer_audio_command_2(void) {
    audio_command(0x503);
    mode_step_next();
}

void date_scene_megumi_summer_audio_command_3(void) {
    audio_command(0x504);
    mode_step_next();
}

void date_scene_megumi_summer_audio_command_4(void) {
    audio_command(0x505);
    mode_step_next();
}

void date_scene_megumi_summer_audio_command_5(void) {
    audio_command(0x506);
    mode_step_next();
}

void date_scene_megumi_autumn(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl42 tbl;

    tbl = date_scene_megumi_autumn_steps;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
}

void date_scene_megumi_autumn_load_data(void) {
    cd_read_sectors(0x3D, 0x801B0000, 0x8B4F);
    date_scene_megumi_autumn_data_init();
    mode_step_next();
}

void date_scene_megumi_winter(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl36 tbl;

    tbl = date_scene_megumi_winter_steps;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
}

void date_scene_megumi_winter_load_data(void) {
    cd_read_sectors(0x3DU, 0x801B0000, 0x8B8C);
    date_scene_megumi_winter_data_init();
    mode_step_next();
}
