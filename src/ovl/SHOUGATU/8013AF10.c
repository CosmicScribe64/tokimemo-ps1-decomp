#include "common.h"
#include "ovl/SHOUGATU.h"

void func_8013AF10(void) {
    func_8013AF30();
}

void func_8013AF30(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl42 tbl;

    tbl = D_801458B0;
    idx = D_800E6280.step;
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
    func_80048DAC(1);
    func_80041584();
    func_80048EB8(0);
    back_clear_switch(1);
    tpage_buf_clear();
    func_8004E58C();
    D_800E6280.tile_layer_enabled = 0;
    D_800E6280.quads_enabled = 1;
    ((ShouByteFlags *)&D_800E6280.girls[10].flags.b[0])->b3 = 1;
    func_8008585C();
    D_800E6280.screen_brightness = 0x80;
    D_800B593C = 0;
    D_800B5940 = 0;
    addr_init_bustup();
    hizuke_init();
    message_window_init();
    func_80084E4C();
    /* Chain store through byte views keeps one li for both stores and the argument (T-9150 idiom). */
    *(u8 *)&D_800E6280.date_girl = *(u8 *)&D_800E6280.current_girl = 0xA;
    func_800847B8(*(u8 *)&D_800E6280.current_girl);
    sprintf(D_800CA17C, "%sさん", D_800E6280.player_family_name);
    func_80136F90();
    D_80144DFC = D_80144DB0;
    D_80144E00 = D_80144DD4;
    D_80144E04 = D_80144DF8;
    func_8004284C();
}

void func_8013B114(void) {
    D_80144E08 = 0;
    D_80144E0C = 0;
    strcpy(D_800CA19C, "学校前");
    func_8004284C();
}

void func_8013B158(void) {
    bg_read_sub2(0x4016);
    func_8004284C();
}

void func_8013B180(void) {
    if ((u8) D_800E6280.girls[13].flags.b[3] < 2U) {
        D_80144E08 += 1;
    }
    func_8004284C();
}

void func_8013B1C4(void) {
    if ((u8) D_800E6280.girls[13].flags.b[3] >= 2U) {
        D_80144E08 += 1;
    }
    func_8004284C();
}
