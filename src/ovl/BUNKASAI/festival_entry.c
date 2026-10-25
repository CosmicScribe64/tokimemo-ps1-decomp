/* festival_entry.c: src/ovl/BUNKASAI/80142700.c:4: entry point and phase machine, result, event log, banner titles. Original object 0x80142700-0x80143770 (config/objects/BUNKASAI.txt). */
#include "common.h"
#include "ovl/BUNKASAI.h"

void culture_festival_mode_handler(void) {
    s32 i;
    GameState *g = &game_state;
    CultureUnusedBytes sp3C;

    sp3C = culture_unused_bytes;
    portrait_wink_update();
    switch (D_80122EC0) {
    case 0:
        audio_command(0xBF);
        audio_bank_load_start(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        scene_screen_reset(1, 0);
        talk_clear();
        for (i = 0; i < 11; i++) {
            select_girl_menu_enabled[i] = g->girls[i].flags.f.met;
        }
        dec_bg_reset();
        message_window_init();
        k_reset(0);
        message_window_set_title((s32) "文化祭");
        message_window_title_show(0);
        hud_mascot_display_switch(0);
        hizuke_init();
        icon_disp_switch(0);
        parameter_disp_switch(0);
        D_80122EBC = 0xFF;
        D_80122EA4 = 0xFF;
        D_80122E98 = 0;
        /* FAKE: shared D_80122E98 base retains the state load after the clear; block layout unknown (T-24030). */
        (&D_80122E98)[10] += 1;
        break;
    case 1:
        set_dec_bri(0);
        D_80122EC0 += 1;
        break;
    case 2:
        if (audio_bank_load_poll() == 1) {
            audio_command(0x200);
            dec_bg_cd_read(0x4029, 1);
            D_80122EC0 += 1;
        }
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(5U, 0x801E8000, 0x49BA);
            message_window_title_show(1);
            D_80122EC0 += 1;
        }
        break;
    case 4:
        normal_date_bg_fadein();
        culture_phase_next_on_step();
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            text_marker_hide();
            talk_names_set(0U);
            culture_script_set_intro();
            talk_group = 0;
            talk_line = 0;
            talk_text_table = (s32) culture_script_text_table;
            talk_command_table = (s32) culture_script_command_table;
            talk_voice_table = (s32) culture_script_voice_table;
            D_80122EC0 += 1;
        }
        break;
    case 6:
        normal_date_speak();
        culture_phase_next_on_step();
        break;
    case 7:
        culture_stage_dispatch();
        break;
    case 8:
        if ((g->month == (g->girls[0].birthday.w & 0xF)) && (g->day == ((u32) (g->girls[0].birthday.w << 0x17) >> 0x1B)) && !(g->day_event_done_bits & 1)) {
            D_80122EC0 = 9;
            g->mode_stage = 0;
            g->mode_step = 0;
        } else if (g->mode_stage == 0) {
            audio_command(0xBF);
            hud_day_bgm_load();
            mode_stage_next();
        } else if (audio_bank_load_poll() == 1) {
            culture_result_apply();
            culture_event_log_add();
            for (i = 0; i < 64; i++) {
                ((s32 *)D_80122DD0)[i] = 0;
            }
            talk_clear();
            dec_bg_reset();
            set_dec_bri(0);
            day_event_next(1);
        }
        break;
    case 9:
        culture_birthday_scene();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
    if (D_80122E98 == 0) {
        Default_Disp();
    }
}

void culture_phase_next_on_step(void) {
    if (game_state.mode_step != 0) {
        game_state.mode_step = 0;
        D_80122EC0 += 1;
    }
}

void culture_scene_end(void) {
    game_state.mode_stage = 0;
    game_state.mode_step = 0;
    D_80122EC0 = 8;
}

