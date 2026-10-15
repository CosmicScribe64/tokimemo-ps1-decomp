#include "common.h"
#include "ovl/SHOUGATU.h"

typedef struct {
    void (*f[42])();
} FnTbl42; /* size 0xA8 */
extern FnTbl42 D_801458B0;

void func_8013AF10(void) {
    func_8013AF30();
}

void func_8013AF30(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl42 tbl;

    tbl = D_801458B0;
    idx = D_800E6280.unk_110A;
    tbl.f[idx]();
}

void func_8013AFA4(void) {
    func_80137AB4();
    if (D_80144E0C == 2) {
        D_80122D20 = 1;
        return;
    }
    D_80122D20 = 0;
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013AF10", func_8013AFEC);

void func_8013B114(void) {
    D_80144E08 = 0;
    D_80144E0C = 0;
    func_800AE0F0(D_800CA19C, "学校前");
    func_8004284C();
}

void func_8013B158(void) {
    bg_read_sub2(0x4016);
    func_8004284C();
}

void func_8013B180(void) {
    if ((u8) D_800E6280.unk_1BC[13].unk_0C.b[3] < 2U) {
        D_80144E08 += 1;
    }
    func_8004284C();
}

void func_8013B1C4(void) {
    if ((u8) D_800E6280.unk_1BC[13].unk_0C.b[3] >= 2U) {
        D_80144E08 += 1;
    }
    func_8004284C();
}
