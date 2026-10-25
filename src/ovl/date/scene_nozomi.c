/* scene_nozomi.c: src/ovl/DATE/80156600.c:7: mode 0x5E scenes of Nozomi (girl 6). Original object 0x80156600-0x80157880 (config/objects/DATE.txt). */
#define MAIN_API_OVERRIDE_talk_work /* switched as u32: selector in $v1 (main_api.h: s32) */
#include "common.h"
#include "ovl/date.h"

extern u32 talk_work;

void date_scene_nozomi_spring_data_init(void) {
    D_8015FCE0 = 0x801CE0DC;
    D_8015FCE4 = 0x801CE0E0;
    D_8015FCE8 = 0x801CE0F8;
    D_8015FCEC = *(s16 *)0x801CE10C;
    D_8015FCF0 = 0x801B0000;
    D_8015FCF4 = 0x801B2000;
    D_8015FCF8 = 0x801B6000;
    D_8015FCFC = 0x801BA000;
    D_8015FD00 = 0x801BE000;
    D_8015FD04 = 0x801C2000;
    D_8015FD08 = 0x801C6000;
}

void date_scene_nozomi_summer_data_init(void) {
    D_8015FD0C = 0x801D2100;
    D_8015FD10 = 0x801D2108;
    D_8015FD14 = 0x801D2128;
    D_8015FD18 = *(s16 *)0x801D2144;
    D_8015FD1C = 0x801B0000;
    D_8015FD20 = 0x801B2000;
    D_8015FD24 = 0x801B6000;
    D_8015FD28 = 0x801BA000;
    D_8015FD2C = 0x801BE000;
    D_8015FD30 = 0x801C2000;
    D_8015FD34 = 0x801C6000;
}

void date_scene_nozomi_autumn_data_init(void) {
    D_8015FD38 = 0x801D2240;
    D_8015FD3C = 0x801D2248;
    D_8015FD40 = 0x801D2288;
    D_8015FD44 = *(s16 *)0x801D22A0;
    D_8015FD48 = 0x801B0000;
    D_8015FD4C = 0x801B2000;
    D_8015FD50 = 0x801B6000;
    D_8015FD54 = 0x801BA000;
    D_8015FD58 = 0x801BE000;
    D_8015FD5C = 0x801C2000;
    D_8015FD60 = 0x801C6000;
}

void date_scene_nozomi_winter_data_init(void) {
    D_8015FD64 = 0x801CE080;
    D_8015FD68 = 0x801CE084;
    D_8015FD6C = 0x801CE094;
    D_8015FD70 = *(s16 *)0x801CE0A8;
    D_8015FD74 = 0x801B0000;
    D_8015FD78 = 0x801B2000;
    D_8015FD7C = 0x801B6000;
    D_8015FD80 = 0x801BA000;
    D_8015FD84 = 0x801BE000;
    D_8015FD88 = 0x801C2000;
    D_8015FD8C = 0x801C6000;
}

void date_scene_nozomi_dispatch(void) {
    switch (talk_work) {
    case 2:
        date_scene_nozomi_spring();
        return;
    case 3:
        date_scene_nozomi_summer();
        return;
    case 4:
        date_scene_nozomi_autumn();
        return;
    case 5:
        date_scene_nozomi_winter();
        return;
    default:
        game_mode_reset_to_title();
        return;
    }
}

