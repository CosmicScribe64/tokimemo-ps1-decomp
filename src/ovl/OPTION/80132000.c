#define MAIN_API_OVERRIDE_func_80053D10 /* implicit int view: the call's $v0 keeps unk_1115 in $v1 in func_80132BA8 (T-24301) */
#define MAIN_API_OVERRIDE_func_80049A40 /* unprototyped view avoids 16-bit coordinate narrowing in func_80132000 (T-24134) */
#include "common.h"
#include "ovl/OPTION.h"
/* FAKE: scoped minimum aggregate view of the word and adjacent flags, based on observed +0x14 indexing. T-9540 */

void func_80049A40();
s32 func_80053D10();

void func_80132000(void) {
    if ((D_800E6280.unk_110A != 0x21) && (D_800E6280.unk_110A != 0x22) && (D_800E6280.unk_110A != 0x23)) {
        if (D_800E6280.unk_03A != 0) {
            /* FAKE: dead tests extend unk_110A in $v0 and promote unk_03A ($v1), D_80120676 ($v0), D_8012067A ($v1). T-24134 */
            if ((s32)D_800E6280.unk_110A) {}
            if ((s32)D_800E6280.unk_03A) {}
            if ((u8) D_801220E0 >= 0x7FU) {
                if (D_80120676) {}
                if (D_8012067A) {}
                func_80049A40(D_80120676 + 0x18, D_8012067A + 0x10, 0x7C, 0xBC, 0xA, 0x10907, 0x83);
                func_800494BC(0, 0, 0xA, 0, 0);
            }
        }
    }
}

void func_801320B0(void) {
}

void func_801320B8(void) {
}

void func_801320C0(void) {
    if ((D_800E6280.unk_110A != 0x21) && (D_800E6280.unk_110A != 0x22) && (D_800E6280.unk_110A != 0x23)) {
        if (D_800E6280.unk_03A != 0) {
            switch (D_800E6280.unk_1109) {
            case 0:
                break;
            case 1:
                func_80049A40(8, -0x68, 0x40, 0x90, 8, 0x10907, 0x80);
                break;
            case 2:
            case 4:
                func_80049A40(-0x28, -0x48, 0xB0, 0x80, 8, 0x10907, 0x80);
                break;
            case 3:
                func_80049A40(-0x80, -0x30, 0x100, 0x80, 8, 0x10907, 0x80);
                break;
            case 5:
                func_80049A40(8, -0x20, 0x40, 0x80, 8, 0x10907, 0x80);
                break;
            case 6:
                func_80049A40(-0x10, 0x10, 0x90, 0x50, 8, 0x10907, 0x80);
                break;
            case 7:
                if ((u8) D_800E6280.unk_110A < 0xAU) {
                    func_80049A40(-0x50, 8, 0x80, 0x58, 8, 0x10907, 0x80);
                } else if ((u8) D_800E6280.unk_110A < 0x14U) {
                    func_80049A40(-0x58, -8, 0xDC, 0x40, 8, 0x10907, 0x80);
                } else {
                    func_80049A40(-0x90, -0x58, 0x130, 0x90, 8, 0x10907, 0x80);
                }
                break;
            case 8:
                func_80049A40(-0x50, 8, 0x80, 0x58, 8, 0x10907, 0x80);
                break;
            default:
                func_80049A40(8, -0x68, 0x40, 0x90, 8, 0x10907, 0x80);
                break;
            }
            func_800494BC(0, 0, 8, 0, 0);
        }
    }
}

void func_80132384(void) {
    s32 i;
    s32 n;
    s32 y;
    s16 *p;
    Arrs4x8 sp50;
    s32 pad[2]; /* FAKE: places sp50 at sp+0x50 (0xA0 frame); real source unknown. T-24140 */

    if (D_80120676 < -0xA0) {
        D_80120676 += 0xC;
    } else {
        D_80120676 = -0xA0;
    }
    func_801325A0(&sp50);
    i = 0;
    do {
        n = i + 1;
        if ((n ^ 0) != D_800E6280.unk_1109) { /* FAKE: '^ 0' swaps the beq operand order to the original's; real source unknown. T-24140 */
            p = func_8004E970(i);
            if (p[0] < -0x78) {
                p[0] += 0xC;
            } else {
                p[0] = -0x78;
            }
        } else {
            /* FAKE: 20-byte array stride (Uixa 20) and i++ prevent uopt from hoisting 20 into $s5 or strength-reducing i * 20; real source unknown. T-24140 */
            y = (s32)(*(u8 (*)[8][20])0)[i] - 0x58;
            p = func_8004E970(i++);
            if (p[1] < y) {
                p[1] += 0xC;
                D_801206BE += 0xC;
                if (p[1] >= y) {
                    p[1] = y;
                    D_801206BE = sp50.b[D_800E6280.unk_1093] + 6;
                }
            } else {
                p[1] = y;
                D_801206BE = sp50.b[D_800E6280.unk_1093] + 6;
            }
        }
        i = n;
    } while (n != 8);
}

void func_801324E8(void) {
    s32 i;
    s32 n;
    s16 *p;

    if (D_80120676 >= -0x11F) {
        D_80120676 -= 8;
    }
    i = 0;
    do {
        n = i + 1;
        if ((n ^ 0) != D_800E6280.unk_1109) { /* FAKE: '^ 0' swaps the beq operand order to the original's; real source unknown. T-4070 */
            p = func_8004E970(i);
            if (p[0] >= -0xF7) {
                p[0] -= 8;
            }
        } else {
            p = func_8004E970(i);
            if (p[1] >= -0x57) {
                p[1] -= 8;
                D_801206BE -= 8;
            }
        }
        i = n;
    } while (n != 8);
}

void func_801325A0(Arrs4x8 *p) {
    s32 i;

    for (i = 0; i < 8; i++) {
        p->a[i] = -0x78;
        p->b[i] = -0x58 + i * 0x14;
        p->c[i] = 0x60;
        p->d[i] = 0x14;
    }
}

void func_80132624(OptionGrid80 *grid) {
    s32 i;
    for (i = 0; i < 16; i++) {
        if (i < 8) {
            grid->x[i] = -0x20;
            grid->y[i] = i * 20 - 0x46;
            grid->w[i] = 0x50;
            grid->h[i] = 0x14;
        } else {
            grid->x[i] = 0x30;
            grid->y[i] = i * 20 - 0xE6;
            grid->w[i] = 0x50;
            grid->h[i] = 0x14;
        }
    }
}

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_801326BC);

void func_801328C0(void) {
    s32 i;
    s32 j;
    u8 t; /* FAKE: single load of D_800E6280.unk_03A into $v0 for unk_10..unk_12; real source may use macro. T-24119 */
    void *p; /* FAKE: dead pointer stores order outer/inner induction variables in uopt bittab; real source unknown. T-24119 */

    ((SpriteSlot24 *)&D_801220D0)->unk_00 = 0x01000000;
    ((SpriteSlot24 *)&D_801220D0)->unk_04 = -0xA0;
    ((SpriteSlot24 *)&D_801220D0)->unk_06 = -0x78;
    ((SpriteSlot24 *)&D_801220D0)->unk_08 = 0x200;
    ((SpriteSlot24 *)&D_801220D0)->unk_0A = 0x140;
    ((SpriteSlot24 *)&D_801220D0)->unk_0C = 0;
    ((SpriteSlot24 *)&D_801220D0)->unk_0E = 0;
    t = D_800E6280.unk_03A;
    ((SpriteSlot24 *)&D_801220D0)->unk_10 = t;
    ((SpriteSlot24 *)&D_801220D0)->unk_11 = t;
    ((SpriteSlot24 *)&D_801220D0)->unk_12 = t;
    ((SpriteSlot24 *)&D_801220D0)->unk_14 = (u32)&D_80122120[0];
    ((SpriteSlot24 *)&D_801220D0)->unk_18 = 0;
    ((SpriteSlot24 *)&D_801220D0)->unk_1A = 0;
    ((SpriteSlot24 *)&D_801220D0)->unk_1C = 0x1000;
    ((SpriteSlot24 *)&D_801220D0)->unk_1E = 0x1000;
    ((SpriteSlot24 *)&D_801220D0)->unk_20 = 0;
    D_80122120[0].cellw = 0x80;
    D_80122120[0].cellh = 0x40;
    D_80122120[0].ncellw = 4;
    D_80122120[0].ncellh = 5;
    D_80122120[0].base = D_80122140[0];
    D_80122120[0].index = D_80122540[0].v;

    p = &D_80122540[0].v[i * 4];
    for (i = 0; i < 4; i++) {
        p = &D_80122140[0][j + i * 4];
        for (j = 0; j < 5; j++) {
            D_80122540[0].v[j + i * 4] = 0;
            D_80122140[0][j + i * 4].cba = func_8009ED74(0, 0x1FA);
            D_80122140[0][j + i * 4].u = 0;
            D_80122140[0][j + i * 4].v = 0;
            D_80122140[0][j + i * 4].tpage = 5;
            D_80122140[0][j + i * 4].flag = 0;
        }
    }
    D_800E6280.unk_10EC[0] = 0xD;
}

void func_80132A8C(void) {
    if (D_800E6280.unk_110A == 0) {
        func_80132AB8();
    }
}

void func_80132AB8(void) {
    if (D_800E6280.unk_03A >= 2U) {
        D_800E6280.unk_03A -= 2;
    }
    if (D_800E6280.unk_03A < 0x41U) {
        func_8004E58C();
    }
    if (D_800E6280.unk_03A < 2U) {
        D_800E6280.unk_03A = 0;
        func_80048EB8(0);
        main_state_set(0x12);
    }
}

void func_80132B30(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80132BA8();
        return;
    case 1:
        func_80133010();
        return;
    case 2:
        func_80133330();
        return;
    default:
        func_80046500();
        return;
    }
}

/* Option screen state machine (loader, CD checks). */
void func_80132BA8(void) {
    s32 pad[5]; /* FAKE: T-24301, puts `rows` at sp+0x3C in the 0x90 frame */
    Arrs4x8 rows;
    u8 r;

    switch (*(u8 *)&D_800E6280.unk_110D) {
    case 0:
        D_800E6280.unk_1124 = 1;
        func_800438DC(1, 0);
        func_80048DAC(1);
        func_80041584();
        func_80048E78();
        *(s32 *)&D_800E6280.unk_1095[3] = 0xA0;
        func_800438F0(1);
        D_800E6280.unk_037 = 0;
        D_800E6280.unk_036 = 0;
        D_800E6280.unk_038 = 0;
        func_80048390();
        D_800E6280.unk_10A2 = 0;
        func_8004E58C();
        D_800E6280.unk_1093 = 0;
        D_800E6280.unk_1094 = 0;
        func_8006BC28(0);
        D_800E6280.unk_03A = 0xFF;
        D_8013D300 = 0;
        D_800B3220 = 0;
        D_801230D0 = 0;
        text_set_advance(0xE);
        func_801325A0(&rows);
        func_8004F870(0, 8, rows.a, rows.b, rows.c, rows.d);
        func_8004E788(rows.a[0], rows.b[0], 0, "文字表示速度", 0);
        func_8004E788(rows.a[1], rows.b[1], 0, "アイコンモード", 0);
        func_8004E788(rows.a[2], rows.b[2], 0, "マウス設定", 0);
        func_8004E788(rows.a[3], rows.b[3], 0, "メニューモード", 0);
        func_8004E788(rows.a[4], rows.b[4], 0, "カーソル速度", 0);
        func_8004E788(rows.a[5], rows.b[5], 0, "音声モード", 0);
        func_8004E788(rows.a[6], rows.b[6], 0, "おまけ", 0);
        func_8004E788(rows.a[7], rows.b[7], 0, "オプション終了", 0);
        k_sub_reset_point_set();
        k_sub_disp_start(0);
        ((DisplayActor44 *)D_8011ECD0)[96].unk_18 = 0;
        ((DisplayActor44 *)D_8011ECD0)[96].unk_26 = -0xA0;
        ((DisplayActor44 *)D_8011ECD0)[96].unk_2A = -0x78;
        D_800E6280.unk_110D = 1;
        func_80053CE0();
        break;
    case 1:
        if (func_80053F04() != 0) {
            r = D_800E6280.unk_1115;
            if (r == 1) {
                D_800E6280.unk_110D += 1;
            } else if (r == 0xF1) {
                D_800E6280.unk_110D += 1;
            } else if (r == 0xFF) {
                D_800E6280.unk_110D += 1;
            }
        } else if ((u32)D_800E6280.unk_1120 > 0x200) {
            D_800E6280.unk_110D += 1;
        }
        break;
    case 2:
        func_80053D10();
        if (D_800E6280.unk_1115 == 1) {
            if (func_80054AF4(0) == 1) {
                D_800E6280.unk_110D += 1;
                func_80053CE0();
            } else {
                D_800E6280.unk_110D = 0xFF;
            }
            if (func_80054AF4(0xF) == 1) {
                D_800B3220 = 1;
            } else {
                D_800B3220 = 0;
            }
        } else {
            D_800E6280.unk_110D = 0xFF;
        }
        break;
    case 3:
        if (func_80054284() == 3) {
            func_80053D10();
            D_800E6280.unk_110D += 1;
        } else if ((u32)D_800E6280.unk_1120 > 0x200 || D_800E6280.unk_1115 == 0xF0) {
            func_80053D10();
            D_800E6280.unk_110D = 0xFF;
        }
        break;
    case 4:
        if (func_800552DC(0) == -1) {
            D_800E6280.unk_110D = 0xFF;
        } else {
            D_800E6280.unk_110D = 0xFF;
        }
        break;
    case 0xFF:
    default:
        cd_read_sectors(0x3D, 0x801B0000, 0x9EA4);
        func_8004284C();
        break;
    }
    func_8004AC18(D_800E6280.unk_03A);
}

