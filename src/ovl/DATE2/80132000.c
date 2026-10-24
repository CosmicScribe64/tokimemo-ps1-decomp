#include "common.h"
#include "ovl/DATE2.h"

void func_80132000(void) {
    D_8013A380 = 0x80197410;
    D_8013A384 = 0x80198070;
    D_8013A388 = 0x80198CBC;
    D_8013A38C = 0x801999E4;
    D_8013A390 = 0x8019A6C0;
    D_8013A394 = 0x8019B404;
    D_8013A398 = 0x8019BFF8;
    D_8013A39C = 0x8019CADC;
    D_8013A3A0 = 0x8019D688;
    D_8013A3A4 = 0x8019E2AC;
    D_8013A3A8 = 0x8019EE88;
    D_8013A3AC = 0x80197558;
    D_8013A3B0 = 0x801981B8;
    D_8013A3B4 = 0x80198E04;
    D_8013A3B8 = 0x80199B2C;
    D_8013A3BC = 0x8019A808;
    D_8013A3C0 = 0x8019B54C;
    D_8013A3C4 = 0x8019C140;
    D_8013A3C8 = 0x8019CC1C;
    D_8013A3CC = 0x8019D7D0;
    D_8013A3D0 = 0x8019E3F4;
    D_8013A3D4 = 0x8019EF4C;
    D_8013A3D8 = 0x80197A78;
    D_8013A3DC = 0x801986D8;
    D_8013A3E0 = 0x80199324;
    D_8013A3E4 = 0x8019A04C;
    D_8013A3E8 = 0x8019AD60;
    D_8013A3EC = 0x8019BA6C;
    D_8013A3F0 = 0x8019C660;
    D_8013A3F4 = 0x8019D11C;
    D_8013A3F8 = 0x8019DD0C;
    D_8013A3FC = 0x8019E914;
    D_8013A400 = 0x8019F39C;
}

void func_80132214(void) {
    bustup_wink_update();
    switch (game_state.stage) {
    case 0:
        func_801326F0();
        break;
    case 1:
        func_80132DE0();
        break;
    case 2:
        func_80136660();
        break;
    case 3:
        func_801375D4();
        break;
    case 4:
        func_80136FC4();
        break;
    default:
        game_reset_to_title();
        break;
    }
    check_k_scroll();
    k_disp_inc2();
    if ((game_state.stage == 0) && ((u8) game_state.step < 0x13U)) {
        parameter_show();
        dec_bg_draw(0);
    } else {
        screen_frame_draw(2);
        func_80083A10();
    }
    message_window_show();
    message_window_title_update();
    hizuke_show();
}

void func_80132334(void) {
    dialogue_tables = &D_8013A428;
    D_800CA138 = &D_8013A42C;
    D_800CA13C = D_8013A404;
    D_800CA140 = D_8013A408;
    D_800CA144 = D_8013A40C;
    dialogue_run(D_80122CDC, 1, 0);
}

void func_801323B0(void) {
    dialogue_tables = &D_8013A430;
    D_800CA138 = &D_8013A434;
    D_800CA13C = D_8013A384;
    D_800CA140 = D_8013A3B0;
    D_800CA144 = D_8013A3DC;
    dialogue_run(D_80122CDC, 1, 0);
}

void func_8013242C(void) {
    dialogue_tables = &D_8013A438;
    D_800CA138 = &D_8013A43C;
    D_800CA13C = D_8013A410;
    D_800CA140 = D_8013A414;
    D_800CA144 = D_8013A418;
    dialogue_run(D_80122CDC, 1, 0);
}

void func_801324A8(void) {
    if (D_80122CDC != 0) {
        dialogue_tables = &D_8013A438;
        D_800CA138 = &D_8013A43C;
        D_800CA13C = D_8013A410;
        D_800CA140 = D_8013A414;
        D_800CA144 = D_8013A418;
    } else {
        dialogue_tables = &D_8013A430;
        D_800CA138 = &D_8013A434;
        D_800CA13C = D_8013A384;
        D_800CA140 = D_8013A3B0;
        D_800CA144 = D_8013A3DC;
    }
    dialogue_run(D_80122CDC, 1, 0);
}

void func_8013257C(void) {
    dialogue_tables = &D_8013A428;
    D_800CA138 = &D_8013A42C;
    D_800CA13C = D_8013A404;
    D_800CA140 = D_8013A408;
    D_800CA144 = D_8013A40C;
    dialogue_run(0xFF, 1, 0);
}

void func_801325F4(void) {
    dialogue_tables = &D_8013A428;
    D_800CA138 = &D_8013A42C;
    D_800CA13C = D_8013A404;
    D_800CA140 = D_8013A408;
    D_800CA144 = D_8013A40C;
    dialogue_run(D_80122CDC, 1, 2);
}

void func_80132670(void) {
    dialogue_tables = (u8 *) &D_8013A428;
    D_800CA138 = (u8 *) &D_8013A42C;
    D_800CA13C = D_8013A404;
    D_800CA140 = D_8013A408;
    D_800CA144 = D_8013A40C;
    dialogue_run(D_80122CDC, 1, 1);
}
