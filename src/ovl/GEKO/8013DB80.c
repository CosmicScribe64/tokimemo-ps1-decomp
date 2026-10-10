#include "common.h"
#include "ovl/GEKO.h"

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013DB80", func_8013DB80);

void func_8013DC30(void) {
    if (D_80122CD0 == 1) {
        func_8013DC70();
        return;
    }
    func_80046500();
}

void func_8013DC70(void) {
    if (D_800E7389 == 0) {
        func_8013DCAC();
        return;
    }
    func_80046500();
}

typedef struct {
    void (*f[33])();
} FnTbl33; /* size 0x84 */
extern FnTbl33 D_80146DCC;

void func_8013DCAC(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl33 tbl;

    tbl = D_80146DCC;
    idx = D_800E738A;
    tbl.f[idx]();
}

void func_8013DD20(void) {
    func_80046318(0x3D, 0x801B0000, 0x8947);
    func_8013DB80();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013DB80", func_8013DD58);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013DB80", func_8013DE90);

void func_8013DF40(void) {
    bg_read_sub2(0x403B);
    func_8004284C();
}
