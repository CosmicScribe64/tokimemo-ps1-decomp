#include "common.h"
#include "ovl/DATE2.h"

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2/80136D50", func_80136D50);

typedef struct {
    void (*f[8])();
} FnTbl8; /* size 0x20 */
extern FnTbl8 D_8013A74C;

void func_80136FC4(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl8 tbl;

    tbl = D_8013A74C;
    idx = D_800E738A;
    tbl.f[idx](0x80);
}

void func_80137064(void) {
    func_80046318(3, 0x801B0000, 0xAF43);
    func_80136D50();
    func_8004284C();
}

void func_8013709C(void) {
    D_800CA148 = 1;
    D_800CA14C = 0;
    func_800847B8(D_800E71DF);
    func_801370E4();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2/80136D50", func_801370E4);
