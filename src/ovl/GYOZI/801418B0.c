#include "common.h"
#include "game.h"
#include "ovl/GYOZI.h"
#include "psx_types.h"
/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
INCLUDE_RODATA("asm/ovl/GYOZI/data/GYOZI/801418B0.data", D_801487F0);
INCLUDE_RODATA("asm/ovl/GYOZI/data/GYOZI/801418B0.data", D_801487F4);
INCLUDE_RODATA("asm/ovl/GYOZI/data/GYOZI/801418B0.data", D_801487F8);
INCLUDE_RODATA("asm/ovl/GYOZI/data/GYOZI/801418B0.data", D_801487FC);
INCLUDE_RODATA("asm/ovl/GYOZI/data/GYOZI/801418B0.data", D_80148800);
INCLUDE_RODATA("asm/ovl/GYOZI/data/GYOZI/801418B0.data", D_80148804);
INCLUDE_RODATA("asm/ovl/GYOZI/data/GYOZI/801418B0.data", D_80148808);
INCLUDE_RODATA("asm/ovl/GYOZI/data/GYOZI/801418B0.data", D_8014880C);
INCLUDE_RODATA("asm/ovl/GYOZI/data/GYOZI/801418B0.data", D_80148810);
INCLUDE_RODATA("asm/ovl/GYOZI/data/GYOZI/801418B0.data", D_80148814);
INCLUDE_RODATA("asm/ovl/GYOZI/data/GYOZI/801418B0.data", D_80148818);
INCLUDE_RODATA("asm/ovl/GYOZI/data/GYOZI/801418B0.data", D_8014881C);
INCLUDE_RODATA("asm/ovl/GYOZI/data/GYOZI/801418B0.data", D_80148820);
INCLUDE_RODATA("asm/ovl/GYOZI/data/GYOZI/801418B0.data", D_80148824);
u8 D_80148828[4] = { 0 };
u32 D_8014882C[6][2] = {
    { 0x00F870A0, 0x00F8A0C0 },
    { 0x0030C050, 0x0070E090 },
    { 0x005078F8, 0x0098C0F8 },
    { 0x00F8A828, 0x00F8D868 },
    { 0x00F88030, 0x00F8D078 },
    { 0x005850B0, 0x009098E8 },
};
u8 D_8014885C[36] = {
    0, 1, 2, 3, 4, 3, 2, 4, 4, 1, 0, 2,
    1, 2, 3, 0, 1, 0, 2, 4, 1, 3, 0, 3,
    2, 3, 0, 1, 0, 1, 4, 2, 3, 0, 5, 1,
};

void func_801418B0(void) {
    D_801487F0 = (u8 *)0x801D6360;
    D_801487F4 = (u8 *)0x801D6384;
    D_801487F8 = (u8 *)0x801D6414;
    D_801487FC = *(s16 *)0x801D641C;
    D_80148800 = (u8 *)0x801B0000;
}

void func_80141900(void) {
    D_80148804 = 0x801DDBB4;
    D_80148808 = 0x801DDBB8;
    D_8014880C = 0x801DDC70;
    D_80148814 = 0x801DB000;
}

/* Return type preserves the original fall-through schedule. T-20006. */
s32 func_80141944(void) {
    s32 idx; /* Index precedes the copied table (T-3330, T-20006). */
    GzCallbacks68 table;

    table = D_80148880;
    idx = D_800F647A;
    table.callbacks[idx](0x80);
    if (D_800F647A < 8) {
        D_8012B8C0[0].unk_07 = D_801317EB;
        if (D_8012B8C0[0].unk_18 == 0 && D_8012B8C0[0].unk_08 == 0) {
            if (++D_8012B8C0[0].unk_16 >= 3) {
                D_8012B8C0[0].unk_16 = 0;
            }
        }
    }
    if (D_800F647A >= 0xA && D_80148820 != 0) {
        func_801428EC();
    }
    if (D_800F647A >= 0xA && D_80148824 != 0) {
        func_80142C14();
    }
}

void func_80141A94(void) {
    func_8004C6A0(0, 0x40);
    func_80050D60(0, 0);
    func_80081D30(1);
}

