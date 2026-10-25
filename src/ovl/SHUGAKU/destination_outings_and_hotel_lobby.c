/* destination_outings_and_hotel_lobby.c: src/ovl/SHUGAKU/80139D00.c:14,151-215,232-335; destination-specific outing callbacks, hotel-lobby branch, girl-dependent choices and birthday/return handling. Original object 0x80139D00-0x8013AA70 (config/objects/SHUGAKU.txt). */
#define SHUGAKU_OVERRIDE_trip_destination_response_talk_run /* s32 return view occupies $v0 after call so unk_F5F promotes to $v1 (T-24108) */
#include "common.h"
#include "ovl/SHUGAKU.h"

s32 trip_destination_response_talk_run(void);

void trip_destination_scene_resource_pointer_setup(void) {
    D_8013CAF0 = 0x801E20DC;
    D_8013CAF4 = 0x801E20F8;
    D_8013CAF8 = 0x801E2114;
    D_8013CB00 = 0x801E0000;
}

void trip_destination_outing_callback_dispatch(void) {
    u8 kind;
    u8 index;
    ShugakuFn68 tbl;

    tbl = D_8013CB08;
    if (D_800CA2EC != 0) {
        index = game_state.mode_step;
        if (tbl.f[index] == normal_date_two_select) {
            tbl.f[index] = normal_date_three_select;
        }
    }
    tbl.f[game_state.mode_step](0x40);
    if (tbl.f[game_state.mode_step] == trip_default_destination_talk_run && D_8013CB04 == 0) {
        kind = ((u8 **)D_800CA2D4)[D_800CA2DC][D_800CA2E0];
        if (kind == 8 || kind == 3) {
            audio_command(0x207);
            D_8013CB04 = 1;
        }
    }
}

/* FAKE: byte view keeps selector base in a0; direct member access uses v0. Original source unknown. T-9710. */
void trip_destination_outing_scene_initialize(void) {
    u8 var_a0;
    RECT rect;

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
    rect.x = 0x140;
    rect.y = 0x1A0;
    rect.w = 0x140;
    rect.h = 0x58;
    func_8009C7F8(&rect, 8, 0, 0);
    rect.x = 0x2C0;
    rect.y = 0xA0;
    func_8009C7F8(&rect, 8, 0, 0);
    var_a0 = ((u8 *)&game_state)[0xF5F];
    game_state.tile_layer_enabled = 0;
    game_state.quads_enabled = 1;
    if (var_a0 >= 0xEU) {
        var_a0 = 0xD;
    }
    game_state.current_girl = var_a0;
    talk_names_set(var_a0);
    addr_init_bustup();
    game_state.screen_brightness = 0x80;
    bg_brightness = 0;
    hizuke_init();
    message_window_init();
    text_marker_hide();
    D_80122CE0 = 0xFF;
    D_800CA2F4 = 1;
    mode_step_next();
}

s32 trip_destination_lobby_actor_data_validate(void) {
    if (D_800CA2CC == 1) {
        if (((*(s32 *)D_8013CAF0) != 0x801E2120) || (*(s32 *)((unsigned char *)D_8013CAF0 + (4)) != 0x801E6120) || (*(s32 *)((unsigned char *)D_8013CAF0 + (8)) != 0x801EA120) || (*(s32 *)((unsigned char *)D_8013CAF0 + (12)) != 0x801EE120) || (*(s32 *)((unsigned char *)D_8013CAF0 + (16)) != 0x801F2120) || (*(s32 *)((unsigned char *)D_8013CAF0 + (20)) != 0x801F6120) || (*(s32 *)((unsigned char *)D_8013CAF0 + (24)) != 0x801FA120) || (*(s32 *)D_8013CAF8 != 0x801E20A8)) {
            game_state.mode_step -= 2;
            return 0;
        }
        load_palette(D_8013CB00, 0x11, 0, 1, 0);
        actor_clear(0x61);
        D_80120695 = 0xC;
        D_80120696 = 1;
        D_801206CC = 0x01000000;
        D_80120697 = 0x84;
        D_801206A0 = D_8013CAF4;
        D_801206A4 = D_8013CAF8;
        D_801206C8 = D_8013CAF0;
        D_801206A8 = D_8013CAFC;
        D_801206AA = 0;
        D_801206AC = 0;
        D_8012069A = 1;
        D_801206D7 = 0x11;
        D_801206BA = 0x10;
        D_801206BE = -0x77;
        D_8012069B = 0;
        D_80120699 = 1;
        D_80120698 = 0;
        goto block_11;
    }
block_11:
    return mode_step_next();
}

