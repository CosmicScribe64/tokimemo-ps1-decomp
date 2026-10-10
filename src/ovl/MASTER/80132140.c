#include "ovl/MASTER.h"

typedef struct {
    void (*f[4])();
} FnTbl4; /* size 0x10 */
extern FnTbl4 D_8013C2E4;

void func_80132140(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl4 tbl;

    tbl = D_8013C2E4;
    idx = D_800E7389;
    tbl.f[idx]();
}

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER/80132140", func_801321B0);

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER/80132140", func_80132444);

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER/80132140", func_801326B4);

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER/80132140", func_80133374);

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER/80132140", func_8013348C);
