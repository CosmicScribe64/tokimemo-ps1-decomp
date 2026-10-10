#include "common.h"
#include "ovl/BUNKASAI.h"

typedef struct {
    void (*f[165])();
} FnTbl165; /* size 0x294 */
extern FnTbl165 D_80160810;

void func_80150930(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl165 tbl;

    tbl = D_80160810;
    idx = D_800E6280.unk_1109;
    tbl.f[idx]();
}

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI/80150930", func_801509A4);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI/80150930", func_80150B7C);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI/80150930", func_80150F20);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI/80150930", func_80151064);
