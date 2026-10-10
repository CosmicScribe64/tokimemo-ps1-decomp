#include "common.h"
#include "ovl/GEKO.h"

void func_80141AF0(void) {
    D_80147530 = 0x801CE130;
    D_80147534 = 0x801CE134;
    D_80147538 = 0x801CE15C;
    D_8014753C = (*(s16 *)0x801CE170);
    D_80147540 = 0x801B0000;
    D_80147544 = 0x801B2000;
    D_80147548 = 0x801B6000;
    D_8014754C = 0x801BA000;
    D_80147550 = 0x801BE000;
    D_80147554 = 0x801C2000;
    D_80147558 = 0x801C6000;
}

void func_80141BA0(void) {
    func_80141BC0();
}

void func_80141BC0(void) {
    if (D_800E7389 == 0) {
        func_80141BFC();
        return;
    }
    func_80046500();
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80141AF0", func_80141BFC);

void func_80141C94(void) {
    func_80046318(0x3D, 0x801B0000, 0x8FB5);
    func_80141AF0();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80141AF0", func_80141CCC);

void func_80141D74(void) {
    D_800E683E = D_800E69DD;
    D_800E71DF = D_800E69DD;
    func_800847B8(D_800E69DD);
    D_800E6280[D_800E71DF * 0x38 + 0x1C8] = D_800E6280[D_800E71DF * 0x38 + 0x1C8] & 0xFFFD;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80141AF0", func_80141DDC);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80141AF0", func_80141E48);

void func_80141FB8(void) {
    D_800CA150 = (u16) D_800CA150 + D_800E71DF;
    func_8004284C();
}

void func_80141FF0(void) {
    D_800CA150 = 0xC;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80141AF0", func_80142018);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80141AF0", func_801420E4);

void func_801425CC(void) {
    func_80065F34(0);
    func_80141AF0();
    load_palette(D_80147540, 0x11, 1, 2, 0);
    func_80084E90(D_80147544, D_80147548, D_8014754C, D_80147550, D_80147554, D_80147558);
    func_800850D4(D_80147534, D_80147538, D_80147530, D_8014753C);
    func_8004284C();
}

void func_8014267C(void) {
    bg_read_sub2(0x4055);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80141AF0", func_801426A4);
