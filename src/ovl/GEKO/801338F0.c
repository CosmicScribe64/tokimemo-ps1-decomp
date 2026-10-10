#include "common.h"
#include "ovl/GEKO.h"

typedef struct {
    void (*f[19])();
} FnTbl19; /* size 0x4C */
extern FnTbl19 D_80144D40;

void func_801338F0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl19 tbl;

    tbl = D_80144D40;
    idx = D_800E738A;
    tbl.f[idx](0x80);
}

void func_8013396C(void) {
    if (D_80144C50 == 0x64) {
        func_801322E8();
        return;
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/801338F0", func_801339AC);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/801338F0", func_80133AA0);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/801338F0", func_80133C50);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/801338F0", func_80133F94);

void func_80133FE8(void) {
    D_80144C3C = 0x27;
    func_8004284C();
}

void func_80134010(void) {
    bg_read_sub2(0x42D8);
    func_8004284C();
}
