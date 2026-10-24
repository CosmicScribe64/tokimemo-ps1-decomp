#include "common.h"
#include "ovl/SHOUGATU.h"

void func_8013A950(void) {
    u8 sel = D_80144E14; /* FAKE: copy of unit-private data, which the original does not promote (T-5010) */

    switch (sel) {
    case 0:
        func_8013A9B0();
        return;
    case 1:
        func_8013AA24();
        return;
    default:
        func_8013AAAC();
        return;
    }
}

void func_8013A9B0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl42 tbl;

    tbl = D_80145760;
    idx = game_state.step;
    tbl.f[idx]();
}

void func_8013AA24(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl20 tbl;

    tbl = D_80145808;
    idx = game_state.step;
    tbl.f[idx]();
}

void func_8013AAAC(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl20 tbl;

    tbl = D_80145858;
    idx = game_state.step;
    tbl.f[idx]();
}

void func_8013AB34(void) {
    bustup_read_start(0x66EC);
    main_step_next();
}

void func_8013AB5C(void) {
    draw2d3d(1, 0);
    actors_draw_enable(1);
    sprite_tables_clear();
    actor_clear_range(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_system_reset();
    game_state.tile_layer_enabled = 0;
    game_state.quads_enabled = 1;
    dialogue_clear();
    game_state.screen_brightness = 0x80;
    dec_bg_brightness = 0;
    D_800B5940 = 0;
    addr_init_bustup();
    hizuke_init();
    message_window_init();
    text_marker_hide();
    /* FAKE: the byte-typed store as the argument keeps the constant in $a0 (T-9020 idiom); T-9120 */
    dialogue_names_set(*(u8 *)&game_state.current_girl = 4);
    func_80136F90();
    D_80144DFC = D_80144DAC;
    D_80144E00 = D_80144DD0;
    D_80144E04 = D_80144DF4;
    game_state.girls[game_state.current_girl].flags.f.b2 = 1;
    main_step_next();
}

void func_8013AC7C(void) {
    D_80144E08 = 0;
    D_80144E0C = 0;
    strcpy(D_800CA19C, "教室");
    main_step_next();
}

void func_8013ACC0(void) {
    D_80144E08 = 0xC;
    D_80144E0C = 0;
    strcpy(D_800CA19C, "グランド");
    main_step_next();
}

void func_8013AD08(void) {
    D_80144E08 = 0xF;
    D_80144E0C = 0;
    strcpy(D_800CA19C, "グランド");
    main_step_next();
}

void func_8013AD50(void) {
    if (((u8) game_state.month >= 6U) && ((u8) game_state.month < 0xAU)) {
        bg_read_sub2(0x404D);
    } else {
        bg_read_sub2(0x4055);
    }
    main_step_next();
}

void func_8013ADA4(void) {
    bg_read_sub2(0x4097);
    main_step_next();
}

void func_8013ADCC(void) {
    if (((u32) game_state.girls[4].flags.b[2] >> 4) == 7) {
        D_80144E08 = 6;
    }
    main_step_next();
}

void func_8013AE0C(void) {
    u8 raw;
    s32 nibble;
    raw = game_state.girls[4].flags.b[2];
    if (raw) {} /* FAKE: dead raw-value test shapes allocation, T-9540 */
    nibble = raw >> 4;
    if (nibble == (game_state.player_flags.h >> 12)) {
        if (nibble == 7) {
            D_80144E08 = 10;
        } else {
            D_80144E08 = 8;
        }
    }
    main_step_next();
}

void func_8013AE6C(void) {
    if (((u32) game_state.girls[4].flags.b[2] >> 4) != ((u32) game_state.player_flags.h >> 0xC)) {
        D_80144E08 = 4;
    }
    main_step_next();
}

void func_8013AEB4(void) {
    if (((u32) game_state.girls[4].flags.b[2] >> 4) == ((u32) game_state.player_flags.h >> 0xC)) {
        game_state.step += 0xD;
        return;
    }
    main_step_next();
}
