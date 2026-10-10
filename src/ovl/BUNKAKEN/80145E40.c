#include "common.h"
#include "ovl/BUNKAKEN.h"

typedef struct {
    void (*f[142])();
} FnTbl142; /* size 0x238 */
extern FnTbl142 D_80156060;

void func_80145E40(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl142 tbl;

    tbl = D_80156060;
    idx = D_800E6280.unk_1109;
    tbl.f[idx]();
}

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80145E40", func_80145EBC);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80145E40", func_801462A0);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80145E40", func_801463E4);
