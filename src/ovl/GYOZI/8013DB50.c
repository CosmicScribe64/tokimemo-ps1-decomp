#include "common.h"
#include "ovl/GYOZI.h"

void func_8013DB50(void) {
    func_8013DB70();
}

typedef struct {
    void (*f[42])();
} FnTbl42; /* size 0xA8 */
extern FnTbl42 D_80147FB0;

void func_8013DB70(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl42 tbl;

    tbl = D_80147FB0;
    idx = D_800F647A;
    tbl.f[idx]();
}

void func_8013DBE4(void) {
    func_8013A820();
    if (D_801474AC == 2) {
        D_8012E6B0 = 1;
        return;
    }
    D_8012E6B0 = 0;
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/8013DB50", func_8013DC2C);

INCLUDE_RODATA("asm/ovl/GYOZI/data/GYOZI/8013DB50.rodata", D_80145C40);

void func_8013DD38(void) {
    D_801474A8 = 0;
    D_801474AC = 0;
    func_800BCE10(&D_800D92A0, &D_80145C40);
    func_8004DE1C();
}

void func_8013DD7C(void) {
    func_8008A0D4(0x3FC0);
    func_8004DE1C();
}

void func_8013DDA4(void) {
    if ((u8) D_800F5833 < 2U) {
        D_801474A8 += 1;
    }
    func_8004DE1C();
}

void func_8013DDE8(void) {
    if ((u8) D_800F5833 >= 2U) {
        D_801474A8 += 1;
    }
    func_8004DE1C();
}
