#define MAIN_API_OVERRIDE_D_800E7D10 /* recovered aggregate flag view, canonical word view retained elsewhere, T-9540 */
#include "common.h"
#include "ovl/OPTION.h"
/* FAKE: scoped minimum aggregate view of the word and adjacent flags, based on observed +0x14 indexing. T-9540 */
extern FlagsWork24 D_800E7D10;

/* View of the 0x44-byte sprite records of the D_8011ECD0 table that the option menus number
 * (D_80120650 is record 96; the menus write the s16 at +0x18 of records 107-113). T-9020 */
typedef struct OptSpr44 {
    /* 0x00 */ s32 unk_00[6];
    /* 0x18 */ s16 unk_18;
    /* 0x1A */ u8 pad1A[0x2A];
} OptSpr44; /* size 0x44 */

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_80132000);

void func_801320B0(void) {
}

void func_801320B8(void) {
}

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_801320C0);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_80132384);

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

typedef struct {
    s16 a[8];
    s16 b[8];
    s16 c[8];
    s16 d[8];
} Arrs4x8; /* size 0x40 */

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

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_801328C0);

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

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_80132BA8);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_80133010);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_80133330);

/* FAKE: s32 without an explicit return retains the original dead v0 loop initialization; no meaningful return is established. T-9540 */
s32 func_8013357C(void) {
    s32 i;
    OptDraw44 *p;

    for (i = 0x60; i < 0xA0; i++) {
        p = (OptDraw44 *)D_8011ECD0 + i;
        if (p->unk_00.b[3] & 0x80) {
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

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_80133718);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_8013394C);

s32 func_80133AD8(void) {
    s32 i;

    if (D_800E6280.unk_110D == 0) {
        /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
        for (i = 0; i < 3; i++) D_8011ECD0[0x1C6F + i * 0x44] &= ~0x80;
        D_80120912[0] = D_80120912[0] / 9 * 8;
        if (D_80120912[0] < 9) {
            D_800E6280.unk_110D = 1;
        }
    } else if (D_800E6280.unk_110D == 2) {
        /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
        for (i = 0; i < 3; i++) D_8011ECD0[0x1C6F + i * 0x44] &= ~0x80;
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

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_80133CB8);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_80133F50);

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

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_801342A8);

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

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_801344E4);

void func_80134804(void) {
    s32 i;
    s32 j;
    s32 sel;

    /* one table: as1 keeps the loads after the store only when both go through D_8011ECD0 (T-9020) */
    D_8011ECD0[0x1C2B] = 0x80;
    func_8004F984(1, *(s16 *)&D_8011ECD0[0x26], *(s16 *)&D_8011ECD0[0x2A]);
    func_8004FC10(1);
    func_801320C0();
    sel = D_800E6280.unk_1094;
    for (i = 0, j = 0; i < 3; i++, j += 2) {
        if (i == sel) {
            ((OptSpr44 *)D_80120650)[i + 11].unk_18 = j + 0x1B;
        } else {
            ((OptSpr44 *)D_80120650)[i + 11].unk_18 = j + 0x1C;
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

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_8013493C);

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
            ((OptSpr44 *)D_80120650)[i + 14].unk_18 = j + 0x21;
        } else {
            ((OptSpr44 *)D_80120650)[i + 14].unk_18 = j + 0x22;
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
            ((OptSpr44 *)D_80120650)[i + 16].unk_18 = j + 0x25;
        } else {
            ((OptSpr44 *)D_80120650)[i + 16].unk_18 = j + 0x26;
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
    *(s16 *)&D_80120650[0x568] = 0x79;
    *(s16 *)&D_80120650[0x576] = 0x40;
    *(s16 *)&D_80120650[0x57A] = 0x30;
    D_80120650[0x553] |= 0x80;
    D_80120650[0x593] = 0x1F;
    D_8013D180 = 0;
    func_8004284C();
    func_801320C0();
}

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_80135110);

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

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_8013554C);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_801359A4);

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

