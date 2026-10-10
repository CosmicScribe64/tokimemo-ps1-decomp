#include "common.h"
#include "ovl/RPG_BAT.h"

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801321D0", func_801321D0);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801321D0", func_80132348);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801321D0", func_801328C4);

void func_80132A94(s32 arg0, s32 arg1) {
    s32 i;

    for (i = 0; i < 8; i++) {
        func_8013E97C(i + 0x28, arg0, arg1);
        func_8013E810(i + 0x28, 9, 1, 1);
        D_8011ECD0[0x2421 + i * 0x44] = 4;
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801321D0", func_80132B34);

void func_80132CE4(void) {
    if (D_8015EDB0.unk_04 == 0) {
        D_801215FD = 4;
        D_8015EDB0.unk_04 += 4;
        func_8013E97C(0x34, 0xF0 - D_8015EDB0.unk_04, 0x58);
        func_8013E97C(0x33, 0xF0 - D_8015EDB0.unk_04, 0x58);
        func_8013E810(0x34, 3, 1, 1);
        func_8013E810(0x33, 4, 1, 1);
    }
    func_8013E97C(0x34, 0xF0 - D_8015EDB0.unk_04, 0x58);
    func_8013E97C(0x33, 0xF0 - D_8015EDB0.unk_04, 0x58);
    D_80121426 = 0x82;
    D_8015EDB0.unk_04 += 6;
    if (D_8015EDB0.unk_04 >= 0x150) {
        D_801215FD = 5;
        func_8014EBA8();
    }
}

void func_80132DE8(void) {
    if (D_8015EDB0.unk_04 == 0) {
        D_801213DD = 4;
        D_80121421 = 4;
        func_8013E97C(0x34, D_8015EDB0.unk_04 - 0x10, 0x8A);
        func_8013E97C(0x33, D_8015EDB0.unk_04 - 0x10, 0x8A);
        func_8013E810(0x34, 0xB, 1, 1);
        func_8013E810(0x33, 0xC, 1, 1);
    }
    func_8013E97C(0x34, D_8015EDB0.unk_04 - 0x10, 0x8A);
    func_8013E97C(0x33, D_8015EDB0.unk_04 - 0x10, 0x8A);
    D_8015EDB0.unk_04 += 6;
    if (D_8015EDB0.unk_04 >= 0x186) {
        D_801213DD = 5;
        D_80121421 = 5;
        func_8014EBA8();
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801321D0", func_80132EE0);
