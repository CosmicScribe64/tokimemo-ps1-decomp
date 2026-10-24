/* yumi_scene_handlers.c: ; src/ovl/SHOUGATU/8013AF10.c:4-5 dispatches Yumi scene setup. Original object 0x8013AF10-0x8013B210 (config/objects/SHOUGATU.txt). */
#include "common.h"
#include "ovl/SHOUGATU.h"

void func_8013AF10(void) {
    func_8013AF30();
}

void func_8013AF30(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl42 tbl;

    tbl = D_801458B0;
    idx = game_state.mode_step;
    tbl.f[idx]();
}

void func_8013AFA4(void) {
    func_80137AB4();
    if (D_80144E0C == 2) {
        D_80122D20 = 1;
        return;
    }
    D_80122D20 = 0;
}

void func_8013AFEC(void) {
    draw2d3d(1, 0);
    actor_draw_enable(1);
    sprite_tables_clear();
    actor_clear_range(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_system_reset();
    game_state.tile_layer_enabled = 0;
    game_state.quads_enabled = 1;
    ((ShouByteFlags *)&game_state.girls[10].flags.b[0])->b3 = 1;
    talk_clear();
    game_state.screen_brightness = 0x80;
    dec_bg_brightness = 0;
    D_800B5940 = 0;
    addr_init_bustup();
    hizuke_init();
    message_window_init();
    text_marker_hide();
    /* Chain store through byte views keeps one li for both stores and the argument (T-9150 idiom). */
    *(u8 *)&game_state.date_girl = *(u8 *)&game_state.current_girl = 0xA;
    talk_names_set(*(u8 *)&game_state.current_girl);
    sprintf(name_code_player_form, "%sさん", game_state.player_family_name);
    func_80136F90();
    D_80144DFC = D_80144DB0;
    D_80144E00 = D_80144DD4;
    D_80144E04 = D_80144DF8;
    mode_step_next();
}

void func_8013B114(void) {
    D_80144E08 = 0;
    D_80144E0C = 0;
    strcpy(D_800CA19C, "学校前");
    mode_step_next();
}

void func_8013B158(void) {
    bg_read_sub2(0x4016);
    mode_step_next();
}

void func_8013B180(void) {
    if ((u8) game_state.girls[13].flags.b[3] < 2U) {
        D_80144E08 += 1;
    }
    mode_step_next();
}

void func_8013B1C4(void) {
    if ((u8) game_state.girls[13].flags.b[3] >= 2U) {
        D_80144E08 += 1;
    }
    mode_step_next();
}
