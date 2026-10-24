#include "common.h"
#include "ovl/SHOUGATU.h"

void func_8013D660(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl33 tbl;

    tbl = D_80145F40;
    idx = D_800E6280.step;
    tbl.f[idx](0x80);
}

void func_8013D6D4(void) {
    func_80044890(1, 0xC621, 0xC613, 0xD93B, 0xD8F0, 0xD8D0);
    if (func_80044E8C() == 1) {
        func_8004284C();
    }
    func_8004284C();
}

void func_8013D734(void) {
    if (func_80044E8C() == 1) {
        func_80044750(0x202);
        func_8004284C();
    }
    if ((u32) D_800E6280.step_timer.w++ >= 0x400U) {
        func_800452C4();
        func_8004482C();
        func_80042940((D_800E6280.step - 1) & 0xFF);
    }
}

void func_8013D7BC(void) {
    cd_read_sectors(0xE, 0x80197000, 0xAFBC);
    func_8013C070();
    func_8004284C();
}

void func_8013D7F8(void) {
    D_80122CFC = 0x1E;
    func_8004284C();
}

void func_8013D820(void) {
    s16 thresh = D_800E6280.girls[12].flags.f.story_stage * 25 + 150;

    if (D_800E6280.param_athletics.value >= thresh &&
        D_800E6280.girls[14].flags.f.story_stage != 0) {
        func_8004284C();
        func_8004284C();
        func_8004284C();
        func_8004284C();
        return;
    }
    if (D_800E6280.param_looks.value >= (s16)(D_800E6280.girls[12].flags.f.story_stage * 30 - (D_800E6280.girls[12].flags.f.story_stage >= 2) * 10 + 70)) {
        func_8004284C();
        return;
    }
    func_8004284C();
    func_8004284C();
    func_8004284C();
    func_8004284C();
}

void func_8013D92C(void) {
    s16 v;
    /* FAKE: GameState *g splits the second unk_1BC[14].unk_0C access from the first so uopt keeps it in $t2 instead of promoting to $v0. T-24130 */
    GameState *g = &D_800E6280;

    v = D_800E6280.girls[12].flags.f.story_stage * 25 + 150;
    if (D_800E6280.param_athletics.value >= v) {
        D_80145F2C = (D_800E6280.girls[14].flags.f.story_stage >= 1) * 3 + 4;
        g->girls[14].flags.f.story_stage += 1;
        func_8004284C();
        return;
    }
    D_80145F2C += 1;
    D_800E6280.param_guts.value += 10;
    D_800E6280.param_stress.value += 10;
    check_para_limit();
    func_8004284C();
    func_8004284C();
    func_8004284C();
    func_8004284C();
    func_8004284C();
    func_8004284C();
    func_8004284C();
    func_8004284C();
}

void func_8013DA5C(void) {
    if (((u32) (D_800E6280.girls[14].flags.w << 0x14) >> 0x1D) == 1) {
        func_8013D2A8();
        return;
    }
    func_8004284C();
    func_8004284C();
}

s32 func_8013DAAC(void) {
    GameState *g;

    if (D_800E6280.girls[0].birthday.date.month == 12 &&
        D_800E6280.girls[0].birthday.date.day == 24 &&
        !(*(u8 *)&D_800E6280.daily_event_bits & 0x20)) {
        *(u8 *)&D_800E6280.daily_event_bits |= 0x20;
        D_800E6280.day_events[D_800E6280.day_event_index].girl = 0;
        main_state_set(MODE_GIRL_BIRTHDAY);
        return 0;
    }
    if (((WordView *)&D_800E6280.player_birthday)->date.month == 12 &&
        ((WordView *)&D_800E6280.player_birthday)->date.day == 24 &&
        !(*(u8 *)&D_800E6280.daily_event_bits & 0x40)) {
        *(u8 *)&D_800E6280.daily_event_bits |= 0x40;
        main_state_set(MODE_PLAYER_BIRTHDAY);
        return 0;
    }
    draw2d3d(1, 0);
    func_80048DAC(1);
    func_80041584();
    func_80048EB8(0);
    back_clear_switch(1);
    tpage_buf_clear();
    func_8004E58C();
    func_8008585C();
    strcpy(D_800CA19C, "伊集院邸");
    D_800E6280.tile_layer_enabled = 0;
    D_800E6280.quads_enabled = 1;
    D_800E6280.screen_brightness = 0x80;
    D_800B593C = 0;
    D_800B5940 = 0;
    D_80145F2C = 0;
    D_80145F30 = 0;
    addr_init_bustup();
    hizuke_init();
    message_window_init();
    func_80084E4C();
    g = &D_800E6280;
    strcpy(D_800CA16C, g->player_family_name);
    if (g->girls[12].flags.f.story_stage >= 3) {
        g->girls[12].flags.f.story_stage = 2;
    }
    if (g->girls[12].flags.f.story_stage == 0) {
        strcpy(D_800CA188, "黒服");
    } else {
        strcpy(D_800CA188, "外井");
    }
    D_800E6280.current_girl = GIRL_REI;
    func_8013C070();
    D_80145F20 = D_80145AFC;
    D_80145F24 = D_80145B30;
    D_80145F28 = D_80145B64;
    func_8004284C();
}

void func_8013DCFC(void) {
    bg_read_sub2(0x41CA);
    func_8004284C();
}

void func_8013DD24(void) {
    func_80062CD0(0x6B24);
    func_8004284C();
}

void func_8013DD4C(void) {
    func_80042808();
}

void func_8013DD6C(void) {
    func_80042908(4);
}
