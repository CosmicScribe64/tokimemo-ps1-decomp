/* scene_mio.c: src/ovl/DATE/80154260.c:7: mode 0x5E scenes of Mio (girl 1). Original object 0x80154260-0x801552B0 (config/objects/DATE.txt). */
#define MAIN_API_OVERRIDE_talk_work /* switched as u32: selector in $v1 (main_api.h: s32) */
#include "common.h"
#include "ovl/DATE.h"

extern u32 talk_work;

void date_scene_mio_spring_data_init(void) {
    D_8015F520 = 0x801D2240;
    D_8015F524 = 0x801D2248;
    D_8015F528 = 0x801D2288;
    D_8015F52C = *(s16 *)0x801D22A0;
    D_8015F530 = 0x801B0000;
    D_8015F534 = 0x801B2000;
    D_8015F538 = 0x801B6000;
    D_8015F53C = 0x801BA000;
    D_8015F540 = 0x801BE000;
    D_8015F544 = 0x801C2000;
    D_8015F548 = 0x801C6000;
}

void date_scene_mio_summer_data_init(void) {
    D_8015F54C = 0x801D21E0;
    D_8015F550 = 0x801D21E8;
    D_8015F554 = 0x801D2228;
    D_8015F558 = *(s16 *)0x801D2240;
    D_8015F55C = 0x801B0000;
    D_8015F560 = 0x801B2000;
    D_8015F564 = 0x801B6000;
    D_8015F568 = 0x801BA000;
    D_8015F56C = 0x801BE000;
    D_8015F570 = 0x801C2000;
    D_8015F574 = 0x801C6000;
}

void date_scene_mio_autumn_data_init(void) {
    D_8015F578 = 0x801CE078;
    D_8015F57C = 0x801CE07C;
    D_8015F580 = 0x801CE08C;
    D_8015F584 = *(s16 *)0x801CE098;
    D_8015F588 = 0x801B0000;
    D_8015F58C = 0x801B2000;
    D_8015F590 = 0x801B6000;
    D_8015F594 = 0x801BA000;
    D_8015F598 = 0x801BE000;
    D_8015F59C = 0x801C2000;
    D_8015F5A0 = 0x801C6000;
}

void date_scene_mio_winter_data_init(void) {
    D_8015F5A4 = 0x801CE148;
    D_8015F5A8 = 0x801CE14C;
    D_8015F5AC = 0x801CE170;
    D_8015F5B0 = *(s16 *)0x801CE184;
    D_8015F5B4 = 0x801B0000;
    D_8015F5B8 = 0x801B2000;
    D_8015F5BC = 0x801B6000;
    D_8015F5C0 = 0x801BA000;
    D_8015F5C4 = 0x801BE000;
    D_8015F5C8 = 0x801C2000;
    D_8015F5CC = 0x801C6000;
}

void date_scene_mio_dispatch(void) {
    switch (talk_work) {
    case 2:
        date_scene_mio_spring();
        return;
    case 3:
        date_scene_mio_summer();
        return;
    case 4:
        date_scene_mio_autumn();
        return;
    case 5:
        date_scene_mio_winter();
        return;
    default:
        game_mode_reset_to_title();
        return;
    }
}

void date_scene_mio_spring_ride_select(void) {
    date_ride_index = DATE_RIDE_SCREAM_MACHINE;
    date_scene_text = D_8015DEE8;
    date_scene_command = D_8015E024;
    date_scene_voice = D_8015E160;
    mode_step_next();
}

void date_scene_mio_spring_still_setup(void) {
    date_scene_mio_spring_data_init();
    load_palette(D_8015F530, 0x11, 1, 2, 0);
    scene_still_show(D_8015F534, D_8015F538, D_8015F53C, D_8015F540, D_8015F544, D_8015F548);
    scene_anim_show(D_8015F524, D_8015F528, D_8015F520, D_8015F52C);
    scene_mouth_frames_reversed_saved = 1;
    mode_step_next();
}