void func_80141ACC(void) {
    s32 unused; /* FAKE: restore observed implicit-copy offsets. T-13202. */
    s16 i;

    D_8014881C = 0;
    D_80148820 = 0;
    D_80148824 = 0;
    func_801418B0();
    i = 0;
    do {
        func_8004EE18(D_80148800, (i + 0x10) & 0xFF, 1, 2, i);
        i++;
    } while (i < 9);
    func_800549E8(0x60);
    ((DisplayActor44 *)D_80129F40)[96].unk_01 = 8;
    ((DisplayActor44 *)D_80129F40)[96].unk_02 = 0x40;
    ((DisplayActor44 *)D_80129F40)[96].unk_38 = 0x01000000;
    ((DisplayActor44 *)D_80129F40)[96].unk_03 = 4;
    ((DisplayActor44 *)D_80129F40)[96].unk_0C = D_801487F4;
    ((DisplayActor44 *)D_80129F40)[96].unk_10 = D_801487F8;
    ((DisplayActor44 *)D_80129F40)[96].unk_34 = D_801487F0;
    ((DisplayActor44 *)D_80129F40)[96].unk_14 = D_801487FC;
    ((DisplayActor44 *)D_80129F40)[96].unk_16 = 0;
    ((DisplayActor44 *)D_80129F40)[96].unk_18 = 0;
    ((DisplayActor44 *)D_80129F40)[96].unk_06 = 1;
    ((DisplayActor44 *)D_80129F40)[96].unk_43 = 0;
    ((DisplayActor44 *)D_80129F40)[96].unk_26 = 0;
    ((DisplayActor44 *)D_80129F40)[96].unk_2A = 8;
    ((DisplayActor44 *)D_80129F40)[96].unk_07 = 0x80;
    ((DisplayActor44 *)D_80129F40)[96].unk_05 = 0x80;
    ((DisplayActor44 *)D_80129F40)[96].unk_04 = 0;
    ((DisplayActor44 *)D_80129F40)[97] = ((DisplayActor44 *)D_80129F40)[96];
    func_80141900();
    func_8004EE18((u8 *) D_80148814, 0x1F, 1, 2, 0);
    ((DisplayActor44 *)D_80129F40)[98] = ((DisplayActor44 *)D_80129F40)[96];
    ((DisplayActor44 *)D_80129F40)[98].unk_01 = 0xB;
    ((DisplayActor44 *)D_80129F40)[98].unk_04 = 8;
    ((DisplayActor44 *)D_80129F40)[98].unk_02 = 1;
    ((DisplayActor44 *)D_80129F40)[98].unk_0C = D_80148808;
    ((DisplayActor44 *)D_80129F40)[98].unk_10 = D_8014880C;
    ((DisplayActor44 *)D_80129F40)[98].unk_34 = D_80148804;
    ((DisplayActor44 *)D_80129F40)[98].unk_14 = D_80148810;
    ((DisplayActor44 *)D_80129F40)[98].unk_16 = (s16) (func_800BCE40() % 3);
    ((DisplayActor44 *)D_80129F40)[98].unk_18 = 0;
    ((DisplayActor44 *)D_80129F40)[98].unk_26 = -0xA0;
    ((DisplayActor44 *)D_80129F40)[98].unk_2A = -0x78;
    ((DisplayActor44 *)D_80129F40)[98].unk_43 = 0x1F;
    ((DisplayActor44 *)D_80129F40)[101] = ((DisplayActor44 *)D_80129F40)[99] = ((DisplayActor44 *)D_80129F40)[100] = ((DisplayActor44 *)D_80129F40)[98];
    ((DisplayActor44 *)D_80129F40)[99].unk_26 += 0x48;
    ((DisplayActor44 *)D_80129F40)[99].unk_16 = (s16) (func_800BCE40() % 3);
    ((DisplayActor44 *)D_80129F40)[99].unk_18 = 4;
    ((DisplayActor44 *)D_80129F40)[100].unk_26 += 0x90;
    ((DisplayActor44 *)D_80129F40)[100].unk_16 = (s16) (func_800BCE40() % 3);
    ((DisplayActor44 *)D_80129F40)[100].unk_18 = 0xA;
    ((DisplayActor44 *)D_80129F40)[101].unk_01 = 0xA;
    ((DisplayActor44 *)D_80129F40)[101].unk_02 = 0x40;
    ((DisplayActor44 *)D_80129F40)[101].unk_16 = 0;
    D_80148620 = 0;
    D_80148614 = D_801481F8;
    D_80148618 = D_8014822C;
    D_8014861C = D_80148260;
    D_800F53A0.girl[0].unk_0A += 1;
    D_800F53A0.girl[D_80148754].unk_0A += 1;
    D_800F53A0.girl[D_80148758].unk_0A += 1;
    func_8008FB00();
    func_8004DE1C();
}

