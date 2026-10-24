#define MAIN_API_OVERRIDE_get_h_tokimeki /* u8 return narrows the metric byte on return (T-24128). */
#define MAIN_API_OVERRIDE_get_h_yuukou   /* u8 return narrows the metric byte on return (T-24128). */
#include "common.h"
#include "ovl/SHOUGATU.h"

u8 get_h_tokimeki(s32 arg0);
u8 get_h_yuukou(s32 arg0);

void func_80132550(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl13 tbl;

    tbl = D_80143B28;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
}

void func_801325CC(void) {
    audio_bank_load_start(1, 0xC4E5, 0xC4C7, 0xD294, 0xD264, 0xD25E);
    if (audio_bank_load_poll() == 1) {
        audio_command(0x200);
        mode_step_next();
    }
    mode_step_next();
}

void func_80132634(void) {
    if (audio_bank_load_poll() == 1) {
        audio_command(0x200);
        mode_step_next();
    }
    if ((u32) game_state.mode_step_timer.w++ >= 0x400U) {
        cd_command_reset();
        audio_bank_state_reset();
        mode_step_set((game_state.mode_step - 1) & 0xFF);
    }
}

void func_801326BC(void) {
    cd_read_sectors(0x34, 0x80197000, 0xAF48);
    func_80132000();
    mode_step_next();
}

void func_801326F8(void) {
    s16 i;
    GameState *g;
    u8 *flags;

    draw2d3d(1, 0);
    actor_draw_enable(1);
    sprite_tables_clear();
    actor_clear_range(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_system_reset();
    strcpy(D_800CA19C, "正月");
    game_state.tile_layer_enabled = 0;
    game_state.quads_enabled = 1;
    talk_clear();
    game_state.screen_brightness = 0x80;
    dec_bg_brightness = 0;
    D_800B5940 = 0;
    day_event_today_clear(0);
    addr_init_bustup();
    hizuke_init();
    message_window_init();
    text_marker_hide();
    func_80132000();
    talk_names_set(0U);
    D_80143B00 = 0;
    D_80143B04 = 0;
    D_80143B18 = 0;
    D_80143B1C = 0xFF;
    D_80143B20 = 0;
    D_80143B24 = 1;
    g = &game_state;
    flags = D_800CA2B0;
    for (i = 0; i < 11; i++) {
        flags[i] = g->girls[i].flags.f.met >= 1;
    }
    mode_step_next();
}

void func_80132864(void) {
    bg_read_sub2(0x4167);
    mode_step_next();
}

void func_8013288C(void) {
    s32 i;
    u8 best_i;
    u8 best_s;
    u8 s;

    best_i = 0xFF;
    best_s = 4;
    D_80122D40 = 0;
    for (i = 0; i < 11; i++) {
        /* FAKE: dead branch adjusts the loop basic-block count so uopt assigns best_s to $s5 (T-24122, T-24128). */
        if (1) {}
        if (game_state.girls[i].flags.f.met != 0) {
            s = get_g_zyotai_s(i) & 0x7F;
            /* FAKE: duplicate `s == best_s` shifts uopt's live-range priority between s and best_s (T-24122). */
            if (s < best_s ||
                (s == best_s && s == best_s &&
                 get_h_tokimeki(i) + get_h_yuukou(i) >
                     get_h_tokimeki(best_i) + get_h_yuukou(best_i))) {
                best_i = i;
                best_s = s;
            }
        }
    }
    s = get_g_zyotai_s(best_i) & 0x7F;
    if (best_i != 0xFF && s < 2U) {
        D_80143B1C = best_i;
        /* FAKE: `(best_i & 0) +` keeps cfe's CVT J L narrowing so uopt emits `andi $a0, $fp, 0xff` (T-24122). */
        game_state.current_girl = (best_i & 0) + best_i;
        talk_names_set(game_state.current_girl);
        func_80132A34();
        D_80143B00 = 0;
        D_80143B04 = 0;
        D_80143B14 = 1;
        D_80122D40 = best_i + 1;
        game_state.day_event_done_bits |= 0x80;
        mode_stage_set(1);
    } else {
        D_80143B14 = 0;
        mode_stage_set(2);
    }
}

void func_80132A34(void) {
    u8 state;

    switch (game_state.current_girl) {
    case GIRL_SHIORI:
        D_80143AF4 = D_80143A48;
        D_80143AF8 = D_80143A84;
        D_80143AFC = D_80143AC0;
        break;
    case GIRL_MIO:
        D_80143AF4 = D_80143A58;
        D_80143AF8 = D_80143A94;
        D_80143AFC = D_80143AD0;
        break;
    case GIRL_YUINA:
        D_80143AF4 = D_80143A4C;
        D_80143AF8 = D_80143A88;
        D_80143AFC = D_80143AC4;
        break;
    case GIRL_AYAKO:
        D_80143AF4 = D_80143A50;
        D_80143AF8 = D_80143A8C;
        D_80143AFC = D_80143AC8;
        break;
    case GIRL_SAKI:
        D_80143AF4 = D_80143A6C;
        D_80143AF8 = D_80143AA8;
        D_80143AFC = D_80143AE4;
        break;
    case GIRL_YUKARI:
        D_80143AF4 = D_80143A5C;
        D_80143AF8 = D_80143A98;
        D_80143AFC = D_80143AD4;
        break;
    case GIRL_NOZOMI:
        D_80143AF4 = D_80143A64;
        D_80143AF8 = D_80143AA0;
        D_80143AFC = D_80143ADC;
        state = get_g_zyotai_s(GIRL_NOZOMI) & 0x7F;
        if (state < 2) {
            D_80143AF4 = D_80143A60;
            D_80143AF8 = D_80143A9C;
            D_80143AFC = D_80143AD8;
        }
        break;
    case GIRL_MIRA:
        D_80143AF4 = D_80143A54;
        D_80143AF8 = D_80143A90;
        D_80143AFC = D_80143ACC;
        break;
    case GIRL_YUKO:
        D_80143AF4 = D_80143A44;
        D_80143AF8 = D_80143A80;
        D_80143AFC = D_80143ABC;
        break;
    case GIRL_MEGUMI:
        D_80143AF4 = D_80143A68;
        D_80143AF8 = D_80143AA4;
        D_80143AFC = D_80143AE0;
        break;
    case GIRL_YUMI:
        D_80143AF4 = D_80143A74;
        D_80143AF8 = D_80143AB0;
        D_80143AFC = D_80143AEC;
        break;
    }
}
