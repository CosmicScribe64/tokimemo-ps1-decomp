/* festival_frame_control.c: src/ovl/BUNKAKEN/80144F70.c:4; per-frame festival initialization/presentation and calls the mode-stage table dispatcher; includes calendar and scene flow control. Original object 0x80144F70-0x80145E40 (config/objects/BUNKAKEN.txt). */
#define MAIN_API_OVERRIDE_get_g_zyotai_h /* T-18500: tour_event_log_add passes two arguments (main_api.h: one). */
#include "common.h"
#include "ovl/culture_festival_tour.h"

s32 get_g_zyotai_h();

void tour_entry(void) {
    s32 i;
    GameState *g = &game_state;
    TourUnusedBytes sp3C;

    sp3C = tour_unused_table;
    portrait_wink_update();
    switch (festival_scene_phase) {
    case 0:
        audio_command(0xBF);
        audio_bank_load_start(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        scene_screen_reset(1, 0);
        for (i = 0; i < 11; i++) {
            select_girl_menu_enabled[i] = g->girls[i].flags.f.met;
        }
        talk_clear();
        dec_bg_reset();
        message_window_init();
        k_reset(0);
        message_window_set_title((s32) "文化祭");
        message_window_title_show(0);
        hud_mascot_display_switch(0);
        hizuke_init();
        icon_disp_switch(0);
        parameter_disp_switch(0);
        declined_companion_girl = 0xFF;
        culture_festival_return_gate = 0;
        /* FAKE: T-22018, festival_scene_phase read through the culture_festival_return_gate base (one block, aggregate not recovered) so as1 keeps the load after the culture_festival_return_gate store */
        (&culture_festival_return_gate)[10] += 1;
        game_mode_scene_girl = 0xFF;
        break;
    case 1:
        set_dec_bri(0);
        festival_scene_phase += 1;
        break;
    case 2:
        if (audio_bank_load_poll() == 1) {
            audio_command(0x200);
            dec_bg_cd_read(0x4029, 1);
            festival_scene_phase += 1;
        }
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(5U, 0x801E8000, 0x49BA);
            message_window_title_show(1);
            festival_scene_phase += 1;
        }
        break;
    case 4:
        normal_date_bg_fadein();
        tour_phase_next_on_step();
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            text_marker_hide();
            talk_names_set(0U);
            tour_script_common();
            talk_group = 0;
            talk_line = 0;
            talk_text_table = (s32) tour_script_text;
            talk_command_table = (s32) tour_script_commands;
            talk_voice_table = (s32) tour_script_voices;
            festival_scene_phase += 1;
        }
        break;
    case 6:
        normal_date_speak();
        tour_phase_next_on_step();
        break;
    case 7:
        tour_stage_run();
        break;
    case 8:
        if (((g->girls[0].birthday.w & 0xF) == g->month) && (((u32) (g->girls[0].birthday.w << 0x17) >> 0x1B) == g->day) && !(g->day_event_done_bits & 1) && ((game_mode_scene_girl == 0) || (game_mode_scene_girl == 0xB))) {
            festival_scene_phase = 9;
            g->mode_stage = 0;
            g->mode_step = 0;
        } else {
            /* FAKE: T-22018, the original stores the heroine index it just read back to game_mode_scene_girl before the call */
            if (((g->player_birthday & 0xF) == g->month) && (((u32) (g->player_birthday << 0x17) >> 0x1B) == g->day) && !(g->day_event_done_bits & 4) && (game_mode_scene_girl >= 4) && (game_mode_scene_girl < 0xB) && ((game_mode_scene_girl = game_mode_scene_girl, girl_state_rank(game_mode_scene_girl) < 2) || ((game_mode_scene_girl == 0xB) && (girl_state_rank(0) < 2)))) {
                festival_scene_phase = 0xA;
                g->mode_stage = 0;
                g->mode_step = 0;
            } else if (g->mode_stage == 0) {
                audio_command(0xBF);
                hud_day_bgm_load();
                mode_stage_next();
            } else if (audio_bank_load_poll() == 1) {
                tour_apply_points();
                if (game_mode_scene_girl != 0xFF) {
                    game_mode_scene_girl = game_mode_scene_girl; /* FAKE: T-22018, same store-back before the call */
                    if (girl_state_rank(game_mode_scene_girl) < 4) {
                        tour_event_log_add();
                    }
                }
                for (i = 0; i < 0x40; i++) {
                    ((s32 *)overlay_scratch)[i] = 0;
                }
                talk_clear();
                dec_bg_reset();
                set_dec_bri(0);
                day_event_next(1);
            }
        }
        break;
    case 9:
        tour_phase_shiori_birthday();
        break;
    case 10:
        tour_phase_player_birthday();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
    if (culture_festival_return_gate == 0) {
        Default_Disp();
    }
}

