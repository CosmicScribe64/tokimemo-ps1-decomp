#include "common.h"
#include "ovl/OPTION.h"

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

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_80132624);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_801326BC);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_801328C0);

void func_80132A8C(void) {
    if (D_800E6280.unk_110A == 0) {
        func_80132AB8();
    }
}

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_80132AB8);

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

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_8013357C);

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
        for (i = 0; i < 3; i++) D_8011ECD0[0x1C6F + i * 0x44] &= ~0x80;
        D_80120912[0] = D_80120912[0] / 9 * 8;
        if (D_80120912[0] < 9) {
            D_800E6280.unk_110D = 1;
        }
    } else if (D_800E6280.unk_110D == 2) {
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

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_80134804);

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

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_80134C00);

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

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_80134E64);

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

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_801352FC);

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

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_80135450);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_8013554C);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_801359A4);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_80135C0C);

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

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_80135E48);

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

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_801368FC);

void func_80136A50(void) {
    s32 i;

    if (D_800E6280.unk_110D == 0) {
        for (i = 0; i < 3; i++) D_8011ECD0[0x1C6F + i * 0x44] &= ~0x80;
        D_80120912[0] = D_80120912[0] / 9 * 8;
        if (D_80120912[0] < 9) {
            D_800E6280.unk_110D = 1;
        }
    } else if (D_800E6280.unk_110D == 2) {
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

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_80136F3C);

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

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_80137864);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_801378DC);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_80137C1C);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_80137E38);

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
    func_80042878(0x91);
    func_801320C0();
}

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_80138708);

void func_801387A8(void) {
    if (func_800460CC() & 1) {
        func_80042878(0x63);
        func_80042908(3);
    }
}

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_801387E4);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_8013884C);
