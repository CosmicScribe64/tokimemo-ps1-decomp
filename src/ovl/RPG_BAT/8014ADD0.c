#define RPG_BAT_OVERRIDE_func_8013E7C0 /* implicit int view: the call's $v0 moves the D_8015EE50 state of func_8014BB80 to $v1 (T-24301) */
#include "common.h"
#include "ovl/RPG_BAT.h"

s32 func_8013E7C0();

/* s32 without a return: keeps the dead `move $v0, $zero` of the `&&` truth value in the original (T-24124). */
s32 func_8014ADD0(s32 arg0, s32 arg1) {
    if (D_8015EDB4 == 0) {
        func_8013F0F4(0x501, 1, 0);
        func_8013E97C(0x2A, arg0, arg1);
        func_8013E97C(0x2B, arg0, arg1);
        func_8013E97C(0x2F, arg0, arg1);
        func_8013E810(0x2B, 0xE, 5, 1);
    }
    if (D_8015EDB4 == 1) {
        func_8013E810(0x2A, 9, 1, 1);
    }
    if ((D_8015ECEC > 0) && (D_8015ECEC < 0xF)) {
        if (((D_8015ECF8 == 0) || (D_8015ECF8 >= 9)) && ((D_8015ECF4 == 0) || (D_8015ECF4 >= 9)) && (D_8015ECF0 == 0) && (func_8013E9A8(5) == 0)) {
            D_8015ECF0 = 1;
            func_8013E97C(0x2C, (func_8013E9A8(0x20) + arg0) - 0x10, arg1 - 8);
            func_8013E810(0x2C, 0xA, 1, 1);
        }
        if (((D_8015ECF0 == 0) || (D_8015ECF0 >= 9)) && ((D_8015ECF8 == 0) || (D_8015ECF8 >= 9)) && (D_8015ECF4 == 0) && (func_8013E9A8(5) == 0)) {
            D_8015ECF4 = 1;
            func_8013E97C(0x2D, (func_8013E9A8(0x20) + arg0) - 0x10, arg1 - 8);
            func_8013E810(0x2D, 0xB, 1, 1);
        }
        if (((D_8015ECF4 == 0) || (D_8015ECF4 >= 9)) && ((D_8015ECF0 == 0) || (D_8015ECF0 >= 9)) && (D_8015ECF8 == 0) && (func_8013E9A8(5) == 0)) {
            D_8015ECF8 = 1;
            func_8013E97C(0x2E, (func_8013E9A8(0x20) + arg0) - 0x10, arg1 - 8);
            func_8013E810(0x2E, 0xC, 1, 1);
        }
    }
    if (D_8015ECF0 != 0) {
        ((DisplayActor44 *)D_8011ECD0)[140].unk_2A = (arg1 - (D_8015ECF0 * 4)) - 0x80;
        D_8015ECF0 += 1;
        if (D_8015ECF0 >= 0x21) {
            D_8015ECF0 = 0;
        }
    }
    if (D_8015ECF4 != 0) {
        ((DisplayActor44 *)D_8011ECD0)[141].unk_2A = (arg1 - (D_8015ECF4 * 4)) - 0x80;
        D_8015ECF4 += 1;
        if (D_8015ECF4 >= 0x21) {
            D_8015ECF4 = 0;
        }
    }
    if (D_8015ECF8 != 0) {
        ((DisplayActor44 *)D_8011ECD0)[142].unk_2A = (arg1 - (D_8015ECF8 * 4)) - 0x80;
        D_8015ECF8 += 1;
        if (D_8015ECF8 >= 0x21) {
            D_8015ECF8 = 0;
        }
    }
    if (!(((DisplayActor44 *)D_8011ECD0)[139].unk_02 & 1)) {
        func_8013E810(0x2B, 0xF, 5, 1);
        D_8015ECEC += 1;
        if (D_8015ECEC == 5) {
            func_8013F0F4(0x502, 1, 0);
            func_8013E810(0x2F, 0x10, 5, 1);
        }
        if (D_8015ECEC == 0x10) {
            func_8013E810(0x2B, 0x11, 5, 1);
            func_8013E810(0x2C, 0xFF, 1, 0);
            func_8013E810(0x2D, 0xFF, 1, 0);
            func_8013E810(0x2E, 0xFF, 1, 0);
        }
        if (D_8015ECEC == 0x11) {
            func_8013E810(0x2B, 0xFF, 1, 0);
            func_8014EBA8();
        }
    }
    if ((D_8015ECEC == 0x10) && (((DisplayActor44 *)D_8011ECD0)[139].unk_18 == 0x10)) {
        func_8013E810(0x2A, 0xFF, 1, 0);
    }
    if (!(((DisplayActor44 *)D_8011ECD0)[143].unk_02 & 1)) {
        func_8013E810(0x2F, 0xFF, 1, 0);
    }
    D_8015EDB4 += 1;
}