void tour_phase_next_on_step(void) {
    if (game_state.mode_step != 0) {
        game_state.mode_step = 0;
        festival_scene_phase += 1;
    }
}

void tour_finish_visit(void) {
    game_state.mode_stage = 0;
    game_state.mode_step = 0;
    festival_scene_phase = 8;
}

void tour_apply_points(void) {
    GameState *state = &game_state;
    u8 pad; /* FAKE: T-18500, an unused byte above tokimeki_gain puts tokimeki_gain at sp+0xDA, the home its uninitialized default read uses */
    u8 tokimeki_gain;
    TourPoints1995 points_1995;
    TourPointsLaterYears points_1996;
    TourPointsLaterYears points_1997;
    s32 year_index;

    points_1995 = tour_points_1995;
    points_1996 = tour_points_1996;
    points_1997 = tour_points_1997;
    year_index = (state->year - 0x5F) & 0xFF;
    if (game_mode_scene_girl == 0xB) {
        state->girls[0].affection += 2;
        switch (year_index) {
        case 0:
            tokimeki_gain = points_1995.codes[0][culture_exhibit_club];
            break;
        case 1:
            tokimeki_gain = points_1996.codes[0][culture_exhibit_club];
            break;
        case 2:
            tokimeki_gain = points_1997.codes[0][culture_exhibit_club];
            break;
        }
        if (tokimeki_gain != 0) {
            tokimeki_gain--;
        }
        state->girls[0].tokimeki += tokimeki_gain;
    } else if (game_mode_scene_girl < 0xB) {
        state->girls[game_mode_scene_girl].affection += 2;
        switch (year_index) {
        case 0:
            tokimeki_gain = points_1995.codes[game_mode_scene_girl][culture_exhibit_club];
            break;
        case 1:
            tokimeki_gain = points_1996.codes[game_mode_scene_girl][culture_exhibit_club];
            break;
        case 2:
            tokimeki_gain = points_1997.codes[game_mode_scene_girl][culture_exhibit_club];
            break;
        }
        if (tokimeki_gain != 0) {
            tokimeki_gain--;
        }
        state->girls[game_mode_scene_girl].tokimeki += tokimeki_gain;
    }
    if (declined_companion_girl != 0xFF) {
        state->girls[declined_companion_girl].affection -= 0xA;
        state->girls[declined_companion_girl].syoushin += 0xA;
    }
}

void tour_window_title_set(void) {
    k_reset(0);
    switch (culture_exhibit_club) {
    case 0:
        switch (game_state.year) {
        case 0x5F:
            message_window_set_title((s32) "詩集展示");
            break;
        case 0x60:
            message_window_set_title((s32) "文集展示");
            break;
        case 0x61:
            message_window_set_title((s32) "弁論大会");
            break;
        }
        break;
    case 1:
        message_window_set_title((s32) "演劇部公演");
        break;
    case 2:
        message_window_set_title((s32) "実験ショー");
        break;
    case 3:
        switch (game_state.year) {
        case 0x5F:
            message_window_set_title((s32) "パソコン占い");
            break;
        case 0x60:
            message_window_set_title((s32) "ゲーム");
            break;
        case 0x61:
            message_window_set_title((s32) "実験ショー");
            break;
        }
        break;
    case 4:
        switch (game_state.year) {
        case 0x5F:
            message_window_set_title((s32) "似顔絵");
            break;
        case 0x60:
            message_window_set_title((s32) "公開デッサン");
            break;
        case 0x61:
            message_window_set_title((s32) "油絵展示");
            break;
        }
        break;
    case 5:
        message_window_set_title((s32) "演奏会");
        break;
    }
    k_disp_start(1);
}

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/culture_festival_tour/festival_frame_control.rodata", tour_title_backstage);

