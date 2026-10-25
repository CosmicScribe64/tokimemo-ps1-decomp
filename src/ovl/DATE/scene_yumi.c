/* scene_yumi.c: src/ovl/DATE/801594B0.c:7: mode 0x5E scenes of Yumi (girl 10). Original object 0x801594B0-0x8015A190 (config/objects/DATE.txt). */
#define MAIN_API_OVERRIDE_talk_work /* switched as u32: selector in $v1 (main_api.h: s32) */
#include "common.h"
#include "ovl/DATE.h"

extern u32 talk_work;

void date_scene_yumi_spring_data_init(void) {
    D_801607E0 = 0x801CE0F4;
    D_801607E4 = 0x801CE0F8;
    D_801607E8 = 0x801CE118;
    D_801607EC = *(s16 *)0x801CE12C;
    D_801607F0 = 0x801B0000;
    D_801607F4 = 0x801B2000;
    D_801607F8 = 0x801B6000;
    D_801607FC = 0x801BA000;
    D_80160800 = 0x801BE000;
    D_80160804 = 0x801C2000;
    D_80160808 = 0x801C6000;
}

void date_scene_yumi_summer_data_init(void) {
    D_8016080C = 0x801CE0F4;
    D_80160810 = 0x801CE0F8;
    D_80160814 = 0x801CE118;
    D_80160818 = *(s16 *)0x801CE12C;
    D_8016081C = 0x801B0000;
    D_80160820 = 0x801B2000;
    D_80160824 = 0x801B6000;
    D_80160828 = 0x801BA000;
    D_8016082C = 0x801BE000;
    D_80160830 = 0x801C2000;
    D_80160834 = 0x801C6000;
}

void date_scene_yumi_autumn_data_init(void) {
    D_80160838 = 0x801CE0F4;
    D_8016083C = 0x801CE0F8;
    D_80160840 = 0x801CE118;
    D_80160844 = *(s16 *)0x801CE12C;
    D_80160848 = 0x801B0000;
    D_8016084C = 0x801B2000;
    D_80160850 = 0x801B6000;
    D_80160854 = 0x801BA000;
    D_80160858 = 0x801BE000;
    D_8016085C = 0x801C2000;
    D_80160860 = 0x801C6000;
}

void date_scene_yumi_winter_data_init(void) {
    D_80160864 = 0x801D2470;
    D_80160868 = 0x801D2478;
    D_8016086C = 0x801D24B8;
    D_80160870 = *(s16 *)0x801D24D0;
    D_80160874 = 0x801B0000;
    D_80160878 = 0x801B2000;
    D_8016087C = 0x801B6000;
    D_80160880 = 0x801BA000;
    D_80160884 = 0x801BE000;
    D_80160888 = 0x801C2000;
    D_8016088C = 0x801C6000;
}

void date_scene_yumi_dispatch(void) {
    switch (talk_work) {
    case 3:
        date_scene_yumi_spring();
        return;
    case 4:
        date_scene_yumi_summer();
        return;
    case 5:
        date_scene_yumi_autumn();
        return;
    case 6:
        date_scene_yumi_winter();
        return;
    default:
        game_mode_reset_to_title();
        return;
    }
}

void date_scene_yumi_spring_setup(void) {
    date_scene_text = D_8015DF64;
    date_scene_command = D_8015E0A0;
    date_scene_voice = D_8015E1DC;
    game_state.girls[10].tokimeki += 1;
    game_state.girls[10].affection += 1;
    game_state.girls[10].syoushin -= 0xA;
    check_para_limit();
    date_scene_yumi_spring_data_init();
    load_palette(D_801607F0, 0x11, 1, 2, 0);
    scene_still_show(D_801607F4, D_801607F8, D_801607FC, D_80160800, D_80160804, D_80160808);
    scene_anim_show(D_801607E4, D_801607E8, D_801607E0, D_801607EC);
    D_800CA360 = 1;
    D_800CA224[0] = 2;
    D_800CA224[1] = 2;
    D_800CA224[2] = 2;
    D_800CA234[0] = 1;
    D_800CA234[1] = 1;
    D_800CA234[2] = 1;
    mode_step_next();
}

void date_scene_yumi_summer_setup(void) {
    date_scene_text = D_8015DF68;
    date_scene_command = D_8015E0A4;
    date_scene_voice = D_8015E1E0;
    strcpy(D_800CA1DC, "池");
    game_state.girls[10].tokimeki += 3;
    game_state.girls[10].affection += 2;
    game_state.girls[10].syoushin -= 0x14;
    check_para_limit();
    date_scene_yumi_summer_data_init();
    load_palette(D_8016081C, 0x11, 1, 2, 0);
    scene_still_show(D_80160820, D_80160824, D_80160828, D_8016082C, D_80160830, D_80160834);
    scene_anim_show(D_80160810, D_80160814, D_8016080C, D_80160818);
    mode_step_next();
}

