/* scene_mira.c: src/ovl/DATE/80152FC0.c:4: mode 0x5E scenes of Mira (girl 7). Original object 0x80152FC0-0x80154260 (config/objects/DATE.txt). */
#include "common.h"
#include "ovl/DATE.h"

void date_scene_mira_spring_data_init(void) {
    D_8015F160 = 0x801CE124;
    D_8015F164 = 0x801CE128;
    D_8015F168 = 0x801CE148;
    D_8015F16C = *(s16 *)0x801CE15C;
    D_8015F170 = 0x801B0000;
    D_8015F174 = 0x801B2000;
    D_8015F178 = 0x801B6000;
    D_8015F17C = 0x801BA000;
    D_8015F180 = 0x801BE000;
    D_8015F184 = 0x801C2000;
    D_8015F188 = 0x801C6000;
}

void date_scene_mira_summer_data_init(void) {
    D_8015F18C = 0x801CE0F4;
    D_8015F190 = 0x801CE0F8;
    D_8015F194 = 0x801CE118;
    D_8015F198 = *(s16 *)0x801CE12C;
    D_8015F19C = 0x801B0000;
    D_8015F1A0 = 0x801B2000;
    D_8015F1A4 = 0x801B6000;
    D_8015F1A8 = 0x801BA000;
    D_8015F1AC = 0x801BE000;
    D_8015F1B0 = 0x801C2000;
    D_8015F1B4 = 0x801C6000;
}

void date_scene_mira_autumn_data_init(void) {
    D_8015F1B8 = (u8 *)0x801CE088;
    D_8015F1BC = (u8 *)0x801CE08C;
    D_8015F1C0 = (u8 *)0x801CE0A0;
    D_8015F1C4 = *(s16 *)0x801CE0B4;
    D_8015F1C8 = (u8 *)0x801B0000;
    D_8015F1CC = (u8 *)0x801B2000;
    D_8015F1D0 = (u8 *)0x801B6000;
    D_8015F1D4 = (u8 *)0x801BA000;
    D_8015F1D8 = (u8 *)0x801BE000;
    D_8015F1DC = (u8 *)0x801C2000;
    D_8015F1E0 = (u8 *)0x801C6000;
}

void date_scene_mira_winter_data_init(void) {
    D_8015F1E4 = 0x801CE0F4;
    D_8015F1E8 = 0x801CE0F8;
    D_8015F1EC = 0x801CE118;
    D_8015F1F0 = *(s16 *)0x801CE12C;
    D_8015F1F4 = 0x801B0000;
    D_8015F1F8 = 0x801B2000;
    D_8015F1FC = 0x801B6000;
    D_8015F200 = 0x801BA000;
    D_8015F204 = 0x801BE000;
    D_8015F208 = 0x801C2000;
    D_8015F20C = 0x801C6000;
}

