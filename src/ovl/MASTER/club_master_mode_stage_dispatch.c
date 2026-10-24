/* club_master_mode_stage_dispatch.c: src/ovl/MASTER/80132140.c:6-14; indexes the four-function callback table by game_state.mode_stage and owns its entry/load/talk route helpers; wiki/spec/o-master-ending.md describes the inspected flow. Original object 0x80132140-0x80133680 (config/objects/MASTER.txt). */
#define MAIN_API_OVERRIDE_talk_names_set /* T-25130: called without a prototype, as an implicit int; func_801326B4 passes the full word D_8013C2E0 (main_api.h narrows it to u8, lbu), and a void view moves its v0/v1 ranges (39 rows). */
#include "ovl/MASTER.h"

s32 talk_names_set();

void func_80132140(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl4 tbl;

    tbl = D_8013C2E4;
    idx = game_state.mode_stage;
    tbl.f[idx]();
}

void func_801321B0(void) {
    switch (game_state.mode_step) {
    case 0:
        audio_command(0xBF);
        audio_bank_load_start(1, 0xC043, 0xC01D, 0xC982, 0xC935, 0xC91B);
        scene_screen_reset(1, 0);
        dec_bg_reset();
        set_dec_bri(0);
        talk_clear();
        message_window_init();
        hizuke_init();
        parameter_disp_switch(0);
        icon_disp_switch(0);
        func_8006764C(0);
        mode_step_next();
        break;
    case 1:
        if (audio_bank_load_poll() == 1) {
            k_reset(0);
            switch ((u16) game_state.player_flags.h >> 0xC) {
            case 0:
                message_window_set_title((s32) "植木賞授賞式");
                dec_bg_cd_read(0x4644, 1);
                break;
            case 1:
                message_window_set_title((s32) "演劇グランプリ");
                dec_bg_cd_read(0x464D, 1);
                break;
            case 2:
                message_window_set_title((s32) "ノーベル賞受賞");
                dec_bg_cd_read(0x4656, 1);
                break;
            case 3:
                message_window_set_title((s32) "ゲーム大賞");
                dec_bg_cd_read(0x465F, 1);
                break;
            case 4:
                message_window_set_title((s32) "目展大賞");
                dec_bg_cd_read(0x4669, 1);
                break;
            case 5:
                message_window_set_title((s32) "コンクール優勝");
                dec_bg_cd_read(0x4673, 1);
                break;
            default:
                game_mode_reset_to_title();
                break;
            }
            message_window_title_show(0);
            audio_command(0x202);
            mode_step_next();
        }
        break;
    case 2:
        mode_step_next();
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            talk_names_set(0U);
            text_marker_hide();
            message_window_title_show(1);
            mode_step_next();
        }
        break;
    case 4:
        normal_date_bg_fadein();
        break;
    case 5:
        mode_stage_next();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
    Default_Disp();
}

void func_80132444(void) {
    u32 temp_t8;
    u8 temp_t6;

    temp_t6 = game_state.mode_step;
    switch (temp_t6) {
    case 0:
        temp_t8 = (u16) game_state.player_flags.h >> 0xC;
        switch (temp_t8) {
        case 0:
            cd_read_sectors(3U, 0x801E8000, 0xA9DE);
            func_80132104();
            break;
        case 1:
            cd_read_sectors(3U, 0x801E8000, 0xA9B4);
            func_80132000();
            break;
        case 2:
            cd_read_sectors(3U, 0x801E8000, 0xA9BF);
            func_8013209C();
            break;
        case 3:
            cd_read_sectors(3U, 0x801E8000, 0xA9B7);
            func_80132034();
            break;
        case 4:
            cd_read_sectors(3U, 0x801E8000, 0xA9BC);
            func_80132068();
            break;
        case 5:
            cd_read_sectors(3U, 0x801E8000, 0xA9C2);
            func_801320D0();
            break;
        default:
            game_mode_reset_to_title();
            break;
        }
        talk_text_table = (s32) D_8013C2D0;
        talk_command_table = (s32) D_8013C2D4;
        talk_voice_table = (s32) D_8013C2D8;
        mode_step_next();
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            talk_group = 0;
            talk_line = 0;
            mode_step_next();
        }
        break;
    case 2:
        normal_date_speak();
        break;
    case 3:
        normal_date_bg_fadeout();
        break;
    case 4:
        if (((u16) game_state.player_flags.h >> 0xC) == 3) {
            audio_command(0xBF);
            /* byte view: the original does lbu/ori/sb on D_800E7D50 (main_api.h: s32), T-22015.
             * Two or more ORs make uopt promote the byte to $v1 (folded into one ori 3).
             * FAKE: T-24208, the third OR: its ugen temporaries move the copies to $t1..$t3 */
            *(u8 *)&D_800E7D50 |= 1;
            *(u8 *)&D_800E7D50 |= 2;
            *(u8 *)&D_800E7D50 |= 1;
            game_state.saved_game_mode = game_state.game_mode;
            game_state.saved_mode_stage = game_state.mode_stage + 1;
            game_state.saved_mode_step = 0;
            game_mode_set(GAME_MODE_SHOOTER);
        } else {
            mode_stage_next();
        }
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
    Default_Disp();
}