void date_scene_nozomi_spring_setup(void) {
    date_scene_text = D_8015DF18;
    date_scene_command = D_8015E054;
    date_scene_voice = D_8015E190;
    date_scene_nozomi_spring_data_init();
    load_palette(D_8015FCF0, 0x11, 1, 2, 0);
    scene_still_show(D_8015FCF4, D_8015FCF8, D_8015FCFC, D_8015FD00, D_8015FD04, D_8015FD08);
    scene_anim_show(D_8015FCE4, D_8015FCE8, D_8015FCE0, (s32) D_8015FCEC);
    game_state.girls[6].tokimeki += 1;
    game_state.girls[6].affection += 1;
    game_state.girls[6].syoushin -= 0x14;
    check_para_limit();
    game_state.girls[game_state.current_girl].scene_counters[0xD] += 1;
    D_800CA21C[0] = 2;
    D_800CA21C[2] = 2;
    D_800CA21C[1] = 2;
    date_choice_mood_face[0] = 2;
    date_choice_mood_face[2] = 2;
    date_choice_mood_face[1] = 2;
    D_800CA22C[0] = 2;
    D_800CA22C[2] = 2;
    D_800CA22C[1] = 2;
    date_choice_tier[0] = 1;
    date_choice_tier[2] = 1;
    date_choice_tier[1] = 1;
    mode_step_next();
}

void date_scene_nozomi_summer_setup(void) {
    date_scene_text = D_8015DF1C;
    date_scene_command = D_8015E058;
    date_scene_voice = D_8015E194;
    date_scene_nozomi_summer_data_init();
    load_palette(D_8015FD1C, 0x11, 1, 2, 0);
    scene_still_show(D_8015FD20, D_8015FD24, D_8015FD28, D_8015FD2C, D_8015FD30, D_8015FD34);
    scene_anim_show(D_8015FD10, D_8015FD14, D_8015FD0C, (s32) D_8015FD18);
    scene_mouth_frames_reversed_saved = 1;
    game_state.girls[6].tokimeki += 3;
    game_state.girls[6].affection += 2;
    game_state.girls[6].syoushin -= 0x14;
    check_para_limit();
    mode_step_next();
}

void date_scene_nozomi_autumn_setup(void) {
    date_scene_text = D_8015DF20;
    date_scene_command = D_8015E05C;
    date_scene_voice = D_8015E198;
    date_scene_nozomi_autumn_data_init();
    load_palette(D_8015FD48, 0x11, 1, 2, 0);
    scene_still_show(D_8015FD4C, D_8015FD50, D_8015FD54, D_8015FD58, D_8015FD5C, D_8015FD60);
    scene_anim_show(D_8015FD3C, D_8015FD40, D_8015FD38, (s32) D_8015FD44);
    scene_mouth_frames_reversed_saved = 1;
    game_state.girls[6].tokimeki += 2;
    game_state.girls[6].affection += 2;
    game_state.girls[6].syoushin -= 0xA;
    check_para_limit();
    mode_step_next();
}

void date_scene_nozomi_winter_setup(void) {
    date_scene_text = D_8015DF24;
    date_scene_command = D_8015E060;
    date_scene_voice = D_8015E19C;
    if ((u8)(get_g_zyotai_s(GIRL_NOZOMI) & 0x7F) < 2) {
        game_state.girls[6].tokimeki += 3;
        game_state.girls[6].affection += 2;
    } else {
        game_state.girls[6].tokimeki += 1;
        game_state.girls[6].affection += 3;
    }
    game_state.girls[6].syoushin -= 0x14;
    check_para_limit();
    date_scene_nozomi_winter_data_init();
    load_palette(D_8015FD74, 0x11, 1, 2, 0);
    scene_still_show(D_8015FD78, D_8015FD7C, D_8015FD80, D_8015FD84, D_8015FD88, D_8015FD8C);
    scene_anim_show(D_8015FD68, D_8015FD6C, D_8015FD64, D_8015FD70);
    scene_mouth_frames_reversed_saved = 1;
    mode_step_next();
}

void date_scene_nozomi_spring_background_1(void) {
    bg_read_sub2(0x4860);
    mode_step_next();
}

void date_scene_nozomi_spring_background_2(void) {
    bg_read_sub2(0x44F1);
    mode_step_next();
}

void date_scene_nozomi_summer_island_title(void) {
    strcpy(scene_place_title_2, "無人島");
    bg_read_sub2(0x45B6);
    mode_step_next();
}

