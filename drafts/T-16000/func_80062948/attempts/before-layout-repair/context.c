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

void func_800618B0(void) {
    TaiikuBig *p;

    /* FAKE: init and loop head on one source line give the original's addiu order (as1 schedules by line, T-7020) */
    p = D_801227A0; do {
        func_80099E30(0, p);
        p->unk20 = 0xFA0;
        p->unk0 = 0;
        p++;
    } while (p != D_80122CA0);
}

INCLUDE_ASM("asm/nonmatchings/main/80061710", func_8006190C);

INCLUDE_ASM("asm/nonmatchings/main/80061710", func_80061A3C);

void func_80061EFC(void) {
    u32 c;
    c = D_800E6280.unk_03A;
    if (c < 0x80U) {
        D_801220E1 = c;
        D_801220E2 = c;
        D_801220E0 = c;
        D_800E6280.unk_03A = c + 1;
    }
    if ((D_800E6280.unk_110D == 0) && ((u32)D_800E6280.unk_1104.w >= 0x259U)) {
        func_8004284C();
    }
    if (D_800E6280.unk_F88 & 0x860) {
        /* Chain written as element stores: one li for all three (T-9210). */
        (&D_800E6280.unk_036)[0] = (&D_800E6280.unk_037)[0] = (&D_800E6280.unk_038)[0] = 0xFF;
        back_clear_switch(1);
        func_80059048();
        D_800E6280.unk_03A = 0x80;
        func_80042808();
    }
}

void func_80061FC4(void) {
    func_8004AC18((s32)((u32)D_800E6280.unk_1104.w >> 1));
    if (((u32)D_800E6280.unk_1104.w >> 1 >= 0xFFU) || (D_800E6280.unk_F88 & 0x860)) {
        D_801220D0 = 0x80000000;
        func_80059048();
        func_8006BC28(0);
        func_8006BD6C(0);
        back_clear_switch(1);
        /* Chain written as element stores: one li for all three (T-9210). */
        (&D_800E6280.unk_036)[0] = (&D_800E6280.unk_037)[0] = (&D_800E6280.unk_038)[0] = 0xFF;
        func_80048F64(0x60);
        D_80120651 = 0xD;
        D_80120652 = 0x40;
        D_80120688 = 0x01000000;
        D_80120653 = 0xA4;
        D_8012065C = (u8 *)0x801EAB3C;
        D_80120660 = (u8 *)0x801EAB6C;
        D_80120684 = (u8 *)0x801EAB20;
        D_80120654 = 8;
        D_80120655 = 0x80;
        D_80120676 = -0xA0;
        D_8012067A = -0x78;
        if (D_800E6280.unk_F88 & 0x860) {
            func_80042808();
            return;
        }
        func_8004284C();
    }
}

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
        main_state_set(0x11);
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
void func_800626B0(void) {
    func_8004AC18(0xFF);
    switch (*(u32 *)&D_801230D0) {
    case 0:
        main_state_set(0x13);
        break;
    case 1:
        main_state_set(0x20);
        if (D_800B3220 == 1) {
            if (D_800E6280.unk_F80 & 0x2000) {
                main_state_set(0x21);
            } else if (D_800E6280.unk_F80 & 0x8000) {
                main_state_set(0x22);
            }
        }
        break;
    default:
        func_80046500();
        break;
    }
}

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
                main_state_set(0x11);
                func_80042908(1);
                func_80044750(0x501);
            }
        }
    }
}

