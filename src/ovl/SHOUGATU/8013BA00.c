#include "common.h"
#include "ovl/SHOUGATU.h"

typedef struct {
    void (*f[19])();
} FnTbl19; /* size 0x4C */
extern FnTbl19 D_80145A80;

void func_8013BA00(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl19 tbl;

    tbl = D_80145A80;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013BA00", func_8013BA7C);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013BA00", func_8013BC2C);

void func_8013BCE0(void) {
    D_800E6280.unk_F5F = (u8) D_80122CDC;
    func_80085B3C(7, 0);
    func_800847B8(D_800E6280.unk_F5F);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013BA00", func_8013BD2C);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013BA00", func_8013BDE0);