void date_scene_mio_summer_setup(void) {
    date_scene_text = D_8015DEEC;
    date_scene_command = D_8015E028;
    date_scene_voice = D_8015E164;
    date_scene_mio_summer_data_init();
    load_palette(D_8015F55C, 0x11, 1, 2, 0);
    scene_still_show(D_8015F560, D_8015F564, D_8015F568, D_8015F56C, D_8015F570, D_8015F574);
    scene_anim_show(D_8015F550, D_8015F554, D_8015F54C, (s32) D_8015F558);
    game_state.girls[1].tokimeki += 1;
    game_state.girls[1].affection += 2;
    game_state.girls[1].syoushin -= 0x14;
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

void date_scene_mio_autumn_setup(void) {
    date_scene_text = D_8015DEF0;
    date_scene_command = D_8015E02C;
    date_scene_voice = D_8015E168;
    date_scene_mio_autumn_data_init();
    load_palette(D_8015F588, 0x11, 1, 2, 0);
    scene_still_show(D_8015F58C, D_8015F590, D_8015F594, D_8015F598, D_8015F59C, D_8015F5A0);
    scene_anim_show(D_8015F57C, D_8015F580, D_8015F578, D_8015F584);
    D_80120688 |= 0x80000000;
    D_80120666 = 0;
    game_state.girls[1].tokimeki += 2;
    game_state.girls[1].affection += 2;
    game_state.girls[1].syoushin -= 0x14;
    check_para_limit();
    mode_step_next();
}

void date_scene_mio_winter_setup(void) {
    date_scene_text = D_8015DEF4;
    date_scene_command = D_8015E030;
    date_scene_voice = D_8015E16C;
    load_palette(D_8015F5B4, 0x11, 1, 2, 0);
    load_palette(D_8015F5B4, 0x12, 1, 2, 1);
    scene_still_show(D_8015F5B8, D_8015F5BC, D_8015F5C0, D_8015F5C4, D_8015F5C8, D_8015F5CC);
    scene_anim_show(D_8015F5A8, D_8015F5AC, D_8015F5A4, (s32) D_8015F5B0);
    game_state.girls[1].tokimeki += 3;
    game_state.girls[1].affection += 2;
    game_state.girls[1].syoushin -= 0x14;
    check_para_limit();
    D_8015F5D0 = 0;
    mode_step_next();
}

void date_scene_mio_spring_background_1(void) {
    bg_read_sub2(0x4359);
    mode_step_next();
}

void date_scene_mio_spring_background_2(void) {
    bg_read_sub2(0x4809);
    mode_step_next();
}

void date_scene_mio_spring_background_3(void) {
    bg_read_sub2(0x432B);
    mode_step_next();
}

void date_scene_mio_summer_background_1(void) {
    bg_read_sub2(0x48BD);
    mode_step_next();
}

void date_scene_mio_summer_background_2(void) {
    bg_read_sub2(0x45A3);
    mode_step_next();
}

void date_scene_mio_background_unused(void) {
    bg_read_sub2(0x454E);
    mode_step_next();
}

void date_scene_mio_autumn_background_1(void) {
    bg_read_sub2(0x4876);
    mode_step_next();
}

void date_scene_mio_autumn_background_2(void) {
    bg_read_sub2(0x4544);
    mode_step_next();
}

void date_scene_mio_winter_background_1(void) {
    bg_read_sub2(0x4833);
    mode_step_next();
}

void date_scene_mio_winter_background_2(void) {
    bg_read_sub2(0x43F9);
    mode_step_next();
}

void date_scene_mio_winter_audio_command_background_1(void) {
    audio_command(0x500);
    bg_read_sub2(0x43F0);
    mode_step_next();
}

void date_scene_mio_spring(void) {
    s32 step_index; /* declared before step_handlers: its stack slot sits above step_handlers (T-3330) */
    DateMioSpringSteps step_handlers;

    step_handlers = date_scene_mio_spring_steps;
    step_index = game_state.mode_step;
    step_handlers.callbacks[step_index](0x80);
}

void date_scene_mio_spring_load_data(void) {
    cd_read_sectors(0x45, 0x801B0000, 0x8664);
    date_scene_mio_spring_data_init();
    mode_step_next();
}

void date_scene_mio_spring_answer_branch(void) {
    if (D_80122CDC == 1) {
        portrait_layout_set(2);
        mode_step_set(0x2E);
        return;
    }
    game_state.scene_flags[9] = 1;
    D_800CA224[0] = 3;
    D_800CA224[1] = 3;
    D_800CA224[2] = 3;
    D_800CA234[0] = 2;
    D_800CA234[1] = 2;
    D_800CA234[2] = 2;
    game_state.girls[1].tokimeki -= 2;
    game_state.girls[1].affection -= 1;
    game_state.girls[1].syoushin += 0x14;
    check_para_limit();
    mode_step_next();
}

void date_scene_mio_spring_scream_machine_title(void) {
    strcpy(scene_place_title_2, "絶叫マシーンビビール");
    mode_step_next();
}

void date_scene_mio_summer(void) {
    s32 step_index; /* declared before step_handlers: its stack slot sits above step_handlers (T-3330) */
    FnTbl42 step_handlers;

    step_handlers = date_scene_mio_summer_steps;
    if (game_state.mode_step >= 0xF && game_state.mode_step < 0x12) {
        date_scene_light_beams();
    }
    if (D_80122D04 == 0 && bg_brightness == 0x80) {
        hud_sprite_light_effect_draw();
    }
    step_index = game_state.mode_step;
    step_handlers.callbacks[step_index](0x80);
}

void date_scene_mio_summer_load_data(void) {
    cd_read_sectors(0x45, 0x801B0000, 0x86A9);
    date_scene_mio_summer_data_init();
    mode_step_next();
}

void date_scene_mio_autumn(void) {
    s32 step_index; /* declared before step_handlers: its stack slot sits above step_handlers (T-3330) */
    FnTbl41 step_handlers;

    step_handlers = date_scene_mio_autumn_steps;
    step_index = game_state.mode_step;
    step_handlers.callbacks[step_index](0x80);
}

void date_scene_mio_autumn_load_data(void) {
    cd_read_sectors(0x3D, 0x801B0000, 0x86EE);
    date_scene_mio_autumn_data_init();
    mode_step_next();
}

void date_scene_mio_winter(void) {
    s32 step_index; /* declared before step_handlers with i: two words above step_handlers (T-3330) */
    s16 i;
    DateMioWinterSteps step_handlers;

    step_handlers = date_scene_mio_winter_steps;
    step_index = game_state.mode_step;
    step_handlers.callbacks[step_index](0x80);
    if (game_state.mode_step == 0xE) {
        if (game_state.pad_pressed & 0xFF9FFF9F) {
            D_8015F5D0 = 1 - (u8) D_8015F5D0;
        }
        if ((u32) game_state.frame_count % 3U == 0) {
            if ((u8) D_8015F5D0 != 0) {
                bg_brightness = 0x80;
            } else if (bg_brightness == 0x80) {
                bg_brightness = 0x60;
            } else {
                bg_brightness = 0x80;
            }
            ((Actor *)actor_pool)[96].brightness = bg_brightness;
            ((Actor *)actor_pool)[97].brightness = bg_brightness;
            /* One chain assignment for the three bytes: separate stores give other registers (T-20600). */
            for (i = 0x3A; i < 0x40; i++) {
                sprite_quads[i].tint_r = sprite_quads[i].tint_g = sprite_quads[i].tint_b = bg_brightness;
            }
        }
    }
}

void date_scene_mio_winter_narration_speak(void) {
    if (game_state.mode_step_timer.w++ == 0) {
        game_state.current_girl = GIRL_SOTOI;
    } else {
        game_state.current_girl = game_state.date_girl;
    }
    date_scene_speak();
}

s32 date_scene_mio_winter_load_data(void) {
    GameState *game;
    u32 elapsed_frames;

    game = &game_state;
    elapsed_frames = ++game->mode_step_timer.u;
    if (elapsed_frames < 16) {
        return 0;
    }
    cd_read_sectors(0x3D, 0x801B0000, 0x872B);
    date_scene_mio_winter_data_init();
    mode_step_next();
}

void date_scene_mio_winter_answer_effect(void) {
    if (D_80122CDC != 0) {
        D_800CA150 = (u16) D_800CA150 + 1;
    } else {
        game_state.girls[1].flags.f.b14 = 1;
    }
    mode_step_next();
}

void date_scene_mio_spring_ride(void) {
    date_ride_dispatch();
}
