#include "common.h"
#include "ovl/TACO.h"

void func_80139170(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8013920C();
        return;
    case 1:
        func_80139AB0();
        return;
    case 2:
        func_80139BCC();
        return;
    case 3:
        func_80139B54();
        return;
    case 4:
        func_80139394();
        return;
    case 5:
        func_801393F4();
        return;
    }
}

void func_8013920C(void) {
    s32 previous;

    func_8013A764();
    func_8013AFDC();
    func_800450F4(0, 0x202);
    D_800E6280.unk_036 = 0;
    D_800E6280.unk_037 = 0;
    D_800E6280.unk_038 = 0;
    back_clear_switch(1);
    D_8015EDB0 = 0;
    /* FAKE: the scalar address view and shared source line retain the two
     * score-load registers and their order. Original spelling unknown. T-9530 */
    func_80059048(); previous = *(s32 *)&D_8015EDF8;
    if (previous < D_8015EDF0) {
        D_8015F4D8 = 0;
        D_8015EE00 = D_8015EDFC;
        D_8015EDF8 = D_8015EDF0;
        D_8015EDFC = previous;
        func_800AE090(D_8015EE0C, D_8015EE08, 4);
        func_800AE090(D_8015EE08, D_8015EE04, 4);
        func_800AE090(D_8015EE04, "   ", 4);
    } else if (D_8015EDFC < D_8015EDF0) {
        D_8015F4D8 = 1;
        D_8015EE00 = D_8015EDFC;
        D_8015EDFC = D_8015EDF0;
        func_800AE090(D_8015EE0C, D_8015EE08, 4);
        func_800AE090(D_8015EE08, "   ", 4);
    } else {
        D_8015F4D8 = 2;
        D_8015EE00 = D_8015EDF0;
        func_800AE090(D_8015EE0C, "   ", 4);
    }
    func_8004284C();
}

void func_80139394(void) {
    D_800E6280.unk_110D += 1;
    func_8013A790(0, 0);
    if ((u32)(D_800E6280.unk_110D * 3) >= 0x81U) {
        func_8004284C();
    }
}

