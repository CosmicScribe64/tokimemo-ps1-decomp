#include "common.h"
#include "ovl/KANGEI.h"

typedef struct {
    void (*f[15])();
} FnTbl15; /* size 0x3C */
extern FnTbl15 D_80139AE8;

void func_80133C10(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl15 tbl;

    tbl = D_80139AE8;
    idx = D_800E6280.step;
    tbl.f[idx](0x80);
}

void func_80133C84(void) {
    D_800E6280.current_girl = D_800E6280.date_girl;
    func_80042808();
}

void func_80133CB0(void) {
    cd_read_sectors(0x3B, 0x80197000, 0xAE60);
    func_80132E74();
    func_8004284C();
}

void func_80133CEC(void) {
    cd_read_sectors(0x2D, 0x801B4400, 0xAE9B);
    func_80132E40();
    func_8004284C();
}

void func_80133D28(void) {
    s16 i;
    u32 v;

    draw2d3d(1, 0);
    func_80048DAC(1);
    func_80041584();
    back_clear_switch(1);
    func_80048EB8(0);
    message_window_init();
    hizuke_init();
    tpage_buf_clear();
    func_8004E58C();
    D_800E6280.tile_layer_enabled = 0;
    D_800E6280.quads_enabled = 1;
    D_800B593C = 0;
    func_8008585C();
    parameter_disp_switch(0);
    icon_disp_switch(0);
    D_80139ADC = 0;
    D_80139AE0 = 0;
    addr_init_bustup();
    func_80084E4C();
    D_800E6280.current_girl = 0xFF;
    v = D_800E6280.player_flags.h >> 12;
    for (i = 0; i < 0xB; i++) {
        if (v == (D_800E6280.girls[i].flags.b[2] >> 4)) {
            D_800E6280.current_girl = i;
            break;
        }
    }
    if (v == 0xA && D_800E6280.girls[0].tokimeki + D_800E6280.girls[0].affection < D_800E6280.girls[10].tokimeki + D_800E6280.girls[10].affection && D_800E6280.year >= 0x60) {
        D_800E6280.current_girl = GIRL_YUMI;
    }
    D_800E6280.date_girl = D_800E6280.current_girl;
    func_800847B8(D_800E6280.current_girl);
    func_80132E74();
    D_80139AD0 = (KObj *)D_80139A5C[0][6];
    D_80139AD4 = D_80139A5C[1][6];
    D_80139AD8 = D_80139A5C[2][6];
    D_80139AE4 = 0;
    D_80139AC0 = 0;
    func_8004284C();
}

void func_80133EF8(void) {
    D_800CA134 = (u8 *)&D_80139ADC;
    D_800CA138 = (u8 *)&D_80139AE0;
    D_800CA13C = (s32)D_80139AD0;
    D_800CA140 = D_80139AD4;
    D_800CA144 = D_80139AD8;
    func_80082764(0xFF, 1, *(u8 *)&D_80139AE4);
}

void func_80133F74(void) {
    D_800CA134 = (u8 *)&D_80139ADC;
    D_800CA138 = (u8 *)&D_80139AE0;
    D_800CA13C = (s32)D_80139AD0;
    D_800CA140 = D_80139AD4;
    D_800CA144 = D_80139AD8;
    func_80082764(D_80122CDC, 1, *(u8 *)&D_80139AE4);
}

void func_80133FF4(void) {
    D_800CA134 = (u8 *)&D_80139ADC;
    D_800CA138 = (u8 *)&D_80139AE0;
    D_800CA13C = (s32)D_80139AD0;
    D_800CA140 = D_80139AD4;
    D_800CA144 = D_80139AD8;
    if (func_80082764(0xFF, 1, *(u8 *)&D_80139AE4) == 1) {
        func_8004284C();
    }
}

void func_80134084(void) {
    RECT rect;

    dec_bg_reset();
    rect.x = 0x140;
    rect.y = 0x100;
    rect.w = 0x140;
    rect.h = 0xF8;
    func_8009C7F8(&rect, 8, 0, 0);
    rect.x = 0x2C0;
    rect.y = 0;
    func_8009C7F8(&rect, 8, 0, 0);
    bg_read_sub2(0x428D);
    func_8004284C();
    strcpy(D_800CA19C, "合宿所");
}
