#define MAIN_API_OVERRIDE_get_g_zyotai_h /* T-18500: func_80145A64 passes two arguments (main_api.h: one). */
#include "common.h"
#include "ovl/BUNKAKEN.h"

s32 get_g_zyotai_h();

void func_80144F70(void) {
    s32 i;
    GameState *g = &game_state;
    BunkakenBytes11 sp3C;

    sp3C = D_80155F90;
    portrait_wink_update();
    switch (D_80122EC0) {
    case 0:
        audio_command(0xBF);
        audio_bank_load_start(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        scene_screen_reset(1, 0);
        for (i = 0; i < 11; i++) {
            D_800CA2B0[i] = g->girls[i].flags.f.met;
        }
        talk_clear();
        dec_bg_reset();
        message_window_init();
        k_reset(0);
        message_window_set_title((s32) "文化祭");
        message_window_title_show(0);
        func_8006764C(0);
        hizuke_init();
        icon_disp_switch(0);
        parameter_disp_switch(0);
        D_80122EA4 = 0xFF;
        D_80122E98 = 0;
        /* FAKE: T-22018, D_80122EC0 read through the D_80122E98 base (one block, aggregate not recovered) so as1 keeps the load after the D_80122E98 store */
        (&D_80122E98)[10] += 1;
        D_80122EBC = 0xFF;
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
        func_80145500();
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            text_marker_hide();
            talk_names_set(0U);
            func_801324E0();
            talk_group = 0;
            talk_line = 0;
            talk_text_table = (s32) D_80155E90;
            talk_command_table = (s32) D_80155E94;
            talk_voice_table = (s32) D_80155E98;
            D_80122EC0 += 1;
        }
        break;
    case 6:
        normal_date_speak();
        func_80145500();
        break;
    case 7:
        func_80145E40();
        break;
    case 8:
        if (((g->girls[0].birthday.w & 0xF) == g->month) && (((u32) (g->girls[0].birthday.w << 0x17) >> 0x1B) == g->day) && !(g->day_event_done_bits & 1) && ((D_80122EBC == 0) || (D_80122EBC == 0xB))) {
            D_80122EC0 = 9;
            g->mode_stage = 0;
            g->mode_step = 0;
        } else {
            /* FAKE: T-22018, the original stores the heroine index it just read back to D_80122EBC before the call */
            if (((g->player_birthday & 0xF) == g->month) && (((u32) (g->player_birthday << 0x17) >> 0x1B) == g->day) && !(g->day_event_done_bits & 4) && (D_80122EBC >= 4) && (D_80122EBC < 0xB) && ((D_80122EBC = D_80122EBC, girl_state_rank(D_80122EBC) < 2) || ((D_80122EBC == 0xB) && (girl_state_rank(0) < 2)))) {
                D_80122EC0 = 0xA;
                g->mode_stage = 0;
                g->mode_step = 0;
            } else if (g->mode_stage == 0) {
                audio_command(0xBF);
                func_800674B0();
                mode_stage_next();
            } else if (audio_bank_load_poll() == 1) {
                func_80145550();
                if (D_80122EBC != 0xFF) {
                    D_80122EBC = D_80122EBC; /* FAKE: T-22018, same store-back before the call */
                    if (girl_state_rank(D_80122EBC) < 4) {
                        func_80145A64();
                    }
                }
                for (i = 0; i < 0x40; i++) {
                    ((s32 *)D_80122DD0)[i] = 0;
                }
                talk_clear();
                dec_bg_reset();
                set_dec_bri(0);
                day_event_next(1);
            }
        }
        break;
    case 9:
        func_80149144();
        break;
    case 10:
        func_8014A0E0();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
    if (D_80122E98 == 0) {
        Default_Disp();
    }
}

void func_80145500(void) {
    if (game_state.mode_step != 0) {
        game_state.mode_step = 0;
        D_80122EC0 += 1;
    }
}

void func_80145530(void) {
    game_state.mode_stage = 0;
    game_state.mode_step = 0;
    D_80122EC0 = 8;
}

void func_80145550(void) {
    GameState *g = &game_state;
    u8 pad; /* FAKE: T-18500, an unused byte above v puts v at sp+0xDA, the home its uninitialized default read uses */
    u8 v;
    BunkakenBytes9x6 a;
    BunkakenBytes11x6 b;
    BunkakenBytes11x6 c;
    s32 sel;

    a = D_80155F9C;
    b = D_80155FD4;
    c = D_80156018;
    sel = (g->year - 0x5F) & 0xFF;
    if (D_80122EBC == 0xB) {
        g->girls[0].affection += 2;
        switch (sel) {
        case 0:
            v = a.b[0][D_80122EB8];
            break;
        case 1:
            v = b.b[0][D_80122EB8];
            break;
        case 2:
            v = c.b[0][D_80122EB8];
            break;
        }
        if (v != 0) {
            v--;
        }
        g->girls[0].tokimeki += v;
    } else if (D_80122EBC < 0xB) {
        g->girls[D_80122EBC].affection += 2;
        switch (sel) {
        case 0:
            v = a.b[D_80122EBC][D_80122EB8];
            break;
        case 1:
            v = b.b[D_80122EBC][D_80122EB8];
            break;
        case 2:
            v = c.b[D_80122EBC][D_80122EB8];
            break;
        }
        if (v != 0) {
            v--;
        }
        g->girls[D_80122EBC].tokimeki += v;
    }
    if (D_80122EA4 != 0xFF) {
        g->girls[D_80122EA4].affection -= 0xA;
        g->girls[D_80122EA4].syoushin += 0xA;
    }
}

void func_80145844(void) {
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

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/80144F70.rodata", D_80154B84);

void func_801459FC(void) {
    k_reset(0);
    message_window_set_title(&D_80154B84);
    k_disp_start(1);
}

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/80144F70.rodata", D_80154B8C);

void func_80145A30(void) {
    k_reset(0);
    message_window_set_title(&D_80154B8C);
    message_window_title_show(0);
}

void func_80145A64(void) {
    GameState *g = &game_state;
    u8 v;

    if ((girl_state_rank(D_80122EBC) != 4) && (D_80122EA4 != D_80122EBC)) {
        switch (D_80122EB8) {
        case 0:
            if ((((u8) g->girls[0].flags.b[2] >> 4) == 0) && (D_80122EBC == 0)) {
                v = 0x3;
            } else if ((((u8) g->girls[1].flags.b[2] >> 4) == 0) && (D_80122EBC == 1)) {
                v = 0x3;
            } else {
                v = 0x4;
            }
            break;
        case 1:
            if ((((u8) g->girls[0].flags.b[2] >> 4) == 1) && (D_80122EBC == 0)) {
                v = 0x7;
            } else if ((((u8) g->girls[1].flags.b[2] >> 4) == 1) && (D_80122EBC == 1)) {
                v = 0x7;
            } else {
                v = 0x8;
            }
            break;
        case 2:
            if ((((u8) g->girls[0].flags.b[2] >> 4) == 2) && (D_80122EBC == 0)) {
                v = 0xB;
            } else if ((((u8) g->girls[2].flags.b[2] >> 4) == 2) && (D_80122EBC == 2)) {
                v = 0xB;
            } else {
                v = 0xC;
            }
            break;
        case 3:
            if ((((u8) g->girls[0].flags.b[2] >> 4) == 3) && (D_80122EBC == 0)) {
                v = 0xF;
            } else if ((((u8) g->girls[2].flags.b[2] >> 4) == 3) && (D_80122EBC == 2)) {
                v = 0xF;
            } else {
                v = 0x10;
            }
            break;
        case 4:
            if ((((u8) g->girls[0].flags.b[2] >> 4) == 4) && (D_80122EBC == 0)) {
                v = 0x13;
            } else if ((((u8) g->girls[3].flags.b[2] >> 4) == 4) && (D_80122EBC == 3)) {
                v = 0x13;
            } else {
                v = 0x14;
            }
            break;
        case 5:
            if ((((u8) g->girls[0].flags.b[2] >> 4) == 5) && (D_80122EBC == 0)) {
                v = 0x17;
            } else if ((((u8) g->girls[3].flags.b[2] >> 4) == 5) && (D_80122EBC == 3)) {
                v = 0x17;
            } else {
                v = 0x18;
            }
            break;
        }
        if (D_80122EBC == 0xB) {
            g->event_log[g->event_log_count].girl = 0;
            g->event_log[g->event_log_count].rank = get_g_zyotai_h(GIRL_SHIORI, v);
        } else {
            g->event_log[g->event_log_count].girl = D_80122EBC;
            g->event_log[g->event_log_count].rank = get_g_zyotai_h(D_80122EBC, v);
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
