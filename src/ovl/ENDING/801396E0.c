#include "common.h"
#include "ovl/ENDING.h"
#include "game.h"

void func_801396E0(void) {
    D_8013CAC0 = 0x80180308;
    D_8013CAC4 = 0x80180728;
    D_8013CAC8 = 0x80180BCC;
    D_8013CACC = 0x80180ED4;
    D_8013CAD0 = 0x801812E4;
    D_8013CAD4 = 0x80181648;
    D_8013CAD8 = 0x80181A84;
    D_8013CADC = 0x80181ED0;
    D_8013CAE0 = 0x80182258;
    D_8013CAE4 = 0x80182684;
    D_8013CAE8 = 0x80182AB4;
    D_8013CAEC = 0x80182C74;
    D_8013CAF0 = 0x801835E4;
    D_8013CAF4 = 0x80183CC4;
    D_8013CAF8 = 0x80184008;
    D_8013CAFC = 0x80180314;
    D_8013CB00 = 0x80180734;
    D_8013CB04 = 0x80180BD8;
    D_8013CB08 = 0x80180EE0;
    D_8013CB0C = 0x801812F0;
    D_8013CB10 = 0x80181654;
    D_8013CB14 = 0x80181A90;
    D_8013CB18 = 0x80181EDC;
    D_8013CB1C = 0x80182264;
    D_8013CB20 = 0x80182690;
    D_8013CB24 = 0x80182AC0;
    D_8013CB28 = 0x80182C80;
    D_8013CB2C = 0x8018360C;
    D_8013CB30 = 0x80183CD4;
    D_8013CB34 = 0x80184014;
    D_8013CB38 = 0x801803DC;
    D_8013CB3C = 0x801807FC;
    D_8013CB40 = 0x80180CA0;
    D_8013CB44 = 0x80180FA8;
    D_8013CB48 = 0x801813B8;
    D_8013CB4C = 0x8018171C;
    D_8013CB50 = 0x80181B58;
    D_8013CB54 = 0x80181FA4;
    D_8013CB58 = 0x8018232C;
    D_8013CB5C = 0x80182758;
    D_8013CB60 = 0x80182B88;
    D_8013CB64 = 0x80182D48;
    D_8013CB68 = 0x801839E4;
    D_8013CB6C = 0x80183DB8;
    D_8013CB70 = 0x801840DC;
}

typedef struct {
    void (*f[14])();
} FnTbl14; /* size 0x38 */
extern FnTbl14 D_8013CB74;

void func_801399B4(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl14 tbl;

    tbl = D_8013CB74;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
    if (D_80122CD4 != 0) {
        func_80139AA0(0x404040);
    }
}

void func_80139A58(void) {
    if (D_800E6280.unk_F5F == 0xD) {
        func_80042908(7);
        func_80042940(0xF);
        return;
    }
    func_80042908(8);
}

void func_80139AA0(s32 arg0) {
    func_80139B48(-0x64, -0x50, 0xC8, 0x10, arg0, 0, 1);
    func_80139B48(-0x64, 0x40, 0xC8, 0x10, arg0, 0, 1);
    dtd_on(0);
    func_80139B48(-0x64, -0x40, 0xC8, 0x80, arg0, 0xB, 1);
    dtd_on(0xB);
}

