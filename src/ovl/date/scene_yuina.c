/* scene_yuina.c: src/ovl/DATE/80150010.c:7: mode 0x5E scenes of Yuina (girl 2). Original object 0x80150010-0x80151460 (config/objects/DATE.txt). */
#define MAIN_API_OVERRIDE_talk_work /* switched as u32: selector in $v1 (main_api.h: s32) */
#include "common.h"
#include "ovl/date.h"

extern u32 talk_work;

void date_scene_yuina_spring_data_init(void) {
    D_8015EA40 = 0x801B0000;
    D_8015EA44 = 0x801B2000;
    D_8015EA48 = 0x801B6000;
    D_8015EA4C = 0x801BA000;
    D_8015EA50 = 0x801BE000;
    D_8015EA54 = 0x801C2000;
    D_8015EA58 = 0x801C6000;
}

void date_scene_yuina_summer_data_init(void) {
    D_8015EA5C = (u8 *)0x801CE1A4;
    D_8015EA60 = (u8 *)0x801CE1A8;
    D_8015EA64 = (u8 *)0x801CE1E0;
    D_8015EA68 = *(s16 *)0x801CE1FC;
    D_8015EA6C = (u8 *)0x801B0000;
    D_8015EA70 = (u8 *)0x801B2000;
    D_8015EA74 = (u8 *)0x801B6000;
    D_8015EA78 = (u8 *)0x801BA000;
    D_8015EA7C = (u8 *)0x801BE000;
    D_8015EA80 = (u8 *)0x801C2000;
    D_8015EA84 = (u8 *)0x801C6000;
}

void date_scene_yuina_autumn_data_init(void) {
    D_8015EA88 = (u8 *)0x801D2370;
    D_8015EA8C = (u8 *)0x801D2374;
    D_8015EA90 = (u8 *)0x801D23A4;
    D_8015EA94 = *(s16 *)0x801D23C0;
    D_8015EA98 = (u8 *)0x801B0000;
    D_8015EA9C = (u8 *)0x801D2000;
    D_8015EAA0 = (u8 *)0x801B2000;
    D_8015EAA4 = (u8 *)0x801B6000;
    D_8015EAA8 = (u8 *)0x801BA000;
    D_8015EAAC = (u8 *)0x801BE000;
    D_8015EAB0 = (u8 *)0x801C2000;
    D_8015EAB4 = (u8 *)0x801C6000;
    D_8015EAB8 = (u8 *)0x801CE000;
}

void date_scene_yuina_winter_data_init(void) {
    D_8015EABC = (u8 *)0x801D2174;
    D_8015EAC0 = (u8 *)0x801D217C;
    D_8015EAC4 = (u8 *)0x801D21A8;
    D_8015EAC8 = *(s16 *)0x801D21C4;
    D_8015EACC = (u8 *)0x801B0000;
    D_8015EAD0 = (u8 *)0x801B2000;
    D_8015EAD4 = (u8 *)0x801B6000;
    D_8015EAD8 = (u8 *)0x801BA000;
    D_8015EADC = (u8 *)0x801BE000;
    D_8015EAE0 = (u8 *)0x801C2000;
    D_8015EAE4 = (u8 *)0x801C6000;
}

void date_scene_yuina_dispatch(void) {
    switch (talk_work) {
    case 2:
        date_scene_yuina_spring();
        return;
    case 3:
        date_scene_yuina_summer();
        return;
    case 4:
        date_scene_yuina_autumn();
        return;
    case 5:
        date_scene_yuina_winter();
        return;
    default:
        game_mode_reset_to_title();
        return;
    }
}

void date_scene_yuina_spring_setup(void) {
    date_scene_text = D_8015DE8C;
    date_scene_command = D_8015DFC8;
    date_scene_voice = D_8015E104;
    date_scene_yuina_spring_data_init();
    k_reset(0);
    message_window_set_title("園内");
    k_reset(1);
    load_palette(D_8015EA40, 0x11, 1, 2, 0);
    scene_still_show(D_8015EA44, D_8015EA48, D_8015EA4C, D_8015EA50, D_8015EA54, D_8015EA58);
    scene_anim_show(0, 0, 0, 0);
    game_state.girls[2].tokimeki += 3;
    game_state.girls[2].affection += 2;
    game_state.girls[2].syoushin -= 0x14;
    check_para_limit();
    mode_step_next();
}

