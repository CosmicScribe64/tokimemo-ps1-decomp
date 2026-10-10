#include "common.h"
#include "ovl/GEKO.h"

void func_8013B930(void) {
    bg_read_sub2(0x46A3);
    func_8004284C();
}

void func_8013B958(void) {
    bg_read_sub2(0x42D8);
    func_8004284C();
}

void func_8013B980(void) {
    D_80146780 = 0x801B0000;
    D_80146784 = 0x801B2000;
    D_80146788 = 0x801B6000;
    D_8014678C = 0x801BA000;
    D_80146790 = 0x801BE000;
    D_80146794 = 0x801C2000;
    D_80146798 = 0x801C6000;
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013B930", func_8013B9F0);

void func_8013BA6C(void) {
    if (D_800E7389 == 0) {
        func_8013BBE0();
        return;
    }
    func_80046500();
}

void func_8013BAA8(void) {
    if (D_800E7389 == 0) {
        func_8013BD04();
        return;
    }
    func_80046500();
}

void func_8013BAE4(void) {
    if (D_800E7389 == 0) {
        func_8013BE58();
        return;
    }
    func_80046500();
}

void func_8013BB20(void) {
    func_8004500C(0, 0x200);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013B930", func_8013BB4C);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013B930", func_8013BBE0);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013B930", func_8013BC74);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013B930", func_8013BCBC);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013B930", func_8013BD04);

void func_8013BD78(void) {
    func_80046290(0, 0, 0xC);
    func_80044750(0x300);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013B930", func_8013BDB0);

void func_8013BDF8(void) {
    func_80044750(0x500);
    func_8004284C();
}

void func_8013BE20(void) {
    func_80046318(0x35, 0x801B0000, 0x8265);
    func_8013B980();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013B930", func_8013BE58);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013B930", func_8013BEE0);

void func_8013BFA4(void) {
    func_80044750(0x24);
    func_80044750(0x503);
    func_8004284C();
}

void func_8013BFD4(void) {
    if (D_80122CDC != 0) {
        D_800CA150 = (u16) D_800CA150 + 5;
    }
    func_8004284C();
}

void func_8013C014(void) {
    func_8004284C();
    if (D_80122CDC != 0) {
        D_800E738A += 0x14;
        D_800E683B = 1;
        return;
    }
    D_800E683B = 2;
}

void func_8013C070(void) {
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013B930", func_8013C090);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013B930", func_8013C200);

void func_8013C330(void) {
    func_8013B980();
    load_palette(D_80146780, 0x11, 1, 2, 0);
    func_80084E90(D_80146784, D_80146788, D_8014678C, D_80146790, D_80146794, D_80146798);
    func_800850D4(0, 0, 0, 0);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013B930", func_8013C3C4);

void func_8013C41C(void) {
    bg_read_sub2(0x4045);
    func_8004284C();
}

void func_8013C444(void) {
    bg_read_sub2(0x4943);
    func_8004284C();
}

void func_8013C46C(void) {
    bg_read_sub2(0x403B);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013B930", func_8013C494);