void date_scene_yumi_autumn_setup(void) {
    date_scene_text = D_8015DF6C;
    date_scene_command = D_8015E0A8;
    date_scene_voice = D_8015E1E4;
    game_state.girls[10].affection += 2;
    game_state.girls[10].syoushin -= 0xA;
    check_para_limit();
    date_scene_yumi_autumn_data_init();
    load_palette(D_80160848, 0x11, 1, 2, 0);
    scene_still_show(D_8016084C, D_80160850, D_80160854, D_80160858, D_8016085C, D_80160860);
    scene_anim_show(D_8016083C, D_80160840, D_80160838, D_80160844);
    D_800CA360 = 1;
    mode_step_next();
}

void date_scene_yumi_winter_setup(void) {
    date_scene_text = D_8015DF70;
    date_scene_command = D_8015E0AC;
    date_scene_voice = D_8015E1E8;
    game_state.girls[10].tokimeki += 2;
    game_state.girls[10].affection += 1;
    game_state.girls[10].syoushin -= 0x14;
    check_para_limit();
    date_scene_yumi_winter_data_init();
    load_palette(D_80160874, 0x11, 1, 2, 0);
    scene_still_show(D_80160878, D_8016087C, D_80160880, D_80160884, D_80160888, D_8016088C);
    scene_anim_show(D_80160868, D_8016086C, D_80160864, D_80160870);
    D_800CA360 = 1;
    D_800CA224[0] = 2;
    D_800CA224[1] = 2;
    D_800CA224[2] = 2;
    D_800CA234[0] = 1;
    D_800CA234[1] = 1;
    D_800CA234[2] = 1;
    mode_step_next();
}

void date_scene_yumi_spring_background_1(void) {
    bg_read_sub2(0x4821);
    mode_step_next();
}

void date_scene_yumi_spring_background_2(void) {
    bg_read_sub2(0x432B);
    mode_step_next();
}

void date_scene_yumi_spring_background_3(void) {
    bg_read_sub2(0x4335);
    mode_step_next();
}

void date_scene_yumi_summer_background_1(void) {
    bg_read_sub2(0x42FD);
    mode_step_next();
}

void date_scene_yumi_summer_background_2(void) {
    bg_read_sub2(0x47E8);
    mode_step_next();
}

void date_scene_yumi_summer_background_3(void) {
    bg_read_sub2(0x42EB);
    mode_step_next();
}

void date_scene_yumi_autumn_background_1(void) {
    bg_read_sub2(0x4913);
    mode_step_next();
}

void date_scene_yumi_autumn_background_2(void) {
    bg_read_sub2(0x45F8);
    mode_step_next();
}

void date_scene_yumi_winter_background_1(void) {
    bg_read_sub2(0x489A);
    mode_step_next();
}

void date_scene_yumi_winter_background_2(void) {
    bg_read_sub2(0x4562);
    mode_step_next();
}

void date_scene_yumi_winter_background_3(void) {
    bg_read_sub2(0x4558);
    mode_step_next();
}

void date_scene_yumi_spring(void) {
    s32 step_index; /* declared before step_handlers: its stack slot sits above step_handlers (T-3330) */
    FnTbl44 step_handlers;

    step_handlers = date_scene_yumi_spring_steps;
    step_index = game_state.mode_step;
    step_handlers.callbacks[step_index](0x80);
}

void date_scene_yumi_spring_audio_command_1(void) {
    audio_command(0x508);
    mode_step_next();
}

void date_scene_yumi_spring_load_data(void) {
    cd_read_sectors(0x3D, 0x801B0000, 0x8E02);
    date_scene_yumi_spring_data_init();
    mode_step_next();
}

void date_scene_yumi_summer(void) {
    GameState *game = &game_state; /* declared before step_handlers: its stack slot sits above step_handlers (T-3330) */
    DateYumiSummerSteps step_handlers;

    step_handlers = date_scene_yumi_summer_steps;
    if (((game->year == 0x5F && game->month == 7 && game->day == 0x17) ||
         (game->year == 0x60 && game->month == 7 && game->day == 0x1C) ||
         (game->year == 0x61 && game->month == 7 && game->day == 0x1B)) &&
        dec_bg_brightness == 0x80 && D_80122D04 == 0 && (game->mode_step < 0xD || game->mode_step >= 0x21)) {
        hud_sprite_light_effect_draw();
    }
    step_handlers.callbacks[game->mode_step](0x80);
    if (D_80122D04 != 0 && game->frame_count % 150 == 0) {
        date_scene_yumi_summer_audio_command_1();
    }
}

s32 date_scene_yumi_autumn_speak_with_face(void) {
    date_scene_speak();
    if (game_state.mode_step_timer.w++ == 1) {
        if ((u16)D_800CA154 == 3) {
            portrait_layout_set(1);
        }
    }
}

void date_scene_yumi_summer_audio_command_1(void) {
    audio_command(0x500);
}
