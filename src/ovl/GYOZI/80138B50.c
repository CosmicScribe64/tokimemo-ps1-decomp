#include "common.h"
#include "ovl/GYOZI.h"

void func_80138B50(void) {
    func_8008A0D4(0x3FED);
    func_80090960(6, 0);
    func_8004DE1C();
}

void func_80138B84(void) {
    func_8008A0D4(0x40C1);
    func_8004DE1C();
}

void func_80138BAC(void) {
    D_80147198 = (D_80147198 + (D_800F53DE * 3)) - 0x120;
    func_8004DE1C();
}

typedef struct {
    void (*f[56])();
} FnTbl56; /* size 0xE0 */
extern FnTbl56 D_80147248;

void func_80138BF0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl56 tbl;

    tbl = D_80147248;
    idx = D_800F647A;
    tbl.f[idx](0x80);
}

void func_80138C78(void) {
    D_800F62CF = 0;
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/80138B50", func_80138C9C);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/80138B50", func_8013907C);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/80138B50", func_801393C8);

void func_801396D8(void) {
    D_80147220 += 1;
    func_8004DEE4(1);
}

void func_8013970C(void) {
    if (func_800BDC20(&D_800D92A0, &D_800D92E0) == 0) {
        func_8004DE1C();
    }
    func_800BCE10(&D_800D92A0, &D_800D92E0);
    func_8004DE1C();
}

void func_80139764(void) {
    if ((D_80147240 == 1) && (D_800F62CF == 9)) {
        D_800F647A += 6;
        return;
    }
    func_8004DE1C();
}

INCLUDE_RODATA("asm/ovl/GYOZI/data/GYOZI/80138B50.rodata", D_80145A40);

void func_801397BC(void) {
    func_800BCE10(&D_800D92E0, &D_80145A40);
    func_8008A0D4(0x401A);
    func_8004DE1C();
}

void func_801397F8(void) {
    func_8008F618(0);
    func_80072734(0x53A1);
    func_8004DE1C();
}

void func_80139828(void) {
    func_8008F618(9);
    func_80072734(0x63CD);
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/80138B50", func_80139858);
