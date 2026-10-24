#include "common.h"
#include "ovl/SHOUGATU.h"

void func_80136F90(void) {
    D_80144D90 = 0x801976F8;
    D_80144D94 = 0x80198BF0;
    D_80144D98 = 0x8019A37C;
    D_80144D9C = 0x8019ADCC;
    D_80144DA0 = 0x8019BC80;
    D_80144DA4 = 0x8019C918;
    D_80144DA8 = 0x8019D460;
    D_80144DAC = 0x8019E168;
    D_80144DB0 = 0x8019EB4C;
    D_80144DB4 = 0x80197774;
    D_80144DB8 = 0x80198CF8;
    D_80144DBC = 0x8019A414;
    D_80144DC0 = 0x8019ADF8;
    D_80144DC4 = 0x8019BD34;
    D_80144DC8 = 0x8019C960;
    D_80144DCC = 0x8019D4C4;
    D_80144DD0 = 0x8019E210;
    D_80144DD4 = 0x8019EBA8;
    D_80144DD8 = 0x80197C94;
    D_80144DDC = 0x80199738;
    D_80144DE0 = 0x8019AB30;
    D_80144DE4 = 0x8019B000;
    D_80144DE8 = 0x8019C4B4;
    D_80144DEC = 0x8019CC70;
    D_80144DF0 = 0x8019D910;
    D_80144DF4 = 0x8019E7B4;
    D_80144DF8 = 0x8019EE04;
}

void func_80137144(void) {
    bustup_wink_update();
    switch (game_state.stage) {
    case 0:
        func_80137664();
        break;
    case 1:
        func_80137818();
        break;
    case 2:
        func_80137A04();
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            main_stage_next();
        } else {
            if (game_state.stage_timer++ >= 0x401U) {
                main_stage_set(2);
            }
        }
        break;
    case 4:
        switch (D_80144E10) {
        case 0:
            game_reset_to_title();
            break;
        case 1:
            func_801398F0();
            break;
        case 2:
            func_80137FF0();
            break;
        case 3:
            func_80138B40();
            break;
        case 4:
            func_8013A950();
            break;
        case 5:
            func_8013A190();
            break;
        case 6:
            func_8013A500();
            break;
        case 7:
            func_801391D4();
            break;
        case 8:
            func_80137BB0();
            break;
        case 9:
            game_reset_to_title();
            break;
        case 10:
            func_8013AF10();
            break;
        default:
            game_reset_to_title();
            break;
        }
        break;
    case 5:
        func_801377CC();
        break;
    case 6:
        func_80137818();
        break;
    case 7:
        func_8013737C();
        break;
    default:
        game_reset_to_title();
        break;
    }
    check_k_scroll();
    k_disp_inc2();
    screen_frame_draw(2);
    message_window_show();
    message_window_title_update();
    hizuke_show();
    func_80083A10();
    screen_shake_update();
}

s32 func_8013737C(void) {
    GameState *g = &game_state;
    s32 pad;

    /* FAKE: touch narrow unsigned globals at entry for cvt_pass.py (T-20610); dead-store eliminated by uopt. T-24130 */
    pad = D_80144E10;
    pad = D_80144E14;
    pad = game_state.month;
    if (game_state.substep == 0) {
        snd_command(0xCF);
        func_800674B0();
        game_state.substep += 1;
    }
    if (game_state.substep == 1) {
        if (game_state.step_timer.u++ >= 0x400U) {
            cd_cmd_reset();
            snd_bank_state_reset();
            game_state.substep = 0;
        }
        if (snd_bank_load_poll() == 1) {
            game_state.substep += 1;
        } else {
            return 0;
        }
    }
    game_state.current_girl = D_80144E10;
    if (D_80144E10 == 2 && (D_80144E14 == 2 || D_80144E14 == 3) &&
        (game_state.girls[2].flags.b[2] >> 4) == 3) {
        event_log_add(8, D_80144E10 * 10 + 6);
    } else if (D_80144E10 == 4 && D_80144E14 == 0 &&
               game_state.month >= 6 && game_state.month < 10) {
        event_log_add(8, D_80144E10 * 10 + 3);
    } else {
        event_log_add(8, D_80144E10 * 10 + D_80144E14);
    }
    if (D_80144E10 == 10) {
        game_state.girls[game_state.day_events[game_state.day_event_index].girl].affection = 0x28;
    } else {
        game_state.girls[game_state.day_events[game_state.day_event_index].girl].affection = 0x32;
    }
    if (game_state.day_events[game_state.day_event_index].girl != 8) {
        game_state.girls[game_state.day_events[game_state.day_event_index].girl].flags.f.met = 1;
    } else {
        g->girls[8].flags.f.story_stage += 1;
        if (g->girls[8].flags.f.story_stage == 4) {
            game_state.girls[game_state.day_events[game_state.day_event_index].girl].flags.f.met = 1;
        }
    }
    snd_command(0x22);
    snd_command(0x21);
    snd_command(0x2F);
    day_event_next(1);
}

