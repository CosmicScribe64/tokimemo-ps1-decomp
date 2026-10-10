#include "common.h"
#include "ovl/SHOUGATU.h"

void func_8013A190(void) {
    if (D_80144E14 == 0) {
        func_8013A1CC();
        return;
    }
    func_8013A27C();
}

typedef struct {
    void (*f[26])();
} FnTbl26; /* size 0x68 */
extern FnTbl26 D_801455E0;

void func_8013A1CC(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl26 tbl;

    tbl = D_801455E0;
    idx = D_800E738A;
    tbl.f[idx]();
}

void func_8013A254(void) {
    func_80044750(0x502);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013A190", func_8013A27C);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013A190", func_8013A2F8);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013A190", func_8013A3FC);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013A190", func_8013A440);

void func_8013A488(void) {
    bg_read_sub2(0x4097);
    func_8004284C();
}

void func_8013A4B0(void) {
    if (D_800E71DF == 5) {
        func_80062CD0(0x5FE4);
    } else {
        func_80062CD0(0x552B);
    }
    func_8004284C();
}
