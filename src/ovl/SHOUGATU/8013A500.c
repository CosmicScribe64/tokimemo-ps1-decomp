#include "common.h"
#include "ovl/SHOUGATU.h"

void func_8013A500(void) {
    if (D_80144E14 == 0) {
        func_8013A53C();
        return;
    }
    func_8013A600();
}

void func_8013A53C(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl27 tbl;

    tbl = D_801456A0;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
}

void func_8013A5B0(void) {
    audio_command(0x502);
    mode_step_next();
}

void func_8013A5D8(void) {
    audio_command(0x503);
    mode_step_next();
}

void func_8013A600(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl20 tbl;

    tbl = D_8014570C;
    idx = game_state.mode_step;
    tbl.f[idx]();
}

void func_8013A688(void) {
    draw2d3d(1, 0);
    actor_draw_enable(1);
    sprite_tables_clear();
    actor_clear_range(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_system_reset();
    game_state.tile_layer_enabled = 0;
    game_state.quads_enabled = 1;
    talk_clear();
    D_80122CE8 = 0;
    game_state.screen_brightness = 0x80;
    dec_bg_brightness = 0;
    D_800B5940 = 0;
    addr_init_bustup();
    hizuke_init();
    message_window_init();
    text_marker_hide();
    *(u8 *)&game_state.current_girl = 6;
    talk_names_set(*(u8 *)&game_state.current_girl);
    func_80136F90();
    D_80144DFC = D_80144DA8;
    D_80144E00 = D_80144DCC;
    D_80144E04 = D_80144DF0;
    game_state.girls[game_state.current_girl].flags.f.b2 = 1;
    mode_step_next();
}

void func_8013A7B0(void) {
    D_80144E08 = 0;
    D_80144E0C = 0;
    strcpy(D_800CA19C, "近所の公園");
    mode_step_next();
}

void func_8013A7F4(void) {
    D_80144E08 = 6;
    D_80144E0C = 0;
    strcpy(D_800CA19C, "プール");
    mode_step_next();
}

void func_8013A83C(void) {
    bg_read_sub2(0x42D8);
    mode_step_next();
}

void func_8013A864(void) {
    bg_read_sub2(0x40A0);
    mode_step_next();
}

void func_8013A88C(void) {
    portrait_read_start(0x6449);
    mode_step_next();
}

void func_8013A8B4(void) {
    portrait_read_start(0x625A);
    mode_step_next();
}

void func_8013A8DC(void) {
    if (((u32) game_state.player_flags.h >> 0xC) == 9) {
        D_80144E08 = 5;
    }
    mode_step_next();
}

void func_8013A91C(void) {
    D_80144E08 = 3;
    mode_step_next();
}
