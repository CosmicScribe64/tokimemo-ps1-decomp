#include "common.h"
#include "ovl/SHOUGATU.h"

void func_80138B40(void) {
    u8 sel = D_80144E14; /* FAKE: copy of unit-private data, which the original does not promote (T-5010) */

    switch (sel) {
    case 0:
        func_80138BA0();
        return;
    case 1:
        func_80138C1C();
        return;
    default:
        func_80138CA4();
        return;
    }
}

void func_80138BA0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl22 tbl;

    tbl = D_80145170;
    idx = game_state.step;
    tbl.f[idx]();
}

void func_80138C1C(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl20 tbl;

    tbl = D_801451C8;
    idx = game_state.step;
    tbl.f[idx]();
}

void func_80138CA4(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl20 tbl;

    tbl = D_80145218;
    idx = game_state.step;
    tbl.f[idx]();
}

void func_80138D2C(void) {
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
    /* FAKE: the byte-typed store as the argument keeps the constant in $a0 (T-9020 idiom); T-10520 */
    dialogue_names_set(*(u8 *)&game_state.current_girl = 3);
    func_80136F90();
    D_80144DFC = D_80144D98;
    D_80144E00 = D_80144DBC;
    D_80144E04 = D_80144DE0;
    main_step_next();
}

void func_80138E1C(void) {
    D_80144E08 = 0;
    D_80144E0C = 0;
    strcpy(D_800CA19C, "中庭");
    main_step_next();
}

void func_80138E60(void) {
    ((Bits64B8 *)&game_state.girls[game_state.current_girl].flags.b[0])->f = 1;
    D_80144E08 = 8;
    D_80144E0C = 0;
    strcpy(D_800CA19C, "美術室");
    main_step_next();
}

void func_80138ED4(void) {
    ((Bits64B8 *)&game_state.girls[game_state.current_girl].flags.b[0])->f = 1;
    D_80144E08 = 0xB;
    D_80144E0C = 0;
    strcpy(D_800CA19C, "音楽室");
    main_step_next();
}

void func_80138F48(void) {
    bg_read_sub2(0x4071);
    main_step_next();
}

void func_80138F70(void) {
    bg_read_sub2(0x40E2);
    main_step_next();
}

void func_80138F98(void) {
    bg_read_sub2(0x40D9);
    main_step_next();
}

void func_80138FC0(void) {
    s32 isA; /* high nibble of GameState.player_flags is 4 */
    s32 isB; /* high nibble of the GirlRecord flag byte +2 of record 3 is 4 */

    game_state.girls[game_state.current_girl].flags.f.b2 = 1;
    if ((game_state.player_flags.h >> 0xC) == 4) {
        isA = 1;
    } else {
        isA = 0;
    }
    if ((game_state.girls[3].flags.b[2] >> 4) == 4) {
        isB = 1;
    } else {
        isB = 0;
    }
    if ((isA == 0) || (isB == 0)) {
        if (isB != 0) {
            D_80144E08 += 2;
        } else {
            D_80144E08 += 4;
            game_state.girls[game_state.current_girl].flags.f.b2 = 0;
        }
    }
    main_step_next();
}