void date_scene_nozomi_summer_background_1(void) {
    bg_read_sub2(0x48CD);
    mode_step_next();
}

void date_scene_nozomi_summer_background_2(void) {
    bg_read_sub2(0x45A3);
    mode_step_next();
}

void date_scene_nozomi_autumn_background_1(void) {
    bg_read_sub2(0x48F7);
    mode_step_next();
}

void date_scene_nozomi_autumn_background_2(void) {
    bg_read_sub2(0x45D3);
    mode_step_next();
}

void date_scene_nozomi_winter_background_1(void) {
    bg_read_sub2(0x492F);
    mode_step_next();
}

void date_scene_nozomi_winter_background_2(void) {
    bg_read_sub2(0x4631);
    mode_step_next();
}

void date_scene_nozomi_spring(void) {
    s32 step_index; /* declared before step_handlers: its stack slot sits above step_handlers (T-3330) */
    FnTbl50 step_handlers;

    step_handlers = date_scene_nozomi_spring_steps;
    step_index = game_state.mode_step;
    step_handlers.callbacks[step_index](0x80);
}

void date_scene_nozomi_spring_audio_command_1(void) {
    audio_command(0x500);
    mode_step_next();
}

void date_scene_nozomi_spring_load_data(void) {
    cd_read_sectors(0x3D, 0x801B0000, 0x8984);
    date_scene_nozomi_spring_data_init();
    mode_step_next();
}

void date_scene_nozomi_spring_mood_face(void) {
    if (get_g_zyotai_s(game_state.current_girl) < 2U) {
        portrait_layout_set(0);
    } else {
        portrait_layout_set(get_g_zyotai_h(game_state.current_girl));
    }
    mode_step_next();
}

void date_scene_nozomi_spring_group_by_mood(void) {
    u8 girl_state;

    girl_state = get_g_zyotai_s(game_state.current_girl) & 0x7F;
    if (girl_state >= 2U) {
        talk_secondary_group = (u16)talk_secondary_group + 5;
    }
    mode_step_next();
}

void date_scene_nozomi_summer(void) {
    s32 fake_padding; /* FAKE: never read; declared first so step_handlers lands at the original frame offset (T-3330) */
    DateNozomiSummerSteps step_handlers;

    step_handlers = date_scene_nozomi_summer_steps;
    if (game_state.mode_step < 4 || game_state.mode_step >= 0x2C) {
        if (bg_brightness == 0x80) {
            hud_sprite_light_effect_draw();
        }
    }
    step_handlers.callbacks[game_state.mode_step](0x80);
}

void date_scene_nozomi_summer_load_data(void) {
    cd_read_sectors(0x45, 0x801B0000, 0x89C1);
    date_scene_nozomi_summer_data_init();
    mode_step_next();
}

void date_scene_nozomi_summer_audio_command_1(void) {
    audio_command(0x500);
    mode_step_next();
}

/* FAKE: element-zero views of the GameState bytes preserve load/store ordering (the record copy is one
 * Actor since T-20900). T-9550 */
void date_scene_nozomi_summer_picture_swap(void) {
    s16 i;

    ((Actor *)actor_pool)[98] = ((Actor *)actor_pool)[96];
    D_801206EE = 4;
    D_801206DA = 9;
    for (i = 0; i < 6; i++) {
        sprite_quads[i + 4].flags |= 0x80000000;
    }
    ((Actor *)actor_pool)[96].draw_flags &= 0xFF7F;
    ((Actor *)actor_pool)[97].draw_flags &= 0xFF7F;
    D_80122CF8 = 0;
    (&game_state.clear_color_r)[0] = (&game_state.clear_color_g)[0] = (&game_state.clear_color_b)[0] = 0x7F;
    mode_step_next();
}