/* Option screen loader: waits for the CD, sets up records 96..159 and the palettes, then fades in. */
void func_80133010(void) {
    s32 i;

    if (D_800E6280.unk_110D == 0) {
        if (cd_read_sync_status() & 1) {
            D_800E6280.unk_110D = 1;
        }
    } else if (D_800E6280.unk_110D == 1) {
        func_80048E78();
        D_8013D300 = 1;
        for (i = 96; i < 160; i++) {
            ((DisplayActor44 *)D_8011ECD0)[i].unk_01 = 5;
            ((DisplayActor44 *)D_8011ECD0)[i].unk_02 = 0x40;
            ((DisplayActor44 *)D_8011ECD0)[i].unk_38 = 0x01000000;
            ((DisplayActor44 *)D_8011ECD0)[i].unk_03 = 0x24;
            ((DisplayActor44 *)D_8011ECD0)[i].unk_0C = (u8 *)0x801C32D8;
            ((DisplayActor44 *)D_8011ECD0)[i].unk_10 = (u8 *)0x801C34E0;
            ((DisplayActor44 *)D_8011ECD0)[i].unk_34 = (u8 *)0x801C32C8;
            ((DisplayActor44 *)D_8011ECD0)[i].unk_04 = 8;
            ((DisplayActor44 *)D_8011ECD0)[i].unk_18 = 0;
            ((DisplayActor44 *)D_8011ECD0)[i].unk_43 = 0;
            ((DisplayActor44 *)D_8011ECD0)[i].unk_05 = 0x80;
            ((DisplayActor44 *)D_8011ECD0)[i].unk_26 = -0x200;
            ((DisplayActor44 *)D_8011ECD0)[i].unk_2A = -0x200;
            ((DisplayActor44 *)D_8011ECD0)[i].unk_07 = 0x80;
        }
        for (i = 0; i < 4; i++) {
            func_80043914(0x801C34F0, (u8)(i + 0x10), 1, 1, i);
        }
        func_80043914(0x801B0000, 0x1F, 1, 1, 0);
        for (i = 0; i != 4; i++) {
            func_80043914(0x801CBEF0, (u8)(i + 0x1A), 1, 1, i);
        }
        load_tpage_buf_lock(5, 0x801C7CF0, 1);
        func_801328C0();
        ((DisplayActor44 *)D_8011ECD0)[96].unk_18 = 0;
        ((DisplayActor44 *)D_8011ECD0)[96].unk_26 = -0xA0;
        ((DisplayActor44 *)D_8011ECD0)[96].unk_2A = -0x78;
        ((DisplayActor44 *)D_8011ECD0)[96].unk_03 |= 0x80;
        ((DisplayActor44 *)D_8011ECD0)[96].unk_01 = 8;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_18 = 0xA;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_26 = -0xA0;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_2A = -0x78;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_03 |= 0x80;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_01 = 3;
        ((DisplayActor44 *)D_8011ECD0)[106].unk_18 = 7;
        ((DisplayActor44 *)D_8011ECD0)[106].unk_26 = -0xA0;
        ((DisplayActor44 *)D_8011ECD0)[106].unk_2A = -0x78;
        ((DisplayActor44 *)D_8011ECD0)[106].unk_03 = 0x24;
        ((DisplayActor44 *)D_8011ECD0)[106].unk_01 = 1;
        ((DisplayActor44 *)D_8011ECD0)[159].unk_18 = 7;
        ((DisplayActor44 *)D_8011ECD0)[159].unk_26 = -0xA0;
        ((DisplayActor44 *)D_8011ECD0)[159].unk_2A = -0x78;
        ((DisplayActor44 *)D_8011ECD0)[159].unk_03 = 0x24;
        ((DisplayActor44 *)D_8011ECD0)[159].unk_01 = 1;
    }
    if (D_800E6280.unk_110D != 0) {
        D_800E6280.unk_03A -= 4;
        if (D_800E6280.unk_03A <= 0x80) {
            D_800E6280.unk_03A = 0x80;
            if (D_800E6280.unk_110D == 1) {
                D_800E6280.unk_110D = 2;
            }
        }
        if (D_800E6280.unk_110D == 2) {
            func_8004284C();
        }
    }
    func_8004AC18(D_800E6280.unk_03A);
}

/* FAKE: s32 without an explicit return matches the unprototyped call at func_80132B30. T-24117 */
s32 func_80133330(void) {
    s32 pad; /* FAKE: unused 4-byte local places sp2C at sp+0x2C (0x70 frame); real source unknown. T-24117 */
    Arrs4x8 sp2C;

    menu_check(0, ((DisplayActor44 *)D_8011ECD0)->unk_26, ((DisplayActor44 *)D_8011ECD0)->unk_2A);
    func_8004FC10(0);
    if (D_800E6280.unk_1093 == -1) {
        ((DisplayActor44 *)D_8011ECD0)[97].unk_03 &= 0xFF7F;
    } else {
        func_801325A0(&sp2C);
        ((DisplayActor44 *)D_8011ECD0)[97].unk_03 |= 0x80;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_26 = sp2C.a[D_800E6280.unk_1093] + 0x30;
        /* FAKE: '^ 0' prevents uopt from sharing the scaled byte offset with sp2C.a[]; real source unknown. T-24117 */
        ((DisplayActor44 *)D_8011ECD0)[97].unk_2A = sp2C.b[D_800E6280.unk_1093 ^ 0] + 6;
    }
    if (D_800E6280.unk_F88 & 0x20) {
        switch (D_800E6280.unk_1093) {
        case 0:
            func_80044750(0x501);
            func_80042908(1);
            break;
        case 1:
            func_80044750(0x501);
            func_80042908(2);
            break;
        case 2:
            if (D_800E6280.unk_F74 == 0) {
                func_80044750(0x501);
                func_80042908(3);
            }
            break;
        case 3:
            if (D_800E6280.unk_F74 == 1) {
                func_80044750(0x501);
                func_80042908(4);
            }
            break;
        case 4:
            func_80044750(0x501);
            func_80042908(5);
            break;
        case 5:
            func_80044750(0x501);
            func_80042908(6);
            break;
        case 6:
            func_80044750(0x501);
            func_80042908(7);
            break;
        case 7:
            func_80044750(0x501);
            func_80042908(8);
            break;
        }
    }
    if ((D_800B3220 == 1) && ((D_800E6280.unk_F80 & 0xF) == 0xF)) {
        main_state_set(0x21);
    }
    /* FAKE: 8-bit bitfield view prevents uopt from CSE-ing D_800B3220 across the untaken first branch; real source unknown. T-24117 */
    if ((((OptionByteBf *)&D_800B3220)->x == 1) && ((D_800E6280.unk_F80 & 0xF0000) == 0xF0000)) {
        main_state_set(0x22);
    }
    if (D_800E6280.unk_F88 & 0x40) {
        D_800E6280.unk_1093 = 7;
    }
}

/* FAKE: s32 without an explicit return retains the original dead v0 loop initialization; no meaningful return is established. T-9540 */
s32 func_8013357C(void) {
    s32 i;
    DisplayActor44 *p;

    for (i = 0x60; i < 0xA0; i++) {
        p = (DisplayActor44 *)D_8011ECD0 + i;
        if (p->unk_03 & 0x80) {
            func_8004AE54(p->unk_18, p->unk_26, p->unk_2A, 0xF);
        }
    }
}

void func_801335E8(void) {
    D_800E6280.unk_1100 += 1;
    D_800E6280.unk_1104.w += 1;
    switch (D_800E6280.unk_1109) {
    case 0:
        func_80132B30();
        break;
    case 1:
        func_80133C4C();
        break;
    case 2:
        func_8013447C();
        break;
    case 3:
        func_80135450();
        break;
    case 4:
        func_80135DE0();
        break;
    case 5:
        func_8013661C();
        break;
    case 6:
        func_80136BC8();
        break;
    case 7:
        func_8013884C();
        break;
    case 8:
        func_80132A8C();
        break;
    default:
        func_80046500();
        break;
    }
    func_801326BC();
    if (D_800E6280.unk_03A != 0) {
        func_80132000();
        func_801320B0();
    }
}

s32 func_80133718(void) {
    s32 i;
    s32 j;
    s32 pad[2]; /* FAKE: unused 8-byte local reproduces the original 0x90 frame and row offsets (T-4070). T-24119 */
    s16 a[6];
    s16 b[6];
    s16 c[6];
    s16 d[6];

    if (D_800E6280.unk_110D == 0) {
        ((DisplayActor44 *)D_8011ECD0)[106].unk_18 = 1;
        ((DisplayActor44 *)D_8011ECD0)[106].unk_26 = -0xA0;
        ((DisplayActor44 *)D_8011ECD0)[106].unk_2A = -0x78;
        ((DisplayActor44 *)D_8011ECD0)[106].unk_03 = 0;
        D_80120912[1] = 0x7FFF;
        D_800E6280.unk_110D = 1;
        D_80120912[0] = 0x7FFF;
        return;
    }
    D_80120912[0] = D_80120912[0] / 17 * 16;
    D_80120912[1] = D_80120912[1] / 17 * 16;
    if (D_80120912[0] <= 0x1000) {
        D_80120912[1] = 0x1000;
        D_80120912[0] = 0x1000;
        for (i = 0; i < 3; i++) {
            a[i] = 0x10;
            b[i] = i * 0x28 - 0x58;
            c[i] = 0x28;
            d[i] = 0x10;
            j = i * 2 + 0xE;
            /* FAKE: actor stores on one line so as1 schedules lbu unk_03 before sh unk_18 (T-7020). T-24119 */
            ((DisplayActor44 *)D_80120650)[i + 11].unk_18 = j; ((DisplayActor44 *)D_80120650)[i + 11].unk_26 = a[i]; ((DisplayActor44 *)D_80120650)[i + 11].unk_2A = b[i]; ((DisplayActor44 *)D_80120650)[i + 11].unk_03 |= 0x80;
        }
        menu_set(1, 3, a, b, c, d);
        switch (get_k_speed()) {
        case 0:
            D_800E6280.unk_1094 = 0;
            break;
        case 1:
            D_800E6280.unk_1094 = 1;
            break;
        case 2:
            D_800E6280.unk_1094 = 2;
            break;
        default:
            k_speed_set(0);
            D_800E6280.unk_1094 = 0;
            break;
        }
        func_8004284C();
    }
}