/* bulk_m2c declarations (T-11190) */
extern s16 D_80120910;
extern s16 D_80120914;
extern s16 D_8012091E;
extern s16 D_80120954;
extern s16 D_80120962;
extern s16 D_80120998;
extern s16 D_801209DC;
extern s16 D_80120A20;
extern s16 D_80120A64;
extern s8 D_800E7314;
extern u8 D_800E71EF;
extern u8 D_800E738D;
/* end bulk_m2c declarations */
s32 func_80135E48(void) {
    s32 sp70[16];
    s16 sp64;
    s16 sp58;
    s16 sp4C;
    s16 *temp_a0;
    s16 *temp_a1;
    s16 *temp_t0;
    s16 *temp_t1;
    s16 temp_a3;
    s16 temp_t8;
    s32 temp_v1;
    u32 temp_hi;
    u8 *temp_v0;

    if (D_800E738D == 0) {
        D_80120910 = 4;
        D_8012091E = -0xA0;
        D_80120922 = -0x78;
        D_801208FB = 0;
        D_80120914 = 0x7FFF;
        D_800E738D = 1;
        *D_80120912 = 0x7FFF;
        return;
    }
    temp_a3 = 1 * 0x18;
    temp_t8 = ((s16) *D_80120912 / 17) * 0x10;
    D_80120914 = ((s16) D_80120914 / 17) * 0x10;
    *D_80120912 = temp_t8;
    if (temp_t8 < 0x1001) {
        D_80120914 = 0x1000;
        *D_80120912 = 0x1000;
        D_80120954 = 0xC;
        D_80120962 = 0x10;
        D_80120966 = -0x18;
        sp58 = 0x30;
        D_8012093F |= 0x80;
        sp4C = 0x10;
        sp64 = -0x18;
        temp_v1 = 1 * 2;
        temp_a0 = &(&sp70[0])[1];
        sp70[0] = 0x10;
        temp_v0 = (1 * 0x44) + D_80120650;
        temp_a1 = &(&sp64)[1];
        (*(s32 *)temp_a1) = temp_a3 - 0x18;
        (*(s32 *)temp_a0) = 0x10;
        *(s16 *)((unsigned char *)temp_v0 + (772)) = (s16) (temp_v1 + 0xC);
        *(s16 *)((unsigned char *)temp_a0 + (2)) = 0x10;
        *(s16 *)((unsigned char *)temp_v0 + (790)) = (s16) (*(s32 *)temp_a1);
        *(u8 *)((unsigned char *)temp_v0 + (751)) = (u8) (*(u8 *)((unsigned char *)temp_v0 + (751)) | 0x80);
        *(s16 *)((unsigned char *)temp_v0 + (786)) = (s16) (*(s32 *)temp_a0);
        *(s16 *)((unsigned char *)temp_a1 + (2)) = temp_a3;
        *(s16 *)((unsigned char *)temp_v0 + (840)) = (s16) (temp_v1 + 0xE);
        *(s16 *)((unsigned char *)temp_v0 + (854)) = (s16) *(s16 *)((unsigned char *)temp_a0 + (2));
        *(s16 *)((unsigned char *)temp_a0 + (4)) = 0x10;
        *(s16 *)((unsigned char *)temp_a1 + (4)) = (s16) (temp_a3 + 0x18);
        *(s16 *)((unsigned char *)temp_v0 + (858)) = (s16) *(s16 *)((unsigned char *)temp_a1 + (2));
        *(s16 *)((unsigned char *)temp_v0 + (908)) = (s16) (temp_v1 + 0x10);
        *(u8 *)((unsigned char *)temp_v0 + (819)) = (u8) (*(u8 *)((unsigned char *)temp_v0 + (819)) | 0x80);
        *(s16 *)((unsigned char *)temp_v0 + (922)) = (s16) *(s16 *)((unsigned char *)temp_a0 + (4));
        *(s16 *)((unsigned char *)temp_a1 + (6)) = (s16) (temp_a3 + 0x30);
        temp_t1 = &(&sp4C)[1];
        *(u8 *)((unsigned char *)temp_v0 + (887)) = (u8) (*(u8 *)((unsigned char *)temp_v0 + (887)) | 0x80);
        *(s16 *)((unsigned char *)temp_a0 + (6)) = 0x10;
        temp_t0 = &(&sp58)[1];
        *(s16 *)((unsigned char *)temp_v0 + (926)) = (s16) *(s16 *)((unsigned char *)temp_a1 + (4));
        *(s16 *)((unsigned char *)temp_v0 + (976)) = (s16) (temp_v1 + 0x12);
        *(s16 *)((unsigned char *)temp_t1 + (2)) = 0x10;
        *(s16 *)((unsigned char *)temp_t1 + (4)) = 0x10;
        *(s16 *)((unsigned char *)temp_t1 + (6)) = 0x10;
        (*(s32 *)temp_t1) = 0x10;
        *(u8 *)((unsigned char *)temp_v0 + (955)) = (u8) (*(u8 *)((unsigned char *)temp_v0 + (955)) | 0x80);
        *(s16 *)((unsigned char *)temp_t0 + (2)) = 0x30;
        *(s16 *)((unsigned char *)temp_t0 + (4)) = 0x30;
        *(s16 *)((unsigned char *)temp_t0 + (6)) = 0x30;
        (*(s32 *)temp_t0) = 0x30;
        *(s16 *)((unsigned char *)temp_v0 + (990)) = (s16) *(s16 *)((unsigned char *)temp_a0 + (6));
        *(s16 *)((unsigned char *)temp_v0 + (994)) = (s16) *(s16 *)((unsigned char *)temp_a1 + (6));
        menu_set(1, 5, &sp70[0], &sp64, &sp58, &sp4C);
        temp_hi = (u8) D_800E71EF % 10U;
        switch (temp_hi) {
        default:
            D_80120954 += 1;
            D_800E7314 = 0;
            break;
        case 2:
            D_80120998 += 1;
            D_800E7314 = 1;
            break;
        case 3:
            D_801209DC += 1;
            D_800E7314 = 1;
            break;
        case 4:
            D_80120A20 += 1;
            D_800E7314 = 1;
            break;
        case 6:
            D_80120A64 += 1;
            D_800E7314 = 1;
            break;
        }
        func_8004284C();
    }
}

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_801361FC);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_80136434);

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

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_80136688);