void trip_destination_lobby_mode_step_skip(void) {
    mode_step_set(0x22);
}

/* FAKE: T-24108, dead entry read of unk_F5F satisfies cvt_pass's entry condition; dead s32 result from trip_destination_response_talk_run takes the sp+0x2C slot (placing temp_a0 at sp+0x2B) and blocks $v0 after the call so unk_F5F is promoted to $v1. */
void trip_yuina_talk_and_portrait_layout_update(void) {
    s32 pad;
    u8 temp_a0;

    pad = game_state.current_girl;
    pad = trip_destination_response_talk_run();
    if (game_state.current_girl == GIRL_YUINA) {
        temp_a0 = get_g_zyotai_h(game_state.current_girl);
        if ((D_800CA2E0 == 7) && (D_80122CDC != 0)) {
            portrait_layout_set(temp_a0);
        }
        if (D_800CA2E0 == 6) {
            if (D_80122CDC == 0) {
                portrait_layout_set(temp_a0);
            }
        }
    }
}

void trip_girl_specific_portrait_layout_set(void) {
    if (game_state.current_girl == GIRL_YUINA) {
        portrait_layout_set(3);
    }
    if (game_state.current_girl == GIRL_MIRA && D_800CA2F4 == 0) {
        portrait_layout_set(4);
    }
    mode_step_next();
}

void trip_destination_choice_dialogue_gate(void) {
    if (D_80122CDC == 0) {
        mode_step_next();
        return;
    }
    trip_destination_response_talk_run();
}

void trip_hotel_lobby_background_and_route_select(void) {
    if (D_80122CDC == 0) {
        bg_read_sub2(0x4225);
        strcpy(scene_place_title, "ホテル・ロビー");
    } else {
        mode_step_next();
        mode_step_next();
        mode_step_next();
    }
    mode_step_next();
}

void trip_destination_counter_add(void) {
    D_800CA2DC += D_800CA2CC;
    mode_step_next();
}

void trip_destination_counter_response_adjust(void) {
    if (D_800CA2EC == 0) {
        D_800CA2DC = (D_800CA2DC - (D_80122CDC * 2)) + 2;
    } else {
        D_800CA2DC = (D_800CA2DC - (D_80122CDC * 2)) + 4;
    }
    mode_step_next();
}

void trip_destination_outing_background_and_log(void) {
    u8 temp;
    D_8013CB04 = 0;
    temp = D_800CA2CC;
    switch (temp) {
    case 0:
        bg_read_sub2(0x4267);
        strcpy(scene_place_title, "土産屋");
        break;
    case 1:
        bg_read_sub2(0x4255);
        strcpy(scene_place_title, "浜辺");
        break;
    default:
        bg_read_sub2(0x427B);
        strcpy(scene_place_title, "クラーク像前");
        break;
    }
    if (D_800CA2EC == 0) {
        event_log_add(3, D_800CA2CC + 3);
    }
    mode_step_next();
}

void trip_destination_outing_background_select(void) {
    s32 temp;
    temp = D_800CA2CC;
    switch (temp) {
    case 0:
        bg_read_sub2(0x4267);
        strcpy(scene_place_title, "土産屋");
        break;
    case 1:
        bg_read_sub2(0x4255);
        strcpy(scene_place_title, "浜辺");
        break;
    default:
        bg_read_sub2(0x427B);
        strcpy(scene_place_title, "クラーク像前");
        break;
    }
    mode_step_next();
}

