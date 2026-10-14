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

void func_8013AAEC(void) {
    s32 temp;
    temp = D_800CA2CC;
    switch (temp) {                           /* irregular */
    case 0:
        func_800AE0F0(D_800CA23C, "京都");
        break;
    case 1:
        func_800AE0F0(D_800CA23C, "沖縄");
        break;
    case 2:
        func_800AE0F0(D_800CA23C, "北海道");
        break;
    }
    D_800CA2DC += D_800CA2F4 * (1 - D_800CA2EC);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/8013AA70", func_8013ABAC);