void func_8014206C(void) {
    /* Signed local preserves the original compare operand order. T-20005. */
    s32 selector = D_801486E4;
    if (selector && selector != 7) {
        ((DisplayActor44 *)D_80129F40)[98].unk_03 |= 0x80;
    }
    if (selector && selector != 7) {
        ((DisplayActor44 *)D_80129F40)[99].unk_03 |= 0x80;
    }
    if (selector && selector != 7) {
        ((DisplayActor44 *)D_80129F40)[100].unk_03 |= 0x80;
    }
    ((DisplayActor44 *)D_80129F40)[101].unk_03 |= 0x80;
    D_80148824 = 1;
    func_8004DE1C();
}

void func_8014210C(void) {
    s16 a[4];
    s16 b[4];
    s16 c[4];
    s16 d[4];

    a[0] = -0x96;
    a[1] = -0x48;
    a[2] = 6;
    a[3] = 0x54;
    b[0] = b[1] = b[2] = b[3] = -0x40;
    c[0] = c[1] = c[2] = c[3] = 0x40;
    d[0] = d[1] = d[2] = d[3] = 0x40;
    func_8005BD20(0, 4, a, b, c, d);
    D_8012E6C0 = 1;
    func_8004DE1C();
}

#ifdef NON_MATCHING
/* NON_MATCHING: score=54 (268/272 insns). First 231 instructions match 100% with the .data island; in the bottom condition block IDO 5.3 uopt CSEs (g->unk_03E == 0x5F) into $a1 instead of keeping 0x5F in $a1 via RLDC. T-24101. */
s32 func_801421B4(void) {
    s16 i;
    GyoziWork *g;

    g = &D_800F53A0;
    D_80148828[0] = g->unk_10C8 % 4;
    i = 1;
    while (1) {
        D_80148828[1] = (g->unk_10C8 * 2 + i) % 4;
        if (D_80148828[0] != D_80148828[1]) {
            break;
        }
        i++;
    }
    while (1) {
        D_80148828[2] = (g->unk_10C8 / 10 + i) % 4;
        if (D_80148828[0] != D_80148828[2] && D_80148828[1] != D_80148828[2]) {
            break;
        }
        i++;
    }
    while (1) {
        D_80148828[3] = i % 4;
        if (D_80148828[0] != D_80148828[3] && D_80148828[1] != D_80148828[3] && D_80148828[2] != D_80148828[3]) {
            break;
        }
        i++;
    }
    for (i = 0; i < 4; i++) {
        if (D_80148828[i] == 1) {
            D_80148828[i] = 0xB;
        } else if (D_80148828[i] == 2) {
            D_80148828[i] = D_80148754;
        } else if (D_80148828[i] == 3) {
            D_80148828[i] = D_80148758;
        }
    }
    func_8005BE34(0, ((DisplayActor44 *)D_80129F40)[0].unk_26, ((DisplayActor44 *)D_80129F40)[0].unk_2A, D_80148828);
    func_8005C1B8(0);
    if (g->unk_F58 & 0x600060) {
        if (func_8005B32C() != 0) {
            if (g->unk_F60[0].unk_06 != 0) {
                D_80148818 = 0;
            } else if (g->unk_F60[2].unk_06 != 0) {
                D_80148818 = 1;
            } else if (g->unk_F60[4].unk_06 != 0) {
                D_80148818 = 2;
            } else if (g->unk_F60[6].unk_06 != 0) {
                D_80148818 = 3;
            } else {
                return 0;
            }
            ((DisplayActor44 *)D_80129F40)[0].unk_03 &= 0xFF7F;
            D_8012E6C0 = 0;
            func_8005B06C(1);
            ((DisplayActor44 *)D_80129F40)[97].unk_03 |= 0x80;
            ((DisplayActor44 *)D_80129F40)[97].unk_18 =
                D_80148828[D_80148818]
                + (g->unk_03E == 0x60) * 0xB
                + (g->unk_03E == 0x61) * 0x17
                - (D_80148828[D_80148818] == 0xB && g->unk_03E == 0x5F)
                + 1;
            ((DisplayActor44 *)D_80129F40)[97].unk_26 = D_80148818 * 0x4E - D_80148818 - 0x75;
            ((DisplayActor44 *)D_80129F40)[97].unk_2A = -0x3C;
            ((DisplayActor44 *)D_80129F40)[97].unk_1C = 0;
            D_80148620 += D_80148828[D_80148818] * 3 + g->unk_03E - 0x5F;
            D_80148820 = 1;
            if ((g->unk_03E == 0x5F && D_80148828[D_80148818] == 0 && g->unk_100.unk_02 >= 0x64)
                || ((D_80148828[D_80148818] == 0 & g->unk_03E == 0x5F) && g->unk_670 >= 0x64)
                || ((D_80148828[D_80148818] == 1 & g->unk_03E == 0x5F) && g->unk_0FA >= 0x46)
                || ((D_80148828[D_80148818] == 1 & g->unk_03E == 0x61) && g->unk_0FA >= 0x64)
                || ((D_80148828[D_80148818] == 2 & g->unk_03E == 0x60) && g->unk_0FA >= 0x64)) {
                D_80148624 = 1;
            }
            func_8004DE1C();
        }
    }
}
#else
INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/801418B0", func_801421B4);
#endif

