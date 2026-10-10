#include "ovl/OMIMAI.h"

INCLUDE_ASM("asm/ovl/OMIMAI/nonmatchings/OMIMAI/801320A0", func_801320A0);

INCLUDE_ASM("asm/ovl/OMIMAI/nonmatchings/OMIMAI/801320A0", func_801322E4);

void func_80132398(void) {
    if ((D_800E62C0 == ((u32)(D_800E6378 << 0x17) >> 0x1B)) && (D_800E62BF == (D_800E6378 & 0xF))) {
        func_80042808();
        return;
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/OMIMAI/nonmatchings/OMIMAI/801320A0", func_801323F8);

INCLUDE_ASM("asm/ovl/OMIMAI/nonmatchings/OMIMAI/801320A0", func_801324BC);

INCLUDE_ASM("asm/ovl/OMIMAI/nonmatchings/OMIMAI/801320A0", func_80132544);

void func_801325C4(void) {
    func_80044750(0x200);
    func_8004284C();
}

void func_801325EC(void) {
    func_80046318(4, 0x80197000, 0xAF3F);
    func_801320A0();
    func_80044750(0x603);
    func_8004284C();
}

void func_80132630(void) {
    func_8004E58C();
    k_reset(1);
    addr_init_bustup();
    func_80084E4C();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/OMIMAI/nonmatchings/OMIMAI/801320A0", func_80132670);

void func_80132788(void) {
    bg_read_sub2(0x4144);
    func_80085B3C(0xF, 0);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/OMIMAI/nonmatchings/OMIMAI/801320A0", func_801327BC);

INCLUDE_ASM("asm/ovl/OMIMAI/nonmatchings/OMIMAI/801320A0", func_8013285C);
