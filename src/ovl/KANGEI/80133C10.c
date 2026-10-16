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
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

void func_80133C84(void) {
    D_800E6280.unk_F5F = D_800E6280.unk_75D;
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

    func_800438DC(1, 0);
    func_80048DAC(1);
    func_80041584();
    func_800438F0(1);
    func_80048EB8(0);
    func_80064E84();
    func_800649D4();
    func_80048390();
    func_8004E58C();
    D_800E6280.unk_10A2 = 0;
    D_800E6280.unk_10E8 = 1;
    D_800B593C = 0;
    func_8008585C();
    func_80063930(0);
    func_80064FA4(0);
    D_80139ADC = 0;
    D_80139AE0 = 0;
    func_8007C740();
    func_80084E4C();
    D_800E6280.unk_F5F = 0xFF;
    v = D_800E6280.unk_0F4.h >> 12;
    for (i = 0; i < 0xB; i++) {
        if (v == (D_800E6280.unk_1BC[i].unk_0C.b[2] >> 4)) {
            D_800E6280.unk_F5F = i;
            break;
        }
    }
    if (v == 0xA && D_800E6280.unk_1BC[0].unk_02 + D_800E6280.unk_1BC[0].unk_06 < D_800E6280.unk_1BC[10].unk_02 + D_800E6280.unk_1BC[10].unk_06 && D_800E6280.unk_03E >= 0x60) {
        D_800E6280.unk_F5F = 0xA;
    }
    D_800E6280.unk_75D = D_800E6280.unk_F5F;
    func_800847B8(D_800E6280.unk_F5F);
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
    func_800AE0F0(D_800CA19C, "合宿所");
}