s32 func_8013394C(void) {
    s32 i;
    s32 j;
    GameState *g;

    /* one table: as1 keeps the loads after the store only when both go through D_8011ECD0 (T-9020) */
    ((DisplayActor44 *)D_8011ECD0)[106].unk_03 = 0x80;
    func_8004F984(1, ((DisplayActor44 *)D_8011ECD0)->unk_26, ((DisplayActor44 *)D_8011ECD0)->unk_2A);
    func_8004FC10(1);
    func_801320C0();
    /* FAKE: pointer view for unk_1094 makes uopt CSE the ILOD into $a2 while keeping move $v0, $a2 in the first switch branch delay slot. T-24082 */
    g = &D_800E6280;
    for (i = 0, j = 0; i < 3; i++, j += 2) {
        if (i == g->unk_1094) {
            ((DisplayActor44 *)D_80120650)[i + 11].unk_18 = j + 0xD;
        } else {
            ((DisplayActor44 *)D_80120650)[i + 11].unk_18 = j + 0xE;
        }
    }
    if (D_800E6280.unk_F88 & 0x20) {
        switch (g->unk_1094) {
        case 0:
            k_speed_set(0);
            D_800E6280.unk_1124 |= 0x80;
            func_8004284C();
            func_80044750(0x501);
            break;
        case 1:
            k_speed_set(1);
            D_800E6280.unk_1124 |= 0x80;
            func_8004284C();
            func_80044750(0x501);
            break;
        case 2:
            k_speed_set(3);
            D_800E6280.unk_1124 |= 0x80;
            func_8004284C();
            func_80044750(0x501);
            break;
        }
    }
    if (D_800E6280.unk_F88 & 0x40) {
        func_8004284C();
        D_800E6280.unk_110D = 2;
    }
}

s32 func_80133AD8(void) {
    s32 i;

    if (D_800E6280.unk_110D == 0) {
        /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
        for (i = 0; i < 3; i++) ((DisplayActor44 *)D_8011ECD0)[i + 107].unk_03 &= ~0x80;
        D_80120912[0] = D_80120912[0] / 9 * 8;
        if (D_80120912[0] < 9) {
            D_800E6280.unk_110D = 1;
        }
    } else if (D_800E6280.unk_110D == 2) {
        /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
        for (i = 0; i < 3; i++) ((DisplayActor44 *)D_8011ECD0)[i + 107].unk_03 &= ~0x80;
        D_80120912[0] = D_80120912[0] * 9 / 8;
        D_80120912[1] = D_80120912[1] * 9 / 8;
        if (D_80120912[0] >= 0x4000) {
            D_800E6280.unk_110D = 1;
        }
    } else {
        func_80042908(0);
        func_80042940(2);
    }
    D_801208FB = 0;
}

s32 func_80133C4C(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80133718();
        break;
    case 1:
        func_8013394C();
        break;
    case 2:
        func_80133AD8();
        break;
    }
}

void func_80133CB8(void) {
    GameState *g = &D_800E6280;
    s32 i;
    s32 j;
    s16 a[4];
    s16 b[4];
    s16 c[4];
    s16 d[4];

    if (g->unk_110D == 0) {
        ((DisplayActor44 *)D_8011ECD0)[106].unk_18 = 2;
        ((DisplayActor44 *)D_8011ECD0)[106].unk_26 = -0xA0;
        ((DisplayActor44 *)D_8011ECD0)[106].unk_2A = -0x78;
        ((DisplayActor44 *)D_8011ECD0)[106].unk_03 = 0;
        D_80120912[1] = 0x7FFF;
        g->unk_110D = 1;
        D_80120912[0] = 0x7FFF;
        if (0) {} /* FAKE: separates the store line from UBJP so as1 keeps li $t2, 0x7FFF before lui $at. T-24119 */
    } else {
        D_80120912[0] = D_80120912[0] / 17 * 16;
        D_80120912[1] = D_80120912[1] / 17 * 16;
        if (D_80120912[0] <= 0x1000) {
            D_80120912[1] = 0x1000;
            D_80120912[0] = 0x1000;
            for (i = 0; i < 3; i++) {
                a[i] = -0x20;
                b[i] = i * 0x18 - 0x38;
                c[i] = 0x50;
                d[i] = 0x10;
                j = i * 2 + 0x16;
                /* FAKE: actor stores on one line so as1 schedules lbu unk_03 before sh unk_18 (T-7020). T-24119 */
                ((DisplayActor44 *)D_80120650)[i + 11].unk_18 = j; ((DisplayActor44 *)D_80120650)[i + 11].unk_26 = a[i]; ((DisplayActor44 *)D_80120650)[i + 11].unk_2A = b[i]; ((DisplayActor44 *)D_80120650)[i + 11].unk_03 |= 0x80;
            }
            menu_set(1, 3, a, b, c, d);
            switch ((u8)g->unk_F6C % 3) {
            case 0:
            default:
                g->unk_1094 = 0;
                break;
            case 1:
                g->unk_1094 = 1;
                break;
            case 2:
                g->unk_1094 = 2;
                break;
            }
            func_8006509C();
            /* FAKE: loop body on the for line, as in the matched func_80134210 (T-7020). T-24119 */
            for (i = 0; i < 16; i++) *(s16 *)&D_80125C10[i * 4] += 200;
            func_800654D0((u8)g->unk_F6D);
            func_8004284C();
        }
    }
    func_801324E8();
}

void func_80133F50(void) {
    s32 pad; /* FAKE: unused 4-byte local reproduces the 0x40 stack frame and stack slot offsets; real source unknown. T-24117 */
    GameState *g;
    s32 i;
    s32 keep;
    s32 old;
    s32 j;

    ((DisplayActor44 *)D_8011ECD0)[106].unk_03 = 0x80;
    g = &D_800E6280;
    old = g->unk_1094;
    menu_check(1, ((DisplayActor44 *)D_8011ECD0)->unk_26, ((DisplayActor44 *)D_8011ECD0)->unk_2A);
    func_8004FC10(1);
    func_801320C0();
    for (i = 0, j = 0; i < 3; i++, j += 2) {
        if (i == g->unk_1094) {
            ((DisplayActor44 *)D_80120650)[i + 11].unk_18 = j + 0x15;
        } else {
            ((DisplayActor44 *)D_80120650)[i + 11].unk_18 = j + 0x16;
        }
    }
    if (old != g->unk_1094) {
        switch (g->unk_1094) {
        case 0:
            keep = (u8)g->unk_F6C;
            g->unk_F6C = 0;
            func_8006509C();
            /* FAKE: loop body on the for line, as in the matched func_80134210 (T-7020). T-24117 */
            for (i = 0; i < 16; i++) *(s16 *)&D_80125C10[i * 4] += 200;
            func_800654D0(g->unk_F6D = 0xE);
            g->unk_F6C = keep;
            break;
        case 1:
            keep = (u8)g->unk_F6C;
            g->unk_F6C = 1;
            func_8006509C();
            /* FAKE: loop body on the for line, as in the matched func_80134210 (T-7020). T-24117 */
            for (i = 0; i < 16; i++) *(s16 *)&D_80125C10[i * 4] += 200;
            func_800654D0(g->unk_F6D = 0xE);
            g->unk_F6C = keep;
            break;
        }
    }
    if (g->unk_F88 & 0x20) {
        switch (g->unk_1094) {
        case 0:
            g->unk_F6C = 3;
            func_80134210();
            func_8004284C();
            func_80044750(0x501);
            break;
        case 1:
            g->unk_F6C = 4;
            func_80134210();
            func_8004284C();
            func_80044750(0x501);
            break;
        case 2:
            g->unk_F6C = 5;
            func_80134210();
            func_8004284C();
            func_80044750(0x501);
            break;
        }
    }
    if (g->unk_F88 & 0x40) {
        func_80134210();
        func_8004284C();
        g->unk_110D = 2;
    }
}

void func_80134210(void) {
    s32 i;

    /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
    for (i = 0; i < 16; i++) *(s16 *)&D_80125C10[i * 4] -= 200;
    D_800E6280.unk_F6D = 0xF;
    func_8006509C();
    if ((u8)D_800E6280.unk_F6C % 3U != 2) {
        D_800E6280.unk_F6D = 0xE;
    }
}

void func_801342A8(void) {
    u8 mode;
    s32 i;
    s32 frame[16]; /* FAKE: observed unused 0x40-byte frame gap, T-24040. */
    Arrs4x8 rows;

    func_8006509C();
    icon_disp_switch(0);
    mode = D_800E6280.unk_110D;
    if (mode == 0) {
        /* FAKE: inherited same-line loop scheduling recipe, T-7020, T-24040. */
        icon_disp_switch(0);
        for (i = 0; i < 3; i++) ((DisplayActor44 *)D_8011ECD0)[i + 107].unk_03 &= ~0x80;
        D_80120912[0] = D_80120912[0] / 9 * 8;
        if (D_80120912[0] < 9) {
            D_800E6280.unk_110D += 1;
        }
    } else if (mode == 2) {
        /* FAKE: inherited same-line loop scheduling recipe, T-7020, T-24040. */
        icon_disp_switch(0);
        for (i = 0; i < 3; i++) ((DisplayActor44 *)D_8011ECD0)[i + 107].unk_03 &= ~0x80;
        D_80120912[0] = D_80120912[0] * 9 / 8;
        D_80120912[1] = D_80120912[1] * 9 / 8;
        if (D_80120912[0] >= 0x4000) {
            D_800E6280.unk_110D = 1;
        }
    } else {
        func_801325A0(&rows);
        menu_set(0, 8, rows.a, rows.b, rows.c, rows.d);
        func_80042908(0);
        func_80042940(2);
    }
    func_80132384();
    D_801208FB = 0;
}

void func_8013447C(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80133CB8();
        return;
    case 1:
        func_80133F50();
        return;
    case 2:
        func_801342A8();
        return;
    }
}

