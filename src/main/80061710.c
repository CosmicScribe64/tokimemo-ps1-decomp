#include "common.h"
#include "game.h"

void func_80061710(void) {
    func_8009AD30(0x3E8);
    D_80122740 = 0;
    D_80122744 = 0;
    D_80122748 = 0x3E8;
    D_8012274C = 0;
    D_80122750 = 0;
    D_80122754 = 0;
    D_80122758 = 0;
    D_8012275C = 0;
    func_80099540(&D_80122740);
    func_8009AD50(0x64);
    func_8009AD60(0x10000);
}

INCLUDE_ASM("asm/nonmatchings/main/80061710", func_80061790);

INCLUDE_ASM("asm/nonmatchings/main/80061710", func_800618B0);

INCLUDE_ASM("asm/nonmatchings/main/80061710", func_8006190C);

INCLUDE_ASM("asm/nonmatchings/main/80061710", func_80061A3C);

INCLUDE_ASM("asm/nonmatchings/main/80061710", func_80061EFC);

INCLUDE_ASM("asm/nonmatchings/main/80061710", func_80061FC4);

void func_8006211C(void) {
    func_8004AC18(0xFF - D_800E7384);
    if ((u32)D_800E7384 >= 0xFFU) {
        func_80042808();
    } else if (D_800E7208 & 0x860) {
        func_80042808();
    }
}

INCLUDE_ASM("asm/nonmatchings/main/80061710", func_8006218C);

INCLUDE_ASM("asm/nonmatchings/main/80061710", func_80062210);

INCLUDE_ASM("asm/nonmatchings/main/80061710", func_800623E4);

void func_80062524(void) {
    func_800623E4();
    if (D_800E7208 & 0x820) {
        func_8006BD6C(0);
        func_8004284C();
        func_80044750(0x501);
    }
    if ((u32)D_800E7384 >= 0x259U || (D_800E7208 & 0x100)) {
        func_80048EB8(0);
        func_80044750(0x7F);
        func_8006BD6C(0);
        func_80042878(0x11);
    }
}

void func_800625C0(void) {
    u8 *p = D_8011ECD0 + D_801230D0 * 0x44;

    p[0x19C7] |= 0x40;
}

void func_800625F4(void) {
    func_800625C0();
    if ((u32)D_800E7384 >= 0x21U) {
        func_80042808();
    }
}

INCLUDE_ASM("asm/nonmatchings/main/80061710", func_80062634);

INCLUDE_ASM("asm/nonmatchings/main/80061710", func_800626B0);

void func_80062764(void) {
    func_80044750(0xD1);
    func_8004AC18(D_800E7384 * 2);
    if ((u32)D_800E7384 >= 0x80U) {
        func_80048DD0(0);
        func_8006BC28(0);
        func_8006BD6C(0);
        func_80041584();
        func_8004284C();
    }
}

INCLUDE_ASM("asm/nonmatchings/main/80061710", func_800627DC);

INCLUDE_ASM("asm/nonmatchings/main/80061710", func_80062840);

INCLUDE_ASM("asm/nonmatchings/main/80061710", func_80062948);

INCLUDE_ASM("asm/nonmatchings/main/80061710", func_80062C28);