void func_801425F4(void) {
    func_8008A0D4(0x4183);
    func_8004DE1C();
}

void func_8014261C(void) {
    s16 i;

    for (i = 0; i < 6; i++) {
        ((DisplayActor44 *)D_80129F40)[i + 96].unk_03 = 0;
    }
    D_80148820 = 0;
    D_80148824 = 0;
    func_8004DE1C();
}

s32 func_8014267C(void) {
    D_8012E6BC = 0;
    if (D_801486E4 == 0) {
        D_800F62CF = 0;
        D_8012E664 = 0;
        D_8012E660 = 7;
        func_8004DEAC(6);
        D_800F5902 += 1;
        return 0;
    }
    if (D_801486E4 == 7) {
        D_800F62CF = 7;
        D_8012E664 = 7;
        D_8012E660 = 7;
        func_8004DEAC(7);
        D_800F592F += 1;
        return 0;
    }
    if (D_801486E4 == 9) {
        D_800F62CF = 9;
        D_8012E664 = 9;
        D_8012E660 = 7;
        func_8004DEAC(8);
        D_800F593D += 1;
        return 0;
    }
    func_8004DE1C();
}

void func_80142790(void) {
    if (D_80148828[D_80148818] == 0xB) {
        func_80086AB0(0x200);
    } else {
        func_80086AB0(0x201);
    }
    func_8004DE1C();
}

void func_801427E4(void) {
    s16 v;

    v = D_8012B920 + 0x200;
    D_8012B920 = v;
    if (v >= 0x1000) {
        func_8004DE1C();
    }
}

void func_80142828(void) {
    func_80051DD8(0x4D, 0x801B0000, 0xB0C5);
    func_8004DE1C();
}

void func_80142858(void) {
    func_80051DD8(0xE, 0x801D7000, 0xB112);
    func_8004DE1C();
}

void func_8014288C(void) {
    D_80148624 = 0;
    if (D_80148828[D_80148818] == 0xB) {
        D_80148620 = 0x27;
    } else {
        D_80148620 = 0x26;
    }
    func_8004DE1C();
}

