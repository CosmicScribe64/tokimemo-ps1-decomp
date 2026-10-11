#include "common.h"
#include "ovl/EN_NICHI.h"
/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_80139200);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_80139204);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_80139208);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_8013920C);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_80139210);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_80139214);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_80139218);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_8013921C);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_80139220);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_80139224);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_80139228);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_80139278);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_801392A0);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_801392A4);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_801392A8);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_801392AC);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_8013930C);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_801393CC);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_8013984C);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_80139868);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_8013987C);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_801398AC);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_80139A2C);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_80139A34);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_80139AF4);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_80139AF8);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_80139AFC);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_80139B00);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_80139B04);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_80139B08);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_80139B14);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_80139B18);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_80139B1C);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_80139B20);
s32 D_80139B24 = 0;
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_80139B28);

void func_80132000(void) {
    if (D_800E6280.unk_03E == 0x5F) {
        func_801320C0();
    } else {
        func_80136B10();
    }
}

void func_80132040(void) {
    D_801391E0 = 0x801E0400;
    D_801391E4 = 0x801E20F8;
    D_801391E8 = 0x801E2120;
    D_801391EC = 0x801E21C4;
    D_801391F0 = *(s16 *)0x801E5A10;
    D_801391F4 = 0x801B0000;
    D_801391F8 = 0x801E21CC;
    D_801391FC = 0x801E0408;
}

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI/80132000", func_801320C0);

void func_8013216C(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_801321EC();
        return;
    case 1:
        func_80132308();
        return;
    case 2:
        func_80132378();
        return;
    default:
    case 3:
        func_80042808();
        return;
    }
}
void func_801321EC(void) {
    func_80085E30(1, 0);
    func_80044750(0x7F);
    func_8006BC28(0);
    func_8006BD6C(0);
    func_800AE120(D_800E6280.unk_10F4);
    hizuke_disp_switch(0);
    func_80065F34(0);
    message_disp_switch(0);
    func_8006764C(0);
    func_8004284C();
    D_80139B14 = D_800E6280.unk_F6F;
    D_800E6280.unk_F6F = 1;
}

void func_80132278(void) {
    func_80048390();
    func_80048E78();
    func_8006BC28(0);
    D_800E6280.unk_036 = 0;
    D_800E6280.unk_037 = 0;
    D_800E6280.unk_038 = 0;
    func_800438F0(1);
    D_800E6280.unk_F6F = (s8) D_80139B14;
    func_80042878((s32) D_800E6280.unk_720);
    func_80042908((s32) D_800E6280.unk_721);
    func_80042940((s32) D_800E6280.unk_722);
}

void func_80132308(void) {
    if (D_800E6280.unk_110D == 0) {
        func_80044750(0x201);
        func_80046318(0x3F, 0x801B0000, 0x7C8C);
        D_800E6280.unk_110D += 1;
    } else if (func_800460CC() & 1) {
        func_8004284C();
    }
}

void func_80132378(void) {
    load_palette(D_80139210, 0x10, 1, 1, 0);
    load_palette(D_80139210, 0x11, 1, 1, 1);
    load_palette(D_80139210, 0x12, 1, 1, 2);
    load_palette(D_80139210, 0x13, 1, 1, 3);
    load_palette(D_80139210, 0x1F, 1, 3, 0);
    load_palette(D_80139210, 0x1E, 1, 3, 1);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI/80132000", func_80132450);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI/80132000", func_8013275C);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI/80132000", func_80132834);

void func_80132A0C(void) {
    s32 v;

    func_80136A2C();
    v = D_80139B1C - 1;
    D_80139B1C = v;
    if (v < 0) {
        D_80139B1C = 0;
    }
    func_80132B40();
    func_8013556C();
    func_80132ADC();
    func_80132D44();
    func_801338EC();
}

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI/80132000", func_80132A74);

void func_80132ADC(void) {
    D_80139B24 += 1;
    if (D_80139B24 >= 0x36) {
        D_80139B24 = 0;
    }
    switch (D_80139B24) {
    case 0:
        D_8012158C = 0x20;
        break;
    case 0x1B:
        D_8012158C = 0x21;
        break;
    }
}