void func_801344E4(void) {
    s32 k;
    s32 i;
    s32 j;
    s16 a[4];
    s16 b[4];
    s16 c[4];
    s16 d[4];

    if (D_800E6280.unk_110D == 0) {
        ((DisplayActor44 *)D_8011ECD0)[106].unk_18 = 3;
        ((DisplayActor44 *)D_8011ECD0)[106].unk_26 = -0xA0;
        ((DisplayActor44 *)D_8011ECD0)[106].unk_2A = -0x78;
        ((DisplayActor44 *)D_8011ECD0)[106].unk_03 = 0;
        D_80120912[1] = 0x7FFF;
        D_800E6280.unk_110D = 1;
        D_80120912[0] = 0x7FFF;
        if (0) {} /* FAKE: separates the store line from UBJP so as1 keeps li $t2, 0x7FFF before lui $at. T-24126 */
    } else {
        D_80120912[0] = D_80120912[0] / 17 * 16;
        D_80120912[1] = D_80120912[1] / 17 * 16;
        if (D_80120912[0] <= 0x1000) {
            D_80120912[1] = 0x1000;
            D_80120912[0] = 0x1000;
            for (i = 0; i < 3; i++) {
                a[i] = -0x78;
                b[i] = i * 0x28 - 0x20;
                c[i] = 0x6C;
                d[i] = 0x10;
                j = i * 2 + 0x1C;
                /* FAKE: actor stores on one line so as1 schedules lbu unk_03 before sh unk_18 (T-7020). T-24126 */
                ((DisplayActor44 *)D_80120650)[i + 11].unk_18 = j; ((DisplayActor44 *)D_80120650)[i + 11].unk_26 = a[i]; ((DisplayActor44 *)D_80120650)[i + 11].unk_2A = b[i]; ((DisplayActor44 *)D_80120650)[i + 11].unk_03 |= 0x80;
            }
            menu_set(1, 3, a, b, c, d);
            for (i = 96, j = 0x22, k = -0x28; i < 98; i++, j += 2, k += 0x10) {
                ((DisplayActor44 *)D_8011ECD0)[i + 14].unk_18 = j; ((DisplayActor44 *)D_8011ECD0)[i + 14].unk_03 |= 0x80;
                ((DisplayActor44 *)D_8011ECD0)[i + 14].unk_26 = 0;
                ((DisplayActor44 *)D_8011ECD0)[i + 14].unk_2A = k;
                if (D_800E6280.unk_F75 == 1) {
                    ((DisplayActor44 *)D_8011ECD0)[14].unk_18++;
                } else {
                    ((DisplayActor44 *)D_8011ECD0)[15].unk_18++;
                }
            }
            for (i = 96, j = 0, k = 0x26; i < 98; i++, j += 0x10, k += 2) {
                ((DisplayActor44 *)D_8011ECD0)[i + 16].unk_18 = k; ((DisplayActor44 *)D_8011ECD0)[i + 16].unk_03 |= 0x80;
                ((DisplayActor44 *)D_8011ECD0)[i + 16].unk_26 = 0;
                ((DisplayActor44 *)D_8011ECD0)[i + 16].unk_2A = j;
                if (*(u8 *)&D_800E6280.unk_F70 == 0) {
                    ((DisplayActor44 *)D_8011ECD0)[16].unk_18++;
                } else {
                    ((DisplayActor44 *)D_8011ECD0)[17].unk_18++;
                }
            }
            for (i = 96, j = 0x2A, k = 0x28; i < 98; i++, k += 0x10, j += 2) {
                ((DisplayActor44 *)D_8011ECD0)[i + 18].unk_18 = j; ((DisplayActor44 *)D_8011ECD0)[i + 18].unk_26 = 0; ((DisplayActor44 *)D_8011ECD0)[i + 18].unk_2A = k; ((DisplayActor44 *)D_8011ECD0)[i + 18].unk_03 |= 0x80;
            }
            func_8004284C();
        }
    }
    func_801324E8();
}

void func_80134804(void) {
    s32 i;
    s32 j;
    s32 sel;

    /* one table: as1 keeps the loads after the store only when both go through D_8011ECD0 (T-9020) */
    ((DisplayActor44 *)D_8011ECD0)[106].unk_03 = 0x80;
    func_8004F984(1, ((DisplayActor44 *)D_8011ECD0)->unk_26, ((DisplayActor44 *)D_8011ECD0)->unk_2A);
    func_8004FC10(1);
    func_801320C0();
    sel = D_800E6280.unk_1094;
    for (i = 0, j = 0; i < 3; i++, j += 2) {
        if (i == sel) {
            ((DisplayActor44 *)D_80120650)[i + 11].unk_18 = j + 0x1B;
        } else {
            ((DisplayActor44 *)D_80120650)[i + 11].unk_18 = j + 0x1C;
        }
    }
    if (D_800E6280.unk_F88 & 0x20) {
        switch (sel) {
        case 0:
            func_80042940(3);
            break;
        case 1:
            func_80042940(6);
            break;
        case 2:
            if (*(u8 *)&D_800E6280.unk_F70 == 1) {
                func_80042940(9);
            }
            break;
        }
    }
    if (D_800E6280.unk_F88 & 0x40) {
        func_8004284C();
        D_800E6280.unk_110D = 2;
    }
}

void func_8013493C(void) {
    s32 i;
    s32 frame[18]; /* FAKE: observed unused 0x48-byte frame area, T-24040. */
    Arrs4x8 rows;

    if (D_800E6280.unk_110D == 0) {
        /* FAKE: inherited same-line loop scheduling recipe, T-7020, T-24040. */
        icon_disp_switch(0);
        for (i = 0; i < 9; i++) ((DisplayActor44 *)D_8011ECD0)[i + 107].unk_03 &= ~0x80;
        D_80120912[0] = D_80120912[0] / 9 * 8;
        if (D_80120912[0] < 9) {
            D_800E6280.unk_110D += 1;
        }
    } else if (D_800E6280.unk_110D == 2) {
        /* FAKE: inherited same-line loop scheduling recipe, T-7020, T-24040. */
        icon_disp_switch(0);
        for (i = 0; i < 9; i++) ((DisplayActor44 *)D_8011ECD0)[i + 107].unk_03 &= ~0x80;
        D_80120912[0] = D_80120912[0] * 9 / 8;
        D_80120912[1] = D_80120912[1] * 9 / 8;
        if (D_80120912[0] >= 0x4000) {
            D_800E6280.unk_110D = 1;
        }
    } else {
        func_801325A0(&rows);
        menu_set(0, 8, rows.a, rows.b, rows.c, rows.d);
        func_80042908(0);
        func_80042940(2);
    }
    func_80132384();
    D_801208FB = 0;
}

void func_80134B6C(void) {
    s32 i;
    s32 pad[2]; /* FAKE: unused 8-byte local declared first reproduces the original frame (0x60); real source unknown. T-4070 */
    s16 a[2];
    s16 b[2];
    s16 c[2];
    s16 d[2];

    for (i = 0; i < 2; i++) {
        a[i] = 0;
        b[i] = i * 0x10 - 0x28;
        c[i] = 0x6C;
        d[i] = 0x10;
    }
    func_8004F870(1, 2, a, b, c, d);
    func_8004284C();
    func_801320C0();
}

void func_80134C00(void) {
    s32 i;
    s32 j;
    s32 sel;

    func_8004F984(1, D_8011ECF6, D_8011ECFA);
    func_8004FC10(1);
    func_801320C0();
    sel = D_800E6280.unk_1094;
    for (i = 0, j = 0; i < 2; i++, j += 2) {
        if (i == sel) {
            ((DisplayActor44 *)D_80120650)[i + 14].unk_18 = j + 0x21;
        } else {
            ((DisplayActor44 *)D_80120650)[i + 14].unk_18 = j + 0x22;
        }
    }
    if (D_800E6280.unk_F88 & 0x20) {
        switch (sel) {
        case 0:
            D_800E6280.unk_F75 = 1;
            D_800E6280.unk_1124 |= 0x10;
            func_8004284C();
            func_80044750(0x501);
            break;
        case 1:
            D_800E6280.unk_F75 = 0;
            D_800E6280.unk_1124 |= 0x10;
            func_8004284C();
            func_80044750(0x501);
            break;
        }
    }
    if (D_800E6280.unk_F88 & 0x40) {
        func_8004284C();
    }
}

void func_80134D38(void) {
    s32 pad[2]; /* FAKE: unused 8-byte local declared first reproduces the original frame (0x70) and array offsets; real source unknown. T-4070 */
    s16 a[4];
    s16 b[4];
    s16 c[4];
    s16 d[4];
    s32 i;

    for (i = 0; i < 3; i++) {
        a[i] = -0x78;
        b[i] = i * 0x28 - 0x20;
        c[i] = 0x6C;
        d[i] = 0x10;
    }
    func_8004F870(1, 3, a, b, c, d);
    func_801320C0();
    func_80042940(1);
}

void func_80134DD0(void) {
    s32 i;
    s32 pad[2]; /* FAKE: unused 8-byte local declared first reproduces the original frame (0x60); real source unknown. T-4070 */
    s16 a[2];
    s16 b[2];
    s16 c[2];
    s16 d[2];

    for (i = 0; i < 2; i++) {
        a[i] = 0;
        b[i] = i * 0x10;
        c[i] = 0x6C;
        d[i] = 0x10;
    }
    func_8004F870(1, 2, a, b, c, d);
    func_8004284C();
    func_801320C0();
}

void func_80134E64(void) {
    s32 i;
    s32 j;
    s32 sel;

    func_8004F984(1, D_8011ECF6, D_8011ECFA);
    func_8004FC10(1);
    func_801320C0();
    sel = D_800E6280.unk_1094;
    for (i = 0, j = 0; i < 2; i++, j += 2) {
        if (i == sel) {
            ((DisplayActor44 *)D_80120650)[i + 16].unk_18 = j + 0x25;
        } else {
            ((DisplayActor44 *)D_80120650)[i + 16].unk_18 = j + 0x26;
        }
    }
    if (D_800E6280.unk_F88 & 0x20) {
        switch (sel) {
        case 0:
            D_800E6280.unk_F70 = 0;
            D_800E6280.unk_1124 |= 0x40;
            func_8004284C();
            func_80044750(0x501);
            break;
        case 1:
            D_800E6280.unk_F70 = 1;
            D_800E6280.unk_1124 |= 0x40;
            func_8004284C();
            func_80044750(0x501);
            break;
        }
    }
    if (D_800E6280.unk_F88 & 0x40) {
        func_8004284C();
    }
}

void func_80134F9C(void) {
    s32 pad[2]; /* FAKE: unused 8-byte local declared first reproduces the original frame (0x70) and array offsets; real source unknown. T-4070 */
    s16 a[4];
    s16 b[4];
    s16 c[4];
    s16 d[4];
    s32 i;

    for (i = 0; i < 3; i++) {
        a[i] = -0x78;
        b[i] = i * 0x28 - 0x20;
        c[i] = 0x6C;
        d[i] = 0x10;
    }
    func_8004F870(1, 3, a, b, c, d);
    func_801320C0();
    func_80042940(1);
}

void func_80135034(void) {
    s32 i;
    s32 pad[2]; /* FAKE: unused 8-byte local declared first reproduces the original frame; real source unknown. T-4070 */
    s16 a[2];
    s16 b[2];
    s16 c[2];
    s16 d[2];

    for (i = 0; i < 2; i++) {
        a[i] = 0;
        b[i] = i * 0x10 + 0x28;
        c[i] = 0x6C;
        d[i] = 0x10;
    }
    func_8004F870(1, 2, a, b, c, d);
    /* record 20 of the 0x44-byte table at D_80120650: one base symbol keeps the load behind the stores (T-4070) */
    ((DisplayActor44 *)D_80120650)[20].unk_18 = 0x79;
    ((DisplayActor44 *)D_80120650)[20].unk_26 = 0x40;
    ((DisplayActor44 *)D_80120650)[20].unk_2A = 0x30;
    ((DisplayActor44 *)D_80120650)[20].unk_03 |= 0x80;
    ((DisplayActor44 *)D_80120650)[20].unk_43 = 0x1F;
    D_8013D180 = 0;
    func_8004284C();
    func_801320C0();
}

void func_80135110(void) {
    s32 i;
    s32 j;
    s32 sel;

    func_8004F984(1, D_8011ECF6, D_8011ECFA);
    func_8004FC10(1);
    func_801320C0();
    sel = D_800E6280.unk_1094;
    for (i = 0, j = 0; i < 2; i++, j += 2) {
        if (i == sel) {
            ((DisplayActor44 *)D_80120650)[i + 18].unk_18 = j + 0x29;
        } else {
            ((DisplayActor44 *)D_80120650)[i + 18].unk_18 = j + 0x2A;
        }
    }
    if (((D_800E6280.unk_F75 != 0) && (D_800E6280.unk_F77 & 4)) || ((D_800E6280.unk_F75 == 0) && (D_800E6280.unk_F77 & 8))) {
        switch (sel) {
        case 0:
            D_800E6280.unk_1124 |= 0x20;
            *(u8 *)&D_800E6280.unk_F73 = 0x20;
            func_80044750(0x501);
            func_8004284C();
            break;
        case 1:
            if (*(u8 *)&D_8013D180 != 0) {
                D_800E6280.unk_1124 |= 0x20;
                *(u8 *)&D_800E6280.unk_F73 = 0xFF - *(u8 *)&D_8013D180;
                if (*(u8 *)&D_800E6280.unk_F73 < 0x10U) {
                    *(u8 *)&D_800E6280.unk_F73 = 0x10;
                }
                if (*(u8 *)&D_800E6280.unk_F73 >= 0x41U) {
                    *(u8 *)&D_800E6280.unk_F73 = 0x40;
                }
                func_80044750(0x501);
                func_8004284C();
                *(u8 *)&D_8013D180 = 0;
            } else {
                *(u8 *)&D_8013D180 = 0xFF;
            }
            break;
        }
    }
    if (D_800E6280.unk_F88 & 0x40) {
        D_80120BA3 &= 0xFF7F;
        func_8004284C();
    }
    func_801352FC();
    if (*(u8 *)&D_8013D180 != 0) {
        *(u8 *)&D_8013D180 -= 1;
    }
}

