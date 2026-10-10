#include "common.h"
#include "ovl/SHOUGATU.h"

void func_80137BB0(void) {
    switch (D_80144E14) {
    case 0:
        func_80137C28();
        return;
    case 1:
        func_80137C28();
        return;
    case 2:
        func_80137C28();
        return;
    default:
        func_80137D68();
        return;
    }
}

typedef struct {
    void (*f[24])();
} FnTbl24; /* size 0x60 */
extern FnTbl24 D_80144E20;

void func_80137C28(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl24 tbl;

    tbl = D_80144E20;
    idx = D_800E738A;
    tbl.f[idx]();
}

void func_80137C9C(void) {
    func_80044750(0x23);
    func_80044750(0x24);
    func_80044750(0x500);
    func_8004284C();
}

void func_80137CD4(void) {
    func_80044750(0x23);
    func_80044750(0x24);
    func_80044750(0x507);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80137BB0", func_80137D0C);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80137BB0", func_80137D68);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80137BB0", func_80137DE4);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80137BB0", func_80137E54);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80137BB0", func_80137F70);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80137BB0", func_80137FC4);
