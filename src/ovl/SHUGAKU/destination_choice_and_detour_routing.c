/* destination_choice_and_detour_routing.c: src/ovl/SHUGAKU/801378B0.c:4-38,103-151,236-265,370-439; departure/arrival setup, destination selection, script cursor/choice flow and Yuina/Yukari detours. Original object 0x801378B0-0x80138A60 (config/objects/SHUGAKU.txt). */
#include "common.h"
#include "ovl/SHUGAKU.h"

void trip_departure_scene_resource_pointer_setup(void) {
    D_8013C870 = 0x801D60E0;
    D_8013C874 = 0x801D60E4;
    D_8013C878 = 0x801D60F4;
    D_8013C880 = 0x801D0000;
}

void trip_departure_mode_step_callback_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    TripAlternateSceneCallbacks tbl;

    tbl = D_8013C884;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
}

void trip_trip_day_counter_reset(void) {
    D_800CA2DC = 0;
    mode_step_next();
}

void trip_health_status_departure_message(void) {
    s32 pad; /* FAKE: unused local above buf, puts buf at sp+0x30 as in the original; real source unknown. T-4010 */
    u8 buf[3] = "　";

    if (game_state.player_flags.b[1] & 0xF) {
        if (game_state.substep == 0) {
            D_800CA2DC = 2;
            (**(u8 ***)(D_800CA2D0 + 8))[0] = buf[0];
            (**(u8 ***)(D_800CA2D0 + 8))[1] = buf[1];
            (**(u8 ***)(D_800CA2D0 + 8))[2] = buf[2];
        }
        trip_destination_response_talk_run();
        if (game_state.substep == 0) {
            if (D_800CA2DC == 2) {
                set_kanji_string(-0x48, 0x30, 0, "くぅー、", 0);
                set_kanji_string(-0x48, 0x40, 0, "体の調子が、最悪だぜ。", 0);
                set_kanji_string(-0x48, 0x50, 0, "でも、修学旅行だから", 0);
                set_kanji_string(-0x48, 0x60, 0, "集合場所に行かなきゃ。", 0);
                game_state.substep = 1;
            }
        }
    } else {
        mode_step_next();
    }
}

void trip_departure_talk_and_wait_tick(void) {
    trip_destination_response_talk_run();
    if ((D_800CA2E0 == 2) && (game_state.mode_step_timer.w == 1)) {
        D_80122CFC = 0x3C;
    }
    game_state.mode_step_timer.w += 1;
}

void trip_audio_bank_load_start(void) {
    cd_read_checked = 0;
    audio_bank_load_start(1, 0xC512, 0xC4E7, 0xD557, 0xD508, 0xD4F1);
    if (audio_bank_load_poll() == 1) {
        mode_step_next();
    }
    mode_step_next();
}

void trip_audio_bank_load_poll_retry(void) {
    if (audio_bank_load_poll() == 1) {
        mode_step_next();
    }
    if ((u32) game_state.mode_step_timer.w++ >= 0x400U) {
        cd_command_reset();
        audio_bank_state_reset();
        mode_step_set((game_state.mode_step - 1) & 0xFF);
    }
}

void trip_destination_audio_transition(void) {
    audio_command(0x23);
    audio_command(0x24);
    audio_command(0x2E);
    switch (D_800CA2CC) {
    case 0:
        audio_command(0x200);
        break;
    case 1:
        audio_command(0x201);
        break;
    default:
        audio_command(0x202);
        break;
    }
    mode_step_next();
}

void trip_departure_dialogue_sector_load(void) {
    cd_read_sectors(0x2D, 0x80197000, 0xAF7C);
    trip_girl_script_pointer_table_setup();
    mode_step_next();
}