s32 func_80132B40(void) {
    s32 i;

    if (D_8013921C == 0x7C) {
        D_8013921C = 0x80;
        for (i = 0; i < 64; i++) {
            D_8011ECD0[0x1987 + i * 0x44] = 0x80;
        }
        D_800E6280.unk_036 = 0x40;
        D_800E6280.unk_037 = 0x40;
        D_800E6280.unk_038 = 0x80;
        return;
    }
    if (D_8013921C < 0x80) {
        D_8013921C += 4;
        for (i = 0; i < 64; i++) {
            D_8011ECD0[0x1987 + i * 0x44] += 4;
        }
        D_800E6280.unk_036 += 2;
        D_800E6280.unk_037 += 2;
        D_800E6280.unk_038 += 4;
    }
}

void func_80132C4C(void) {
    s32 i;

    if (D_8013921C == 4) {
        D_8013921C = 0;
        for (i = 0; i < 64; i++) {
            D_8011ECD0[0x1987 + i * 0x44] = 0;
        }
        D_800E6280.unk_036 = 0;
        D_800E6280.unk_037 = 0;
        D_800E6280.unk_038 = 0;
        return;
    }
    if (D_8013921C > 0) {
        D_8013921C -= 4;
        for (i = 0; i < 64; i++) {
            D_8011ECD0[0x1987 + i * 0x44] -= 4;
        }
        D_800E6280.unk_036 -= 2;
        D_800E6280.unk_037 -= 2;
        D_800E6280.unk_038 -= 4;
    }
}

void func_80132D44(void) {
    switch (D_80139AF8) {                           /* irregular */
    case 0:
        func_801333B0();
        return;
    case 1:
        func_8013358C();
        return;
    case 2:
        func_80132EE8();
        return;
    case 3:
        func_80132DC4();
        return;
    }
}

void func_80132DC4(void) {
    if (D_80139AFC == 0) {
        func_80044750(0x504);
    }
    D_80139AFC += 1;
    D_80121548 = 0x1E;
    func_80132E3C();
    if (D_80139AFC >= 0xB4) {
        D_80121533 = 0;
        D_80121531 = 3;
        func_80042808();
    }
}

void func_80132E3C(void) {
    func_801330D0();
    if (D_800E6280.unk_F80 & 0x20) {
        if (D_80121531 != 7) {
            func_80135A48(D_8011ECF6, D_8011ECFA);
            func_801369F4(0x501);
        }
        D_80121531 = 7;
        return;
    }
    if (D_80121531 != 3) {
        func_80135A48(D_8011ECF6, D_8011ECFA);
        func_801369F4(0x502);
    }
    D_80121531 = 3;
}

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI/80132000", func_80132EE8);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI/80132000", func_801330D0);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI/80132000", func_801333B0);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI/80132000", func_8013358C);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI/80132000", func_801337E0);

void func_801338EC(void) {
    func_801340CC();
    func_80134258(1);
    func_80133924();
    func_80133CA8();
}

void func_80133924(void) {
    s32 i;
    s32 *p;
    u8 n;

    p = (s32 *) D_8013984C;
    for (i = 0, n = 10; i != n; i++) {
        if (!(p[0] & 0x80)) {
            if ((D_80121531 == 7) && (p[1] != 0)) {
                func_80134C1C(i);
                func_801339D4(i);
            } else {
                func_80134800(i);
                func_801339D4(i);
            }
        }
        p += 12;
    }
}

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI/80132000", func_801339D4);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI/80132000", func_80133CA8);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI/80132000", func_80133F00);

s32 func_80134008(s32 arg0) {
    s32 v;

    v = D_80139868[arg0].val;
    if (v < 0x100) {
        return 0;
    }
    if (v < 0x300) {
        return 1;
    }
    if (v < 0x500) {
        return 2;
    }
    if (v < 0x700) {
        return 3;
    }
    if (v < 0x900) {
        return 4;
    }
    if (v < 0xB00) {
        return 5;
    }
    if (v < 0xD00) {
        return 6;
    }
    if (v < 0xF00) {
        return 7;
    }
    return 0;
}

