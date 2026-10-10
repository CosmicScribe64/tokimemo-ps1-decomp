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
        if ((n ^ 0) != D_800E7389) { /* FAKE: '^ 0' swaps the beq operand order to the original's; real source unknown. T-4070 */
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

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_801325A0);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_80132624);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_801326BC);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_801328C0);

void func_80132A8C(void) {
    if (D_800E738A == 0) {
        func_80132AB8();
    }
}

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_80132AB8);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_80132B30);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_80132BA8);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_80133010);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_80133330);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_8013357C);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_801335E8);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_80133718);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_8013394C);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_80133AD8);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_80133C4C);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_80133CB8);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_80133F50);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_80134210);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_801342A8);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_8013447C);

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

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_80134F9C);

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

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_80135DE0);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_80135E48);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_801361FC);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_80136434);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_8013661C);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_80136688);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_801368FC);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_80136A50);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_80136BC8);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_80136C30);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_80136F3C);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_80137040);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_801371FC);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_801373C8);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_801376A8);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_80137864);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_801378DC);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_80137C1C);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_80137E38);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_80137FA0);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_80138080);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_801383C4);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/80132000", func_801385F0);

void func_801386C8(void) {
    func_80048EB8(0);
    D_800E71DF = D_800E7D68;
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
