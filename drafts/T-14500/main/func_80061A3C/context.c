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

/* bulk_m2c declarations (T-11190) */
extern s16 D_800B5A40;
extern s32 D_800B5A42;
extern s32 D_800B5A5A;
extern s32 D_800E7318;
extern s32 D_800E7374;
extern s32 D_80184000;
extern s8 D_800B3D00;
extern s8 D_800E62B6;
extern s8 D_800E62B7;
extern s8 D_800E62B8;
extern s8 D_800E62BA;
extern s8 D_800E736C;
extern u8 D_800E738D;
/* end bulk_m2c declarations */
void func_80061A3C(void) {
    s32 sp60;
    s32 *var_s1;
    s32 *var_v0_2;
    s16 temp_t6;
    s16 temp_t7;
    s16 temp_t8;
    s16 temp_t9;
    s32 *var_v1;
    s32 *var_v1_2;
    s32 var_a0;
    s32 var_a1;
    s32 var_s0;
    s32 var_s0_2;
    s32 var_s0_3;
    s32 var_s0_4;
    s32 var_s0_5;
    s32 var_s1_2;
    s32 var_s1_3;
    s32 var_v0;

    switch (D_800E738D) {
    case 0:
        D_800B3CFC = 0;
        D_800B3D00 = 0;
        D_800B3CF8 = 0;
        tpage_buf_clear();
        func_80048EB8(0);
        dec_bg_reset();
        func_80041168(0xFE);
        func_8004E58C();
        func_80065F34(0);
        back_clear_switch(1);
        func_80048DAC(1);
        D_800E7318 = 0x9F;
        func_8006BD6C(0);
        D_800E62B7 = 0;
        D_800E62B6 = 0;
        D_800E62BA = 0;
        D_800E62B8 = 0;
        func_80044750(0x7F);
        func_80044890(1, 0xC592, 0xC580, 0xCFA7, 0xCF8B, 0xCF83);
        D_800E738D += 1;
        return;
    case 1:
        if (func_80044E8C() == 1) {
            func_8004500C(1, 0);
            cd_read_sectors(1U, 0x801F0000, 0xBE13);
            D_800E738D += 1;
            return;
        }
        return;
    case 2:
        if (cd_read_sync_status() & 1) {
            var_s0 = 0x801F0800;
            var_a0 = 0x801F0800;
            do {
                memcpy(var_a0, 0x801F0000, 0x800);
                var_s0 += 0x800;
                var_a0 = var_s0;
            } while (var_s0 != 0x801F6800);
            D_800E738D += 1;
            cd_read_sectors(0xD6U, 0x80180000, 0xA0CC);
            return;
        }
        break;
    case 3:
        var_s0_2 = 1;
        if (cd_read_sync_status() & 1) {
            var_a1 = 0x10;
            do {
                load_palette(0x801CC200, var_a1 & 0xFF, 1, 2, /* extra? */ var_s0_2);
                var_s0_2 += 1;
                var_a1 = var_s0_2 + 0xF;
            } while (var_s0_2 < 0xE);
            load_palette(0x801CC200, 0x1F, 1, 2, /* extra? */ 0xF);
            load_palette(0x801CC200, 0x1E, 1, 2, /* extra? */ 0xE);
            var_s0_3 = 0;
            var_s1 = &D_80184000;
            do {
                load_tpage_buf_lock(var_s0_3 + 0x15, (s32) var_s1, 1);
                var_s0_3 += 1;
            } while (var_s0_3 < 6);
            var_s1_2 = 0x80198000;
            var_s0_4 = 0;
            do {
                load_tpage_buf_lock(var_s0_4 + 0x35, var_s1_2, 1);
                var_s0_4 += 1;
                var_s1_2 += 0x4000;
            } while (var_s0_4 < 4);
            var_s1_3 = 0x801A8000;
            var_s0_5 = 0;
            do {
                load_tpage_buf_lock(var_s0_5 + 0x25, var_s1_3, 1);
                var_s0_5 += 1;
                var_s1_3 += 0x4000;
            } while (var_s0_5 < 3);
            var_v0 = 0x801B4000;
            var_v1 = &sp60;
            do {
                *var_v1 = var_v0;
                var_v0 += 0x4000;
                var_v1 += 4;
            } while (var_v0 < 0x801D4000);
            func_8004A1A0((s32) &sp60, 0, 6, 3, 2, 0x1EF, -0xA0, -0x78, 8, 0xD);
            load_palette(0x801CC000, 0xF, 1, 3, /* extra? */ 0);
            func_80048F64(0x60);
            D_80120651 = 0xD;
            D_80120652 = 1;
            D_80120688 = 0x41000000;
            D_80120653 = 0xA4;
            D_8012065C = (u8 *)0x801EAB3C;
            D_80120660 = (u8 *)0x801EAB6C;
            D_80120684 = (u8 *)0x801EAB20;
            D_80120666 = 0;
            D_80120654 = 8;
            D_80120655 = 0x80;
            D_80120676 = -0xA0;
            D_8012067A = -0x78;
            D_801220E1 = 0;
            D_801220E0 = 0;
            D_800E736C = 0x7F;
            D_801220E2 = 0;
            back_clear_switch(0);
            srand(D_800E7374);
            draw2d3d(1U, 1U);
            func_80061710();
            func_80061790();
            func_800618B0();
            func_8006190C();
            var_v0_2 = &D_800B5A42;
            var_v1_2 = &D_801230D4;
            D_801230D4 = (s32) D_800B5A40;
            do {
                temp_t7 = *(s16 *)((unsigned char *)var_v0_2 + (2));
                temp_t8 = *(s16 *)((unsigned char *)var_v0_2 + (4));
                temp_t9 = *(s16 *)((unsigned char *)var_v0_2 + (6));
                temp_t6 = (*(s32 *)var_v0_2);
                var_v0_2 += 8;
                var_v1_2 += 0x10;
                *(s32 *)((unsigned char *)var_v1_2 + (-8)) = (s32) temp_t7;
                *(s32 *)((unsigned char *)var_v1_2 + (-4)) = (s32) temp_t8;
                (*(s32 *)var_v1_2) = (s32) temp_t9;
                *(s32 *)((unsigned char *)var_v1_2 + (-12)) = (s32) temp_t6;
            } while (var_v0_2 != &D_800B5A5A);
            func_80042808();
        }
        break;
    }
}

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
    main_state_set(0);
}