void func_8014B294(void) {
    s32 i;
    s32 div;

    div = 1;
    for (i = 0; i != 4; i++, div *= 10) {
        k_disp_switch(D_8015EDBC, 1);
        if (D_8015ED94 / div != 0) {
            func_8013E7C0(0x13 - i, ((D_8015ED94 / div) % 10) + 9, 1, 1);
        } else {
            func_8013E7C0(0x13 - i, 0xFF, 1, 0);
        }
        if (D_8015EDEC >= 0x10000001) {
            if (D_8015EB9C != 0xE) {
                k_disp_switch(D_8015EE14, 0);
            }
            ((DisplayActor44 *)D_8011ECD0)[129].unk_03 = 0;
            ((DisplayActor44 *)D_8011ECD0)[119 - i].unk_03 = 0;
        } else {
            k_disp_switch(D_8015EE14, 1);
            ((DisplayActor44 *)D_8011ECD0)[129].unk_03 = 0x80;
            if (D_8015EDEC / div != 0) {
                func_8013E7C0(0x17 - i, ((D_8015EDEC / div) % 10) + 9, 1, 1);
            } else {
                func_8013E7C0(0x17 - i, 0xFF, 1, 0);
            }
        }
        if (D_8015EE18 >= 0x10000001) {
            if (D_8015EB98 & 0xF0000) {
                k_disp_switch(D_8015EE40, 0);
            }
            if (D_8015EB9C != 0xE) {
                ((DisplayActor44 *)D_8011ECD0)[128].unk_03 = 0;
                ((DisplayActor44 *)D_8011ECD0)[123 - i].unk_03 = 0;
            } else {
                ((DisplayActor44 *)D_8011ECD0)[129].unk_03 = 0;
                ((DisplayActor44 *)D_8011ECD0)[119 - i].unk_03 = 0;
            }
        } else {
            k_disp_switch(D_8015EE40, 1);
            if (D_8015EB9C != 0xE) {
                ((DisplayActor44 *)D_8011ECD0)[128].unk_03 = 0x80;
                if (D_8015EE18 / div != 0) {
                    func_8013E7C0(0x1B - i, ((D_8015EE18 / div) % 10) + 9, 1, 1);
                } else {
                    func_8013E7C0(0x1B - i, 0xFF, 1, 0);
                }
            } else {
                ((DisplayActor44 *)D_8011ECD0)[129].unk_03 = 0x80;
                if (D_8015EE18 / div != 0) {
                    func_8013E7C0(0x17 - i, ((D_8015EE18 / div) % 10) + 9, 1, 1);
                } else {
                    func_8013E7C0(0x1B - i, 0xFF, 1, 0);
                }
            }
        }
    }
    if (D_8015ED94 == 0) {
        k_disp_switch(D_8015EDBC, 1);
        func_8013E7C0(0x13, 9, 1, 1);
    }
}

void func_8014B738(s32 arg0, s32 arg1, s32 arg2) {
    k_sub_reset();
    D_8015E740 = arg0;
    D_8015E744 = arg2;
    D_8015E748 = arg1;
    D_8015EE44.unk_00 = 0;
    D_8015EE44.unk_08 = 0;
    D_8015EE44.unk_04 = 1;
}

/* D_8015EE48 and D_8015EE4C are the scalar views of D_8015EE44.unk_04 and .unk_08: uopt only
 * promotes them into registers as scalars (as in the other RPG_BAT files); T-17110. */