s32 date_scene_nozomi_summer_fade_to_white(void) {
    game_state.clear_color_r += 0x10;
    game_state.clear_color_g += 0x10;
    game_state.clear_color_b += 0x10;
    if (game_state.clear_color_r >= 0xF0U) {
        mode_step_next();
    }
}

void date_scene_nozomi_summer_fade_reset(void) {
    s16 i;

    game_state.clear_color_g = 0;
    game_state.clear_color_r = 0;
    /* FAKE: the last store and the loop on one source line; as1 then places the store after the
     * loop setup as the original does (line-based scheduling, T-6070). Not a separate symbol. T-7010 */
    game_state.clear_color_b = 0; for (i = 0; i < 6; i++) {
        sprite_quads[i + 4].flags &= 0x7FFFFFFF;
    }
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    D_80122CF8 = 1;
    mode_step_next();
}

void date_scene_nozomi_autumn(void) {
    s32 step_index; /* declared before step_handlers: its stack slot sits above step_handlers (T-3330) */
    FnTbl41 step_handlers;

    step_handlers = date_scene_nozomi_autumn_steps;
    step_index = game_state.mode_step;
    step_handlers.callbacks[step_index](0x80);
}

void date_scene_nozomi_autumn_load_data(void) {
    cd_read_sectors(0x45, 0x801B0000, 0x8A06);
    date_scene_nozomi_autumn_data_init();
    mode_step_next();
}

void date_scene_nozomi_autumn_group_by_mood(void) {
    u8 girl_state;

    girl_state = get_g_zyotai_s(game_state.current_girl) & 0x7F;
    if (girl_state >= 2U) {
        talk_secondary_group = (u16)talk_secondary_group + 3;
    }
    mode_step_next();
}

void date_scene_nozomi_winter(void) {
    s32 step_index; /* declared before step_handlers: its stack slot sits above step_handlers (T-3330) */
    FnTbl43 step_handlers;

    step_handlers = date_scene_nozomi_winter_steps;
    step_index = game_state.mode_step;
    step_handlers.callbacks[step_index](0x80);
}

void date_scene_nozomi_winter_speak_with_face(void) {
    u8 girl_state_flags;
    u32 girl_state;
    /* FAKE: constant-1 local keeps the original bne operand order (constant register first); T-17206 */
    s32 fake_first_tick;

    fake_first_tick = 1;
    date_scene_speak();
    girl_state_flags = get_g_zyotai_s(GIRL_NOZOMI);
    girl_state = girl_state_flags & 0x7F;
    if ((girl_state >= 2U) && (talk_secondary_line == 8) && (fake_first_tick == game_state.mode_step_timer.w)) {
        portrait_layout_set(2);
    } else if ((girl_state >= 2U) && (talk_secondary_line == 0xA) && (fake_first_tick == game_state.mode_step_timer.w)) {
        portrait_layout_set(3);
    } else if ((girl_state >= 2U) && (talk_secondary_line == 6) && (fake_first_tick == game_state.mode_step_timer.w)) {
        portrait_layout_set(1);
    } else if ((girl_state >= 2U) && (talk_secondary_line == 0xC) && (fake_first_tick == game_state.mode_step_timer.w)) {
        portrait_layout_set(1);
    }
    game_state.mode_step_timer.w += 1;
}

void date_scene_nozomi_winter_load_data(void) {
    cd_read_sectors(0x3D, 0x801B0000, 0x8A4B);
    date_scene_nozomi_winter_data_init();
    mode_step_next();
}

void date_scene_nozomi_winter_audio_command_1(void) {
    audio_command(0x501);
    mode_step_next();
}

void date_scene_nozomi_winter_group_by_mood(void) {
    u8 girl_state;

    girl_state = get_g_zyotai_s(game_state.current_girl) & 0x7F;
    if (girl_state >= 2U) {
        talk_secondary_group = (u16)talk_secondary_group + 3;
    }
    mode_step_next();
}