void func_80139B48(arg0, arg1, arg2, arg3, arg4, arg5, arg6)
s32 arg0;
s32 arg1;
s32 arg2;
s32 arg3;
u32 arg4;
s32 arg5;
s32 arg6;
{
    s32 unused; /* FAKE: unused local, only its frame slot above p is observed (T-6030) */
    u8 *p;

    p = func_800490F0(0x24, D_8011ECA0);
    func_8009F054(p);
    func_8009EF84(p, arg6);
    p[5] = arg4 >> 8;
    p[4] = arg4 >> 16;
    p[6] = arg4;
    p[0xD] = arg4 >> 8;
    p[0xC] = arg4 >> 16;
    p[0xE] = arg4;
    p[0x15] = arg4 >> 8;
    p[0x14] = arg4 >> 16;
    p[0x16] = arg4;
    p[0x1C] = arg4 >> 16;
    p[0x1E] = arg4;
    p[0x1D] = arg4 >> 8;
    *(s16 *)(p + 8) = D_800E6280.unk_014[D_8011ECA0] + arg0;
    *(s16 *)(p + 0xA) = D_800E6280.unk_018[D_8011ECA0] + arg1;
    *(s16 *)(p + 0x10) = D_800E6280.unk_014[D_8011ECA0] + arg0 + arg2;
    *(s16 *)(p + 0x12) = D_800E6280.unk_018[D_8011ECA0] + arg1;
    *(s16 *)(p + 0x18) = D_800E6280.unk_014[D_8011ECA0] + arg0;
    *(s16 *)(p + 0x1A) = D_800E6280.unk_018[D_8011ECA0] + arg1 + arg3;
    *(s16 *)(p + 0x20) = D_800E6280.unk_014[D_8011ECA0] + arg0 + arg2;
    *(s16 *)(p + 0x22) = D_800E6280.unk_018[D_8011ECA0] + arg1 + arg3;
    func_8009EED0(&D_800E8CA0[(D_8011ECA0 << 0xA) + (arg5 * 4)], p);
}

void func_80139D24(void) {
    func_80046318(9, 0x80180000, 0xAE18);
    func_8004284C();
}

void func_80139D54(void) {
    func_80048EB8(0);
    func_8006BD6C(0);
    func_80041584();
    func_8004E58C();
    func_8004E500(3);
    D_80122CE4 = 0;
    D_80122CD4 = 0;
    func_8004284C();
}

void func_80139DA8(void) {
    if (D_800E6280.unk_F5F != 0xD) {
        func_800462C8(9, 0x80162000, 0x497C);
    } else {
        func_800462C8(0xA, 0x80162000, 0x4985);
    }
    func_8004284C();
}

s32 func_80139E08(void) {
    if ((((u32 *)D_80125C04)[0] & 0xFFFF0000) != 0x38000000 || (((u32 *)D_80125C04)[1] & 0xFF010000) != 0x10000) {
        func_800AE0B0("DEC_BS header Error \n");
        func_800AE0B0("%x %x\n", ((u32 *)D_80125C04)[0], ((u32 *)D_80125C04)[1]);
        D_800B5920[2] = 0;
        D_800B5920[3] = 0;
        func_8004284C();
        D_800E6280.unk_110A -= 3;
        return 0;
    }
    func_800536AC(0x140, 0x100, 0x140, 0xF0);
    func_8004284C();
}

void func_80139EC4(void) {
    s16 i;

    D_80121874 += 4;
    if (D_80121874 >= 0x80) {
        D_80121874 = 0x80;
        func_8004284C();
    }
    for (i = 4; i < 0xA; i++) {
        /* FAKE: the comma expression preserves the brightness register.
         * Use the existing unprototyped callee view. T-9530. */
        func_80084C98(i, (0, D_80121874));
    }
}

void func_80139F3C(void) {
    s16 i;
    s32 shade;
    /* FAKE: keep the existing scalar brightness view for a1 promotion;
     * Rec24 field/pointer views use v0 and do not match. T-10010. */
    if (D_80121874 == 128) {
        func_80044750(0xD1);
        D_80122CD4 = 0;
    }
    D_80121874 -= 4;
    if (D_80121874 < 5U) {
        func_8004284C();
        D_80121874 = 0;
    }
    for (i = 4; i < 10; i++) {
        /* FAKE: comma expression retains the brightness register, as in
         * the adjacent fade-in routine. T-10010. */
        func_80084C98(i, (0, D_80121874));
    }
    shade = (((u32)(D_80121874 << 6)) >> 7) & 0xFF;
    /* FAKE: the empty test preserves the final shift register. T-10010. */
    if (1) {
    }
    func_80139AA0((shade << 16) | (shade << 8) | shade);
}