void func_8014B79C(void) {
    s32 pad; /* FAKE: unused local, reserves the top spill word so idx sits at 0x30(sp); T-17110 */
    s32 idx;

    if (D_8015EE48 != 0) {
        switch (D_8015EE44.unk_00) {
        case 0:
            func_8013E7C0(0x24, 0x32, 5, 1);
            func_8014EB58(0);
            break;
        case 1:
            if (func_8013E90C(0x24) != 0) {
                func_8014EB58(0);
            }
            break;
        case 2:
            func_8013E7C0(0x24, 3, 1, 1);
            func_8014EB58(0);
            break;
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
            idx = D_8015EE44.unk_00 - 3;
            if (idx >= D_8015E748) {
                D_8015EE44.unk_00 = 0xFFFE;
            } else {
                D_8015EE4C += 1;
                if (D_8015EE4C == 1) {
                    k_disp_start(set_kanji_string(-0x7E, -0x66, 0, D_8015E740[idx], 0));
                }
                if (D_8015EE48 == 4 && func_800460EC() == 2) {
                    D_8015EE4C = D_8015E744 + 1;
                    D_8015EE48 = 1;
                }
                if (D_8015E744 < D_8015EE4C) {
                    switch (D_8015EE48) {
                    case 1:
                        k_sub_reset();
                        func_8014EB58(0);
                        break;
                    case 3:
                        /* empty on purpose: gives the compare tree of the original; T-17110 */
                        break;
                    case 4:
                        D_8015EE48 = 1;
                        break;
                    case 0x80:
                    case 0x81:
                    case 0x82:
                    case 0x83:
                    case 0x84:
                    case 0x85:
                    case 0x86:
                    case 0x87:
                    case 0x88:
                    case 0x89:
                    case 0x8A:
                        if (idx != (D_8015EE48 & 0x7F)) {
                            k_sub_reset();
                            func_8014EB58(0);
                        }
                        break;
                    }
                }
            }
            break;
        case 0xFFFE:
            func_8013E7C0(0x24, 0x32, 0x21, 1);
            ((DisplayActor44 *) D_8011ECD0)[132].unk_18 = 6;
            func_8014EB58(0);
            break;
        case 0xFFFF:
        default:
            if (func_8013E90C(0x24) != 0) {
                func_8013E7C0(0x24, 0xFF, 0, 0);
                D_8015EE4C = 0;
                D_8015EE44.unk_00 = 0;
                D_8015EE48 = 0;
            }
            break;
        }
    }
}

void func_8014BAA4(s32 arg0) {
    switch (arg0) {
    case 1:
        D_8015EE44.unk_04 = 0x80;
        return;
    case 2:
        D_8015EE44.unk_04 = 0x81;
        return;
    case 3:
        D_8015EE44.unk_04 = 0x82;
        return;
    case 4:
        D_8015EE44.unk_04 = 0x83;
        return;
    case 5:
        D_8015EE44.unk_04 = 0x84;
        return;
    case 6:
        D_8015EE44.unk_04 = 0x85;
        return;
    case 7:
        D_8015EE44.unk_04 = 0x86;
        return;
    case 8:
        D_8015EE44.unk_04 = 0x87;
        return;
    case 9:
        D_8015EE44.unk_04 = 0x88;
        return;
    case 10:
        D_8015EE44.unk_04 = 0x89;
        return;
    default:
        D_8015EE44.unk_04 = 1;
        return;
    }
}

/* NON_MATCHING: T-24301, 22 rows; with the implicit int view of func_8013E7C0 the state machine matches up to
 * the case 3 test: the original reloads unk_1093 into a temporary after the inner switch (`lui; lb t1`),
 * this volatile read materialises the struct address first (one instruction more). */