/* Copies display record 96 (actor_pool_96) over record 98 (D_801206D8). */
void date_scene_yuina_summer_setup(void) {
    date_scene_text = D_8015DE90;
    date_scene_command = D_8015DFCC;
    date_scene_voice = D_8015E108;
    game_state.girls[2].tokimeki += 2;
    game_state.girls[2].syoushin -= 0xA;
    check_para_limit();
    date_scene_yuina_summer_data_init();
    load_palette((s32) D_8015EA6C, 0x11, 1, 2, 0);
    scene_still_show((s32) D_8015EA70, (s32) D_8015EA74, (s32) D_8015EA78, (s32) D_8015EA7C, (s32) D_8015EA80, (s32) D_8015EA84);
    scene_anim_show((s32) D_8015EA60, (s32) D_8015EA64, (s32) D_8015EA5C, (s32) D_8015EA68);
    ((Actor *)actor_pool)[98] = ((Actor *)actor_pool)[96];
    scene_mouth_frames_reversed_saved = 1;
    mode_step_next();
}

void date_scene_yuina_autumn_setup(void) {
    date_scene_text = D_8015DE94;
    date_scene_command = D_8015DFD0;
    date_scene_voice = D_8015E10C;
    date_scene_yuina_autumn_data_init();
    load_palette((s32) D_8015EA98, 0x11, 1, 2, 0);
    scene_still_show((s32) D_8015EAA0, (s32) D_8015EAA4, (s32) D_8015EAA8, (s32) D_8015EAAC, (s32) D_8015EAB0, (s32) D_8015EAB4);
    scene_anim_show((s32) D_8015EA8C, (s32) D_8015EA90, (s32) D_8015EA88, (s32) D_8015EA94);
    actor_clear(0x62);
    D_801206D9 = 9;
    D_801206DA = 1;
    D_80120710 = 0x41000000;
    D_801206DB = 4;
    D_801206E4 = (s32) D_8015EA8C;
    D_801206E8 = (s32) D_8015EA90;
    D_8012070C = (s32) D_8015EA88;
    D_801206EC = D_8015EA94;
    D_801206EE = 4;
    D_801206F0 = 0;
    D_801206DE = 1;
    D_8012071B = 0x11;
    D_801206FE = -0xA0;
    D_80120702 = -0x78;
    D_801206DF = 0;
    D_801206DD = 0;
    D_801206DC = 0;
    load_palette((s32) D_8015EA9C, 0x12, 1, 0, 0);
    game_state.tpage_buf[26] = 1;
    LoadSquare(0x280, 0x100, 0x40, 0x80, D_8015EAB8);
    D_80121FD4 = 0x81000000;
    D_80121FDC = 0x55;
    D_80121FDE = 0x38;
    D_80121FE2 = 0;
    D_80121FE3 = 0;
    D_80121FE4 = 0;
    D_80121FE0 = 0x1A;
    D_80121FE6 = 0x1F2;
    D_80121FE8 = 0;
    D_80121FE9 = 0;
    D_80121FEA = 0;
    D_80121FEC = 0;
    D_80121FEE = 0;
    D_80121FF0 = 0x1000;
    D_80121FF2 = 0x1000;
    D_80121FF4 = 0;
    D_80121FD8 = 5;
    D_80121FDA = -0x1E;
    game_state.quad_layer[57] = 0xC;
    game_state.girls[2].affection += 2;
    game_state.girls[2].syoushin -= 0xA;
    check_para_limit();
    mode_step_next();
}