void tour_title_backstage_show(void) {
    k_reset(0);
    message_window_set_title(&tour_title_backstage);
    k_disp_start(1);
}

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/culture_festival_tour/festival_frame_control.rodata", tour_title_blank);

void tour_title_clear(void) {
    k_reset(0);
    message_window_set_title(&tour_title_blank);
    message_window_title_show(0);
}

void tour_event_log_add(void) {
    GameState *g = &game_state;
    u8 v;

    if ((girl_state_rank(game_mode_scene_girl) != 4) && (declined_companion_girl != game_mode_scene_girl)) {
        switch (culture_exhibit_club) {
        case 0:
            if ((((u8) g->girls[0].flags.b[2] >> 4) == 0) && (game_mode_scene_girl == 0)) {
                v = 0x3;
            } else if ((((u8) g->girls[1].flags.b[2] >> 4) == 0) && (game_mode_scene_girl == 1)) {
                v = 0x3;
            } else {
                v = 0x4;
            }
            break;
        case 1:
            if ((((u8) g->girls[0].flags.b[2] >> 4) == 1) && (game_mode_scene_girl == 0)) {
                v = 0x7;
            } else if ((((u8) g->girls[1].flags.b[2] >> 4) == 1) && (game_mode_scene_girl == 1)) {
                v = 0x7;
            } else {
                v = 0x8;
            }
            break;
        case 2:
            if ((((u8) g->girls[0].flags.b[2] >> 4) == 2) && (game_mode_scene_girl == 0)) {
                v = 0xB;
            } else if ((((u8) g->girls[2].flags.b[2] >> 4) == 2) && (game_mode_scene_girl == 2)) {
                v = 0xB;
            } else {
                v = 0xC;
            }
            break;
        case 3:
            if ((((u8) g->girls[0].flags.b[2] >> 4) == 3) && (game_mode_scene_girl == 0)) {
                v = 0xF;
            } else if ((((u8) g->girls[2].flags.b[2] >> 4) == 3) && (game_mode_scene_girl == 2)) {
                v = 0xF;
            } else {
                v = 0x10;
            }
            break;
        case 4:
            if ((((u8) g->girls[0].flags.b[2] >> 4) == 4) && (game_mode_scene_girl == 0)) {
                v = 0x13;
            } else if ((((u8) g->girls[3].flags.b[2] >> 4) == 4) && (game_mode_scene_girl == 3)) {
                v = 0x13;
            } else {
                v = 0x14;
            }
            break;
        case 5:
            if ((((u8) g->girls[0].flags.b[2] >> 4) == 5) && (game_mode_scene_girl == 0)) {
                v = 0x17;
            } else if ((((u8) g->girls[3].flags.b[2] >> 4) == 5) && (game_mode_scene_girl == 3)) {
                v = 0x17;
            } else {
                v = 0x18;
            }
            break;
        }
        if (game_mode_scene_girl == 0xB) {
            g->event_log[g->event_log_count].girl = 0;
            g->event_log[g->event_log_count].rank = get_g_zyotai_h(GIRL_SHIORI, v);
        } else {
            g->event_log[g->event_log_count].girl = game_mode_scene_girl;
            g->event_log[g->event_log_count].rank = get_g_zyotai_h(game_mode_scene_girl, v);
        }
        g->event_log[g->event_log_count].scene = 0x14;
        g->event_log[g->event_log_count].flag = v;
        g->event_log[g->event_log_count].year = g->year;
        g->event_log[g->event_log_count].month = g->month;
        g->event_log[g->event_log_count].day = g->day;
        g->event_log[g->event_log_count].weekday = g->weekday;
        g->event_log_count++;
        if (g->event_log_count >= 0xFF) {
            g->event_log_count = 0xFE;
        }
    }
}