#ifdef NON_MATCHING
void func_8014BB80(void) {
    s32 pad; /* FAKE: T-24301, puts sp34 at sp+0x34 */
    RpgHalfTbl12 sp34;

    sp34 = D_8015E720;
    if (D_8015EE50.unk_00 != 3 && !(D_800E6280.unk_1092 & 1)) {
        func_8006BA40();
    }
    if (!(D_8015ED98 & 0x10000) && D_8015EE50.unk_00 != 4 && D_8015EE50.unk_00 != 5) {
        D_8015EE50.unk_00 = 6;
    }
    switch (D_8015EE50.unk_00) {
    case 0:
        func_8013E7C0(0x25, 0x31, 5, 1);
        if (D_8015EBA8 == 1) {
            func_8004F870(0, 3, sp34.v, &sp34.v[3], &sp34.v[6], &sp34.v[9]);
        } else {
            func_8004F870(0, 2, sp34.v, &sp34.v[3], &sp34.v[6], &sp34.v[9]);
        }
        func_8014EB58(1);
    default:
        break;
    case 1:
        if (func_8013E90C(0x25) != 0) {
            func_8014EB58(1);
        }
        break;
    case 2:
        func_8013E7C0(0x25, 2, 1, 1);
        if (D_8015EBA8 == 1) {
            func_8013E7C0(0x23, 0xFF, 1, 1);
        } else {
            func_8013E7C0(0x23, 0xFF, 1, 0);
        }
        func_8014EB58(1);
        break;
    case 3:
        menu_check(0, D_8011ECF6, D_8011ECFA);
        func_8004FC10(0);
        func_8013E7C0(0x22, 0xFF, 1, 1);
        switch (D_800E6280.unk_1093) {
        case 0:
            func_8013E7C0(0x22, 5, 1, 1);
            break;
        case 1:
            func_8013E7C0(0x22, 6, 1, 1);
            break;
        case 2:
            func_8013E7C0(0x22, 7, 1, 1);
            break;
        case -1:
            func_8013E7C0(0x22, 0xFF, 1, 0);
            break;
        }
        /* FAKE: T-24301, the volatile read keeps unk_1093 out of the promoted range of the two switches */
        if ((*(volatile s8 *)&D_800E6280.unk_1093 != -1) && (D_800E6280.unk_F88 & 0x20)) {
            func_8013E7C0(0x22, 0xFF, 1, 0);
            func_8013E7C0(0x23, 0xFF, 1, 0);
            switch (D_800E6280.unk_1093) {
            case 0:
                D_8015ED98 ^= 0x01000000;
                break;
            case 1:
                D_8015ED98 ^= 0x02000000;
                break;
            case 2:
                D_8015ED98 ^= 0x04000000;
                break;
            }
            func_8014EB58(1);
        }
        break;
    case 4:
        func_8013E7C0(0x25, 0x31, 0x21, 1);
        ((DisplayActor44 *)D_8011ECD0)[133].unk_18 = 6;
        func_8013E7C0(0x22, 0xFF, 1, 0);
        func_8013E7C0(0x23, 0xFF, 1, 0);
        func_8014EB58(1);
        break;
    case 5:
        if (func_8013E90C(0x25) != 0) {
            func_8013E7C0(0x25, 0xFF, 1, 0);
            func_8014EB58(1);
        }
        break;
    case 6:
        D_8015ED98 &= 0xFFFEFFFF;
        func_8013E7C0(0x22, 0xFF, 1, 0);
        func_8013E7C0(0x23, 0xFF, 1, 0);
        func_8013E7C0(0x25, 0xFF, 1, 0);
        D_8015EE50.unk_00 = 0;
        break;
    }
}
#else
INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8014ADD0", func_8014BB80);
#endif

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8014ADD0", func_8014BFE4);

void func_8014C360(s32 arg0, s32 arg1, s32 arg2) {
    s32 t;
    s32 u;
    s32 i;
    DisplayActor44 *rec;

    arg1 -= 0x9E;
    arg2 -= 0x78;
    switch (arg0) {
    case 0:
        D_8015EE5C = 0;
        D_8015EE60 = 1;
        D_8015EE64 = 0;
        break;
    case 1:
        D_8015EE68 = 0;
        D_8015EE6C = 1;
        D_8015EE70 = 0;
        break;
    case 2:
        D_8015EE74 = 0;
        D_8015EE78 = 1;
        D_8015EE7C = 0;
        break;
    case 3:
        D_8015EE80 = 0;
        D_8015EE84 = 1;
        D_8015EE88 = 0;
        break;
    }
    rec = (DisplayActor44 *) D_8011ECD0;
    t = arg0 * 12;
    u = t + 0x40; /* named local: t and u get the two spill slots of the original; T-17110 */
    rec[64 + t].unk_26 = arg1;
    rec[64 + t].unk_2A = arg2;
    rec[68 + t].unk_26 = arg1;
    rec[68 + t].unk_2A = arg2;
    rec[72 + t].unk_26 = arg1;
    rec[72 + t].unk_2A = arg2;
    func_8013E864(u, 0x34, 0);
    func_8013E864(u + 4, 0x35, 0);
    func_8013E864(u + 8, 0x35, 0);
    for (i = 1; i < 4; i++) {
        rec[64 + t + i].unk_26 = 0xA0;
        rec[64 + t + i].unk_2A = 0x78;
        rec[68 + t + i].unk_26 = 0xA0;
        rec[68 + t + i].unk_2A = 0x78;
        rec[72 + t + i].unk_26 = 0xA0;
        rec[72 + t + i].unk_2A = 0x78;
    }
}