void func_801352FC(void) {
    if ((*(u8 *)&D_800E6280.unk_F73 != 0) && ((u32) ((D_800E6280.unk_10F8 / *(u8 *)&D_800E6280.unk_F73) & 3) < 2U)) {
        if ((u32) ((u32) D_800E6280.unk_10F8 % *(u8 *)&D_800E6280.unk_F73) < 2U) {
            if (D_800E6280.unk_F75 == 0) {
                D_80120BB8 = 0x78;
                return;
            }
            D_80120BB8 = 0x79;
            return;
        }
        D_80120BB8 = 0x77;
    }
}

void func_8013539C(void) {
    s32 i;
    s32 pad[2]; /* FAKE: unused 8-byte local declared first reproduces the original frame; real source unknown. T-4070 */
    s16 a[4];
    s16 b[4];
    s16 c[4];
    s16 d[4];

    for (i = 0; i < 3; i++) {
        a[i] = -0x78;
        b[i] = i * 0x28 - 0x20;
        c[i] = 0x6C;
        d[i] = 0x10;
    }
    func_8004F870(1, 3, a, b, c, d);
    func_801320C0();
    D_80120BA3 &= 0xFF7F;
    D_80120BE3 = 0;
    func_80042940(1);
}

s32 func_80135450() {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_801344E4();
        return;
    case 1:
        func_80134804();
        return;
    case 2:
        func_8013493C();
        return;
    case 3:
        func_80134B6C();
        return;
    case 4:
        func_80134C00();
        return;
    case 5:
        func_80134D38();
        return;
    case 6:
        func_80134DD0();
        return;
    case 7:
        func_80134E64();
        return;
    case 8:
        func_80134F9C();
        return;
    case 9:
        func_80135034();
        return;
    case 10:
        func_80135110();
        return;
    case 11:
        func_8013539C();
        /* fallthrough */
    default:
        return;
    }
}

void func_8013554C(void) {
    GameState *g = &D_800E6280;
    s32 i;
    u8 t;
    s16 a[6];
    s16 b[6];
    s16 c[6];
    s16 d[6];

    t = g->unk_110D;
    switch (t) {
    case 0:
        ((DisplayActor44 *)D_8011ECD0)[106].unk_18 = 2;
        ((DisplayActor44 *)D_8011ECD0)[106].unk_26 = -0xA0;
        ((DisplayActor44 *)D_8011ECD0)[106].unk_2A = -0x78;
        ((DisplayActor44 *)D_8011ECD0)[106].unk_03 = 0;
        D_80120912[1] = 0x7FFF;
        g->unk_110D = 1;
        D_80120912[0] = 0x7FFF;
        break;
    case 1:
        D_80120912[0] = D_80120912[0] / 17 * 16;
        D_80120912[1] = D_80120912[1] / 17 * 16;
        if (D_80120912[0] <= 0x1000) {
            D_80120912[1] = 0x1000;
            D_80120912[0] = 0x1000;
            k_sub_reset();
            for (i = 0; i < 4; i++) {
                a[i] = -0x20;
                b[i] = i * 0x18 - 0x38;
                c[i] = 0xA0;
                d[i] = 0x10;
                set_kanji_string(a[i] - 4, b[i], 0, (s32)D_8013D350[i], 0);
            }
            menu_set(1, 4, a, b, c, d);
            switch (g->unk_1092 & 3) {
            case 0:
                ((DisplayActor44 *)D_80120650)[11].unk_18--;
                g->unk_1094 = 0;
                break;
            case 1:
                ((DisplayActor44 *)D_80120650)[12].unk_18--;
                g->unk_1094 = 1;
                break;
            case 3:
                ((DisplayActor44 *)D_80120650)[13].unk_18--;
                g->unk_1094 = 2;
                break;
            case 2:
                ((DisplayActor44 *)D_80120650)[14].unk_18--;
                g->unk_1094 = 3;
                break;
            }
            set_kanji_string(-0x80, 0x40, 0xF, (s32)"　カーソルが、方向キーの上下左右に", 0);
            set_kanji_string(-0x80, 0x50, 0xF, (s32)"反応して動きます。", 0);
            set_kanji_string(-0x80, 0x60, 0xF, (s32)"（マウスと同等です）", 0);
            set_kanji_string(-0x80, 0x40, 0xF, (s32)"　メニュー選択時に、方向キーを押すと", 0);
            set_kanji_string(-0x80, 0x50, 0xF, (s32)"メニューアイテムの順番通りにカーソル", 0);
            set_kanji_string(-0x80, 0x60, 0xF, (s32)"が移動します", 0);
            set_kanji_string(-0x80, 0x40, 0xF, (s32)"　メニュー選択時に、方向キーを押すと", 0);
            set_kanji_string(-0x80, 0x50, 0xF, (s32)"その方向のメニューアイテムにカーソル", 0);
            set_kanji_string(-0x80, 0x60, 0xF, (s32)"が移動します", 0);
            set_kanji_string(-0x80, 0x40, 0xF, (s32)"　通常は、マウスライクと同じ動作を", 0);
            set_kanji_string(-0x80, 0x50, 0xF, (s32)"します。Ｌ１ボタンを押している間は、", 0);
            set_kanji_string(-0x80, 0x60, 0xF, (s32)"メニューＡと同じ動作をします", 0);
            k_sub_disp_start(0x10);
            for (i = 0; i < 12; i++) {
                k_disp_switch(i + 0xC, 0);
            }
            func_8004284C();
        }
        break;
    }
    func_801324E8();
}

void func_801359A4(void) {
    s32 i;
    s32 k;
    GameState *g;

    ((DisplayActor44 *)D_8011ECD0)[106].unk_03 = 0x80;
    menu_check(1, ((DisplayActor44 *)D_8011ECD0)->unk_26, ((DisplayActor44 *)D_8011ECD0)->unk_2A);
    func_8004FC10(1);
    func_801320C0();
    g = &D_800E6280;
    for (i = 0; i < 4; i++) {
        if (i == g->unk_1094) {
            k_disp_switch(i * 3 + 0xC, 1);
            k_disp_switch(i * 3 + 0xD, 1);
            k_disp_switch(i * 3 + 0xE, 1);
            ((Entry8 *)text_list)[i + 8].unk_04 = 0;
            k = i * 2 + 0x7A;
            ((DisplayActor44 *)D_80120650)[i + 11].unk_18 = k;
        } else {
            k_disp_switch(i * 3 + 0xC, 0);
            k_disp_switch(i * 3 + 0xD, 0);
            k_disp_switch(i * 3 + 0xE, 0);
            ((Entry8 *)text_list)[i + 8].unk_04 = 1;
            k = i * 2 + 0x7B;
            ((DisplayActor44 *)D_80120650)[i + 11].unk_18 = k;
        }
    }
    func_80049A40(-0x85, 0x3A, 0x100, 0x34, 8, 0x80808, 0x80);
    if (D_800E6280.unk_F88 & 0x20) {
        switch (g->unk_1094) {
        case 0:
            D_800E6280.unk_1092 = 4;
            func_8004284C();
            func_80044750(0x501);
            break;
        case 1:
            D_800E6280.unk_1092 = 5;
            func_8004284C();
            func_80044750(0x501);
            break;
        case 2:
            D_800E6280.unk_1092 = 7;
            func_8004284C();
            func_80044750(0x501);
            break;
        case 3:
            D_800E6280.unk_1092 = 6;
            func_8004284C();
            func_80044750(0x501);
            break;
        }
    }
    if (D_800E6280.unk_F88 & 0x40) {
        k_sub_reset();
        func_8004284C();
        D_800E6280.unk_110D = 2;
    }
}

void func_80135C0C(void) {
    s32 i;

    if (D_800E6280.unk_110D == 0) {
        /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
        for (i = 0; i < 4; i++) ((u8 *)D_80120912)[0x2D + i * 0x44] &= ~0x80;
        D_80120912[0] = D_80120912[0] / 9 * 8;
        if (D_80120912[0] < 9) {
            D_800E6280.unk_110D = 1;
        }
    } else if (D_800E6280.unk_110D == 2) {
        /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
        for (i = 0; i < 4; i++) ((u8 *)D_80120912)[0x2D + i * 0x44] &= 0xFF7F;
        D_80120912[0] = D_80120912[0] * 9 / 8;
        D_80120912[1] = D_80120912[1] * 9 / 8;
        if (D_80120912[0] >= 0x4000) {
            D_800E6280.unk_110D = 1;
        }
    } else {
        func_80042908(0);
        func_80042940(2);
    }
    func_80132384();
    D_801208FB = 0;
    func_8004EAAC();
}

void func_80135DE0(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8013554C();
        return;
    case 1:
        func_801359A4();
        return;
    case 2:
        func_80135C0C();
        return;
    }
}

s32 func_80135E48(void) {
    GameState *g = &D_800E6280;
    s32 i;
    s32 pad; /* FAKE: unused word reproduces the original frame (0x88) and the row offsets, like the pad of the matched four-array pages (T-4070); real source unknown. T-20910 */
    s16 a[6];
    s16 b[6];
    s16 c[6];
    s16 d[6];

    if (g->unk_110D == 0) {
        ((DisplayActor44 *)D_8011ECD0)[106].unk_18 = 4;
        ((DisplayActor44 *)D_8011ECD0)[106].unk_26 = -0xA0;
        ((DisplayActor44 *)D_8011ECD0)[106].unk_2A = -0x78;
        ((DisplayActor44 *)D_8011ECD0)[106].unk_03 = 0;
        D_80120912[1] = 0x7FFF;
        g->unk_110D = 1;
        D_80120912[0] = 0x7FFF;
        return;
    }
    D_80120912[0] = D_80120912[0] / 17 * 16;
    D_80120912[1] = D_80120912[1] / 17 * 16;
    if (D_80120912[0] <= 0x1000) {
        D_80120912[1] = 0x1000;
        D_80120912[0] = 0x1000;
        for (i = 0; i < 5; i++) {
            a[i] = 0x10;
            b[i] = i * 0x18 - 0x18;
            c[i] = 0x30;
            d[i] = 0x10;
            ((DisplayActor44 *)D_80120650)[i + 11].unk_18 = i * 2 + 0xC;
            ((DisplayActor44 *)D_80120650)[i + 11].unk_26 = a[i];
            ((DisplayActor44 *)D_80120650)[i + 11].unk_2A = b[i];
            ((DisplayActor44 *)D_80120650)[i + 11].unk_03 |= 0x80;
        }
        func_8004F870(1, 5, a, b, c, d);
        switch ((u8)g->unk_F6F % 10) {
        case 1:
        default:
            ((DisplayActor44 *)D_80120650)[11].unk_18++;
            g->unk_1094 = 0;
            break;
        case 2:
            ((DisplayActor44 *)D_80120650)[12].unk_18++;
            g->unk_1094 = 1;
            break;
        case 3:
            ((DisplayActor44 *)D_80120650)[13].unk_18++;
            g->unk_1094 = 1;
            break;
        case 4:
            ((DisplayActor44 *)D_80120650)[14].unk_18++;
            g->unk_1094 = 1;
            break;
        case 6:
            ((DisplayActor44 *)D_80120650)[15].unk_18++;
            g->unk_1094 = 1;
            break;
        }
        func_8004284C();
    }
}