/* FAKE: T-25130. An always-false test of the mode halfword (D_800E6374) right after each call in the
 * case 4 arms. It adds one use of the global in each arm, which makes the promoted global profitable,
 * so uopt keeps it in $a0 and reloads it there after the call, as the original does; ugen folds the
 * test away and no code is emitted. Removing any one of the six gives 229 register rows. */
#define KEEP_MODE_LIVE() if ((u32) game_state.player_flags.h < 0) {}

void func_801326B4(void) {
    u32 mode;
    s32 i;
    s32 best;
    MasterWords13 sp1A8;
    MasterWords13 sp174;
    MasterWords13 sp140;
    MasterWords13 sp10C;
    MasterWords13 spD8;
    MasterWords13 spA4;
    MasterWords13 sp70;
    MasterWords13 sp3C;

    sp1A8 = D_8013C2F4;
    sp174 = D_8013C328;
    sp140 = D_8013C35C;
    sp10C = D_8013C390;
    spD8 = D_8013C3C4;
    spA4 = D_8013C3F8;
    sp70 = D_8013C42C;
    sp3C = D_8013C460;
    portrait_wink_update();
    switch (game_state.mode_step) {
    case 0:
        if (game_state.substep == 0) {
            set_dec_bri(0);
            cursor_actor_init(0);
            text_marker_hide();
            cursor_show(0);
            hizuke_init();
            message_window_init();
            best = func_8008647C();
            if ((u8) game_state.girls[0xC].flags.b[3] >= 0x41U) {
                D_8013C2E0 = 0xC;
            } else {
                if (girl_state_rank(best) < 2) {
                    D_8013C2E0 = best;
                    for (i = 0; i < 0xB; i++) {
                        if ((((u16) game_state.player_flags.h >> 0xC) == ((u8) game_state.girls[i].flags.b[2] >> 4)) && (((u32) (game_state.girls[i].flags.w << 0x1E) >> 0x1F) == 1)) {
                            if (girl_state_rank(i) < 2) {
                                D_8013C2E0 = i;
                            }
                        }
                    }
                } else if ((u8) game_state.girls[0xB].flags.b[3] >= 2U) {
                    D_8013C2E0 = 0xB;
                } else {
                    D_8013C2E0 = 0xFF;
                    mode_step_set(0xE);
                }
            }
            if (D_8013C2E0 != 0xFF) {
                audio_command(0xBF);
                audio_bank_load_start(1, 0xC043, 0xC01D, D_800B3688[D_8013C2E0], D_800B36C8[D_8013C2E0], D_800B3708[D_8013C2E0]);
            }
            game_state.substep = game_state.substep + 1;
        } else if (audio_bank_load_poll() == 1) {
            k_reset(0);
            if ((((u16) game_state.player_flags.h >> 0xC) == 1) && (D_8013C2E0 == 0) && (((u8) game_state.girls[0].flags.h[1] >> 4) == 1)) {
                message_window_set_title((s32) "近所の公園");
                D_80125CA0 = 0xC0;
                dec_bg_cd_read(0x42D8, 1);
            } else if (((((u16) game_state.player_flags.h >> 0xC) == 1) && (D_8013C2E0 == 1) && (((u8) game_state.girls[1].flags.h[1] >> 4) == 1)) || (((u16) game_state.player_flags.h >> 0xC) == 5) || (((u16) game_state.player_flags.h >> 0xC) == 0)) {
                message_window_set_title((s32) "舞台裏");
                D_80125CA0 = 0xC0;
                dec_bg_cd_read(0x40F4, 1);
            } else {
                message_window_set_title((s32) "自宅前");
                D_80125CA0 = 0xC0;
                dec_bg_cd_read(0x414C, 1);
            }
            mode_step_next();
        }
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            talk_names_set(D_8013C2E0);
            if ((((u16) game_state.player_flags.h >> 0xC) == 1) && (D_8013C2E0 == 0) && (((u8) game_state.girls[0].flags.h[1] >> 4) == 1)) {
                portrait_read_start(0x544A);
            } else if ((((u16) game_state.player_flags.h >> 0xC) == 1) && (D_8013C2E0 == 1) && (((u8) game_state.girls[1].flags.h[1] >> 4) == 1)) {
                portrait_read_start(0x5D6E);
            } else if ((((u16) game_state.player_flags.h >> 0xC) == 5) || (((u16) game_state.player_flags.h >> 0xC) == 0)) {
                portrait_read_start(sp174.v[D_8013C2E0]);
            } else {
                portrait_read_start(sp1A8.v[D_8013C2E0]);
            }
            mode_step_next();
        }
        break;
    case 2:
        normal_date_bg_fadein();
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            game_state.current_girl = (u8) D_8013C2E0;
            portrait_show_checked();
            if (((u16) game_state.player_flags.h >> 0xC) == 3) {
                cd_read_sectors(3U, 0x801E8000, 0xA9B7);
            }
            if (D_8013C2E0 == 0xC) {
                portrait_layout_set(0U);
            }
        }
        break;
    case 4:
        switch ((u16) game_state.player_flags.h >> 0xC) {
        case 0:
            talk_group = (s16) sp140.v[D_8013C2E0];
            break;
        case 1:
            if ((D_8013C2E0 == 0) && (((u8) game_state.girls[0].flags.h[1] >> 4) == 1)) {
                talk_group = 7;
                mode_step_next();
                KEEP_MODE_LIVE();
            } else if ((D_8013C2E0 == 1) && (((u8) game_state.girls[1].flags.h[1] >> 4) == 1)) {
                talk_group = 8;
                mode_step_next();
                KEEP_MODE_LIVE();
            } else {
                talk_group = (s16) sp10C.v[D_8013C2E0];
            }
            break;
        case 2:
            talk_group = (s16) spD8.v[D_8013C2E0];
            break;
        case 3:
            if (cd_read_sync_status() & 1) {
                func_80132034();
                talk_text_table = (s32) D_8013C2D0;
                talk_command_table = (s32) D_8013C2D4;
                talk_voice_table = (s32) D_8013C2D8;
                talk_group = (s16) spA4.v[D_8013C2E0];
                if (D_8013C2E0 == 0xB) {
                    func_80047550();
                    audio_command(0x500);
                }
                audio_command(0x200);
                mode_step_next();
            }
            KEEP_MODE_LIVE();
            break;
        case 4:
            talk_group = (s16) sp70.v[D_8013C2E0];
            break;
        case 5:
            if ((D_8013C2E0 == 0) && (((u8) game_state.girls[0].flags.h[1] >> 4) == 5)) {
                talk_group = 5;
                mode_step_next();
                KEEP_MODE_LIVE();
            } else if ((D_8013C2E0 == 3) && (((u8) game_state.girls[3].flags.h[1] >> 4) == 5)) {
                talk_group = 0xC;
                mode_step_next();
                KEEP_MODE_LIVE();
            } else {
                talk_group = (s16) sp3C.v[D_8013C2E0];
            }
            break;
        default:
            game_mode_reset_to_title();
            KEEP_MODE_LIVE();
            break;
        }
        talk_line = 0;
        /* FAKE: T-25130. The switch reads the mode as (u16) D_800E6374 >> 12 and this test through a u32 copy:
         * two different expressions to uopt, so the shift is not shared between them (both are ugen
         * temporaries); the original source of the join test is unknown. */
        mode = game_state.player_flags.h;
        if ((mode >> 0xC) != 3) {
            if (D_8013C2E0 == 0xB) {
                func_80047550();
                audio_command(0x500);
            }
            audio_command(0x200);
            mode_step_next();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_girl_in();
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        switch ((u16) game_state.player_flags.h >> 0xC) {
        case 0:
        case 2:
            if (D_8013C2E0 == 0xA) {
                talk_group = 2;
                mode_step_next();
            } else if (D_8013C2E0 == 0xB) {
                if (((u32) (game_state.girls[0xB].flags.w << 0x14) >> 0x1D) == 0) {
                    talk_group = 3;
                } else {
                    talk_group = 4;
                }
                mode_step_set(0xC);
            } else if (D_8013C2E0 == 0xC) {
                talk_group = 5;
                mode_step_set(0xC);
            } else {
                talk_group = 1;
                mode_step_next();
            }
            break;
        case 1:
        case 3:
        case 5:
            if (D_8013C2E0 == 0xB) {
                if (((u32) (game_state.girls[0xB].flags.w << 0x14) >> 0x1D) == 0) {
                    talk_group = 2;
                } else {
                    talk_group = 3;
                }
                mode_step_set(0xC);
            } else if (D_8013C2E0 == 0xC) {
                talk_group = 4;
                mode_step_set(0xC);
            } else {
                talk_group = 1;
                mode_step_next();
            }
            break;
        case 4:
            if (D_8013C2E0 == 0) {
                mode_step_set(0xA);
            } else if (D_8013C2E0 == 0xB) {
                if (((u32) (game_state.girls[0xB].flags.w << 0x14) >> 0x1D) == 0) {
                    talk_group = 2;
                } else {
                    talk_group = 3;
                }
                mode_step_set(0xC);
            } else if (D_8013C2E0 == 0xC) {
                talk_group = 4;
                mode_step_set(0xC);
            } else {
                talk_group = 1;
                mode_step_next();
            }
            break;
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        normal_date_bggirl_fadeout();
        break;
    case 11:
        mode_stage_next();
        break;
    case 12:
        normal_date_girl_out();
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        if ((D_8013C2E0 == 0xFF) && (((u16) game_state.player_flags.h >> 0xC) == 3)) {
            mode_step_next();
        } else {
            normal_date_bg_fadeout();
        }
        break;
    case 15:
        mode_stage_next();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
    Default_Disp();
}

void func_80133374(void) {
    s32 i;

    game_state.day_event_done_bits |= 0x10;
    game_state.girls[0].tokimeki += 0x14;
    for (i = 1; i < 0xD; i++) {
        game_state.girls[i].tokimeki += 0x14;
    }
    if (D_8013C2E0 != 0xFF) {
        game_state.girls[D_8013C2E0].affection += 2;
    }
    ((ClubFlagBits *)&game_state.clubs[(u32)game_state.player_flags.h >> 0xC])->b30 = 1;
    talk_clear();
    dec_bg_reset();
    set_dec_bri(0);
    func_8013348C();
    day_event_next(1);
    Default_Disp();
}

void func_8013348C(void) {
    u32 sel; /* declared before kind: its slot sits above kind's stack home (sp+0x2B), T-18600 */
    u8 kind;

    sel = (u16) game_state.player_flags.h >> 0xC;
    switch (sel) {
    case 0:
        kind = 0;
        break;
    case 1:
        if ((((u8) game_state.girls[0].flags.b[2] >> 4) == 1) && (D_8013C2E0 == 0)) {
            kind = 0xB;
        } else if (((u8) game_state.girls[1].flags.b[2] >> 4) == 1) {
            kind = 1;
            if (D_8013C2E0 == 1) {
                kind = 0xC;
            }
        } else {
            kind = 1;
        }
        break;
    case 2:
        kind = 2;
        break;
    case 3:
        kind = 3;
        break;
    case 4:
        kind = 4;
        break;
    case 5:
        kind = 5;
        break;
    }
    game_state.event_log[game_state.event_log_count].girl = D_8013C2E0;
    game_state.event_log[game_state.event_log_count].scene = 0x15;
    game_state.event_log[game_state.event_log_count].flag = kind;
    game_state.event_log[game_state.event_log_count].rank = get_g_zyotai_h(D_8013C2E0);
    game_state.event_log[game_state.event_log_count].year = game_state.year;
    game_state.event_log[game_state.event_log_count].month = game_state.month;
    game_state.event_log[game_state.event_log_count].day = game_state.day;
    game_state.event_log[game_state.event_log_count].weekday = game_state.weekday;
    game_state.event_log_count++;
    if (game_state.event_log_count >= 0xFF) {
        game_state.event_log_count = 0xFE;
    }
}
