#include "common.h"
#include "ovl/RPG_BAT.h"

void func_801321D0(void) {
    if ((s32) D_8015EDAC >= 3 && (s32) D_8015EDAC < 6) {
        if (D_80121438 == 0 && D_80121428 == 0) {
            func_8013F0F4(0x503, 1, 0);
        }
    }
    switch (D_8015EDAC) {
    case 0:
        func_800AE0F0(D_8015E74C, "　　　　　ナイアガラダンク　　　　　");
        func_8014B738((s32) &D_8015E814, 1, 0x5A);
        func_80144480();
        func_8013EEAC(D_8015EBAC);
        func_80148F70();
        func_8014EBA8();
        func_8014EBA8();
        return;
    case 2:
        func_8013D418();
        return;
    case 3:
        func_80132CE4();
        return;
    case 4:
        func_80132DE8();
        return;
    case 5:
        func_80132EE0();
        return;
    case 6:
        func_80132348();
        return;
    case 7:
        func_8013E810(0x34, 2, 1, 1);
        func_8014ED44(0x14);
        return;
    case 8:
        func_8013D59C();
        return;
    case 9:
        func_8014EBEC();
        break;
    }
}


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

void func_80132EE0(void) {
    s32 temp_v0_2;


    if (D_8015EDB0.unk_04 == 0) {
        func_8013E97C(0x34, 0x170 - D_8015EDB0.unk_04, 0x58);
        func_8013E97C(0x33, 0x170 - D_8015EDB0.unk_04, 0x58);
        func_8013E810(0x34, 3, 1, 1);
        func_8013E810(0x33, 4, 1, 1);
    }
    func_8013E97C(0x34, 0x170 - D_8015EDB0.unk_04, 0x58);
    func_8013E97C(0x33, 0x170 - D_8015EDB0.unk_04, 0x58);
    temp_v0_2 = D_8015EDB0.unk_04 + 6;
    D_8015EDB0.unk_04 = temp_v0_2;
    if (temp_v0_2 >= 0x61) {
        D_80121426 = 1;
        func_8013E810(0x33, 4, 1, 0);
        func_8014EBA8();
    }
}


