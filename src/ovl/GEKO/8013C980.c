#include "common.h"
#include "ovl/GEKO.h"

typedef struct {
    void (*f[22])();
} FnTbl22; /* size 0x58 */
extern FnTbl22 D_80146B64;

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013C980", func_8013C980);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013C980", func_8013CA30);

void func_8013CA94(void) {
    if (D_800E7389 == 0) {
        func_8013CAD0();
        return;
    }
    func_80046500();
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013C980", func_8013CAD0);

void func_8013CB58(void) {
    func_80046318(0x3D, 0x801B0000, 0x8440);
    func_8013C980();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013C980", func_8013CB90);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013C980", func_8013CBE4);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013C980", func_8013CC20);

void func_8013CC60(void) {
    if (D_800E7389 == 0) {
        func_8013CC9C();
        return;
    }
    func_80046500();
}

void func_8013CC9C(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl22 tbl;

    tbl = D_80146B64;
    idx = D_800E738A;
    tbl.f[idx](0x80);
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013C980", func_8013CD18);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013C980", func_8013CE98);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013C980", func_8013CF40);

void func_8013D06C(void) {
    bg_read_sub2(0x403B);
    func_8004284C();
}

void func_8013D094(void) {
    bg_read_sub2(0x403B);
    func_8004284C();
}

void func_8013D0BC(void) {
    bg_read_sub2(0x403B);
    func_8004284C();
}
