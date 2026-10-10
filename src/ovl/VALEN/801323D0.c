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

INCLUDE_ASM("asm/ovl/VALEN/nonmatchings/VALEN/801323D0", func_80132578);

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
