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


void func_80132348(void) {
    RpgMotion20 *p = (RpgMotion20 *)&D_8015EC50;

    switch (D_8015EDB0.unk_00) {
    case 0:
        p->dx = -0x30;
        p->dy = -0x140;
        func_8013E810(0x34, 5, 5, 1);
        func_8013F0F4(0x505, 1, 0);
        func_8014EBD0();
    default:
        break;
    case 1:
        if (D_80121438 == 2) {
            func_8014EBD0();
        }
        break;
    case 2:
        func_8013E97C(0x34, ((s32) (p->dx * D_8015EDB0.unk_08) / 1000) + 0x110, ((s32) (p->dy * D_8015EDB0.unk_08) / 1000) + 0x58);
        D_8015EDB0.unk_08 += 0x14;
        if (D_8015EDB0.unk_08 >= 0x3E9) {
            func_8013E810(0x34, 6, 1, 1);
            func_80132B34();
            p->dx = p->rect.unk_08 - p->rect.unk_00;
            p->dy = p->rect.unk_0C - p->rect.unk_04;
            func_8014EBD0();
        }
        break;
    case 3:
        func_8013E97C(0x34, ((s32) (p->dx * D_8015EDB0.unk_08) / 1000) + p->rect.unk_00, ((s32) (p->dy * D_8015EDB0.unk_08) / 1000) + p->rect.unk_04);
        D_8015EDB0.unk_08 += 0x1E;
        if ((p->rect.unk_14 - 0x10) < (p->rect.unk_04 + ((s32) (p->dy * D_8015EDB0.unk_08) / 1000))) {
            func_8013F0F4(0x504, 0, 1);
            func_80132A94(p->rect.unk_10, p->rect.unk_14);
            func_801328C4(p->rect.unk_10, p->rect.unk_14);
            func_8013E810(0x34, 0xD, 1, 1);
        }
        if (D_8015EDB0.unk_08 >= 0x3E9) {
            if (D_8015EBAC == 1) {
                D_801215FD = 4;
                D_80121685 = 4;
            }
            func_8013E810(0x34, 7, 5, 1);
            D_80121438 = 1;
            func_8014EBD0();
        }
        break;
    case 4:
        func_801328C4(p->rect.unk_10, p->rect.unk_14);
        func_8014ED80(0x1E);
        break;
    case 5:
        func_8013D3E4();
        func_8014EBD0();
        break;
    case 6:
        func_801328C4(p->rect.unk_10, p->rect.unk_14);
        func_8013E97C(0x34, ((s32) ((0xF0 - p->rect.unk_08) * D_8015EDB0.unk_08) / 1000) + p->rect.unk_08, (p->rect.unk_0C + ((s32) ((0x58 - p->rect.unk_0C) * D_8015EDB0.unk_08) / 1000)) - (func_800A0070((s32) (D_8015EDB0.unk_08 << 0xB) / 1000) >> 7));
        D_8015EDB0.unk_08 += 0x14;
        func_8013E810(0x34, 8, 1, 1);
        if (D_8015EDB0.unk_08 >= 0x3E9) {
            func_8013F0F4(0x502, 1, 0);
            func_8014EBA8();
        }
        break;
    }
}


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