/* bulk_m2c declarations (T-11190) */
extern s32 D_800E7378;
void func_80062948(void);
/* end bulk_m2c declarations */
void func_80062948(void) {
    s16 spC4;
    s16 spC2;
    s16 spC0;
    s32 spBC;
    s32 spB8;
    s32 spB4;
    s32 spA0[8];
    s32 sp80[8];
    s32 sp78;
    s32 sp74;
    s32 sp70;
    TaiikuBig *var_s6;
    s32 *var_s0;
    s32 temp_s1;
    s32 temp_s2;
    s32 temp_t0;
    s32 temp_v0;
    s32 temp_v1;
    s32 var_s4;
    u8 *var_s7;

    var_s7 = D_801227A4;
    var_s6 = D_801227A0;
    var_s0 = &D_801230D0;
    var_s4 = 0;
    do {
        temp_s2 = var_s4 & 3;
        temp_s1 = temp_s2 + 1;
        spC4 = (D_800E7378 * (temp_s2 + 2) * -2) - 2;
        spC0 = (rsin(D_800E7378 * 0xA * temp_s1) / 12) + 0x7FD;
        spC2 = (func_800A0140(D_800E7378 * 0xA * temp_s1) / 12) - 5;
        (*(s32 *)&spA0) = (s32) (*(s32 *)D_80125C60);
        *(s32 *)((unsigned char *)(&spA0) + (4)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (4));
        *(s32 *)((unsigned char *)(&spA0) + (8)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (8));
        *(s32 *)((unsigned char *)(&sp80) + (4)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (4));
        *(s32 *)((unsigned char *)(&spA0) + (16)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (16));
        *(s32 *)((unsigned char *)(&spA0) + (24)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (24));
        (*(s32 *)&sp80) = (s32) (*(s32 *)D_80125C60);
        *(s32 *)((unsigned char *)(&spA0) + (12)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (12));
        *(s32 *)((unsigned char *)(&sp80) + (12)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (12));
        *(s32 *)((unsigned char *)(&sp80) + (8)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (8));
        *(s32 *)((unsigned char *)(&spA0) + (20)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (20));
        *(s32 *)((unsigned char *)(&sp80) + (16)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (16));
        *(s32 *)((unsigned char *)(&sp80) + (20)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (20));
        *(s32 *)((unsigned char *)(&spA0) + (28)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (28));
        *(s32 *)((unsigned char *)(&sp80) + (28)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (28));
        *(s32 *)((unsigned char *)(&sp80) + (24)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (24));
        RotMatrix((s32) &spC0, (s32) &spA0);
        spB4 = 0;
        spB8 = 0;
        spBC = 0;
        sp70 = 0x1000;
        sp74 = 0x1000;
        sp78 = 0x1000;
        ScaleMatrix((s32) &sp80, (s32) &sp70);
        MulMatrix((s32) &spA0, (s32) &sp80);
        spB4 = *(s32 *)((unsigned char *)var_s0 + (4));
        temp_v1 = *(s32 *)((unsigned char *)var_s0 + (4));
        spB8 = (rsin((D_800E7378 & 0xFFF) * (temp_s2 + 0xA)) / 32) + ((s32) -temp_v1 / 2);
        spBC = (temp_v1 * -4) - 0x190;
        temp_v0 = abs(rsin(D_800E7378));
        (*(s32 *)var_s7) = (s32) (*(s32 *)&spA0);
        *(s32 *)((unsigned char *)var_s7 + (4)) = (s32) *(s32 *)((unsigned char *)(&spA0) + (4));
        *(s32 *)((unsigned char *)var_s7 + (8)) = (s32) *(s32 *)((unsigned char *)(&spA0) + (8));
        *(s32 *)((unsigned char *)var_s7 + (12)) = (s32) *(s32 *)((unsigned char *)(&spA0) + (12));
        *(s32 *)((unsigned char *)var_s7 + (16)) = (s32) *(s32 *)((unsigned char *)(&spA0) + (16));
        *(s32 *)((unsigned char *)var_s7 + (20)) = (s32) *(s32 *)((unsigned char *)(&spA0) + (20));
        *(s32 *)((unsigned char *)var_s7 + (24)) = (s32) *(s32 *)((unsigned char *)(&spA0) + (24));
        var_s4 += 1;
        temp_t0 = (temp_v0 & 1) + *(s32 *)((unsigned char *)var_s0 + (4)) + 2;
        *(s32 *)((unsigned char *)var_s7 + (28)) = (s32) *(s32 *)((unsigned char *)(&spA0) + (28));
        var_s0 += 4;
        var_s6 += 0x50;
        var_s7 += 0x50;
        *var_s0 = temp_t0;
        *(s32 *)((unsigned char *)var_s6 + (-80)) = 0;
    } while (var_s4 != 0xD);
}

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
    main_state_set(0);
}