/* FAKE: s32 without an explicit return retains the original dead v0 loop initialization; no meaningful return is established. T-24117 */
s32 func_801361FC(void) {
    s32 i;
    s32 j;
    s32 sel;

    /* one table: as1 keeps the loads after the store only when both go through D_8011ECD0 (T-9020) */
    ((DisplayActor44 *)D_8011ECD0)[106].unk_03 = 0x80;
    func_8004F984(1, ((DisplayActor44 *)D_8011ECD0)->unk_26, ((DisplayActor44 *)D_8011ECD0)->unk_2A);
    func_8004FC10(1);
    func_801320C0();
    sel = D_800E6280.unk_1094;
    for (i = 0, j = 0; i < 5; i++, j += 2) {
        if (i == sel) {
            ((DisplayActor44 *)D_80120650)[i + 11].unk_18 = j + 0xB;
        } else {
            ((DisplayActor44 *)D_80120650)[i + 11].unk_18 = j + 0xC;
        }
    }
    if (D_800E6280.unk_F88 & 0x20) {
        switch (sel) {
        case 0:
            D_800E6280.unk_F6F = 1;
            D_800E6280.unk_F74 = 0xFF;
            D_800E6280.unk_1124 |= 8;
            func_8004284C();
            func_80044750(0x501);
            break;
        case 1:
            D_800E6280.unk_F6F = 2;
            D_800E6280.unk_F74 = 0xFF;
            D_800E6280.unk_1124 |= 8;
            func_8004284C();
            func_80044750(0x501);
            break;
        case 2:
            D_800E6280.unk_F6F = 3;
            D_800E6280.unk_F74 = 0xFF;
            D_800E6280.unk_1124 |= 8;
            func_8004284C();
            func_80044750(0x501);
            break;
        case 3:
            D_800E6280.unk_F6F = 4;
            D_800E6280.unk_F74 = 0xFF;
            D_800E6280.unk_1124 |= 8;
            func_8004284C();
            func_80044750(0x501);
            break;
        case 4:
            D_800E6280.unk_F6F = 6;
            D_800E6280.unk_F74 = 0xFF;
            D_800E6280.unk_1124 |= 8;
            func_8004284C();
            func_80044750(0x501);
            break;
        }
    }
    if (D_800E6280.unk_F88 & 0x40) {
        func_8004284C();
        D_800E6280.unk_110D = 2;
    }
}

s32 func_80136434(void) {
    s32 i;

    if (D_800E6280.unk_110D == 0) {
        /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
        for (i = 0; i < 5; i++) ((DisplayActor44 *)D_8011ECD0)[i + 107].unk_03 &= ~0x80;
        D_80120912[0] = D_80120912[0] / 9 * 8;
        if (D_80120912[0] < 9) {
            D_800E6280.unk_110D = 1;
        }
    } else if (D_800E6280.unk_110D == 2) {
        /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
        for (i = 0; i < 5; i++) ((DisplayActor44 *)D_8011ECD0)[i + 107].unk_03 &= ~0x80;
        D_80120912[0] = D_80120912[0] * 9 / 8;
        D_80120912[1] = D_80120912[1] * 9 / 8;
        if (D_80120912[0] >= 0x4000) {
            D_800E6280.unk_110D = 1;
        }
    } else {
        func_80042908(0);
        func_80042940(2);
    }
    D_801208FB = 0;
}

s32 func_8013661C(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80135E48();
        break;
    case 1:
        func_801361FC();
        break;
    case 2:
        func_80136434();
        break;
    }
}

void func_80136688(void) {
    GameState *g = &D_800E6280;
    s32 i;
    s32 j;
    s16 a[6];
    s16 b[6];
    s16 c[6];
    s16 d[6];

    if (g->unk_110D == 0) {
        ((DisplayActor44 *)D_8011ECD0)[106].unk_18 = 5;
        ((DisplayActor44 *)D_8011ECD0)[106].unk_26 = -0xA0;
        ((DisplayActor44 *)D_8011ECD0)[106].unk_2A = -0x78;
        ((DisplayActor44 *)D_8011ECD0)[106].unk_03 = 0;
        D_80120912[1] = 0x7FFF;
        g->unk_110D = 1;
        D_80120912[0] = 0x7FFF;
        if (0) {} /* FAKE: separates the store line from UBJP so as1 keeps li $t2, 0x7FFF before lui $at. T-24119 */
    } else {
        D_80120912[0] = D_80120912[0] / 17 * 16;
        D_80120912[1] = D_80120912[1] / 17 * 16;
        if (D_80120912[0] <= 0x1000) {
            D_80120912[1] = 0x1000;
            D_80120912[0] = 0x1000;
            for (i = 0; i < 3; i++) {
                a[i] = 0;
                b[i] = i * 0x18 + 0x18;
                c[i] = 0x80;
                d[i] = 0x10;
                j = i * 2 + 0x2E;
                /* FAKE: actor stores on one line so as1 schedules lbu unk_03 before sh unk_18 (T-7020). T-24119 */
                ((DisplayActor44 *)D_80120650)[i + 11].unk_18 = j; ((DisplayActor44 *)D_80120650)[i + 11].unk_26 = a[i]; ((DisplayActor44 *)D_80120650)[i + 11].unk_2A = b[i]; ((DisplayActor44 *)D_80120650)[i + 11].unk_03 |= 0x80;
            }
            menu_set(1, 3, a, b, c, d);
            switch ((u8)g->unk_03C % 3) {
            case 1:
                ((DisplayActor44 *)D_80120650)[11].unk_18++;
                g->unk_1094 = 0;
                break;
            case 0:
                ((DisplayActor44 *)D_80120650)[12].unk_18++;
                g->unk_1094 = 1;
                break;
            case 2:
            default:
                ((DisplayActor44 *)D_80120650)[13].unk_18++;
                g->unk_1094 = 2;
                break;
            }
            func_8004284C();
        }
    }
    func_801324E8();
}

void func_801368FC(void) {
    s32 i;
    s32 j;
    s32 sel;

    /* one table: as1 keeps the loads after the store only when both go through D_8011ECD0 (T-9020) */
    ((DisplayActor44 *)D_8011ECD0)[106].unk_03 = 0x80;
    func_8004F984(1, ((DisplayActor44 *)D_8011ECD0)->unk_26, ((DisplayActor44 *)D_8011ECD0)->unk_2A);
    func_8004FC10(1);
    func_801320C0();
    sel = D_800E6280.unk_1094;
    for (i = 0, j = 0; i < 3; i++, j += 2) {
        if (i == sel) {
            ((DisplayActor44 *)D_80120650)[i + 11].unk_18 = j + 0x2D;
        } else {
            ((DisplayActor44 *)D_80120650)[i + 11].unk_18 = j + 0x2E;
        }
    }
    if (D_800E6280.unk_F88 & 0x20) {
        switch (sel) {
        case 0:
            D_800E6280.unk_03C = 4;
            func_8004284C();
            func_80044750(0x501);
            break;
        case 1:
            D_800E6280.unk_03C = 3;
            func_8004284C();
            func_80044750(0x501);
            break;
        case 2:
            D_800E6280.unk_03C = 5;
            func_8004284C();
            func_80044750(0x501);
            break;
        }
    }
    if (D_800E6280.unk_F88 & 0x40) {
        func_8004284C();
        D_800E6280.unk_110D = 2;
    }
}

void func_80136A50(void) {
    s32 i;

    if (D_800E6280.unk_110D == 0) {
        /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
        for (i = 0; i < 3; i++) ((DisplayActor44 *)D_8011ECD0)[i + 107].unk_03 &= ~0x80;
        D_80120912[0] = D_80120912[0] / 9 * 8;
        if (D_80120912[0] < 9) {
            D_800E6280.unk_110D = 1;
        }
    } else if (D_800E6280.unk_110D == 2) {
        /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
        for (i = 0; i < 3; i++) ((DisplayActor44 *)D_8011ECD0)[i + 107].unk_03 &= ~0x80;
        D_80120912[0] = D_80120912[0] * 9 / 8;
        D_80120912[1] = D_80120912[1] * 9 / 8;
        if (D_80120912[0] >= 0x4000) {
            D_800E6280.unk_110D = 1;
        }
    } else {
        func_80042908(0);
        func_80042940(2);
    }
    func_80132384();
    D_801208FB = 0;
}

void func_80136BC8(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80136688();
        return;
    case 1:
        func_801368FC();
        return;
    case 2:
        func_80136A50();
        return;
    }
}

void func_80136C30(void) {
    s32 i;
    s32 pad[3]; /* FAKE: unused words reproduce the original frame (0x70) and the row offsets (T-4070 pad); real source unknown. T-20910 */
    s16 a[6];
    s16 b[6];
    s16 c[6];
    s16 d[6];

    if (D_800E6280.unk_110D == 0) {
        ((DisplayActor44 *)D_8011ECD0)[106].unk_18 = 6;
        ((DisplayActor44 *)D_8011ECD0)[106].unk_26 = -0xA0;
        ((DisplayActor44 *)D_8011ECD0)[106].unk_2A = -0x78;
        ((DisplayActor44 *)D_8011ECD0)[106].unk_03 = 0;
        D_80120912[1] = 0x7FFF;
        D_80120912[0] = 0x7FFF;
        D_800E6280.unk_110D = 1;
    } else {
        D_80120912[0] = D_80120912[0] / 17 * 16;
        D_80120912[1] = D_80120912[1] / 17 * 16;
        if (D_80120912[0] <= 0x1000) {
            D_80120912[1] = 0x1000;
            D_80120912[0] = 0x1000;
            for (i = 0; i < 4; i++) {
                a[i] = -0x40;
                b[i] = i * 0x10 + 0x18;
                c[i] = 0x60;
                d[i] = 0x10;
                /* FAKE: actor stores on one line so as1 schedules lui t6 (unk_03) before li t5, 52 (unk_18) (T-7020). T-24110 */
                ((DisplayActor44 *)D_80120650)[i + 11].unk_18 = i * 2 + 0x34; ((DisplayActor44 *)D_80120650)[i + 11].unk_26 = a[i]; ((DisplayActor44 *)D_80120650)[i + 11].unk_2A = b[i]; ((DisplayActor44 *)D_80120650)[i + 11].unk_03 |= 0x80;
            }
            if (!(D_800E7D4F & 1) && !(D_800E7D50 & 1)) {
                c[0] = 0;
                ((DisplayActor44 *)D_80120650)[11].unk_03 = 0;
            }
            if (func_80136F3C() == 0) {
                ((DisplayActor44 *)D_80120650)[12].unk_03 = 0;
                c[1] = 0;
                c[2] = 0;
                ((DisplayActor44 *)D_80120650)[13].unk_03 = 0;
            }
            if (D_800E7D37 == 0) {
                c[3] = 0;
                ((DisplayActor44 *)D_80120650)[14].unk_03 = 0;
            }
            menu_set(1, 4, a, b, c, d);
            D_800E6280.unk_1094 = 0;
            func_8004284C();
        }
    }
    func_801324E8();
}

s32 func_80136F3C(void) {
    s32 i;
    s32 count;

    count = 0;
    for (i = 0; i < 14; i++) {
        if (D_800E7D10.flags[i] != 0 || D_800E6280.unk_53C[i] != 0) {
            count++;
        }
    }
    return count;
}

