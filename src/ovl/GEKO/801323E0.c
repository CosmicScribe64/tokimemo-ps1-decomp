#include "common.h"
#include "ovl/GEKO.h"

typedef struct {
    void (*f[33])();
} FnTbl33; /* size 0x84 */
extern FnTbl33 D_80144C5C;

void func_801323E0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl33 tbl;

    tbl = D_80144C5C;
    idx = D_800E738A;
    tbl.f[idx](0x80);
}

void func_80132454(void) {
    func_80046318(0x24, 0x80197000, 0xAEC8);
    func_80132000();
    func_8004284C();
}

void func_80132490(void) {
    func_80044890(1, 0xC043, 0xC01D, D_800B3688[D_800E71DF], D_800B36C8[D_800E71DF], D_800B3708[D_800E71DF]);
    if (func_80044E8C() == 1) {
        func_8004284C();
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/801323E0", func_80132518);

void func_80132598(void) {
    func_80044750(0x200);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/801323E0", func_801325C0);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/801323E0", func_801327AC);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/801323E0", func_80132A4C);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/801323E0", func_80132B24);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/801323E0", func_80132D04);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/801323E0", func_80133024);

void func_8013313C(void) {
    bg_read_sub2(0x4032);
    func_8004284C();
}

void func_80133164(void) {
    if (D_80144C58 != 0) {
        func_801322E8();
        return;
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/801323E0", func_801331A0);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/801323E0", func_80133250);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/801323E0", func_80133308);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/801323E0", func_80133598);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/801323E0", func_8013375C);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/801323E0", func_801337EC);