void func_801393F4(void) {
    func_80042908(5);
    func_8004284C();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80139170", func_80139424);

void func_80139AB0(void) {
    if (D_8015F4F8 < 0x100) {
        D_8015F4F0 = D_8015F4F8 / 2;
    } else {
        D_8015F4F0 = 0x80;
    }
    func_8013A790(0, 0);
    func_80139FB8(0x41, 0, 0x280 - D_8015F4F8, 0);
    D_8015F4F8 += 4;
    if (D_8015F4F8 >= 0x1E1) {
        func_8004284C();
    }
}

void func_80139B54(void) {
    func_8013A790(0, 0);
    func_8013A580(D_8015F4F8);
    func_80139FB8(D_8015F4E0, 0, 0x280 - D_8015F4F8, 0);
    D_8015F4F8 -= 8;
    if (D_8015F4F8 < 0) {
        func_8004284C();
    }
}

/* NON_MATCHING: T-19021, 14 differing lines (7 rows); the pad-word range gets $a2 and `right` $v1 (original: pad $v1, `right` $a2), a priority tie
   (regdiff: the pad range is 0.4, the two button masks 0.5); everything else including the frame and slot matches. */
#ifdef NON_MATCHING
void func_80139BCC(void) {
    s32 temp_lo;
    s32 scale;
    s32 right;
    s32 left;

    if (D_8015F4E4 < 0) {
        scale = D_8015F4E4 + 0x50;
    } else {
        scale = 0x50 - D_8015F4E4;
    }
    func_80139FB8(D_8015F4E0, D_8015F4E4, 0xA0, 0);
    temp_lo = scale * 0x7000 / 80;
    func_80139424(D_8015F4B0[D_8015F4E0 % 40], 6, 4, 0x80, 0, 0x3C, temp_lo, temp_lo, 1);
    func_8013A580(0x1E0);
    func_8013A790(0, 0);
    right = D_800E6280.unk_F80 & 0x2000;
    left = D_800E6280.unk_F80 & 0x8000;
    if (right != 0 && D_8015F4E8 < 0) {
        D_8015F4E8 = 1;
    }
    if (left != 0 && D_8015F4E8 > 0) {
        D_8015F4E8 = -1;
    }
    switch (D_8015F4E8) {
    case -2:
        D_8015F4E4 += 0xA;
        break;
    case -1:
        D_8015F4E4 += 5;
        break;
    case 1:
        D_8015F4E4 -= 5;
        break;
    case 2:
        D_8015F4E4 -= 0xA;
        break;
    }
    if (D_8015F4E4 == 0) {
        if (!(D_800E6280.unk_F80 & 0xA000)) {
            D_8015F4E8 = 0;
        }
        if (right != 0) {
            D_8015F4E8 += 1;
        }
        if (left != 0) {
            D_8015F4E8 -= 1;
        }
        if (D_8015F4E8 >= 3) {
            D_8015F4E8 = 2;
        }
        if (D_8015F4E8 < -2) {
            D_8015F4E8 = -2;
        }
    }
    if (D_8015F4E4 >= 0x33) {
        D_8015F4E0 += 1;
        D_8015F4E4 = -0x32;
    }
    if (D_8015F4E4 < -0x32) {
        D_8015F4E4 = 0x32;
        D_8015F4E0 -= 1;
    }
    if (D_8015F4E0 >= 0x28) {
        D_8015F4E0 = 0;
    }
    if (D_8015F4E0 < 0) {
        D_8015F4E0 = 0x27;
    }
    if ((D_800E6280.unk_F88 & 0x20) && !(D_800E6280.unk_F88 & 0x40)) {
        if (D_8015F4E0 == 0x27) {
            func_800AE090((void *)D_8015EE04[D_8015F4D8 * 4 + D_8015F4DC], "", 1);
            func_8004284C();
        } else if (D_8015F4DC != 3) {
            func_800AE090(&D_8015EE04[D_8015F4D8 * 4 + D_8015F4DC], &D_8015F4B0[D_8015F4E0], 1);
            D_8015F4DC += 1;
        }
    }
    if ((D_800E6280.unk_F88 & 0x800) && !(D_800E6280.unk_F88 & 0x40)) {
        func_800AE090((void *)D_8015EE04[D_8015F4D8 * 4 + D_8015F4DC], "", 1);
        func_8004284C();
    }
    if ((D_800E6280.unk_F88 & 0x40) && D_8015F4DC != 0) {
        func_800AE090(&D_8015EE04[D_8015F4D8 * 4 + D_8015F4DC - 1], " ", 1);
        D_8015F4DC -= 1;
    }
}
#else
INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80139170", func_80139BCC);
#endif

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80139170", func_80139FB8);

/* The y offset stays an expression: a fourth named temporary adds a frame word that moves the four
   spilled call values down by 4 (T-19021). */
void func_8013A580(s32 arg0) {
    s32 k;
    s32 a;
    s32 b;
    s32 c;

    switch (D_8015F4D8) {
    case 0:
        k = 1;
        break;
    case 1:
        k = 4;
        break;
    case 2:
        k = 3;
        break;
    }
    a = (arg0 << 7) / 480;
    b = (arg0 * 0x7000) / 480;
    c = (b * 12) / 4096;
    func_80139424(D_8015EE04[D_8015F4D8 * 4], k, 3, a, -c, -0x28 - arg0 / 16, b, b, 0);
    func_80139424(D_8015EE04[D_8015F4D8 * 4 + 1], k, 3, a, 0, -0x28 - arg0 / 16, b, b, 0);
    func_80139424(D_8015EE04[D_8015F4D8 * 4 + 2], k, 3, a, c, -0x28 - arg0 / 16, b, b, 0);
}

void func_8013A764(void) {
    D_8015F4DC = 0;
    D_8015F4E0 = 0;
    D_8015F4E4 = 0;
    D_8015F4E8 = 0;
    D_8015F4F8 = 0;
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80139170", func_8013A790);

/* r_sh and g_sh are single-use named shifts: they reserve the two frame words above the four spilled values
   that the original has (T-19021). */
void func_8013AD64(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 r_sh;
    s32 g_sh;
    s32 r;
    s32 g;
    s32 b;
    s32 x;
    s32 y;
    u8 *p;

    r_sh = arg2 >> 16;
    g_sh = arg2 >> 8;
    r = r_sh & 0xFF;
    g = g_sh & 0xFF;
    b = arg2 & 0xFF;
    if (arg3 < 2) {
        if (arg1 < 0x80) {
            r = r * arg1 / 128;
            g = g * arg1 / 128;
            b = b * arg1 / 128;
        }
        r = r * D_8015F4F0 / 128;
        g = g * D_8015F4F0 / 128;
        b = b * D_8015F4F0 / 128;
    }
    p = func_800490F0(0xC, D_8011ECA0);
    func_8009F0B8(p);
    func_8009EF84(p, 0);
    p[4] = r;
    p[5] = g;
    p[6] = b;
    x = (arg1 * func_800A0070(arg0) >> 12) + D_800E6280.unk_014[D_8011ECA0];
    if (arg3 == 0) {
        y = (arg1 * func_800A0140(arg0) >> 12) + D_800E6280.unk_018[D_8011ECA0] - 0x28;
    } else {
        y = (arg1 * func_800A0140(arg0) >> 12) + D_800E6280.unk_018[D_8011ECA0];
    }
    *(s16 *)(p + 8) = x;
    *(s16 *)(p + 0xA) = y;
    if (arg3 < 2) {
        func_8009EED0((D_8011ECA0 << 10) + 0x3F8 + D_800E8CA0, p);
        return;
    }
    func_8009EED0((D_8011ECA0 << 10) + 0x40 + D_800E8CA0, p);
}

void func_8013AFDC(void) {
    s32 i;

    func_800AE120(D_800E6280.unk_10F4);
    D_8015F4EC = (TacoRnd *)D_800E6280.unk_163C;
    D_8015F4F0 = 0;
    D_8015F4F4 = 0;
    for (i = 0; i < 0x64; i++) {
        if (i < 0x50) {
            if (func_800AE0D0() % 5 < 4) {
                D_8015F4EC[i].a = func_800AE0D0() % 4096;
                D_8015F4EC[i].b = i * 1000 * i / 30;
            } else {
                D_8015F4EC[i].b = 0;
            }
        } else {
            D_8015F4EC[i].b = 0;
        }
    }
}