void trip_departure_scene_ui_and_state_initialize(void) {
    s16 i;

    draw2d3d(1, 0);
    actor_draw_enable(1);
    sprite_tables_clear();
    actor_clear_range(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_system_reset();
    talk_clear();
    strcpy(scene_place_title, "修学旅行");
    k_reset(1);
    game_state.tile_layer_enabled = 0;
    game_state.quads_enabled = 1;
    game_state.screen_brightness = 0x80;
    bg_brightness = 0x80;
    bg_fade_amount = 0;
    hizuke_init();
    message_window_init();
    hud_icon_bar_init();
    for (i = 0; i < 0xF; i++) {
        icon_can_use_set(i, 0);
    }
    hud_icon_bar_update(1);
    parameter_show_init();
    parameter_show_init();
    bg_brightness = 0x80;
    D_800CA2DC = 0;
    D_800CA2E0 = 0;
    addr_init_bustup();
    hizuke_init();
    text_marker_hide();
    talk_names_set(0);
    strcpy(name_code_girl_short, game_state.player_family_name);
    trip_girl_script_pointer_table_setup();
    D_800CA2D0 = D_8013C44C;
    D_800CA2D4 = D_8013C480;
    D_800CA2D8 = D_8013C4B4;
    D_80122CE0 = 0xFF;
    D_800CA2EC = 0;
    for (i = 0; i < 0xA; i++) {
        select_girl_menu_enabled[i] = game_state.girls[i].flags.f.met >= 1;
    }
    select_girl_menu_enabled[10] = 0;
    scene_trip_day_state = 0;
    mode_step_next();
}

void trip_background_decode_branch(void) {
    s32 temp_v0;

    D_80122CE0 = 0xFF;
    cd_read_checked = 0;
    temp_v0 = dec_bg_cd_read(0x3FAD, 0);
    if (temp_v0 == *dec_bg_show) {
        mode_step_next();
        mode_step_next();
        mode_step_next();
    } else if (temp_v0 == (1 - *dec_bg_show)) {
        mode_step_next();
        mode_step_next();
    }
    mode_step_next();
}

s32 trip_background_header_validate_and_decode(void) {
    if ((((u32 *)bg_still_bitstream)[0] & 0xFFFF0000) != 0x38000000 || (((u32 *)bg_still_bitstream)[1] & 0xFF010000) != 0x10000) {
        dec_bg_reset();
        mode_step_next();
        game_state.mode_step -= 3;
        return 0;
    }
    dec_bg_decode_to_vram(0);
    mode_step_next();
}

void trip_background_show_disable(void) {
    dec_bg_show_switch(0);
    mode_step_next();
}

void trip_destination_background_scene_initialize(void) {
    draw2d3d(1, 0);
    actor_draw_enable(1);
    sprite_tables_clear();
    actor_clear_range(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_system_reset();
    talk_clear();
    strcpy(scene_place_title, "修学旅行");
    k_reset(1);
    game_state.tile_layer_enabled = 0;
    game_state.quads_enabled = 1;
    game_state.screen_brightness = 0x80;
    bg_brightness = 0;
    bg_fade_amount = 0;
    hizuke_init();
    message_window_init();
    text_marker_hide();
    switch (D_800CA2CC) {
    case 0:
        bg_read_sub2(0x421C);
        break;
    case 1:
        bg_read_sub2(0x420A);
        break;
    default:
        bg_read_sub2(0x4213);
        break;
    }
    mode_step_next();
}

void trip_destination_background_variant_load(void) {
    u8 sel = D_800CA2CC; /* FAKE: copy of unit-private data, which the original does not promote (T-5010) */

    switch (sel) {
    case 0:
        bg_read_sub2(0x4237);
        break;
    case 1:
        bg_read_sub2(0x422E);
        break;
    default:
        bg_read_sub2(0x4241);
        break;
    }
    mode_step_next();
}

void trip_destination_selection_label_store(void) {
    D_800CA2CC = D_80122CDC;
    switch (D_80122CDC) {
    case 0:
        strcpy(name_code_s, "京都・奈良");
        strcpy(scene_place_title_2, "京都・奈良");
        break;
    case 1:
        strcpy(name_code_s, "沖縄");
        strcpy(scene_place_title_2, "沖縄");
        break;
    case 2:
        strcpy(name_code_s, "北海道");
        strcpy(scene_place_title_2, "北海道");
        break;
    }
    mode_step_next();
}

void trip_destination_city_label_store(void) {
    s32 temp;
    temp = D_800CA2CC;
    switch (temp) {
    case 0:
        strcpy(name_code_s, "京都");
        break;
    case 1:
        strcpy(name_code_s, "沖縄");
        break;
    case 2:
        strcpy(name_code_s, "北海道");
        break;
    }
    mode_step_next();
}

void trip_destination_actor_load_mode_step_timer_dispatch(void) {
    switch (game_state.mode_step_timer.u) {
    case 0:
        trip_destination_actor_sector_load();
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            game_state.mode_step_timer.w += 1;
        }
        break;
    case 2:
        trip_destination_actor_and_palette_setup();
        break;
    case 3:
        trip_destination_actor_slide_update();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void trip_destination_actor_sector_load(void) {
    cd_read_sectors(0xD, 0x801D0000, 0xBB98);
    game_state.mode_step_timer.w += 1;
}

void trip_destination_actor_and_palette_setup(void) {
    u8 temp;
    trip_departure_scene_resource_pointer_setup();
    temp = D_800CA2CC;
    switch (temp) {
    case 0:
        load_palette(D_8013C880, 0x11, 0, 0, 1);
        break;
    case 1:
        load_palette(D_8013C880, 0x11, 0, 0, 0);
        break;
    default:
        load_palette(D_8013C880, 0x11, 0, 0, 2);
        break;
    }
    actor_clear(0x60);
    D_80120651 = 0xA;
    D_80120652 = 1;
    D_80120688 = 0x01000000;
    D_80120653 = 0x84;
    D_8012065C = (u8 *) D_8013C874;
    D_80120660 = (u8 *) D_8013C878;
    D_80120684 = (u8 *) D_8013C870;
    D_80120664 = D_8013C87C;
    D_80120666 = 0;
    D_80120668 = 0;
    D_80120656 = 1;
    D_80120693 = 0x11;
    D_80120676 = 0;
    D_8012067A = -0x78;
    if (D_800CA2CC == 0) {
        D_8012067A = -0x88;
    }
    D_80120657 = 0x80;
    D_80120655 = 1;
    D_80120654 = 0;
    audio_command(0x501);
    game_state.mode_step_timer.w += 1;
}

s16 trip_destination_actor_slide_update(void) {
    if (D_80120676 < -0x1DF) {
        k_reset(1);
        actor_clear(0x60);
        mode_step_next();
    }
    D_80120676 -= 3;
    if (game_state.pad_held & 0x600060) {
        D_80120676 -= 3;
        D_80120658 += 1;
    }
}

void trip_default_destination_talk_run(void) {
    talk_tables = &D_800CA2DC;
    D_800CA138 = &D_800CA2E0;
    D_800CA13C = D_800CA2D0;
    D_800CA140 = D_800CA2D4;
    D_800CA144 = D_800CA2D8;
    talk_run(0xFF, 1, 0);
}

void trip_destination_response_talk_run(void) {
    talk_tables = &D_800CA2DC;
    D_800CA138 = &D_800CA2E0;
    D_800CA13C = D_800CA2D0;
    D_800CA140 = D_800CA2D4;
    D_800CA144 = D_800CA2D8;
    talk_run(D_80122CDC, 1, 0);
}

void trip_birthday_and_special_route_gate(void) {
    /* FAKE: `g = &game_state` breaks uopt's global-load tie so unk_03F gets $a0 and unk_1BC[0].unk_10.w gets $v1; dead entry read `s = D_800CA2CC` puts D_800CA2CC in cvt_pass entry locations (T-24114). */
    GameState *g = &game_state;
    s32 s;

    s = D_800CA2CC;
    s = get_g_zyotai_s(g->current_girl);
    if (((g->girls[0].birthday.w & 0xF) == g->month) && (((u32) (g->girls[0].birthday.w << 0x17) >> 0x1B) == g->day)) {
        g->saved_mode_stage = g->mode_stage + 1;
        g->saved_mode_step = 0;
        g->date_girl = g->current_girl;
        g->current_girl = GIRL_SHIORI;
        mode_stage_set(6);
        return;
    }
    if ((g->month == (g->player_birthday & 0xF)) && (g->day == ((u32) (g->player_birthday << 0x17) >> 0x1B))) {
        g->saved_mode_stage = g->mode_stage + 1;
        g->saved_mode_step = 0;
        g->date_girl = g->current_girl;
        mode_stage_set(7);
        return;
    }
    if ((g->mode_stage == 4) && (g->current_girl == GIRL_YUINA) && (D_800CA2CC == 2)) {
        g->saved_mode_stage = g->mode_stage + 1;
        g->saved_mode_step = 0;
        mode_stage_set(8);
        return;
    }
    if ((g->mode_stage == 2) && (g->current_girl == GIRL_YUKARI) && ((u8) (s & 0x7F) < 2)) {
        g->saved_mode_stage = g->mode_stage + 1;
        g->saved_mode_step = 0;
        mode_stage_set(0xB);
        return;
    }
    mode_step_next();
}

void trip_birthday_route_gate_alt(void) {
    /* FAKE: `g = &game_state` breaks uopt's global-load tie so unk_03F gets $a0 and unk_1BC[0].unk_10.w gets $v1 (T-24114). */
    GameState *g = &game_state;
    s32 s;

    s = get_g_zyotai_s(g->current_girl);
    if (((g->girls[0].birthday.w & 0xF) == g->month) && (((u32) (g->girls[0].birthday.w << 0x17) >> 0x1B) == g->day)) {
        if ((g->current_girl == GIRL_SAKI) && ((u8) (s & 0x7F) < 2)) {
            g->saved_mode_stage = g->mode_stage;
            g->saved_mode_step = g->mode_step + 1;
        } else {
            g->saved_mode_stage = g->mode_stage + 1;
            g->saved_mode_step = 0;
        }
        g->date_girl = g->current_girl;
        g->current_girl = GIRL_SHIORI;
        mode_stage_set(6);
        return;
    }
    if ((g->month == (g->player_birthday & 0xF)) && (g->day == ((u32) (g->player_birthday << 0x17) >> 0x1B))) {
        if ((g->current_girl == GIRL_SAKI) && ((u8) (s & 0x7F) < 2)) {
            g->saved_mode_stage = g->mode_stage;
            g->saved_mode_step = g->mode_step + 1;
        } else {
            g->saved_mode_stage = g->mode_stage + 1;
            g->saved_mode_step = 0;
        }
        g->date_girl = g->current_girl;
        g->current_girl = g->current_girl;
        mode_stage_set(7);
        return;
    }
    mode_step_next();
}
