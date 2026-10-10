#include "ovl/MASTER.h"

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER/80138490", func_80138490);

typedef struct {
    void *p[3];
} PtrTbl3; /* size 0xC */
extern PtrTbl3 D_8013C700;

void func_80138E58(void) {
    s32 idx; /* unused: its stack slot sits above tbl (T-3330) */
    PtrTbl3 tbl;

    tbl = D_8013C700;
    func_800AE0F0(D_800CA25C, tbl.p[D_80122CDC]);
}

INCLUDE_RODATA("asm/ovl/MASTER/data/MASTER/80138490.rodata", D_8013BFB0);

INCLUDE_RODATA("asm/ovl/MASTER/data/MASTER/80138490.rodata", D_8013BFBC);

INCLUDE_RODATA("asm/ovl/MASTER/data/MASTER/80138490.rodata", D_8013BFC8);