void func_8013A004(void) {
    D_801217D0[4].unk_00 = 0x02000000;
    D_801217D0[4].unk_08 = 0xC0;
    D_801217D0[4].unk_0A = 0x80;
    D_801217D0[4].unk_0E = 0;
    D_801217D0[4].unk_0F = 0x38;
    D_801217D0[4].unk_10 = 0;
    D_801217D0[4].unk_0C = 0x15;
    D_801217D0[4].unk_12 = 0x1F1;
    D_801217D0[4].unk_14 = 0;
    D_801217D0[4].unk_15 = 0;
    D_801217D0[4].unk_16 = 0;
    D_801217D0[4].unk_18 = 0;
    D_801217D0[4].unk_1A = 0;
    D_801217D0[4].unk_1C = 0x1000;
    D_801217D0[4].unk_1E = 0x1000;
    D_801217D0[4].unk_20 = 0;
    D_801217D0[4].unk_04 = -0xA0;
    D_801217D0[4].unk_06 = -0x40;
    D_800E6280.unk_10A5[4] = 0xD;
    D_801217D0[5] = D_801217D0[4];
    D_801217D0[5].unk_08 = 0x80;
    D_801217D0[5].unk_0C = 0x18;
    D_801217D0[5].unk_04 = 0x20;
    D_800E6280.unk_10A5[5] = 0xD;
    D_801217D0[6] = D_801217D0[4];
    D_801217D0[6].unk_0F = 0;
    D_801217D0[6].unk_0A = 0x38;
    D_801217D0[6].unk_06 = -0x78;
    D_800E6280.unk_10A5[6] = 1;
    D_801217D0[7] = D_801217D0[5];
    D_801217D0[7].unk_0F = 0;
    D_801217D0[7].unk_0A = 0x38;
    D_801217D0[7].unk_06 = -0x78;
    D_800E6280.unk_10A5[7] = 1;
    D_801217D0[8] = D_801217D0[4];
    D_801217D0[8].unk_06 = 0x40;
    D_801217D0[8].unk_0F = 0xB8;
    D_801217D0[8].unk_0A = 0x38;
    D_800E6280.unk_10A5[8] = 1;
    D_801217D0[9] = D_801217D0[5];
    D_801217D0[9].unk_06 = 0x40;
    D_801217D0[9].unk_0F = 0xB8;
    D_801217D0[9].unk_0A = 0x38;
    D_800E6280.unk_10A5[9] = 1;
    func_80044750(0x201);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/801396E0", func_8013A2E8);

/* FAKE (T-9780): the s32 fall-through preserves the original timer result register. */
s32 func_8013B510(void) {
    s16 timer;
    s16 y;
    u32 max;
    u32 count;
    Entry8 *entry;

    if (D_800E6280.unk_10F8 & 3) {
        return 0;
    }
    max = func_8004ECF0() & 0xFF;
    count = 0;
    do {
        entry = (Entry8 *)func_8004E970(count);
        entry->unk_02--;
        y = entry->unk_02;
        count++;
        if (y >= 81 || y < -80) {
            entry->unk_07 = 0;
        } else {
            entry->unk_07 = 1;
        }
        /* FAKE (T-9780): normalize in the condition to retain the delay-slot copy. */
    } while (max >= (count = count & 0xFF));
    timer = --D_800CA2C0;
    if (timer < -64) {
        func_8004284C();
    }
}

s32 func_8013B5E4(void) {
    if ((u32)D_800E6280.unk_1104.w++ < 0x80U) {
        return 0;
    }
    func_8004E884(D_80122CD0);
    D_80122CD4 = 1;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/801396E0", func_8013B640);
