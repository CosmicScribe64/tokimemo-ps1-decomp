#include "common.h"
#include "ovl/GEKO.h"

void func_8013AF80(void) {
    D_80146600 = 0x801D212C;
    D_80146604 = 0x801D2130;
    D_80146608 = 0x801D2150;
    D_8014660C = (*(s16 *)0x801D216C);
    D_80146610 = 0x801B0000;
    D_80146614 = 0x801B2000;
    D_80146618 = 0x801B6000;
    D_8014661C = 0x801BA000;
    D_80146620 = 0x801BE000;
    D_80146624 = 0x801C2000;
    D_80146628 = 0x801C6000;
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013AF80", func_8013B030);

void func_8013B094(void) {
    if (D_800E7389 == 0) {
        func_8013B10C();
        return;
    }
    func_80046500();
}

void func_8013B0D0(void) {
    if (D_800E7389 == 0) {
        func_8013B2C8();
        return;
    }
    func_80046500();
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013AF80", func_8013B10C);

void func_8013B290(void) {
    func_80046318(0x45, 0x801B0000, 0x80FF);
    func_8013AF80();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013AF80", func_8013B2C8);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013AF80", func_8013B350);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013AF80", func_8013B3C0);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013AF80", func_8013B510);

void func_8013B65C(void) {
    func_8013AF80();
    load_palette(D_80146610, 0x11, 1, 2, 0);
    func_80084E90(D_80146614, D_80146618, D_8014661C, D_80146620, D_80146624, D_80146628);
    func_800850D4(0, 0, 0, 0);
    func_80048F64(0x63);
    D_8012071D = 0xA;
    D_8012071E = 0;
    D_80120754 = 0x41000000;
    D_8012071F = 4;
    D_80120728 = D_80146604;
    D_8012072C = D_80146608;
    D_80120750 = D_80146600;
    D_80120730 = D_8014660C;
    D_80120732 = 4;
    D_80120734 = 0;
    D_80120724 = 0;
    D_80120722 = 1;
    D_8012075F = 0x11;
    D_80120742 = -0xA0;
    D_80120746 = -0x78;
    D_80120723 = 0;
    D_80120721 = 0;
    D_80120720 = 0;
    D_800E7510 = -1;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013AF80", func_8013B7D0);

void func_8013B908(void) {
    bg_read_sub2(0x412C);
    func_8004284C();
}