void func_8014C4FC(void) {
    s32 half;
    s32 i;
    s16 *p;

    if (D_8015EE60 == 1) {
        half = D_8015EE64 / 2;
        switch (half) {
        case 0:
            func_8013E8B8(0, 0xFF, 1);
            func_8013E8B8(4, 0xFF, 1);
            func_8013E8B8(8, 0xFF, 1);
            break;
        case 1:
            func_8013E8B8(1, 0xFF, 1);
            func_8013E8B8(5, 0xFF, 1);
            func_8013E8B8(9, 0xFF, 1);
            break;
        case 2:
            func_8013E8B8(2, 0xFF, 1);
            func_8013E8B8(6, 0xFF, 1);
            func_8013E8B8(0xA, 0xFF, 1);
            break;
        case 3:
            func_8013E8B8(3, 0xFF, 1);
            func_8013E8B8(7, 0xFF, 1);
            func_8013E8B8(0xB, 0xFF, 1);
            break;
        }
        if (half >= 0x10) {
            half = 0xF;
        }
        if (!(D_8015EE64 & 1)) {
            p = D_8015E5A0[half];
            D_8011FDFA += p[0];
            D_8011FF0A += p[1];
            D_8012001A += p[2];
            D_8011FE3E += p[3];
            D_8011FF4E += p[4];
            D_8012005E += p[5];
            D_8011FE82 += p[6];
            D_8011FF92 += p[7];
            D_801200A2 += p[8];
            D_8011FEC6 += p[9];
            D_8011FFD6 += p[10];
            D_801200E6 += p[11];
        }
        D_8015EE64 += 1;
        if (D_8015EE64 / 2 >= 0x1A) {
            for (i = 0; i != 0xC; i++) {
                func_8013E8B8(i, 0xFF, 0);
            }
            D_8015EE60 = 0;
        }
    }
}

void func_8014C804(void) {
    s32 half;
    s32 i;
    s16 *p;

    if (D_8015EE6C == 1) {
        half = D_8015EE70 / 2;
        switch (half) {
        case 0:
            func_8013E8B8(0xC, 0xFF, 1);
            func_8013E8B8(0x10, 0xFF, 1);
            func_8013E8B8(0x14, 0xFF, 1);
            break;
        case 1:
            func_8013E8B8(0xD, 0xFF, 1);
            func_8013E8B8(0x11, 0xFF, 1);
            func_8013E8B8(0x15, 0xFF, 1);
            break;
        case 2:
            func_8013E8B8(0xE, 0xFF, 1);
            func_8013E8B8(0x12, 0xFF, 1);
            func_8013E8B8(0x16, 0xFF, 1);
            break;
        case 3:
            func_8013E8B8(0xF, 0xFF, 1);
            func_8013E8B8(0x13, 0xFF, 1);
            func_8013E8B8(0x17, 0xFF, 1);
            break;
        }
        if (half >= 0x10) {
            half = 0xF;
        }
        if (!(D_8015EE70 & 1)) {
            p = D_8015E5A0[half];
            D_8012012A += p[0];
            D_8012023A += p[1];
            D_8012034A += p[2];
            D_8012016E += p[3];
            D_8012027E += p[4];
            D_8012038E += p[5];
            D_801201B2 += p[6];
            D_801202C2 += p[7];
            D_801203D2 += p[8];
            D_801201F6 += p[9];
            D_80120306 += p[10];
            D_80120416 += p[11];
        }
        D_8015EE70 += 1;
        if (D_8015EE70 / 2 >= 0x1A) {
            for (i = 0; i != 0xC; i++) {
                func_8013E8B8(i + 0xC, 0xFF, 0);
            }
            D_8015EE6C = 0;
        }
    }
}