/* Return type preserves the existing caller declaration and original fall-through schedule. T-24101. */
s32 func_801428EC(void) {
    s32 unused; /* FAKE: unused local pads stack frame to 0x48 bytes. T-24101. */
    s16 dy;
    s16 dx;

    dy = 0x1C - (((DisplayActor44 *)D_80129F40)[97].unk_1C / 512) * 4;
    dx = 1;
    if (((DisplayActor44 *)D_80129F40)[97].unk_1C == 0x1000) {
        dy = 0;
        dx = 0;
    }
    func_80142A8C(
        ((DisplayActor44 *)D_80129F40)[97].unk_26 - dx - 0x1C,
        ((DisplayActor44 *)D_80129F40)[97].unk_2A + dy - 0x1C,
        ((DisplayActor44 *)D_80129F40)[97].unk_26 - 1,
        ((DisplayActor44 *)D_80129F40)[97].unk_2A + dy - 0x1C,
        ((DisplayActor44 *)D_80129F40)[97].unk_26 - dx - 0x1C,
        ((DisplayActor44 *)D_80129F40)[97].unk_2A - 1,
        D_8014882C[D_8014885C[(D_800F53DE - 0x5F) * 12 + D_80148828[D_80148818]]][0],
        D_8014882C[D_8014885C[(D_800F53DE - 0x5F) * 12 + D_80148828[D_80148818]]][1],
        9, 0);
    func_80142A8C(
        ((DisplayActor44 *)D_80129F40)[97].unk_26 + dx + 0x1C,
        ((DisplayActor44 *)D_80129F40)[97].unk_2A - dy + 0x1C,
        ((DisplayActor44 *)D_80129F40)[97].unk_26 + dx + 0x1C,
        ((DisplayActor44 *)D_80129F40)[97].unk_2A,
        ((DisplayActor44 *)D_80129F40)[97].unk_26,
        ((DisplayActor44 *)D_80129F40)[97].unk_2A - dy + 0x1C,
        D_8014882C[D_8014885C[(D_800F53DE - 0x5F) * 12 + D_80148828[D_80148818]]][0],
        D_8014882C[D_8014885C[(D_800F53DE - 0x5F) * 12 + D_80148828[D_80148818]]][1],
        9, 0);
    func_80054FCC(0xB);
}

/* Return type preserves the existing implicit caller ABI and original schedule. T-24019. */
s32 func_80142A8C(s32 x0, s32 y0, s32 x1, s32 y1, s32 x2, s32 y2, u32 c0, u32 c1, s32 z, s32 trans) {
    GyoziWork *g;
    POLY_G3 *poly;

    g = &D_800F53A0;
    poly = func_80054B60(0x1C, D_80129F10);
    func_800ABAB8(poly);
    func_800ABA38(poly, trans);
    /* setRGBn-style comma lines with masks: separate statements give other registers (T-20600, T-24019). */
    poly->r0 = (c0 >> 16) & 0xFF, poly->g0 = (c0 >> 8) & 0xFF, poly->b0 = c0 & 0xFF;
    poly->r1 = (c1 >> 16) & 0xFF, poly->g1 = (c1 >> 8) & 0xFF, poly->b1 = c1 & 0xFF;
    poly->r2 = (c1 >> 16) & 0xFF, poly->g2 = (c1 >> 8) & 0xFF, poly->b2 = c1 & 0xFF;
    poly->x0 = g->unk_014[D_80129F10] + x0;
    poly->y0 = g->unk_018[D_80129F10] + y0;
    poly->x1 = g->unk_014[D_80129F10] + x1;
    poly->y1 = g->unk_018[D_80129F10] + y1;
    poly->x2 = g->unk_014[D_80129F10] + x2;
    poly->y2 = g->unk_018[D_80129F10] + y2;
    func_800AB984(&D_800F7710[(D_80129F10 << 10) + z * 4], poly, poly);
}

void func_80142C14(void) {
    s16 i;

    for (i = 0; i < 3; i++) {
        if (((DisplayActor44 *)D_80129F40)[i + 0x62].unk_18 == 0xE
            && ((DisplayActor44 *)D_80129F40)[i + 0x62].unk_08 == 9) {
            ((DisplayActor44 *)D_80129F40)[i + 0x62].unk_16 = func_800BCE40() % 3;
        }
    }
}
