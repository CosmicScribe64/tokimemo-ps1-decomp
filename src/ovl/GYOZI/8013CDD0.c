#include "common.h"
#include "ovl/GYOZI.h"

typedef struct {
    void (*f[19])();
} FnTbl19; /* size 0x4C */
extern FnTbl19 D_80147D64;

void func_8013CDD0(void) {
    if (D_801474B8 == 0) {
        func_8013CE0C();
        return;
    }
    func_8013CEB0();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/8013CDD0", func_8013CE0C);

void func_8013CE88(void) {
    func_80086AB0(0x502);
    func_8004DE1C();
}

void func_8013CEB0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl19 tbl;

    tbl = D_80147D64;
    idx = D_800F647A;
    tbl.f[idx]();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/8013CDD0", func_8013CF2C);

INCLUDE_RODATA("asm/ovl/GYOZI/data/GYOZI/8013CDD0.rodata", D_80145BE0);

void func_8013D01C(void) {
    D_801474A8 = 0;
    D_801474AC = 0;
    func_800BCE10(&D_800D92A0, &D_80145BE0);
    func_8004DE1C();
}

INCLUDE_RODATA("asm/ovl/GYOZI/data/GYOZI/8013CDD0.rodata", D_80145BEC);

void func_8013D060(void) {
    D_801474A8 = 5;
    D_801474AC = 0;
    func_800BCE10(&D_800D92A0, &D_80145BEC);
    func_8004DE1C();
}

void func_8013D0A8(void) {
    func_8008A0D4(0x4037);
    func_8004DE1C();
}

void func_8013D0D0(void) {
    if (D_800F62CF == 5) {
        func_80072734(0x5F0E);
    } else {
        func_80072734(0x5482);
    }
    func_8004DE1C();
}