void func_80137040(void) {
    s32 i;
    s32 j;
    s32 sel;

    /* one table: as1 keeps the loads after the store only when both go through D_8011ECD0 (T-9020) */
    ((DisplayActor44 *)D_8011ECD0)[106].unk_03 = 0x80;
    func_8004F984(1, ((DisplayActor44 *)D_8011ECD0)->unk_26, ((DisplayActor44 *)D_8011ECD0)->unk_2A);
    func_8004FC10(1);
    func_801320C0();
    sel = D_800E6280.unk_1094;
    for (i = 0, j = 0; i < 4; i++, j += 2) {
        if (i == sel) {
            ((DisplayActor44 *)D_80120650)[i + 11].unk_18 = j + 0x33;
        } else {
            ((DisplayActor44 *)D_80120650)[i + 11].unk_18 = j + 0x34;
        }
    }
    if (D_800E6280.unk_F88 & 0x20) {
        switch (sel) {
        case 0:
            if ((D_800E7D4F & 1) || (D_800E7D50 & 1)) {
                func_80042940(0xA);
                func_80044750(0x501);
            }
            break;
        case 1:
            if (func_80136F3C() != 0) {
                func_80042940(0x14);
                func_80044750(0x501);
            }
            break;
        case 2:
            if (func_80136F3C() != 0) {
                func_80042940(0x1E);
                func_80044750(0x501);
            }
            break;
        case 3:
            if (D_800E7D37 != 0) {
                func_80042940(0x28);
                func_80044750(0x501);
            }
            break;
        }
    }
    if (D_800E6280.unk_F88 & 0x40) {
        func_8004284C();
        D_800E6280.unk_110D = 2;
    }
}

void func_801371FC(void) {
    s32 i;

    if (D_800E6280.unk_110D == 0) {
        /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
        for (i = 0; i < 4; i++) ((u8 *)D_80120912)[0x2D + i * 0x44] &= ~0x80;
        D_80120912[0] = D_80120912[0] / 9 * 8;
        if (D_80120912[0] < 9) {
            D_800E6280.unk_110D = 1;
        }
    } else if (D_800E6280.unk_110D == 2) {
        /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
        for (i = 0; i < 4; i++) ((u8 *)D_80120912)[0x2D + i * 0x44] &= 0xFF7F;
        D_80120912[0] = D_80120912[0] * 9 / 8;
        D_80120912[1] = D_80120912[1] * 9 / 8;
        if (D_80120912[0] >= 0x4000) {
            D_800E6280.unk_110D = 1;
        }
    } else {
        func_80042908(0);
        func_80042940(2);
    }
    func_80132384();
    D_801208FB = 0;
}

/* FAKE: s32 without an explicit return plus the z/p pointer read before the last stores sets $v0 = 0 (as in func_801378DC, T-24087). T-24119 */
s32 func_801373C8(void) {
    s32 i;
    s32 j;
    s32 *p;
    s32 z;
    s16 a[6];
    s16 b[6];
    s16 c[6];
    s16 d[6];

    if (D_800E6280.unk_110D != 0) {
        D_80121726 = D_80121726 / 17 * 16;
        D_80121728 = D_80121728 / 17 * 16;
        if (D_80121726 <= 0x1000) {
            if (D_80121726) {} /* FAKE: empty reads promote D_80121726 and D_80121728 to $t1 and $t3 across the zoom clamp (T-24087). T-24119 */
            if (D_80121728) {}
            D_80121726 = 0x1000;
            D_80121728 = 0x1000;
            for (i = 0; i < 2; i++) {
                a[i] = -0x50;
                b[i] = i * 0x10 + 8;
                c[i] = 0xC4;
                d[i] = 0x10;
                j = i * 2 + 0x3C;
                /* FAKE: actor stores on one line so as1 schedules lbu unk_03 before sh unk_18 (T-7020). T-24119 */
                ((DisplayActor44 *)D_80120650)[i + 15].unk_18 = j; ((DisplayActor44 *)D_80120650)[i + 15].unk_26 = a[i]; ((DisplayActor44 *)D_80120650)[i + 15].unk_2A = b[i]; ((DisplayActor44 *)D_80120650)[i + 15].unk_03 |= 0x80;
            }
            ((DisplayActor44 *)D_80120650)[15].unk_03 = 0;
            ((DisplayActor44 *)D_80120650)[16].unk_03 = 0;
            if (D_800B3220 == 0) {
                if (!(D_800E7D4F & 1)) {
                    c[0] = 0;
                }
                /* FAKE: word load view reproduces the original lw $t9, D_800E7D50 before masking bit 0. T-24119 */
                if (!(*(s32 *)&D_800E7D50 & 1)) {
                    c[1] = 0;
                }
            }
            menu_set(1, 2, a, b, c, d);
        }
    }
    if (D_800E6280.unk_110D == 0) {
        ((DisplayActor44 *)D_8011ECD0)[159].unk_18 = 7;
        ((DisplayActor44 *)D_8011ECD0)[159].unk_26 = -0xA0;
        ((DisplayActor44 *)D_8011ECD0)[159].unk_2A = -0x78;
        /* FAKE: folded s16 casts advance ugen's 5-register temp ring back to $t5 without extra reads of unk_110D. T-24119 */
        i = D_800E6280.unk_110D;
        i = (s16)i;
        i = (s16)i;
        i = (s16)i;
        i = (s16)i;
        D_800E6280.unk_110D = i + 1;
        if (0) {}
    } else if (D_800E6280.unk_110D == 1) {
        /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
        for (i = 0; i < 4; i++) ((u8 *)D_80120912)[0x2D + i * 0x44] &= ~0x80;
        D_80120912[0] = D_80120912[0] / 9 * 8;
        if (D_80120912[0] < 9) {
            D_800E6280.unk_110D = 2;
        }
    } else {
        func_8004284C();
    }
    ((DisplayActor44 *)D_8011ECD0)[106].unk_03 = 0;
    ((DisplayActor44 *)D_8011ECD0)[159].unk_03 = 0;
    z = 0;
    p = &z;
    D_80121726 = 0x1000;
    if (*p) {}
    D_80121728 = 0x1000;
    if (0) {}
}

/* FAKE: s32 without an explicit return retains `or $v0, $a2, $zero` in the switch delay slot. T-24117 */
s32 func_801376A8(void) {
    s32 i;
    s32 j;
    GameState *g;

    /* one table: as1 keeps the loads after the stores only when both go through D_8011ECD0 (T-9020, T-16930) */
    ((DisplayActor44 *)D_8011ECD0)[159].unk_03 = 0x80;
    ((DisplayActor44 *)D_8011ECD0)[159].unk_1A = 0x1000;
    ((DisplayActor44 *)D_8011ECD0)[159].unk_1C = 0x1000;
    menu_check(1, ((DisplayActor44 *)D_8011ECD0)->unk_26, ((DisplayActor44 *)D_8011ECD0)->unk_2A);
    func_8004FC10(1);
    func_801320C0();
    if ((D_800E7D4F & 1) == 1) {
        ((DisplayActor44 *)D_8011ECD0)[111].unk_03 = 0x80;
    }
    /* FAKE: 1U matches u8 D_800E7D4F's -cckr unsigned promotion so uopt CSEs constant 1 into $t1. T-24117 */
    if ((D_800E7D50 & 1U) == 1) {
        ((DisplayActor44 *)D_8011ECD0)[112].unk_03 = 0x80;
    }
    g = &D_800E6280;
    for (i = 0, j = 0; i < 2; i++, j += 2) {
        if (i == g->unk_1094) {
            ((DisplayActor44 *)D_80120650)[i + 15].unk_18 = j + 0x3B;
        } else {
            ((DisplayActor44 *)D_80120650)[i + 15].unk_18 = j + 0x3C;
        }
    }
    if (D_800E6280.unk_F88 & 0x20) {
        switch (g->unk_1094) {
        case 0:
            if ((D_800E7D4F & 1) == 1) {
                /* FAKE: Work800E7D10 view prevents uopt from CSE-ing the byte load with D_800E7D4F across the loop (as in 800674B0.c). T-24117 */
                D_800E7D10.unk_027[0x18] &= 0xFFFD;
                func_80044750(0x501);
                func_80048EB8(0);
                func_8004E58C();
                main_state_set(0xD0);
            }
            break;
        case 1:
            if ((D_800E7D50 & 1U) == 1) {
                /* FAKE: Work800E7D10 view emits byte load-modify-store without CSE-ing D_800E7D50. T-24117 */
                D_800E7D10.unk_027[0x19] &= 0xFFFD;
                func_80044750(0x501);
                func_80048EB8(0);
                func_8004E58C();
                main_state_set(0x90);
            }
            break;
        }
    } else if (D_800E6280.unk_F88 & 0x40) {
        func_8004284C();
    }
}

void func_80137864(void) {
    s32 i;
    s16 m;

    D_8012170F &= (m = 0xFF7F); /* FAKE: keeps the mask in $a0 for the loop's `and`, as the original; real source unknown. T-9340 */
    for (i = 0; i < 2; i++) {
        ((DisplayActor44 *)D_80120650)[i + 15].unk_03 &= ~0x80;
    }
    func_80042940(0);
    D_8012170F = 0;
    D_80121726 = 0x1000;
    D_80121728 = 0x1000;
}

/* FAKE: s32 without an explicit return plus the z/p pointer read before the last stores sets $v0 = 0 and puts sh $t3 in the jr delay slot (as in func_80042940, T-9540/T-11610). T-24087 */
s32 func_801378DC(void) {
    s32 i;
    s32 icon;
    s32 *p;
    s32 z;
    s16 a[16];
    s16 b[16];
    s16 c[16];
    s16 d[16];

    if (D_800E6280.unk_110D != 0) {
        D_80121726 = D_80121726 / 17 * 16;
        D_80121728 = D_80121728 / 17 * 16;
        if (D_80121726 <= 0x1000) {
            if (D_80121728) {} /* FAKE: empty read promotes D_80121728 to $t3 across the zoom clamp as in the original. T-24087 */
            D_80121726 = 0x1000;
            D_80121728 = 0x1000;
            for (i = 0; i < 14; i++) {
                a[i] = (i / 7) * 128 - 0x78;
                b[i] = (i % 7) * 0x12 - 0x48;
                c[i] = 0x80;
                d[i] = 0x10;
                /* FAKE: comma evaluation enters icon and &D_800E7D10.flags[i] into uopt before D_80120650 so the $t1 increment precedes $v1. T-24087 */
                ((DisplayActor44 *)D_80120650)[i + 15].unk_18 = (icon = i * 2 + 0x40, p = (s32 *)&D_800E7D10.flags[i], icon);
                ((DisplayActor44 *)D_80120650)[i + 15].unk_03 = 0;
                ((DisplayActor44 *)D_80120650)[i + 15].unk_26 = a[i];
                ((DisplayActor44 *)D_80120650)[i + 15].unk_2A = b[i];
                if (D_800E7D10.flags[i] == 0 && D_800E6280.unk_53C[i] == 0) {
                    c[i] = 0;
                }
            }
            func_8004F870(1, 14, a, b, c, d);
        }
    }
    if (D_800E6280.unk_110D == 0) {
        ((DisplayActor44 *)D_8011ECD0)[159].unk_18 = 8;
        ((DisplayActor44 *)D_8011ECD0)[159].unk_26 = -0xA0;
        ((DisplayActor44 *)D_8011ECD0)[159].unk_2A = -0x78;
        D_800E6280.unk_110D++;
    } else if (D_800E6280.unk_110D == 1) {
        /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
        for (i = 0; i < 4; i++) ((u8 *)D_80120912)[0x2D + i * 0x44] &= ~0x80;
        D_80120912[1] = D_80120912[1] / 9 * 8;
        if (D_80120912[1] < 9) {
            D_800E6280.unk_110D = 2;
        }
    } else {
        func_8004E93C(6, 0);
        func_8004E788(-0x80, -0x58, 0, (s32)"フリートーク", 0);
        func_8004EAD4(1);
        func_8004284C();
    }
    ((DisplayActor44 *)D_8011ECD0)[106].unk_03 = 0;
    ((DisplayActor44 *)D_8011ECD0)[159].unk_03 = 0;
    z = 0;
    p = &z;
    D_80121726 = 0x1000;
    if (*p) {}
    D_80121728 = 0x1000;
}

