#include "common.h"
#include "ovl/GYOZI.h"

typedef struct {
    void (*f[23])();
} FnTbl23; /* size 0x5C */
extern FnTbl23 D_80145F00;

void func_80134C10(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl23 tbl;

    tbl = D_80145F00;
    idx = D_800F647A;
    tbl.f[idx](0x80);
}

void func_80134C98(void) {
    func_80086AB0(0x603);
    func_8004DE1C();
}

INCLUDE_RODATA("asm/ovl/GYOZI/data/GYOZI/80134C10.rodata", D_801458A0);

void func_80134CC0(void) {
    func_800BCE10(&D_800D92E0, &D_801458A0);
    func_8004DE1C();
}

void func_80134CF4(void) {
    func_8008A0D4(0x40E5);
    func_8004DE1C();
}

void func_80134D1C(void) {
    if (D_8012E66C != 0) {
        if ((D_800F62CF == 2) || (D_800F62CF == 7) || (D_800F62CF == 8) || (D_800F62CF == 9) || (D_800F62CF == 0xA)) {
            func_8008E310();
            return;
        }
        func_8008E2E0();
        return;
    }
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/80134C10", func_80134D9C);

void func_80134EA0(void) {
    if (D_8012E66C == 0) {
        func_8004DEAC(3);
        return;
    }
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/80134C10", func_80134EDC);
