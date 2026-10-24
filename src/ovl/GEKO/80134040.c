#include "common.h"
#include "ovl/GEKO.h"

void func_80134040(void) {
    bustup_wink_update();
    if (game_state.stage == 0) {
        func_801341A0();
    } else {
        game_reset_to_title();
    }
    check_k_scroll();
    k_disp_inc2();
    if (D_80122D38 == 0) {
        screen_frame_draw(2);
    }
    message_window_show();
    message_window_title_update();
    hizuke_show();
    if (D_80122D38 == 0) {
        func_80083A10();
    }
    if (D_80122D38 != 0) {
        screen_frame_draw(1);
        dec_bg_draw(0);
        parameter_show();
        if ((D_80122D2C == 0) && (D_80122D04 == 0) && ((game_state.pad_held & 0xF000F000) || (D_80122D30 == 1))) {
            D_8011ECD3 |= 0x80;
        }
        if (D_80122D04 != 0) {
            D_8011ECD3 &= 0x7F;
        }
        if (D_8011ECD3 & 0x80) {
            cursor_update();
        }
    }
}

typedef struct {
    void (*f[29])();
} FnTbl29; /* size 0x74 */
extern FnTbl29 D_80144E94;

void func_801341A0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl29 tbl;

    tbl = D_80144E94;
    idx = game_state.step;
    tbl.f[idx](0x80);
}

s32 func_80134228(void) {
    u8 state;

    if (game_state.daily_event_bits & 0x40) {
        snd_command(0xCF);
        main_state_set(MODE_NEW_YEAR_A);
        return 0;
    }
    /* FAKE: read through a u8 view so the base and the result share $v0 (T-24035). */
    state = *(u8 *)&game_state.substep;
    if (state == 0) {
        func_800674B0();
        state = game_state.substep + 1;
    }
    game_state.substep = state;
    if (state == 1) {
        if ((u32)game_state.step_timer.w++ >= 0x400) {
            cd_cmd_reset();
            snd_bank_state_reset();
            game_state.substep = 0;
        }
        if (snd_bank_load_poll() == 1) {
            /* FAKE: the dead test gives the byte a second use, so uopt allocates $v0 for the load (T-24035). */
            if ((s32)game_state.substep) {}
            game_state.substep += 1;
        } else {
            return 0;
        }
    }
    return day_event_next(1);
}

s32 func_8013431C(void) {
    if (game_state.month_calendar[0].marker[game_state.day] == 5) {
        if (game_state.step_timer.w++ == 0) {
            snd_command(0xBF);
        }
        if ((u32)game_state.step_timer.w < 0x80U) {
            return 0;
        }
    }
    snd_bank_load_start(1, 0xBF98, 0xBF79, 0xCA95, 0xCA4F, 0xCA3E);
    if (snd_bank_load_poll() == 1) {
        main_step_next();
    }
    main_step_next();
}

void func_801343DC(void) {
    if (snd_bank_load_poll() == 1) {
        main_step_next();
    }
    if ((u32) game_state.step_timer.w++ >= 0x400U) {
        cd_cmd_reset();
        snd_bank_state_reset();
        main_step_set((game_state.step - 1) & 0xFF);
    }
}

void func_8013445C(void) {
    k_reset(1);
    snd_command(0x200);
    main_step_next();
}

void func_8013448C(void) {
    cd_read_sectors(3, 0x801B0000, 0xAF43);
    func_80134F84();
    if (!(game_state.daily_event_bits & 8)) {
        snd_command(0x603);
        set_kanji_string(-0x28, 0x40, 2, "ピンポーン", 0);
    }
    main_step_next();
}

void func_80134500(void) {
    cd_read_sectors(3, 0x801B0000, 0xAF43);
    func_80134F84();
    main_step_next();
}

