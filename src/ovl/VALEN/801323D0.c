#include "ovl/VALEN.h"

typedef struct {
    void (*f[31])();
} FnTbl31; /* size 0x7C */
extern FnTbl31 D_801344A0;

void func_801323D0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl31 tbl;

    tbl = D_801344A0;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

void func_8013244C(void) {
    func_80044890(1, 0xBF98, 0xBF79, 0xCA95, 0xCA4F, 0xCA3E);
    if (func_80044E8C() == 1) {
        func_80044750(0x201);
        func_8004284C();
    }
    func_8004284C();
}

void func_801324B4(void) {
    if (func_80044E8C() == 1) {
        func_80044750(0x201);
        func_8004284C();
    }
    if ((u32) D_800E6280.unk_1104.w++ >= 0x400U) {
        func_800452C4();
        func_8004482C();
        func_80042940((D_800E6280.unk_110A - 1) & 0xFF);
    }
}

void func_8013253C(void) {
    func_80046318(7, 0x80197000, 0xAFAC);
    func_80132000();
    func_8004284C();
}

void func_80132578(void) {
    D_800E6280.unk_71E |= 0x10;
    func_800438DC(1, 0);
    func_80048DAC(1);
    func_80041584();
    func_80048EB8(0);
    func_800438F0(1);
    func_80048390();
    func_8004E58C();
    func_800AE0F0(D_800CA19C, "廊下");
    D_800E6280.unk_10A2 = 0;
    D_800E6280.unk_10E8 = 1;
    func_8008585C();
    D_800E6280.unk_03A = 0x80;
    D_800B593C = 0;
    D_800B5940 = 0;
    func_8007C740();
    func_800649D4();
    func_80064E84();
    func_80084E4C();
    func_80132000();
    D_8013448C = D_80134400;
    D_80134490 = D_80134434;
    D_80134494 = D_80134468;
    D_80134498 = 0;
    D_8013449C = 0;
    D_800E6280.unk_F5F = 0xC;
    func_800847B8(0xC);
    D_800E6280.unk_71E |= 8;
    func_8004284C();
}

void func_801326B8(void) {
    bg_read_sub2(0x4045);
    func_80085B3C(6, 0);
    func_8004284C();
}

void func_801326EC(void) {
    bg_read_sub2(0x4122);
    func_8004284C();
}

void func_80132714(void) {
    D_80134498 = (D_80134498 + (D_800E6280.unk_03E * 4)) - 0x180;
    func_8004284C();
}