void func_801340CC(void) {
    s32 i;
    s32 x;
    s32 y;

    /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
    for (i = 0; i < 10; i++) if (*(s16 *) &D_8013984C[i * 0x30 + 0x16] < -0xC0 || *(s16 *) &D_8013984C[i * 0x30 + 0x16] >= 0xC1 || *(s16 *) &D_8013984C[i * 0x30 + 0x1A] < -0x98 || *(s16 *) &D_8013984C[i * 0x30 + 0x1A] >= 0x99) *(s32 *) &D_8013984C[i * 0x30] = 0;
}

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI/80132000", func_80134258);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI/80132000", func_801344F0);

void func_80134784(s32 *arg0, s32 *arg1) {
    s32 i;

    *arg0 = 0;
    *arg1 = 0;
    for (i = 0; i < 10; i++) {
        switch (*(s32 *)(D_8013984C + i * 0x30)) {
        case 0:
            break;
        case 1:
        case 2:
        case 3:
            (*arg0)++;
            break;
        case 4:
            (*arg1)++;
            break;
        }
    }
}

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI/80132000", func_80134800);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI/80132000", func_80134C1C);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI/80132000", func_80134D74);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI/80132000", func_80134F2C);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI/80132000", func_80135044);

void func_8013515C(s32 arg0, s32 arg1) {
    u8 *p;

    p = D_8013984C + arg0 * 0x30;
    if (*(s32 *) (p + 0x28) < 3) {
        switch (*(s32 *) (p + 4)) {
        case 1:
            if (arg1 < 0xC4) {
                if (!(func_800AE0D0() & 1)) {
                    *(s32 *) (p + 0x2C) = 0;
                } else {
                    *(s32 *) (p + 0x2C) = 1;
                }
                *(s32 *) (p + 0x28) = *(s32 *) (p + 8) * 2 + 4;
                *(s32 *) (p + 0x24) = func_800AE0D0() % 15 + 0x1E;
            }
            break;
        case 2:
            if (arg1 < 0x384) {
                if (!(func_800AE0D0() & 1)) {
                    *(s32 *) (p + 0x2C) = 0;
                } else {
                    *(s32 *) (p + 0x2C) = 1;
                }
                *(s32 *) (p + 0x28) = *(s32 *) (p + 8) * 2 + 4;
                *(s32 *) (p + 0x24) = func_800AE0D0() % 15 + 0x1E;
            }
            break;
        }
    }
}

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI/80132000", func_80135274);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI/80132000", func_801354E0);

void func_8013556C(void) {
    D_80139220 += 1;
    func_80135600();
    D_80139224 += 3;
    if (D_80139224 >= 0x1000) {
        D_80139224 = 0;
    }
    D_801392A4 = (func_800A0070(D_80139224) * 8) >> 12;
    D_801392A8 = (func_800A0140(D_80139224) * 0x10) >> 12;
    func_801359D0();
    func_80135F54();
}

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI/80132000", func_80135600);

void func_801359D0(void) {
    s32 i;

    for (i = 0; i != 10; i++) {
        if (D_80139278[i] != 0) {
            D_80139278[i] += 1;
            func_80135ACC(i);
            if (D_80139278[i] >= 0x51) {
                D_80139278[i] = 0;
            }
        }
    }
}

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI/80132000", func_80135A48);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI/80132000", func_80135ACC);

void func_80135F54(void) {
    s32 i;
    s32 j;

    for (j = 0; j < 11; j++) {
        for (i = 0; i < 14; i++) {
            func_80135FBC(i, j);
        }
    }
}

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI/80132000", func_80135FBC);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI/80132000", func_801362A4);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI/80132000", func_80136700);

void func_80136904(s32 arg0, s32 arg1) {
    D_80121313 = 0x84;
    D_80121357 = 0x84;
    D_8012139B = 0x84;
    D_801213DF = 0x84;
    D_80121328 = (arg0 % 10) + 0x23;
    D_8012136C = (arg0 / 10) + 0x23;
    if (arg0 < 0xA) {
        D_80121357 = 0;
    }
    D_801213B0 = (arg1 % 10) + 0x23;
    D_801213F4 = (arg1 / 10) + 0x23;
    if (arg1 < 0xA) {
        D_801213DF = 0;
    }
}

void func_801369F4(s32 arg0) {
    if (D_80139B1C <= 0) {
        D_80139B1C = 1;
        func_80044750(arg0 & 0xFFFF);
    }
}

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI/80132000", func_80136A2C);