void date_scene_mira_dispatch(void) {
    switch (talk_work) {
    case 2:
        date_scene_mira_spring();
        break;
    case 3:
        date_scene_mira_summer();
        break;
    case 4:
        date_scene_mira_autumn();
        break;
    case 5:
        date_scene_mira_winter();
        break;
    case 6:
        date_scene_mira_special();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void date_scene_mira_spring_setup(void) {
    date_scene_text = D_8015DECC;
    date_scene_command = D_8015E008;
    date_scene_voice = D_8015E144;
    D_800CA21C[0] = 1;
    D_800CA21C[2] = 2;
    D_800CA21C[1] = 4;
    D_800CA224[0] = 1;
    D_800CA224[2] = 2;
    D_800CA224[1] = 3;
    D_800CA22C[0] = 0;
    D_800CA22C[2] = 2;
    D_800CA22C[1] = 4;
    D_800CA234[0] = 0;
    D_800CA234[2] = 1;
    D_800CA234[1] = 2;
    date_scene_mira_spring_data_init();
    load_palette(D_8015F170, 0x11, 1, 2, 0);
    scene_still_show(D_8015F174, D_8015F178, D_8015F17C, D_8015F180, D_8015F184, D_8015F188);
    scene_anim_show(D_8015F164, D_8015F168, D_8015F160, D_8015F16C);
    mode_step_next();
}

void date_scene_mira_summer_setup(void) {
    date_scene_text = D_8015DED0;
    date_scene_command = D_8015E00C;
    date_scene_voice = D_8015E148;
    game_state.girls[7].tokimeki += 1;
    game_state.girls[7].affection += 1;
    game_state.girls[7].syoushin -= 0x14;
    check_para_limit();
    date_scene_mira_summer_data_init();
    load_palette(D_8015F19C, 0x11, 1, 2, 0);
    scene_still_show(D_8015F1A0, D_8015F1A4, D_8015F1A8, D_8015F1AC, D_8015F1B0, D_8015F1B4);
    scene_anim_show(D_8015F190, D_8015F194, D_8015F18C, D_8015F198);
    D_800CA224[0] = 3;
    D_800CA224[1] = 3;
    D_800CA224[2] = 3;
    D_800CA234[0] = 2;
    D_800CA234[1] = 2;
    D_800CA234[2] = 2;
    mode_step_next();
}

void date_scene_mira_autumn_setup(void) {
    date_ride_index = DATE_RIDE_GHOST_HOUSE;
    date_scene_text = D_8015DED4;
    date_scene_command = D_8015E010;
    date_scene_voice = D_8015E14C;
    strcpy(scene_place_title, "遊園地");
    strcpy(scene_place_title, "ゴーストハウス");
    D_800CA21C[0] = 2;
    D_800CA21C[1] = 2;
    D_800CA21C[2] = 2;
    D_800CA224[0] = 2;
    D_800CA224[1] = 2;
    D_800CA224[2] = 2;
    D_800CA22C[0] = 2;
    D_800CA22C[1] = 2;
    D_800CA22C[2] = 2;
    D_800CA234[0] = 1;
    D_800CA234[1] = 1;
    D_800CA234[2] = 1;
    date_scene_mira_autumn_data_init();
    load_palette((s32) D_8015F1C8, 0x11, 1, 2, 0);
    scene_still_show((s32) D_8015F1CC, (s32) D_8015F1D0, (s32) D_8015F1D4, (s32) D_8015F1D8, (s32) D_8015F1DC, (s32) D_8015F1E0);
    scene_anim_show((s32) D_8015F1BC, (s32) D_8015F1C0, (s32) D_8015F1B8, (s32) D_8015F1C4);
    scene_mouth_frames_reversed_saved = 1;
    D_800CA224[0] = 3;
    D_800CA224[1] = 3;
    D_800CA224[2] = 3;
    D_800CA234[0] = 2;
    D_800CA234[1] = 2;
    D_800CA234[2] = 2;
    mode_step_next();
}

void date_scene_mira_winter_setup(void) {
    date_scene_text = D_8015DED8;
    date_scene_command = D_8015E014;
    date_scene_voice = D_8015E150;
    game_state.girls[7].tokimeki += 3;
    game_state.girls[7].affection += 2;
    game_state.girls[7].syoushin -= 0x14;
    check_para_limit();
    date_scene_mira_winter_data_init();
    load_palette(D_8015F1F4, 0x11, 1, 2, 0);
    scene_still_show(D_8015F1F8, D_8015F1FC, D_8015F200, D_8015F204, D_8015F208, D_8015F20C);
    scene_anim_show(D_8015F1E8, D_8015F1EC, D_8015F1E4, (s32) D_8015F1F0);
    scene_mouth_frames_reversed_saved = 1;
    D_800CA224[0] = 3;
    D_800CA224[1] = 3;
    D_800CA224[2] = 3;
    D_800CA234[0] = 2;
    D_800CA234[1] = 2;
    D_800CA234[2] = 2;
    mode_step_next();
}

void date_scene_mira_special_setup(void) {
    date_scene_text = D_8015DEDC;
    date_scene_command = D_8015E018;
    date_scene_voice = D_8015E154;
    game_state.girls[7].tokimeki += 3;
    game_state.girls[7].affection += 2;
    check_para_limit();
    mode_step_next();
}

void date_scene_mira_spring_background_1(void) {
    bg_read_sub2(0x486E);
    mode_step_next();
}

void date_scene_mira_spring_audio_command_background_1(void) {
    audio_command(0x203);
    bg_read_sub2(0x4520);
    mode_step_next();
}

void date_scene_mira_summer_background_1(void) {
    bg_read_sub2(0x48D6);
    mode_step_next();
}

void date_scene_mira_summer_background_2(void) {
    bg_read_sub2(0x45A3);
    mode_step_next();
}

void date_scene_mira_autumn_background_1(void) {
    bg_read_sub2(0x482A);
    mode_step_next();
}

void date_scene_mira_autumn_background_2(void) {
    bg_read_sub2(0x432B);
    mode_step_next();
}

void date_scene_mira_autumn_background_logged(void) {
    bg_read_sub2(0x4347);
    game_state.event_log_count -= 1;
    event_log_add(0, D_80122D08);
    mode_step_next();
}

void date_scene_mira_winter_background_1(void) {
    bg_read_sub2(0x4891);
    mode_step_next();
}

void date_scene_mira_winter_background_2(void) {
    bg_read_sub2(0x4558);
    mode_step_next();
}

void date_scene_mira_spring(void) {
    s32 step_index; /* declared before step_handlers: its stack slot sits above step_handlers (T-3330) */
    FnTbl41 step_handlers;

    step_handlers = date_scene_mira_spring_steps;
    step_index = game_state.mode_step;
    step_handlers.callbacks[step_index](0x80);
}

void date_scene_mira_spring_audio_command_2(void) {
    audio_command(0x500);
    mode_step_next();
}

void date_scene_mira_spring_load_data(void) {
    cd_read_sectors(0x3D, 0x801B0000, 0x847D);
    date_scene_mira_spring_data_init();
    mode_step_next();
}

void date_scene_mira_spring_answer_palette(void) {
    D_800CA150 = (u16) D_800CA150 + D_80122CDC;
    if (D_80122CDC == 0) {
        load_palette(D_8015F170, 0x11, 1, 2, 0);
        D_8015E204 = 0;
    } else if (D_80122CDC == 1) {
        load_palette(D_8015F170, 0x11, 1, 2, 2);
        D_8015E204 = 2;
    } else {
        load_palette(D_8015F170, 0x11, 1, 2, 1);
        D_8015E204 = 1;
    }
    mode_step_next();
}

void date_scene_mira_summer(void) {
    u8 step_index;
    DateMiraSummerSteps step_handlers;

    step_handlers = date_scene_mira_summer_steps;
    step_index = game_state.mode_step;
    if ((step_index >= 27 || step_index < 8) && bg_brightness == 0x80) {
        hud_sprite_light_effect_draw();
    }
    step_index = game_state.mode_step;
    step_handlers.callback[step_index](0x80);
}

void date_scene_mira_summer_group_by_mood(void) {
    u32 alternate_girl_state;

    alternate_girl_state = get_g_zyotai_h(GIRL_MIRA);
    D_800CA150 = (alternate_girl_state >= 1) + (alternate_girl_state >= 2) + (alternate_girl_state >= 3) + 1;
    date_scene_speak();
}

void date_scene_mira_summer_load_data(void) {
    cd_read_sectors(0x3D, 0x801B0000, 0x84BA);
    date_scene_mira_summer_data_init();
    mode_step_next();
}

void date_scene_mira_autumn(void) {
    s32 step_index; /* declared before step_handlers: its stack slot sits above step_handlers (T-3330) */
    DateMiraAutumnSteps step_handlers;

    step_handlers = date_scene_mira_autumn_steps;
    step_index = game_state.mode_step;
    step_handlers.callbacks[step_index](0x80);
}

void date_scene_mira_autumn_answer_branch(void) {
    if (D_80122CDC != 0) {
        D_800CA150 = 6;
        mode_step_set(0x24);
        return;
    }
    game_state.scene_flags[0x31] = 1;
    game_state.girls[7].affection -= 1;
    game_state.girls[7].syoushin += 0xA;
    check_para_limit();
    mode_step_next();
}

void date_scene_mira_autumn_speak_with_face(void) {
    date_scene_speak();
    if (game_state.mode_step_timer.w == 1) {
        if ((u16) D_800CA154 == 7) {
            portrait_layout_set(1);
        }
    }
}

void date_scene_mira_autumn_audio_command_1(void) {
    audio_command(0x200);
    mode_step_next();
}

void date_scene_mira_autumn_bgm_speak(void) {
    if (game_state.substep == 0) {
        audio_command(0x205);
        game_state.substep += 1;
    }
    date_scene_speak();
}

void date_scene_mira_autumn_load_data(void) {
    cd_read_sectors(0x3D, 0x801B0000, 0x84F7);
    date_scene_mira_autumn_data_init();
    mode_step_next();
}

void date_scene_mira_winter(void) {
    s32 step_index;
    DateMiraWinterSteps step_handlers;
    step_handlers = date_scene_mira_winter_steps;
    if (game_state.mode_step < 9) {
        hud_sprite_light_effect_draw();
    }
    if ((game_state.mode_step >= 0x16) && (game_state.mode_step < 0x1C)) {
        hud_sprite_light_effect_draw();
    }
    step_index = game_state.mode_step;
    step_handlers.callbacks[step_index](0x80);
}

void date_scene_mira_winter_audio_command_1(void) {
    audio_command(0x502);
    mode_step_next();
}

void date_scene_mira_winter_load_data(void) {
    cd_read_sectors(0x3D, 0x801B0000, 0x8534);
    date_scene_mira_winter_data_init();
    mode_step_next();
}

/* FAKE: D_800CA2F8 is read through its DateSelectionView; a direct read of the u8 keeps cvt_pass's
 * widening CVT, whose CSE temporary (two compares) costs a 4-byte frame slot the original does not
 * have (0x70 against 0x68). Same words otherwise. T-17203 */
void date_scene_mira_special(void) {
    GameState *game = &game_state; /* declared before step_handlers: its stack slot sits above step_handlers (T-3330) */
    FnTbl15 step_handlers;

    step_handlers = date_scene_mira_special_steps;
    if (game->mode_step < 0xC) {
        if (((game->year == 0x5F && game->month == 7 && game->day == 0x17) ||
             (game->year == 0x60 && game->month == 7 && game->day == 0x1C) ||
             (game->year == 0x61 && game->month == 7 && game->day == 0x1B)) &&
            bg_brightness == 0x80) {
            hud_sprite_light_effect_draw();
        } else if (((DateSelectionView *)&D_800CA2F8)->place == 0x11 ||
                   ((DateSelectionView *)&D_800CA2F8)->place == 0x10) {
            hud_sprite_light_effect_draw();
        }
    }
    step_handlers.callbacks[game->mode_step](0x80);
}
