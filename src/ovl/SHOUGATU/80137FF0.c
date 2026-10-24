#include "common.h"
#include "game.h"
#include "ovl/SHOUGATU.h"

void func_80137FF0(void) {
    switch (D_80144E14) {
    case 0:
        func_8013808C();
        return;
    case 1:
        func_801381B8();
        return;
    case 2:
        func_80138240();
        return;
    case 3:
        func_8013843C();
        return;
    case 4:
        func_801384B0();
        return;
    default:
        func_80138524();
        return;
    }
}

void func_8013808C(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl20 tbl;

    tbl = D_80144F14;
    idx = D_800E6280.step;
    tbl.f[idx]();
    if (D_80144E08 == 1) {
        if (D_80144E0C == 8) {
            if (D_800E6280.step_timer.w++ == 0) {
                func_80044750(0x500);
            }
        }
    }
}

s32 func_80138158(void) {
    func_80137AB4();
    if (D_80144E0C == 2) {
        if (D_800E6280.step_timer.w++ == 1) {
            func_80044750(0x24);
            func_80044750(0x501);
        }
    }
}

void func_801381B8(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl20 tbl;

    tbl = D_80144F64;
    idx = D_800E6280.step;
    tbl.f[idx]();
}

void func_80138240(void) {
    s32 idx;
    FnTbl37 table;

    table = D_80144FB4;
    idx = D_800E6280.step;
    if (D_80144F10 == 0 && D_800E6280.step >= 0x13 && D_800E6280.step < 0x1C) {
        func_8004A414(0x60, -0x10, 0x30, 8, 8, 0xD);
    }
    if (D_80144F10 == 1 && D_800E6280.step >= 0x13 && D_800E6280.step < 0x1C) {
        func_8004A414(-0x3C, -0x20, 0x20, 8, 8, 0xD);
    }
    idx = D_800E6280.step;
    table.f[idx]();
}

void func_80138354(void) {
    func_80137AB4();
    if (D_800E6280.step_timer.w++ == 0) {
        if (D_80144E0C == 3) {
            func_80044750(0x500);
        }
    }
}

void func_801383AC(void) {
    func_80044750(0x500);
    func_8004284C();
}

void func_801383D4(void) {
    func_80044750(0x501);
    func_8004284C();
}

void func_801383FC(void) {
    if (D_80144F10 != 0) {
        D_80144E08 += 1;
    }
    func_8004284C();
}

void func_8013843C(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl30 tbl;

    tbl = D_80145048;
    idx = D_800E6280.step;
    tbl.f[idx]();
}

void func_801384B0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl24 tbl;

    tbl = D_801450C0;
    idx = D_800E6280.step;
    tbl.f[idx]();
}

void func_80138524(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl20 tbl;

    tbl = D_80145120;
    idx = D_800E6280.step;
    tbl.f[idx]();
}

void func_801385AC(void) {
    draw2d3d(1, 0);
    func_80048DAC(1);
    func_80041584();
    func_80048EB8(0);
    back_clear_switch(1);
    tpage_buf_clear();
    func_8004E58C();
    D_800E6280.tile_layer_enabled = 0;
    D_800E6280.quads_enabled = 1;
    func_8008585C();
    D_800E6280.screen_brightness = 0x80;
    D_800B593C = 0;
    D_800B5940 = 0;
    addr_init_bustup();
    hizuke_init();
    message_window_init();
    func_80084E4C();
    func_800847B8(*(u8 *)&D_800E6280.current_girl = 2);
    func_80136F90();
    D_80144DFC = D_80144D94;
    D_80144E00 = D_80144DB8;
    D_80144E04 = D_80144DDC;
    if (((u32)D_800E6280.girls[2].flags.b[2] >> 4) == 3) {
        D_80144F10 = 1;
    } else {
        D_80144F10 = 0;
    }
    func_8004284C();
}

void func_801386CC(void) {
    D_80144E08 = 0;
    D_80144E0C = 0;
    strcpy(D_800CA19C, "教室");
    func_8004284C();
}

void func_80138710(void) {
    D_80144E08 = 3;
    D_80144E0C = 0;
    strcpy(D_800CA19C, "教室");
    func_8004284C();
}

void func_80138758(void) {
    s32 unused; /* FAKE: preserves RECT at stack +0x2C. T-10010. */
    RECT rect;

    rect.x = 0x480;
    rect.y = 0x160;
    rect.w = 0x10;
    rect.h = 0x10;
    func_8009C7F8(&rect, 0, 0, 0);
    ((GsByteNibbles *)&D_800E6280.player_flags.b[1])->high = ((GsByteNibbles *)&D_800E6280.girls[2].flags.b[2])->high;
    D_800E6280.girls[D_800E6280.current_girl].flags.f.b2 = 1;
    ((ShouByteFlags *)&D_800E6280.girls[D_800E6280.current_girl].flags.b[2])->b3 = 1;
    D_80144E08 = 6;
    D_80144E0C = 0;
    strcpy(D_800CA19C, "廊下");
    strcpy(D_800CA1DC, "実験室");
    func_8004284C();
}

void func_80138868(void) {
    D_800E6280.girls[D_800E6280.current_girl].flags.f.b2 = 1;
    D_80144E08 = 0xE;
    D_80144E0C = 0;
    strcpy(D_800CA19C, "教室");
    strcpy(D_800CA1DC, "実験室");
    func_8004284C();
}

void func_801388F0(void) {
    D_800E6280.girls[D_800E6280.current_girl].flags.f.b2 = 1;
    D_80144E08 = 0x12;
    D_80144E0C = 0;
    strcpy(D_800CA19C, "実験室");
    func_8004284C();
}

void func_80138964(void) {
    D_800E6280.girls[D_800E6280.current_girl].flags.f.b2 = 1;
    D_80144E08 = 0x17;
    D_80144E0C = 0;
    strcpy(D_800CA19C, "実験室");
    func_8004284C();
}

void func_801389D8(void) {
    bg_read_sub2(0x405F);
    func_8004284C();
}

void func_80138A00(void) {
    bg_read_sub2(0x403B);
    func_8004284C();
}

void func_80138A28(void) {
    if (D_80144F10 != 0) {
        bg_read_sub2(0x40D0);
    } else {
        bg_read_sub2(0x40C6);
    }
    func_8004284C();
}

void func_80138A6C(void) {
    bg_read_sub2(0x40C6);
    func_8004284C();
}

void func_80138A94(void) {
    bg_read_sub2(0x40D0);
    func_8004284C();
}

void func_80138ABC(void) {
    if (D_80144F10 != 0) {
        D_80144E08 += 1;
    }
    func_8004284C();
}

void func_80138AFC(void) {
    if (D_80144F10 == 0) {
        D_80144E08 += 1;
    }
    func_8004284C();
}
