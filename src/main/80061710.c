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

void func_80061790(void) {
    D_80122760 = 0;
    D_80122764 = 0;
    D_80122768 = 0x64;
    D_8012276C = 0xFF;
    D_8012276D = 0x80;
    D_8012276E = 0x80;
    func_8009AD70(0, &D_80122760);
    D_80122770 = 0;
    D_80122774 = 0x64;
    D_80122778 = 0;
    D_8012277C = 0x80;
    D_8012277D = 0x80;
    D_8012277E = 0xFF;
    func_8009AD70(1, &D_80122770);
    D_80122780 = 0x64;
    D_80122784 = 0;
    D_80122788 = 0;
    D_8012278C = 0x80;
    D_8012278D = 0xFF;
    D_8012278E = 0x80;
    func_8009AD70(2, &D_80122780);
    GsSetAmbient(0, 0, 0);
    func_8009B340(0);
}

INCLUDE_ASM("asm/nonmatchings/main/80061710", func_800618B0);

INCLUDE_ASM("asm/nonmatchings/main/80061710", func_8006190C);

INCLUDE_ASM("asm/nonmatchings/main/80061710", func_80061A3C);

INCLUDE_ASM("asm/nonmatchings/main/80061710", func_80061EFC);

INCLUDE_ASM("asm/nonmatchings/main/80061710", func_80061FC4);

void func_8006211C(void) {
    func_8004AC18(0xFF - D_800E6280.unk_1104.w);
    if ((u32)D_800E6280.unk_1104.w >= 0xFFU) {
        func_80042808();
    } else if (D_800E6280.unk_F88 & 0x860) {
        func_80042808();
    }
}

void func_8006218C(void) {
    D_800E6280.unk_1104.w += 1;
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80061EFC();
        break;
    case 1:
        func_80061FC4();
        break;
    case 2:
        func_8006211C();
        break;
    }
    func_80062948();
}

INCLUDE_ASM("asm/nonmatchings/main/80061710", func_80062210);

void func_800623E4(void) {
    if (D_800E6280.unk_F74 != 0) {
        D_8011ECFA = D_801230D0 * 16 + 0x46;
        D_8011ECF6 = 0x40;
        switch (D_800E6280.unk_F88 & 0x5000) {
        case 0x1000:
            D_800E6280.unk_1104.w = 0;
            D_801230D0 = 0;
            break;
        case 0x4000:
            D_800E6280.unk_1104.w = 0;
            D_801230D0 = 1;
            break;
        }
    } else {
        menu_check(0, D_8011ECF6, D_8011ECFA);
        func_8004FC10(0);
        switch (D_800E6280.unk_1093) {
        case 0:
            D_800E6280.unk_1104.w = 0;
            D_801230D0 = 0;
            break;
        case 1:
            D_800E6280.unk_1104.w = 0;
            D_801230D0 = 1;
            break;
        }
    }
    if (D_801230D0 != 0) {
        D_801206AC = 2;
        D_801206F0 = 3;
    } else {
        D_801206AC = 1;
        D_801206F0 = 4;
    }
}

void func_80062524(void) {
    func_800623E4();
    if (D_800E6280.unk_F88 & 0x820) {
        func_8006BD6C(0);
        func_8004284C();
        func_80044750(0x501);
    }
    if ((u32)D_800E6280.unk_1104.w >= 0x259U || (D_800E6280.unk_F88 & 0x100)) {
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
    if ((u32)D_800E6280.unk_1104.w >= 0x21U) {
        func_80042808();
    }
}

void func_80062634(void) {
    D_800E6280.unk_1104.w += 1;
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80062210();
        return;
    case 1:
        func_80062524();
        return;
    case 2:
        func_800625F4();
        return;
    }
}
INCLUDE_ASM("asm/nonmatchings/main/80061710", func_800626B0);

void func_80062764(void) {
    func_80044750(0xD1);
    func_8004AC18(D_800E6280.unk_1104.w * 2);
    if ((u32)D_800E6280.unk_1104.w >= 0x80U) {
        func_80048DD0(0);
        func_8006BC28(0);
        func_8006BD6C(0);
        func_80041584();
        func_8004284C();
    }
}

void func_800627DC(void) {
    D_800E6280.unk_1104.w += 1;
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80062764();
        return;
    case 1:
        func_800626B0();
        return;
    }
}

void func_80062840(void) {
    D_800E6280.unk_1100 += 1;
    switch (D_800E6280.unk_1109) {
    case 0:
        func_80061A3C();
        break;
    case 1:
        func_8006218C();
        break;
    case 2:
        func_80062634();
        break;
    case 3:
        func_800627DC();
        break;
    default:
        func_80046500();
        break;
    }
    if (D_800B3CFC != 0) {
        if (func_80045288() == 1) {
            if (D_800E6280.unk_1109 >= 2) {
                func_8006BD6C(0);
                func_80044750(0x7F);
                func_80042878(0x11);
                func_80042908(1);
                func_80044750(0x501);
            }
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/main/80061710", func_80062948);

void func_80062C28(void) {
    set_movie_offset(0, 0);
    func_80056BA8(0, 0xA7, 0x190);
    func_80088070();
    func_80045414(9, 0, 0);
    func_80045414(0xA, 0, 0);
    func_80053650(0x80167000, 0x80162000, 0x80162000);
    func_8009C210(1);
    func_80041168(0);
    func_8009C674(0);
    back_clear_switch(1);
    func_80042878(0);
}