void date_scene_yuina_winter_setup(void) {
    date_scene_text = D_8015DE98;
    date_scene_command = D_8015DFD4;
    date_scene_voice = D_8015E110;
    date_scene_yuina_winter_data_init();
    load_palette((s32) D_8015EACC, 0x11, 1, 2, 0);
    scene_still_show((s32) D_8015EAD0, (s32) D_8015EAD4, (s32) D_8015EAD8, (s32) D_8015EADC, (s32) D_8015EAE0, (s32) D_8015EAE4);
    scene_anim_show((s32) D_8015EAC0, (s32) D_8015EAC4, (s32) D_8015EABC, (s32) D_8015EAC8);
    scene_mouth_frames_reversed_saved = 1;
    actor_clear(0x62);
    D_801206D9 = 9;
    D_801206DA = 1;
    D_80120710 = 0x41000000;
    D_801206DB = 4;
    D_801206E4 = (s32) D_8015EAC0;
    D_801206E8 = (s32) D_8015EAC4;
    D_8012070C = (s32) D_8015EABC;
    D_801206EC = D_8015EAC8;
    D_801206EE = 4;
    D_801206F0 = 0;
    D_801206DE = 1;
    D_8012071B = 0x11;
    D_801206FE = -0xA0;
    D_80120702 = -0x78;
    D_801206DF = 0;
    D_801206DD = 0;
    D_801206DC = 0;
    game_state.girls[2].tokimeki += 3;
    game_state.girls[2].affection += 2;
    game_state.girls[2].syoushin -= 0x14;
    check_para_limit();
    mode_step_next();
}

void date_scene_yuina_spring_background_1(void) {
    bg_read_sub2(0x4587);
    mode_step_next();
}

void date_scene_yuina_spring_background_2(void) {
    bg_read_sub2(0x457D);
    mode_step_next();
}

void date_scene_yuina_summer_background_1(void) {
    bg_read_sub2(0x4925);
    mode_step_next();
}

void date_scene_yuina_summer_background_2(void) {
    bg_read_sub2(0x4631);
    mode_step_next();
}

void date_scene_yuina_autumn_background_1(void) {
    bg_read_sub2(0x4845);
    mode_step_next();
}

void date_scene_yuina_autumn_background_2(void) {
    bg_read_sub2(0x44D6);
    mode_step_next();
}

void date_scene_yuina_winter_background_1(void) {
    bg_read_sub2(0x453B);
    mode_step_next();
}

void date_scene_yuina_winter_audio_command_background_1(void) {
    audio_command(0x203);
    bg_read_sub2(0x4520);
    mode_step_next();
}

void date_scene_yuina_spring(void) {
    s32 step_index; /* declared before step_handlers: its stack slot sits above step_handlers (T-3330) */
    FnTbl40 step_handlers;

    step_handlers = date_scene_yuina_spring_steps;
    step_index = game_state.mode_step;
    step_handlers.callbacks[step_index]();
    if (D_80122D04 != 0) {
        date_scene_yuina_spring_radial_lines();
        D_800B5BD4 = 9;
        return;
    }
    D_800B5BD4 = 0xF;
}

/* Sixteen grey lines through prim_gradient_line. All nine s-registers are taken, so uopt keeps col1 in
 * memory: its stack home (0x64) and the frame size follow the declaration order. */
void date_scene_yuina_spring_radial_lines(void) {
    s32 col0;
    s16 i;
    s32 col1;
    GameState *game = &game_state;
    u32 grey;
    s32 offset;

    grey = bg_brightness / 10;
    offset = 0;
    col1 = col0 = (grey * 19 << 16) | (grey * 19 << 8) | (grey * 19);
    if (game->frame_count % 45 < 15) {
        offset = 0xAA;
    } else if (game->frame_count % 45 < 30) {
        offset = 0x155;
    }
    for (i = 0; i < 16; i++) {
        prim_gradient_line((s16)(func_800A0140((i << 8) + offset) / 20),
                      (s16)(rsin((i << 8) + offset) / 20 - 30),
                      (s16)(func_800A0140((i << 8) + offset) / 60),
                      (s16)(rsin((i << 8) + offset) / 60 - 30), col0, col1, 10);
    }
}

void date_scene_yuina_spring_load_data(void) {
    cd_read_sectors(0x35, 0x801B0000, 0x8144);
    date_scene_yuina_spring_data_init();
    mode_step_next();
}