void trip_destination_rpg_battle_launch(void) {
    if (D_800CA2CC == 0) {
        D_80122CE0 = 2;
    } else if (D_800CA2CC == 1) {
        D_80122CE0 = 1;
    } else {
        D_80122CE0 = 3;
    }
    actor_clear_range(0);
    hud_cursor_show(0);
    game_state.saved_game_mode = game_state.game_mode;
    game_state.saved_mode_stage = game_state.mode_stage;
    game_state.saved_mode_step = 0x32;
    hizuke_disp_switch(0);
    k_reset(0);
    game_mode_set(GAME_MODE_RPG_BATTLE);
}

void trip_choice_relationship_update(void) {
    D_800CA2DC = (D_800CA2DC - D_800CA2CC) + (D_80122CDC == 1) + ((D_80122CDC == 2) * 3) + 2;
    D_800CA2F4 = D_80122CDC > 0;
    if (D_80122CDC == 0) {
        trip_destination_audio_transition();
    } else {
        game_state.girls[game_state.current_girl].tokimeki += 5;
    }
    check_para_limit();
    if (D_800CA2EC != 0) {
        if (D_80122CDC == 2) {
            game_state.current_girl = GIRL_YOSHIO;
        } else if (D_80122CDC == 1) {
            game_state.current_girl = 0xFF;
        }
    }
    mode_step_next();
}

void trip_destination_lobby_sector_load_gate(void) {
    if (D_800CA2CC == 1) {
        cd_read_sectors(0x3D, 0x801E0000, 0xBD04);
        trip_destination_scene_resource_pointer_setup();
        mode_step_next();
        return;
    }
    mode_step_next();
    mode_step_next();
}

void trip_destination_lobby_actor_initialize(void) {
    if (D_800CA2CC == 1) {
        load_palette(D_8013CB00, 0x11, 0, 1, 0);
        actor_clear(0x61);
        D_80120695 = 0xC;
        D_80120696 = 1;
        D_801206CC = 0x01000000;
        D_80120697 = 0x84;
        D_801206A0 = D_8013CAF4;
        D_801206A4 = D_8013CAF8;
        D_801206C8 = D_8013CAF0;
        D_801206A8 = D_8013CAFC;
        D_801206AA = 0;
        D_801206AC = 0;
        D_8012069A = 1;
        D_801206D7 = 0x11;
        D_801206BA = 0x10;
        D_801206BE = -0x77;
        D_8012069B = 0;
        D_80120699 = 1;
        D_80120698 = 0;
        mode_step_next();
        return;
    }
    mode_step_next();
}

void trip_destination_background_fade_in(void) {
    if (D_800CA2CC == 1) {
        D_8012069B = bg_brightness;
    }
    normal_date_bg_fadein();
}

void trip_destination_choice_background_fade_in(void) {
    if (D_800CA2CC == 1 && D_80122CDC != 0) {
        D_8012069B = bg_brightness;
        if (bg_brightness != 0) {
            D_80120697 |= 0x80;
        }
    }
    normal_date_bg_fadein();
}

s32 trip_destination_background_girl_fade_out(void) {
    normal_date_bggirl_fadeout();
    if (D_800CA2CC == 1) {
        D_8012069B = bg_brightness;
        if ((u8) bg_brightness < 8U) {
            D_80120697 &= 0x7F;
        }
    }
}

void trip_saki_low_status_birthday_route_gate(void) {
    u32 temp_t8;

    temp_t8 = (u8)get_g_zyotai_s(GIRL_SAKI) & 0x7F;
    if (game_state.current_girl != GIRL_SAKI || temp_t8 >= 2U) {
        mode_step_next();
        return;
    }
    game_state.saved_mode_stage = game_state.mode_stage;
    game_state.saved_mode_step = game_state.mode_step + 2;
    game_state.date_girl = GIRL_SAKI;
    mode_stage_set(0xA);
}