/* FAKE: s32 return type reserves $v0 in uopt so bonus gets $a3; pad1/pad2 place amount at sp+0x11C; (&g->unk_0FC) indexes across the 4-entry Sub_0FC block. T-24094 */
s32 culture_result_apply(void) {
    s32 i;
    GameState *g = &game_state;
    u8 pad1;
    u8 pad2;
    u8 val;
    u8 amount;
    CultureClubParamIndex sel;
    CultureTokimekiGains first;
    CultureTokimekiGains second;
    CultureTokimekiGains third;
    u32 bonus;

    sel = culture_club_param_index;
    first = culture_tokimeki_gain_95;
    second = culture_tokimeki_gain_96;
    third = culture_tokimeki_gain_97;
    bonus = ((u32) (g->clubs[g->player_flags.h >> 12].practice_points >> 3) / (u32) (g->year - 94) +
             (&g->param_condition)[sel.param_index[g->player_flags.h >> 12]].value / 20) & 0xFF;
    if (bonus >= 11) {
        bonus = 10;
    }
    switch ((g->year - 95) & 0xFF) {
    case 0:
        for (i = 0; i < 13; i++) {
            val = first.gain[i][g->player_flags.h >> 12] >> 1;
            g->girls[i].tokimeki += bonus;
            g->girls[i].tokimeki += val;
        }
        amount = first.gain[D_80122EBC][g->player_flags.h >> 12];
        break;
    case 1:
        for (i = 0; i < 13; i++) {
            val = second.gain[i][g->player_flags.h >> 12] >> 1;
            g->girls[i].tokimeki += bonus;
            g->girls[i].tokimeki += val;
        }
        amount = second.gain[D_80122EBC][g->player_flags.h >> 12];
        break;
    case 2:
        for (i = 0; i < 13; i++) {
            val = third.gain[i][g->player_flags.h >> 12] >> 1;
            g->girls[i].tokimeki += bonus;
            g->girls[i].tokimeki += val;
        }
        amount = third.gain[D_80122EBC][g->player_flags.h >> 12];
        break;
    }
    if (amount != 0) {
        amount--;
    }
    g->girls[D_80122EBC].affection += 2;
    g->girls[D_80122EBC].tokimeki += amount;
}

void culture_exhibit_title_show(void) {
    k_reset(0);
    switch (D_80122EB8) {
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

INCLUDE_RODATA("asm/ovl/BUNKASAI/data/BUNKASAI/festival_entry.rodata", culture_backstage_title);

void culture_backstage_title_show(void) {
    k_reset(0);
    message_window_set_title(&culture_backstage_title);
    k_disp_start(1);
}

void culture_event_log_add(void) {
    /* FAKE: 4-byte stack pad places sp2B at sp+0x2B instead of sp+0x2F. T-24094 */
    s32 pad;
    u8 sp2B;
    GameState *game = &game_state;

    switch (game->player_flags.h >> 12) {
    case 0:
        if ((((u8) game->girls[0].flags.b[2] >> 4) == 0) ||
            ((((u8) game->girls[1].flags.b[2] >> 4) == 0) && (game->girls[1].flags.f.met == 1))) {
            sp2B = 2;
        } else {
            sp2B = 1;
        }
        break;
    case 1:
        if ((((u8) game->girls[0].flags.b[2] >> 4) == 1) ||
            ((((u8) game->girls[1].flags.b[2] >> 4) == 1) && (game->girls[1].flags.f.met == 1))) {
            sp2B = 6;
        } else {
            sp2B = 5;
        }
        break;
    case 2:
        if ((((u8) game->girls[0].flags.b[2] >> 4) == 2) ||
            ((((u8) game->girls[2].flags.b[2] >> 4) == 2) && (game->girls[2].flags.f.met == 1))) {
            sp2B = 10;
        } else {
            sp2B = 9;
        }
        break;
    case 3:
        if ((((u8) game->girls[0].flags.b[2] >> 4) == 3) ||
            ((((u8) game->girls[2].flags.b[2] >> 4) == 3) && (game->girls[2].flags.f.met == 1))) {
            sp2B = 14;
        } else {
            sp2B = 13;
        }
        break;
    case 4:
        if ((((u8) game->girls[0].flags.b[2] >> 4) == 4) ||
            ((((u8) game->girls[3].flags.b[2] >> 4) == 4) && (game->girls[3].flags.f.met == 1))) {
            sp2B = 18;
        } else {
            if ((D_80122EBC == 10) && (girl_state_rank(10) == 4)) {
                sp2B = 17;
                return;
            }
            sp2B = 17;
        }
        break;
    case 5:
        if ((((u8) game->girls[0].flags.b[2] >> 4) == 5) ||
            ((((u8) game->girls[3].flags.b[2] >> 4) == 5) && (game->girls[3].flags.f.met == 1))) {
            sp2B = 22;
        } else {
            sp2B = 21;
        }
        break;
    }
    game->event_log[game->event_log_count].girl = D_80122EBC;
    game->event_log[game->event_log_count].scene = 20;
    game->event_log[game->event_log_count].flag = sp2B;
    game->event_log[game->event_log_count].rank = get_g_zyotai_h(D_80122EBC);
    game->event_log[game->event_log_count].year = game->year;
    game->event_log[game->event_log_count].month = game->month;
    game->event_log[game->event_log_count].day = game->day;
    game->event_log[game->event_log_count].weekday = game->weekday;
    if (++game->event_log_count >= 0xFF) {
        game->event_log_count = 0xFE;
    }
}
