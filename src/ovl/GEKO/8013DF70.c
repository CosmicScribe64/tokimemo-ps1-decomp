#include "common.h"
#include "ovl/GEKO.h"

typedef struct {
    void (*f[46])();
} FnTbl46; /* size 0xB8 */
extern FnTbl46 D_80146E7C;

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013DF70", func_8013DF70);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013DF70", func_8013E040);

void func_8013E0A4(void) {
    if (D_800E7389 == 0) {
        func_8013E11C();
        return;
    }
    func_80046500();
}

void func_8013E0E0(void) {
    if (D_800E7389 == 0) {
        func_8013E4F0();
        return;
    }
    func_80046500();
}

void func_8013E11C(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl46 tbl;

    tbl = D_80146E7C;
    idx = D_800E738A;
    tbl.f[idx](0x80);
    func_8013E950();
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013DF70", func_8013E1A0);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013DF70", func_8013E22C);

void func_8013E410(void) {
    func_80046318(0x45, 0x801B0000, 0x8A88);
    func_8013DF70();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013DF70", func_8013E448);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013DF70", func_8013E4F0);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013DF70", func_8013E56C);

void func_8013E5C8(void) {
    if (get_h_yuukou(9) < 0x50U) {
        D_800CA150 = (u16) D_800CA150 + 2;
    }
    func_8004284C();
}

void func_8013E60C(void) {
    if (get_h_yuukou(9) < 0x50U) {
        D_800E738A += 0xE;
    }
    func_8004284C();
}

void func_8013E650(void) {
    if (get_h_yuukou(9) >= 0x50U) {
        func_8004284C();
        func_8004284C();
        func_8004284C();
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013DF70", func_8013E69C);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013DF70", func_8013E760);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013DF70", func_8013E828);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013DF70", func_8013E950);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013DF70", func_8013ECA8);

void func_8013EDC4(void) {
    bg_read_sub2(0x4068);
    func_8004284C();
}

void func_8013EDEC(void) {
    bg_read_sub2(0x46B2);
    func_8004284C();
}

void func_8013EE14(void) {
    func_8007ED84(0x403B);
    func_8004284C();
}