void date_scene_yuina_summer(void) {
    GameState *game = &game_state; /* declared before step_handlers: its stack slot sits above step_handlers (T-3330) */
    FnTbl39 step_handlers;

    step_handlers = date_scene_yuina_summer_steps;
    if (((game->year == 0x5F && game->month == 7 && game->day == 0x17) ||
         (game->year == 0x60 && game->month == 7 && game->day == 0x1C) ||
         (game->year == 0x61 && game->month == 7 && game->day == 0x1B)) &&
        bg_brightness == 0x80 && D_80122D04 == 0 && game->mode_step >= 0x1A) {
        hud_sprite_light_effect_draw();
    }

    step_handlers.callbacks[game->mode_step](0x80);
    if (game->mode_step >= 0xD && game->mode_step < 0x10) {
        if (game->frame_count % 180 == 0) {
            date_scene_yuina_summer_audio_command_1();
        }
        if (game->frame_count % 180 == 0x5A) {
            date_scene_yuina_summer_audio_command_2();
        }
    }
}

void date_scene_yuina_summer_load_data(void) {
    cd_read_sectors(0x3D, 0x801B0000, 0x8179);
    date_scene_yuina_summer_data_init();
    mode_step_next();
}

void date_scene_yuina_summer_still_fade_out(void) {
    scene_still_fade_out();
    D_801206DF = bg_brightness;
    if ((u8) bg_brightness < 8U) {
        D_801206DB &= 0xFF7F;
    }
}

void date_scene_yuina_summer_sprite_show(void) {
    ((Actor *)actor_pool)[98].brightness = 0x80;
    ((Actor *)actor_pool)[98].draw_flags |= 0x80;
    ((Actor *)actor_pool)[98].unk_08 = 0;
    ((Actor *)actor_pool)[98].sequence_slot = 4;
    ((Actor *)actor_pool)[98].unk_18 = 0;
    ((Actor *)actor_pool)[98].flags = 1;
    mode_step_next();
}

void date_scene_yuina_summer_audio_command_1(void) {
    audio_command(0x500);
}

void date_scene_yuina_summer_audio_command_2(void) {
    audio_command(0x503);
}

void date_scene_yuina_summer_audio_command_3(void) {
    audio_command(0x504);
    mode_step_next();
}

/* D_80121FD4 is record 57 of the SpriteQuad table and D_801206D8 record 98 of the actor_pool display
 * records; the chain assignment keeps the loaded byte in one register for all four stores. */
void date_scene_yuina_autumn(void) {
    s32 step_index; /* declared before step_handlers: its stack slot sits above step_handlers (T-3330) */
    FnTbl40 step_handlers;

    step_handlers = date_scene_yuina_autumn_steps;
    step_index = game_state.mode_step;
    step_handlers.callbacks[step_index](0x80);
    if (D_80122D04 != 0) {
        ((Actor *)actor_pool)[98].draw_flags |= 0x80;
        if (game_state.frame_count % 40 < 20) {
            sprite_quads[57].flags &= 0x7FFFFFFF;
        } else {
            sprite_quads[57].flags |= 0x80000000;
        }
        sprite_quads[57].tint_r = sprite_quads[57].tint_g = sprite_quads[57].tint_b =
            ((Actor *)actor_pool)[98].brightness = bg_brightness;
        return;
    }
    ((Actor *)actor_pool)[98].draw_flags &= 0xFF7F;
    sprite_quads[57].flags |= 0x80000000;
}

void date_scene_yuina_autumn_audio_command_1(void) {
    audio_command(0x502);
    mode_step_next();
}

void date_scene_yuina_autumn_load_data(void) {
    cd_read_sectors(0x45, 0x801B0000, 0x81B6);
    date_scene_yuina_autumn_data_init();
    mode_step_next();
}

void date_scene_yuina_winter(void) {
    s32 step_index; /* declared before step_handlers: its stack slot sits above step_handlers (T-3330) */
    FnTbl40 step_handlers;

    step_handlers = date_scene_yuina_winter_steps;
    step_index = game_state.mode_step;
    step_handlers.callbacks[step_index](0x80);
    if (D_80122D04 != 0) {
        D_801206DB |= 0x80;
        D_801206DF = bg_brightness;
        return;
    }
    D_801206DB &= 0xFF7F;
}

void date_scene_yuina_winter_load_data(void) {
    cd_read_sectors(0x45U, 0x801B0000, 0x81FB);
    date_scene_yuina_winter_data_init();
    mode_step_next();
}