void func_8014CB0C(void) {
    s32 half;
    s32 i;
    s16 *p;

    if (D_8015EE78 == 1) {
        half = D_8015EE7C / 2;
        switch (half) {
        case 0:
            func_8013E8B8(0x18, 0xFF, 1);
            func_8013E8B8(0x1C, 0xFF, 1);
            func_8013E8B8(0x20, 0xFF, 1);
            break;
        case 1:
            func_8013E8B8(0x19, 0xFF, 1);
            func_8013E8B8(0x1D, 0xFF, 1);
            func_8013E8B8(0x21, 0xFF, 1);
            break;
        case 2:
            func_8013E8B8(0x1A, 0xFF, 1);
            func_8013E8B8(0x1E, 0xFF, 1);
            func_8013E8B8(0x22, 0xFF, 1);
            break;
        case 3:
            func_8013E8B8(0x1B, 0xFF, 1);
            func_8013E8B8(0x1F, 0xFF, 1);
            func_8013E8B8(0x23, 0xFF, 1);
            break;
        }
        if (half >= 0x10) {
            half = 0xF;
        }
        if (!(D_8015EE7C & 1)) {
            p = D_8015E5A0[half];
            D_8012045A += p[0];
            D_8012056A += p[1];
            D_8012067A += p[2];
            D_8012049E += p[3];
            D_801205AE += p[4];
            D_801206BE += p[5];
            D_801204E2 += p[6];
            D_801205F2 += p[7];
            D_80120702 += p[8];
            D_80120526 += p[9];
            D_80120636 += p[10];
            D_80120746 += p[11];
        }
        D_8015EE7C += 1;
        if (D_8015EE7C / 2 >= 0x1A) {
            for (i = 0; i != 0xC; i++) {
                func_8013E8B8(i + 0x18, 0xFF, 0);
            }
            D_8015EE78 = 0;
        }
    }
}

void func_8014CE14(void) {
    s32 half;
    s32 i;
    s16 *p;

    if (D_8015EE84 == 1) {
        half = D_8015EE88 / 2;
        switch (half) {
        case 0:
            func_8013E8B8(0x24, 0xFF, 1);
            func_8013E8B8(0x28, 0xFF, 1);
            func_8013E8B8(0x2C, 0xFF, 1);
            break;
        case 1:
            func_8013E8B8(0x25, 0xFF, 1);
            func_8013E8B8(0x29, 0xFF, 1);
            func_8013E8B8(0x2D, 0xFF, 1);
            break;
        case 2:
            func_8013E8B8(0x26, 0xFF, 1);
            func_8013E8B8(0x2A, 0xFF, 1);
            func_8013E8B8(0x2E, 0xFF, 1);
            break;
        case 3:
            func_8013E8B8(0x27, 0xFF, 1);
            func_8013E8B8(0x2B, 0xFF, 1);
            func_8013E8B8(0x2F, 0xFF, 1);
            break;
        }
        if (half >= 0x10) {
            half = 0xF;
        }
        if (!(D_8015EE88 & 1)) {
            p = D_8015E5A0[half];
            D_8012078A += p[0];
            D_8012089A += p[1];
            D_801209AA += p[2];
            D_801207CE += p[3];
            D_801208DE += p[4];
            D_801209EE += p[5];
            D_80120812 += p[6];
            D_80120922 += p[7];
            D_80120A32 += p[8];
            D_80120856 += p[9];
            D_80120966 += p[10];
            D_80120A76 += p[11];
        }
        D_8015EE88 += 1;
        if (D_8015EE88 / 2 >= 0x1A) {
            for (i = 0; i != 0xC; i++) {
                func_8013E8B8(i + 0x24, 0xFF, 0);
            }
            D_8015EE84 = 0;
        }
    }
}

void func_8014D11C(u8 *dst, u8 *name, u8 *label, s32 num, u8 *suffix) {
    u32 quot;
    s32 place;
    s32 base;

    func_800AE0F0(dst, name);
    func_800AE100(dst, label);
    if (num == 0) {
        func_800AE100(dst, "０");
    }
    /* radix in one variable: with literal 10 the unsigned % and the signed / keep two copies in
     * two $s registers; T-17110 */
    base = 10;
    for (place = 0x3E8; place > 0; place = place / base) {
        quot = num / place;
        if (quot != 0) {
            switch (quot % base) {
            case 0:
                func_800AE100(dst, "０");
                break;
            case 1:
                func_800AE100(dst, "１");
                break;
            case 2:
                func_800AE100(dst, "２");
                break;
            case 3:
                func_800AE100(dst, "３");
                break;
            case 4:
                func_800AE100(dst, "４");
                break;
            case 5:
                func_800AE100(dst, "５");
                break;
            case 6:
                func_800AE100(dst, "６");
                break;
            case 7:
                func_800AE100(dst, "７");
                break;
            case 8:
                func_800AE100(dst, "８");
                break;
            case 9:
                func_800AE100(dst, "９");
                break;
            }
        }
    }
    func_800AE100(dst, suffix);
}