void func_80137C1C(void) {
    GameState *g = &D_800E6280;
    u32 *e;
    s32 sel;
    s32 i;
    s32 j;

    for (i = 0; i < 14; i++) {
        if (D_800E7D10.flags[i] != 0 || g->unk_53C[i] != 0) {
            ((DisplayActor44 *)D_80120650)[i + 15].unk_03 = 0x80;
        }
    }
    /* one table: as1 keeps the loads after the stores only when both go through D_8011ECD0 (T-9020, T-16930) */
    ((DisplayActor44 *)D_8011ECD0)[159].unk_03 = 0x80;
    ((DisplayActor44 *)D_8011ECD0)[159].unk_1A = 0x1000;
    ((DisplayActor44 *)D_8011ECD0)[159].unk_1C = 0x1000;
    func_8004F984(1, ((DisplayActor44 *)D_8011ECD0)->unk_26, ((DisplayActor44 *)D_8011ECD0)->unk_2A);
    func_8004FC10(1);
    for (i = 0, j = 0; i < 14; i++, j += 2) {
        if (i == g->unk_1094) {
            ((DisplayActor44 *)D_80120650)[i + 15].unk_18 = j + 0x3F;
        } else {
            ((DisplayActor44 *)D_80120650)[i + 15].unk_18 = j + 0x40;
        }
    }
    if (g->unk_F88 & 0x40) {
        func_80044750(0x74);
        g->unk_1094 = -1;
        func_80042940((g->unk_110A + 2) & 0xFF);
    } else if (g->unk_F88 & 0x20) {
        sel = g->unk_1094;
        if (D_800E7D10.flags[sel] != 0 || g->unk_53C[sel] != 0) {
            if ((func_800460EC() & 2) && sel >= 0 && sel < 14) {
                e = D_8013D2C4[sel];
                if (e[0] != 0) {
                    D_801230D0 = 1;
                    D_8013D2FC = 0;
                    func_80046290(e[1], e[2], sel);
                    func_80044750(0x300);
                    func_8004284C();
                }
            }
        } else {
            func_80044750(0x74);
        }
    }
    func_801320C0();
}

void func_80137E38(void) {
    u32 **e;
    s32 sel;

    sel = D_800E6280.unk_1094;
    if (D_8013D2FC == 0) {
        if (func_800460EC() & 4) {
            D_8013D2FC = 1;
        }
    }
    if (D_8013D2FC != 0) {
        e = &D_8013D2C4[sel];
        if (D_801230D0 < **e) {
            if (func_800460EC() & 2) {
                D_8013D2FC = 0;
                /* Word indexing preserves the original temporary allocation. T-12901 */
                func_80046290(((s32 *)*e)[D_801230D0 * 2 + 1], ((s32 *)*e)[D_801230D0 * 2 + 2], sel);
                D_801230D0 += 1;
                func_80044750(0x300);
            }
        } else if (func_800460EC() & 2) {
            D_801230D0 = 0;
            func_80042940((D_800E6280.unk_110A - 1) & 0xFF);
        }
    }
    if (D_800E6280.unk_F88 & 0x40) {
        D_801230D0 = 0;
        func_80044750(0x74);
        func_80042940((D_800E6280.unk_110A - 1) & 0xFF);
    }
    func_801320C0();
}

void func_80137FA0(void) {
    s32 i;

    func_80044750(0x74);
    func_8004EAAC();
    func_8004E93C(6, 1);
    D_801206D8[0x1037] &= ~0x80;
    D_801206D8[0x377] &= ~0x80;
    D_801206D8[0x3BB] &= ~0x80;
    /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
    for (i = 0; i < 12; i++) D_801206D8[0x3FF + i * 0x44] &= ~0x80;
    func_80042940(0);
    D_801206D8[0x1037] = 0;
    D_80121726 = 0x1000;
    D_80121728 = 0x1000;
}

/* FAKE: s32 without an explicit return plus the z/p pointer read before the last stores sets $v0 = 0 and puts sh $t3 in the jr delay slot; pad shifts a..d down by 4 bytes within the 0xE8 frame. T-24087 */
s32 func_80138080(void) {
    s32 i;
    s32 icon;
    s32 *p;
    s32 z;
    s32 pad;
    s16 a[16];
    s16 b[16];
    s16 c[16];
    s16 d[16];

    if (D_800E6280.unk_110D == 1) {
        D_80121726 = D_80121726 / 17 * 16;
        D_80121728 = D_80121728 / 17 * 16;
        if (D_80121726 <= 0x1000) {
            if (D_80121728) {} /* FAKE: empty read promotes D_80121728 to $t3 across the zoom clamp as in the original. T-24087 */
            D_80121726 = 0x1000;
            D_80121728 = 0x1000;
            for (i = 0; i < 14; i++) {
                a[i] = (i / 7) * 128 - 0x78;
                b[i] = (i % 7) * 0x12 - 0x48;
                c[i] = 0x50;
                d[i] = 0x10;
                /* FAKE: comma evaluation enters icon and &D_800E7D10.flags[i] into uopt before D_80120650 so the $t1 increment precedes $v1. T-24087 */
                ((DisplayActor44 *)D_80120650)[i + 15].unk_18 = (icon = i * 2 + 0x5C, p = (s32 *)&D_800E7D10.flags[i], icon);
                ((DisplayActor44 *)D_80120650)[i + 15].unk_03 = 0;
                ((DisplayActor44 *)D_80120650)[i + 15].unk_26 = a[i];
                ((DisplayActor44 *)D_80120650)[i + 15].unk_2A = b[i];
                if (D_800E7D10.flags[i] == 0 && D_800E6280.unk_53C[i] == 0) {
                    c[i] = 0;
                }
            }
            func_8004F870(1, 14, a, b, c, d);
        }
    }
    if (D_800E6280.unk_110D == 0) {
        ((DisplayActor44 *)D_8011ECD0)[159].unk_18 = 8;
        ((DisplayActor44 *)D_8011ECD0)[159].unk_26 = -0xA0;
        ((DisplayActor44 *)D_8011ECD0)[159].unk_2A = -0x78;
        D_800E6280.unk_110D++;
    } else if (D_800E6280.unk_110D == 1) {
        /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
        for (i = 0; i < 4; i++) ((u8 *)D_80120912)[0x2D + i * 0x44] &= ~0x80;
        D_80120912[1] = D_80120912[1] / 9 * 8;
        if (D_80120912[1] < 9) {
            D_800E6280.unk_110D = 2;
        }
    } else {
        func_8004E93C(6, 0);
        func_8004E788(-0x80, -0x58, 0, (s32)"エンディング", 0);
        func_8004EAD4(1);
        func_8004284C();
    }
    ((DisplayActor44 *)D_8011ECD0)[106].unk_03 = 0;
    ((DisplayActor44 *)D_8011ECD0)[159].unk_03 = 0;
    z = 0;
    p = &z;
    D_80121726 = 0x1000;
    if (*p) {}
    D_80121728 = 0x1000;
}

void func_801383C4(void) {
    GameState *g = &D_800E6280;
    s32 i;
    s32 j;
    s32 sel;

    for (i = 0; i < 14; i++) {
        if (D_800E7D10.flags[i] != 0 || g->unk_53C[i] != 0) {
            ((DisplayActor44 *)D_80120650)[i + 15].unk_03 = 0x80;
        }
    }
    /* one table: as1 keeps the loads after the stores only when both go through D_8011ECD0 (T-9020, T-16930) */
    ((DisplayActor44 *)D_8011ECD0)[159].unk_03 = 0x80;
    ((DisplayActor44 *)D_8011ECD0)[159].unk_1A = 0x1000;
    ((DisplayActor44 *)D_8011ECD0)[159].unk_1C = 0x1000;
    func_8004F984(1, ((DisplayActor44 *)D_8011ECD0)->unk_26, ((DisplayActor44 *)D_8011ECD0)->unk_2A);
    func_8004FC10(1);
    func_801320C0();
    for (i = 0, j = 0; i < 14; i++, j += 2) {
        if (i == g->unk_1094) {
            ((DisplayActor44 *)D_80120650)[i + 15].unk_18 = j + 0x5B;
        } else {
            ((DisplayActor44 *)D_80120650)[i + 15].unk_18 = j + 0x5C;
        }
    }
    if (g->unk_F88 & 0x20) {
        sel = g->unk_1094;
        if (D_800E7D10.flags[sel] != 0 || g->unk_53C[sel] != 0) {
            if (sel >= 0 && sel < 14) {
                g->unk_69C[0].unk_01 = sel;
                g->unk_71C = 0;
                g->unk_F5F = sel;
                if (g->unk_F5F < 0xD) {
                    func_80048EB8(0);
                    func_8004E58C();
                    func_80044890(1, 0xBF98, 0xBF79, 0xCE6D, 0xCE33, 0xCE1E);
                    func_80044750(0x501);
                    func_80042940(0x21);
                } else {
                    func_80048EB8(0);
                    func_8004E58C();
                    func_80044750(0x501);
                    func_80042940(0x23);
                }
            }
        }
    }
    if (g->unk_F88 & 0x40) {
        g->unk_1094 = -1;
        func_80042940((g->unk_110A + 1) & 0xFF);
    }
}

void func_801385F0(void) {
    s32 i;

    func_8004E93C(6, 1);
    func_8004EAAC();
    D_801206D8[0x1037] &= ~0x80;
    D_801206D8[0x377] &= ~0x80;
    D_801206D8[0x3BB] &= ~0x80;
    /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
    for (i = 0; i < 12; i++) D_801206D8[0x3FF + i * 0x44] &= ~0x80;
    func_80042940(0);
    D_801206D8[0x1037] = 0;
    D_80121726 = 0x1000;
    D_80121728 = 0x1000;
}

void func_801386C8(void) {
    func_80048EB8(0);
    D_800E6280.unk_F5F = D_800E7D68;
    main_state_set(0x91);
    func_801320C0();
}

void func_80138708(void) {
    s32 temp_v0;
    u8 temp;

    if ((u8) D_800E6280.unk_03A >= 3U) {
        D_800E6280.unk_03A -= 2;
    } else {
        func_80048E78();
        func_80041584();
    }
    temp_v0 = D_801220E0 - 2;
    temp = (u8) D_801220E0;
    if (temp >= 3U) {
        D_801220E2 = temp_v0;
        D_801220E1 = temp_v0;
        D_801220E0 = temp_v0;
    }
    if (func_80044E8C() == 1) {
        cd_read_sectors(0x11U, 0x801F0000, 0xAE07);
        func_8004284C();
    }
}

void func_801387A8(void) {
    if (cd_read_sync_status() & 1) {
        main_state_set(0x63);
        func_80042908(3);
    }
}

s32 func_801387E4(void) {
    if (D_800E6280.unk_03A >= 3U) {
        D_800E6280.unk_03A -= 2;
    } else {
        func_80048E78();
        func_80041584();
        main_state_set(0x63);
        func_80042908(7);
        func_80042940(0xF);
    }
}

s32 func_8013884C() {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80136C30();
        return;
    case 1:
        func_80137040();
        return;
    case 2:
        func_801371FC();
        return;
    case 10:
        func_801373C8();
        return;
    case 11:
        func_801376A8();
        return;
    case 12:
        func_80137864();
        return;
    case 20:
        func_801378DC();
        return;
    case 21:
        func_80137C1C();
        return;
    case 22:
        func_80137E38();
        return;
    case 23:
        func_80137FA0();
        return;
    case 30:
        func_80138080();
        return;
    case 31:
        func_801383C4();
        return;
    case 32:
        func_801385F0();
        return;
    case 33:
        func_80138708();
        return;
    case 34:
        func_801387A8();
        return;
    case 35:
        func_801387E4();
        return;
    case 40:
        func_801386C8();
        /* fallthrough */
    default:
        return;
    }
}