void func_801368FC(void) {
    s32 i;
    s32 j;
    s32 sel;

    /* one table: as1 keeps the loads after the store only when both go through D_8011ECD0 (T-9020) */
    D_8011ECD0[0x1C2B] = 0x80;
    func_8004F984(1, *(s16 *)&D_8011ECD0[0x26], *(s16 *)&D_8011ECD0[0x2A]);
    func_8004FC10(1);
    func_801320C0();
    sel = D_800E6280.unk_1094;
    for (i = 0, j = 0; i < 3; i++, j += 2) {
        if (i == sel) {
            ((OptSpr44 *)D_80120650)[i + 11].unk_18 = j + 0x2D;
        } else {
            ((OptSpr44 *)D_80120650)[i + 11].unk_18 = j + 0x2E;
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
        for (i = 0; i < 3; i++) D_8011ECD0[0x1C6F + i * 0x44] &= ~0x80;
        D_80120912[0] = D_80120912[0] / 9 * 8;
        if (D_80120912[0] < 9) {
            D_800E6280.unk_110D = 1;
        }
    } else if (D_800E6280.unk_110D == 2) {
        /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
        for (i = 0; i < 3; i++) D_8011ECD0[0x1C6F + i * 0x44] &= ~0x80;
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

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_80136C30);

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

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_80137040);

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

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_801373C8);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_801376A8);

void func_80137864(void) {
    s32 i;
    s16 m;

    D_8012170F &= (m = 0xFF7F); /* FAKE: keeps the mask in $a0 for the loop's `and`, as the original; real source unknown. T-9340 */
    for (i = 0; i < 2; i++) {
        D_80120650[0x3FF + i * 0x44] &= ~0x80;
    }
    func_80042940(0);
    D_8012170F = 0;
    D_80121726 = 0x1000;
    D_80121728 = 0x1000;
}

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_801378DC);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_80137C1C);

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

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_80138080);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_801383C4);

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