s32 func_80134538(void) {
    s16 i;
    u8 saved;

    if (*(u8 *)&game_state.daily_event_bits & 4) {
        func_80134228();
        return 0;
    }
    *(u8 *)&game_state.daily_event_bits |= 4;
    draw2d3d(1, 0);
    actors_draw_enable(1);
    sprite_tables_clear();
    actor_clear_range(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_system_reset();
    strcpy(D_800CA19C, "玄関");
    k_reset(1);
    game_state.tile_layer_enabled = 0;
    game_state.quads_enabled = 1;
    saved = D_80122D40;
    dialogue_clear();
    D_80122D40 = saved;
    D_80122D38 = 1;
    game_state.screen_brightness = 0x80;
    dec_bg_brightness = 0;
    D_800B5940 = 0;
    addr_init_bustup();
    hizuke_init();
    message_window_init();
    func_8006509C();
    for (i = 0; i < 0xF; i++) {
        icon_can_use_set(i, 0);
    }
    icon_bar_update(1);
    parameter_show_init();
    parameter_show_init();
    text_marker_hide();
    D_80144E8C = 0;
    D_80144E90 = 0;
    main_step_next();
}

void func_801346A0(void) {
    s32 pad; /* FAKE: unused slot above sp2B, the original frame has it (real source unknown). T-8070 */
    u8 sp2B;

    draw2d3d(1, 0);
    actors_draw_enable(1);
    sprite_tables_clear();
    actor_clear_range(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_system_reset();
    sp2B = D_80122D40;
    dialogue_clear();
    D_80122D40 = sp2B;
    strcpy(D_800CA19C, "玄関");
    k_reset(1);
    game_state.tile_layer_enabled = 0;
    game_state.quads_enabled = 1;
    game_state.screen_brightness = 0x80;
    dec_bg_brightness = 0;
    D_800B5940 = 0;
    hizuke_init();
    message_window_init();
    text_marker_hide();
    bg_read_sub2(0x4144);
    event_log_add(0xD, 0x25);
    main_step_next();
}

void func_8013478C(void) {
    dialogue_tables = &D_80144E8C;
    D_800CA138 = &D_80144E90;
    D_800CA13C = D_80144E80;
    D_800CA140 = D_80144E84;
    D_800CA144 = D_80144E88;
    dialogue_run(D_80122CDC, 1, 0);
}

/* FAKE: T-24503, the two equality wrappers prevent a shared compare copy;
 * the unsigned selector view reverses the branch operands. The empty loop-tail
 * test keeps the s16 conversion in the latch, and the call-result mask spends
 * the deleted narrowing temporary needed by the following ugen registers.
 * cur and newState keep the record base from being hoisted out of the loop. */
s32 func_80134808(void) {
    s16 i;
    s32 next;
    s16 best;
    u32 cur;
    u32 bestState;
    u32 state;
    u32 newState;
    u8 yuukou;
    u8 tokimeki;
    u8 bestTokimeki;
    u8 girl;

    best = 0xFF;
    bestState = 4;
    i = 0;
    do {
        if (!game_state.girls[i].flags.f.met) {
            next = i + 1;
        } else if (!(game_state.daily_event_bits & 0x80) || (next = i + 1, next != (*(u32 *)&D_80122D40))) {
            next = i + 1;
            state = get_g_zyotai_s(i) & 0x7F;
            newState = state & 0xFF;
            if ((u8) state < bestState ||
                (bestState * 1 == (u8) state && (cur = best, bestState << 0 == (u8) state) &&
                 (yuukou = get_h_yuukou(i), tokimeki = get_h_tokimeki(i),
                  bestTokimeki = get_h_tokimeki(cur),
                  get_h_yuukou(cur) + bestTokimeki < tokimeki + yuukou))) {
                best = (u8) i;
                bestState = (u8) newState;
            }
        }
        i = (s16) next; if (i) {}
    } while (i < 11);
    state = get_g_zyotai_s(best) & 0xFF;
    D_80122D40 = 0;
    if ((state & 0x7F) >= 2 || best == 0xFF || (girl = best, game_state.daily_event_bits & 8)) {
        main_step_set(0x17);
        return 0;
    }
    game_state.current_girl = girl;
    dialogue_names_set(girl);
    func_80134F84();
    func_80134A14();
    return main_step_next();
}

void func_801349B8(void) {
    func_80134F84();
    dialogue_names_set(0);
    D_80144E80 = D_80144FD8;
    D_80144E84 = D_8014500C;
    D_80144E88 = D_80145040;
    main_step_next();
}

void func_80134A14(void) {
    switch (game_state.current_girl) {
    case GIRL_SHIORI:
        D_80144E80 = D_80144FB0;
        D_80144E84 = D_80144FE4;
        D_80144E88 = D_80145018;
        return;
    case GIRL_MIO:
        D_80144E80 = D_80144FC4;
        D_80144E84 = D_80144FF8;
        D_80144E88 = D_8014502C;
        return;
    case GIRL_YUINA:
        D_80144E80 = D_80144FB8;
        D_80144E84 = D_80144FEC;
        D_80144E88 = D_80145020;
        return;
    case GIRL_AYAKO:
        D_80144E80 = D_80144FBC;
        D_80144E84 = D_80144FF0;
        D_80144E88 = D_80145024;
        return;
    case GIRL_SAKI:
        D_80144E80 = D_80144FD4;
        D_80144E84 = D_80145008;
        D_80144E88 = D_8014503C;
        return;
    case GIRL_YUKARI:
        D_80144E80 = D_80144FC8;
        D_80144E84 = D_80144FFC;
        D_80144E88 = D_80145030;
        return;
    case GIRL_NOZOMI:
        D_80144E80 = D_80144FCC;
        D_80144E84 = D_80145000;
        D_80144E88 = D_80145034;
        return;
    case GIRL_MIRA:
        D_80144E80 = D_80144FC0;
        D_80144E84 = D_80144FF4;
        D_80144E88 = D_80145028;
        return;
    case GIRL_YUKO:
        D_80144E80 = D_80144FAC;
        D_80144E84 = D_80144FE0;
        D_80144E88 = D_80145014;
        return;
    case GIRL_MEGUMI:
        D_80144E80 = D_80144FD0;
        D_80144E84 = D_80145004;
        D_80144E88 = D_80145038;
        return;
    case GIRL_YUMI:
        D_80144E80 = D_80144FDC;
        D_80144E84 = D_80145010;
        D_80144E88 = D_80145044;
        /* fallthrough */
    default:
        return;
    }
}
