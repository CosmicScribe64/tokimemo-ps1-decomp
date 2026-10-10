#include "common.h"
#include "ovl/SHUGAKU.h"

typedef struct {
    void (*f[25])();
} FnTbl25; /* size 0x64 */
extern FnTbl25 D_8013CC20;

void func_8013AA70(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl25 tbl;

    tbl = D_8013CC20;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/8013AA70", func_8013AAEC);

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/8013AA70", func_8013ABAC);