void func_80137664(void) {
    GameState *g = &game_state;
    s32 a1;
    s32 a2;

    D_800B3D60 = 0;
    draw2d3d(1, 0);
    actors_draw_enable(1);
    sprite_tables_clear();
    actor_clear_range(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_system_reset();
    hizuke_init();
    message_window_init();
    dialogue_clear();
    /* FAKE: g-view store keeps ILOD CSE of unk_69C[unk_71C].unk_01. T-24137 */
    g->current_girl = g->day_events[g->day_event_index].girl;
    D_80144E14 = dialogue_work;
    D_80144E10 = g->current_girl;
    if (g->day_events[g->day_event_index].girl == 2 ||
        g->day_events[g->day_event_index].girl == 6) {
        a1 = 0xC063;
        a2 = 0xC045;
    } else {
        a1 = 0xC043;
        a2 = 0xC01D;
    }
    snd_bank_load_start(1, a1, a2,
                  D_800B3688[g->day_events[g->day_event_index].girl],
                  D_800B36C8[g->day_events[g->day_event_index].girl],
                  D_800B3708[g->day_events[g->day_event_index].girl]);
    if (snd_bank_load_poll() == 1) {
        main_stage_next();
    }
    g->girls[g->day_events[g->day_event_index].girl].affection = 0x46;
    main_stage_next();
}

void func_801377CC(void) {
    snd_command(0xCF);
    func_800674B0();
    if (snd_bank_load_poll() == 1) {
        main_stage_next();
    }
    main_stage_next();
}

void func_80137818(void) {
    if (snd_bank_load_poll() == 1) {
        main_stage_next();
    }
    if ((u32) game_state.step_timer.w++ >= 0x400U) {
        cd_cmd_reset();
        snd_bank_state_reset();
        main_stage_set((game_state.stage - 1) & 0xFF);
    }
}

void func_80137898(void) {
    snd_command(0xCF);
    main_step_next();
}

void func_801378C0(void) {
    D_800B3D60 = 0;
    snd_bank_load_start(0, 0xBF98, 0xBF79, 0xD989, 0xD949, 0xD941);
    snd_command(0x23);
    snd_command(0x24);
    snd_command(0x2E);
    if (snd_bank_load_poll() == 1) {
        main_step_next();
    }
    main_step_next();
}

void func_80137940(void) {
    if (snd_bank_load_poll() == 1) {
        snd_command(0x23);
        snd_command(0x24);
        snd_command(0x2E);
        main_step_next();
    }
    if (game_state.step_timer.u++ >= 0x400U) {
        cd_cmd_reset();
        snd_bank_state_reset();
        main_step_set((game_state.step - 1) & 0xFF);
    }
}

void func_801379D8(void) {
    snd_bank_start(1, 0x200);
    main_step_next();
}

void func_80137A04(void) {
    cd_read_sectors(0x10, 0x80197000, 0xAEFD);
    func_80136F90();
    D_80144E10 = game_state.day_events[game_state.day_event_index].girl;
    main_stage_next();
}

void func_80137A5C(void) {
    cd_read_sectors(0x10, 0x80197000, 0xAEFD);
    func_80136F90();
    D_80144E10 = game_state.day_events[game_state.day_event_index].girl;
    main_step_next();
}

void func_80137AB4(void) {
    dialogue_tables = &D_80144E08;
    D_800CA138 = &D_80144E0C;
    D_800CA13C = D_80144DFC;
    D_800CA140 = D_80144E00;
    D_800CA144 = D_80144E04;
    dialogue_run(D_80122CDC, 1, 0);
}

void func_80137B30(void) {
    dialogue_tables = (u8 *) &D_80144E08;
    D_800CA138 = (u8 *) &D_80144E0C;
    D_800CA13C = D_80144DFC;
    D_800CA140 = D_80144E00;
    D_800CA144 = D_80144E04;
    dialogue_run(D_80122CDC, 1, 1);
}
