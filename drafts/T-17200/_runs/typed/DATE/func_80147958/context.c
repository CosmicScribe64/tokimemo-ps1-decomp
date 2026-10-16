#include "common.h"
#include "ovl/DATE.h"
/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/801461B0.data", D_8015CF80);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/801461B0.data", D_8015CF84);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/801461B0.data", D_8015CF88);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/801461B0.data", D_8015CF8C);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/801461B0.data", D_8015CF90);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/801461B0.data", D_8015CF94);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/801461B0.data", D_8015CF98);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/801461B0.data", D_8015CF9C);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/801461B0.data", D_8015CFA0);
s16 D_8015CFA4[3] = { 0, 0, 0 };
s16 D_8015CFAC[3] = { 0, 0, 0 };
s16 D_8015CFB4[3] = { 0, 0, 0 };
s16 D_8015CFBC[3] = { 0, 0, 0 };
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/801461B0.data", D_8015CFC4);

void func_801461B0(void) {
    D_8015CF80 = 0x801C6000;
    D_8015CF84 = 0x801AE000;
    D_8015CF88 = 0x801B2000;
    D_8015CF8C = 0x801B6000;
    D_8015CF90 = 0x801BA000;
    D_8015CF94 = 0x801BE000;
    D_8015CF98 = 0x801C2000;
}

void func_80146224(void) {
    /* T-10040: this dispatcher reads the signed-declared byte with lbu. */
    switch (*(u8 *)&D_8015B68C) {
    case 0:
        func_80147958();
        return;
    case 1:
        func_801489D0();
        return;
    case 2:
        func_8014718C();
        return;
    case 3:
        func_801462B0();
        return;
    case 4:
        func_8014A1E4();
        /* fallthrough */
    default:
        return;
    }
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/801461B0", func_801462B0);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/801461B0", func_8014718C);

/* FAKE: s32 with normal bare returns preserves the original v0 liveness;
 * only the decoder-error path returns zero. Real return type unknown. T-12100. */
/* bulk_m2c declarations (T-11190) */
extern s16 D_800B5922;
extern s16 D_800B5924;
extern s16 D_800B5926;
extern s16 D_800B592A;
extern s8 D_800B593A;
extern u8 D_80121875;
extern u8 D_80121876;
extern u8 D_80121898;
extern u8 D_80121899;
extern u8 D_8012189A;
extern u8 D_801218BC;
extern u8 D_801218BD;
extern u8 D_801218BE;
extern u8 D_801218E0;
extern u8 D_801218E1;
extern u8 D_801218E2;
/* end bulk_m2c declarations */
void func_80147958(void) {
    s32 sp54;
    s32 sp50;
    s16 sp4E;
    s16 sp4C;
    s16 sp4A;
    s16 sp48;
    Rec24 *temp_v0;
    Rec24 *temp_v0_2;
    s16 temp_t8;
    s16 temp_t8_2;
    s16 var_v1;
    s16 var_v1_10;
    s16 var_v1_11;
    s16 var_v1_12;
    s16 var_v1_13;
    s16 var_v1_14;
    s16 var_v1_15;
    s16 var_v1_2;
    s16 var_v1_3;
    s16 var_v1_4;
    s16 var_v1_5;
    s16 var_v1_6;
    s16 var_v1_7;
    s16 var_v1_8;
    s16 var_v1_9;
    s32 temp_a3;
    s32 temp_lo;
    s32 temp_lo_10;
    s32 temp_lo_11;
    s32 temp_lo_12;
    s32 temp_lo_13;
    s32 temp_lo_2;
    s32 temp_lo_3;
    s32 temp_lo_4;
    s32 temp_lo_5;
    s32 temp_lo_6;
    s32 temp_lo_7;
    s32 temp_lo_8;
    s32 temp_lo_9;
    s32 temp_t9;
    s32 temp_t9_2;
    s32 var_a1;
    u16 temp_t6_3;
    u16 temp_t9_3;
    u16 temp_t9_4;
    u8 temp_v1;
    u8 temp_v1_2;
    u8 temp_v1_3;
    u8 temp_v1_4;
    u8 *temp_t6;
    u8 *temp_t6_2;
    Rec24 *temp_v0_10;
    Rec24 *temp_v0_11;
    Rec24 *temp_v0_12;
    Rec24 *temp_v0_13;
    Rec24 *temp_v0_14;
    Rec24 *temp_v0_15;
    Rec24 *temp_v0_3;
    Rec24 *temp_v0_4;
    Rec24 *temp_v0_5;
    Rec24 *temp_v0_6;
    Rec24 *temp_v0_7;
    Rec24 *temp_v0_8;
    Rec24 *temp_v0_9;

    temp_a3 = D_800E6280.unk_1104.w;
    D_800B3D60 = 0;
    switch (temp_a3) {
    case 0:
        func_80044750(0x7F);
        D_80122D2C = 1;
        D_800B5926 = 0;
        D_800B5922 = 0;
        D_800B5924 = 0;
        *D_800B5920 = 0;
        D_800B5939 = 0;
        *D_800B5938 = 0;
        D_800B5948 = -1;
        D_800B592A = 0;
        D_800B593A = 0;
        hizuke_disp_switch(0);
        D_8015CF9C = 0;
        var_v1 = 0;
        do {
            temp_t8 = (var_v1 * 2) + 0x15;
            temp_v0 = &D_801217D0[var_v1];
            temp_v0[4].unk_08 = (s16) ((var_v1 << 6) + 0x80);
            temp_t9 = var_v1 << 7;
            temp_t6 = &D_800E6280 + var_v1;
            var_v1 += 1;
            temp_v0[4].unk_00 = 0x82000000;
            temp_v0[4].unk_0A = 0xA0;
            temp_v0[4].unk_0E = 0;
            temp_v0[4].unk_0F = 0;
            temp_v0[4].unk_10 = 0;
            temp_v0[4].unk_0C = temp_t8;
            temp_v0[4].unk_12 = 0x1E0;
            temp_v0[4].unk_14 = 0;
            temp_v0[4].unk_15 = 0;
            temp_v0[4].unk_16 = 0;
            temp_v0[4].unk_18 = 0;
            temp_v0[4].unk_1A = 0;
            temp_v0[4].unk_1C = 0x1000;
            temp_v0[4].unk_1E = 0x1000;
            temp_v0[4].unk_20 = 0;
            temp_v0[4].unk_04 = (s16) (temp_t9 - 0xA0);
            temp_v0[4].unk_06 = -0x78;
            *(s8 *)((unsigned char *)temp_t6 + (4265)) = 0xB;
        } while (var_v1 < 2);
        var_v1_2 = 0;
        do {
            temp_t8_2 = (var_v1_2 * 2) + 0xA;
            temp_v0_2 = &D_801217D0[var_v1_2];
            temp_v0_2[6].unk_08 = (s16) ((var_v1_2 << 6) + 0x80);
            temp_t9_2 = var_v1_2 << 7;
            temp_t6_2 = &D_800E6280 + var_v1_2;
            var_v1_2 += 1;
            temp_v0_2[6].unk_00 = 0x82000000;
            temp_v0_2[6].unk_0A = 0;
            temp_v0_2[6].unk_0E = 0;
            temp_v0_2[6].unk_0F = 0xA0;
            temp_v0_2[6].unk_10 = 0;
            temp_v0_2[6].unk_0C = temp_t8_2;
            temp_v0_2[6].unk_12 = 0x1E0;
            temp_v0_2[6].unk_14 = 0;
            temp_v0_2[6].unk_15 = 0;
            temp_v0_2[6].unk_16 = 0;
            temp_v0_2[6].unk_18 = 0;
            temp_v0_2[6].unk_1A = 0;
            temp_v0_2[6].unk_1C = 0x1000;
            temp_v0_2[6].unk_1E = 0x1000;
            temp_v0_2[6].unk_20 = 0;
            temp_v0_2[6].unk_04 = (s16) (temp_t9_2 - 0xA0);
            temp_v0_2[6].unk_06 = -0x78;
            *(s8 *)((unsigned char *)temp_t6_2 + (4267)) = 0xB;
        } while (var_v1_2 < 2);
        dec_bg_cd_read(0x43B1, 1);
        D_800E6280.unk_1104.w += 1;
        return;
    case 1:
    case 4:
    case 7:
    case 10:
    case 13:
    case 16:
    case 19:
    case 27:
    case 41:
    case 44:
    case 47:
    case 49:
        if (cd_read_sync_status() & 1) {
            D_800E6280.unk_1104.w += 1;
            return;
        }
        return;
    case 2:
        func_800536AC(0x140, 0x100, 0x140, 0xA0);
        sp48 = 0x140;
        sp4A = 0x100;
        sp4C = 0x140;
        sp4E = 0xA0;
        func_8009C8E0((RECT *) &sp48, (void *)0x80183000);
        D_800E6280.unk_1104.w += 1;
        return;
    case 3:
        dec_bg_cd_read(0x43AA, 1);
        D_800E6280.unk_1104.w += 1;
        return;
    case 5:
        func_800536AC(0x140, 0x100, 0x140, 0xA0);
        sp48 = 0x140;
        sp4A = 0x100;
        sp4C = 0x140;
        sp4E = 0xA0;
        func_8009C8E0((RECT *) &sp48, (void *)0x8019C000);
        D_800E6280.unk_1104.w += 1;
        return;
    case 6:
        dec_bg_cd_read(0x43A3, 1);
        D_800E6280.unk_1104.w += 1;
        return;
    case 8:
        func_800536AC(0x140, 0x100, 0x140, 0xA0);
        sp48 = 0x140;
        sp4A = 0x100;
        sp4C = 0x140;
        sp4E = 0xA0;
        func_8009C8E0((RECT *) &sp48, (void *)0x801B5000);
        D_800E6280.unk_1104.w += 1;
        return;
    case 9:
        dec_bg_cd_read(0x439C, 1);
        D_800E6280.unk_1104.w += 1;
        return;
    case 11:
        func_800536AC(0x140, 0x100, 0x140, 0xA0);
        sp48 = 0x140;
        sp4A = 0x100;
        sp4C = 0x140;
        sp4E = 0xA0;
        func_8009C8E0((RECT *) &sp48, (void *)0x801CE000);
        D_800E6280.unk_1104.w += 1;
        return;
    case 12:
        dec_bg_cd_read(0x4395, 1);
        D_800E6280.unk_1104.w += 1;
        return;
    case 14:
        func_800536AC(0x140, 0, 0x140, 0xA0);
        D_800E6280.unk_1104.w += 1;
        return;
    case 15:
        dec_bg_cd_read(0x437D, 1);
        D_800E6280.unk_1104.w += 1;
        return;
    case 17:
        func_800536AC(0x140, 0x100, 0x140, 0xA0);
        D_800E6280.unk_1104.w += 1;
        return;
    case 18:
        dec_bg_cd_read(0x4385, 1);
        D_800E6280.unk_1104.w += 1;
        return;
    case 20:
        func_800536AC(0x280, 0, 0x140, 0xA0);
        D_800E6280.unk_1104.w += 1;
        return;
    case 21:
        var_v1_3 = 0;
        do {
            temp_lo = var_v1_3 * 0x24;
            var_v1_3 += 1;
            temp_v0_3 = (Rec24 *)((u8 *)D_801217D0 + temp_lo);
            temp_v0_3[4].unk_00 = (s32) (temp_v0_3[4].unk_00 & 0x7FFFFFFF);
        } while (var_v1_3 < 4);
        D_800E6280.unk_1104.w = temp_a3 + 1;
        return;
    case 22:
        temp_v1 = (D_80121874 + 2) & 0xFF;
        D_801218E2 = temp_v1;
        D_801218E1 = temp_v1;
        D_801218E0 = temp_v1;
        D_801218BE = temp_v1;
        D_801218BD = temp_v1;
        D_801218BC = temp_v1;
        D_8012189A = temp_v1;
        D_80121899 = temp_v1;
        D_80121898 = temp_v1;
        D_80121876 = temp_v1;
        D_80121875 = temp_v1;
        D_80121874 = temp_v1;
        if (temp_v1 == 0x80) {
            D_800E6280.unk_1104.w = temp_a3 + 1;
            return;
        }
        break;
    case 23:
        var_a1 = D_8015CF9C + 1;
        if (!(D_8015CF9C & 7)) {
            func_80148924(4);
            var_a1 = D_8015CF9C + 1;
            if (((u16) D_8015CF9C % 24U) == 0) {
                func_80044750(0x503);
                var_a1 = D_8015CF9C + 1;
            }
        }
        temp_t9_3 = var_a1 & 0xFFFF;
        D_8015CF9C = temp_t9_3;
        if (temp_t9_3 == 0x140) {
            D_8015CF9C = 0;
            D_800E6280.unk_1104.w += 1;
            return;
        }
        break;
    case 24:
        D_800E6280.unk_1104.w = temp_a3 + 1;
        return;
    case 25:
        back_clear_switch(0);
        var_v1_4 = 0;
        do {
            temp_lo_2 = var_v1_4 * 0x24;
            var_v1_4 += 1;
            temp_v0_4 = (Rec24 *)((u8 *)D_801217D0 + temp_lo_2);
            temp_v0_4[4].unk_00 = (s32) (temp_v0_4[4].unk_00 | 0x80000000);
        } while (var_v1_4 < 4);
        D_800E6280.unk_1104.w += 1;
        return;
    case 26:
        dec_bg_cd_read(0x438D, 1);
        D_800E6280.unk_1104.w += 1;
        return;
    case 28:
        func_800536AC(0x140, 0x100, 0x140, 0xA0);
        sp48 = 0x140;
        sp4A = 0;
        sp4C = 0x140;
        sp4E = 0xA0;
        func_8009C93C((RECT *) &sp48, 0x280, 0);
        D_800E6280.unk_1104.w += 1;
        back_clear_switch(1);
        return;
    case 29:
        var_v1_5 = 0;
        do {
            temp_lo_3 = var_v1_5 * 0x24;
            var_v1_5 += 1;
            temp_v0_5 = (Rec24 *)((u8 *)D_801217D0 + temp_lo_3);
            temp_v0_5[4].unk_00 = 0x02000000;
            temp_v0_5[4].unk_0A = 0xA0;
            temp_v0_5[4].unk_0F = 0;
            temp_v0_5[4].unk_06 = -0x78;
        } while (var_v1_5 < 2);
        var_v1_6 = 0;
        do {
            temp_lo_4 = var_v1_6 * 0x24;
            var_v1_6 += 1;
            temp_v0_6 = (Rec24 *)((u8 *)D_801217D0 + temp_lo_4);
            temp_v0_6[6].unk_00 = 0x02000000;
            temp_v0_6[6].unk_0A = 0;
            temp_v0_6[6].unk_0F = 0xA0;
            temp_v0_6[6].unk_06 = -0x78;
        } while (var_v1_6 < 2);
        D_8015CF9C = 0;
        func_80044750(0x500);
        D_800E6280.unk_1104.w += 1;
        return;
    case 30:
    case 32:
    case 34:
        func_80148924(8);
        temp_t9_4 = (D_8015CF9C + 1) & 0xFFFF;
        D_8015CF9C = temp_t9_4;
        if (temp_t9_4 == 0x14) {
            D_800E6280.unk_1104.w += 1;
            return;
        }
        break;
    case 31:
        sp48 = 0x280;
        sp4A = 0;
        sp4C = 0x140;
        sp4E = 0xA0;
        func_8009C93C((RECT *) &sp48, 0x140, 0x100);
        LoadSquare(0x280U, 0U, 0x140U, 0xA0U, 0x801CE000);
        var_v1_7 = 0;
        do {
            temp_lo_5 = var_v1_7 * 0x24;
            var_v1_7 += 1;
            temp_v0_7 = (Rec24 *)((u8 *)D_801217D0 + temp_lo_5);
            temp_v0_7[4].unk_0A = 0xA0;
            temp_v0_7[4].unk_0F = 0;
            temp_v0_7[4].unk_06 = -0x78;
        } while (var_v1_7 < 2);
        var_v1_8 = 0;
        do {
            temp_lo_6 = var_v1_8 * 0x24;
            var_v1_8 += 1;
            temp_v0_8 = (Rec24 *)((u8 *)D_801217D0 + temp_lo_6);
            temp_v0_8[6].unk_0A = 0;
            temp_v0_8[6].unk_0F = 0xA0;
            temp_v0_8[6].unk_06 = -0x78;
        } while (var_v1_8 < 2);
        D_8015CF9C = 0;
        D_800E6280.unk_1104.w += 1;
        return;
    case 33:
        sp48 = 0x280;
        sp4A = 0;
        sp4C = 0x140;
        sp4E = 0xA0;
        func_8009C93C((RECT *) &sp48, 0x140, 0x100);
        LoadSquare(0x280U, 0U, 0x140U, 0xA0U, 0x801B5000);
        var_v1_9 = 0;
        do {
            temp_lo_7 = var_v1_9 * 0x24;
            var_v1_9 += 1;
            temp_v0_9 = (Rec24 *)((u8 *)D_801217D0 + temp_lo_7);
            temp_v0_9[4].unk_0A = 0xA0;
            temp_v0_9[4].unk_0F = 0;
            temp_v0_9[4].unk_06 = -0x78;
        } while (var_v1_9 < 2);
        var_v1_10 = 0;
        do {
            temp_lo_8 = var_v1_10 * 0x24;
            var_v1_10 += 1;
            temp_v0_10 = (Rec24 *)((u8 *)D_801217D0 + temp_lo_8);
            temp_v0_10[6].unk_0A = 0;
            temp_v0_10[6].unk_0F = 0xA0;
            temp_v0_10[6].unk_06 = -0x78;
        } while (var_v1_10 < 2);
        D_8015CF9C = 0;
        D_800E6280.unk_1104.w += 1;
        return;
    case 35:
        sp48 = 0x280;
        sp4A = 0;
        sp4C = 0x140;
        sp4E = 0xA0;
        func_8009C93C((RECT *) &sp48, 0x140, 0x100);
        LoadSquare(0x280U, 0U, 0x140U, 0xA0U, 0x8019C000);
        var_v1_11 = 0;
        do {
            temp_lo_9 = var_v1_11 * 0x24;
            var_v1_11 += 1;
            temp_v0_11 = (Rec24 *)((u8 *)D_801217D0 + temp_lo_9);
            temp_v0_11[4].unk_0A = 0xA0;
            temp_v0_11[4].unk_0F = 0;
            temp_v0_11[4].unk_06 = -0x78;
        } while (var_v1_11 < 2);
        var_v1_12 = 0;
        do {
            temp_lo_10 = var_v1_12 * 0x24;
            var_v1_12 += 1;
            temp_v0_12 = (Rec24 *)((u8 *)D_801217D0 + temp_lo_10);
            temp_v0_12[6].unk_0A = 0;
            temp_v0_12[6].unk_0F = 0xA0;
            temp_v0_12[6].unk_06 = -0x78;
        } while (var_v1_12 < 2);
        D_8015CF9C = 0;
        D_800E6280.unk_1104.w += 1;
        return;
    case 36:
        func_80148924(8);
        if ((u16) D_8015CF9C >= 8U) {
            temp_v1_2 = D_80121874 - 4;
            D_801218E2 = temp_v1_2;
            D_801218E1 = temp_v1_2;
            D_801218E0 = temp_v1_2;
            D_801218BE = temp_v1_2;
            D_801218BD = temp_v1_2;
            D_801218BC = temp_v1_2;
            D_8012189A = temp_v1_2;
            D_80121899 = temp_v1_2;
            D_80121898 = temp_v1_2;
            D_80121876 = temp_v1_2;
            D_80121875 = temp_v1_2;
            D_80121874 = temp_v1_2;
        }
        temp_t6_3 = (D_8015CF9C + 1) & 0xFFFF;
        D_8015CF9C = temp_t6_3;
        if (temp_t6_3 == 0x14) {
            D_800E6280.unk_1104.w += 1;
            return;
        }
        break;
    case 37:
        sp48 = 0x280;
        sp4A = 0;
        sp4C = 0x140;
        sp4E = 0xA0;
        func_8009C93C((RECT *) &sp48, 0x140, 0x100);
        LoadSquare(0x280U, 0U, 0x140U, 0xA0U, 0x80183000);
        var_v1_13 = 0;
        do {
            temp_lo_11 = var_v1_13 * 0x24;
            var_v1_13 += 1;
            temp_v0_13 = (Rec24 *)((u8 *)D_801217D0 + temp_lo_11);
            temp_v0_13[4].unk_0A = 0xA0;
            temp_v0_13[4].unk_0F = 0;
            temp_v0_13[4].unk_06 = -0x78;
        } while (var_v1_13 < 2);
        var_v1_14 = 0;
        do {
            temp_lo_12 = var_v1_14 * 0x24;
            var_v1_14 += 1;
            temp_v0_14 = (Rec24 *)((u8 *)D_801217D0 + temp_lo_12);
            temp_v0_14[6].unk_0A = 0;
            temp_v0_14[6].unk_0F = 0xA0;
            temp_v0_14[6].unk_06 = -0x78;
        } while (var_v1_14 < 2);
        D_8015CF9C = 0;
        D_800E6280.unk_1104.w += 1;
        return;
    case 38:
        func_80148924(8);
        temp_v1_3 = (D_80121874 - 4) & 0xFF;
        D_801218E2 = temp_v1_3;
        D_801218E1 = temp_v1_3;
        D_801218E0 = temp_v1_3;
        D_801218BE = temp_v1_3;
        D_801218BD = temp_v1_3;
        D_801218BC = temp_v1_3;
        D_8012189A = temp_v1_3;
        D_80121899 = temp_v1_3;
        D_80121898 = temp_v1_3;
        D_80121876 = temp_v1_3;
        D_80121875 = temp_v1_3;
        D_80121874 = temp_v1_3;
        if (temp_v1_3 < 4U) {
            D_800E6280.unk_1104.w += 1;
            return;
        }
        break;
    case 39:
        var_v1_15 = 0;
        do {
            temp_lo_13 = var_v1_15 * 0x24;
            var_v1_15 += 1;
            temp_v0_15 = (Rec24 *)((u8 *)D_801217D0 + temp_lo_13);
            temp_v0_15[4].unk_00 = (s32) (temp_v0_15[4].unk_00 | 0x80000000);
        } while (var_v1_15 < 4);
        D_800E6280.unk_1104.w = temp_a3 + 1;
        return;
    case 40:
        sp50 = temp_a3;
        D_800E6280.unk_1104.w = temp_a3;
        sp54 = (s32) D_800E6280.unk_110A;
        read_bustup(D_800E6280.unk_110A);
        D_800E6280.unk_1104.w = temp_a3;
        if (sp54 != D_800E6280.unk_110A) {
            D_800E6280.unk_110A -= 1;
            D_800E6280.unk_1104.w = temp_a3 + 1;
            return;
        }
        break;
    case 42:
        temp_v1_4 = D_800E6280.unk_110A;
        sp50 = temp_a3;
        D_800E6280.unk_1104.w = temp_a3;
        sp54 = (s32) temp_v1_4;
        func_80083474();
        if ((temp_v1_4 + 1) != D_800E6280.unk_110A) {
            D_800E6280.unk_110A = temp_v1_4;
            D_800E6280.unk_1104.w = 0x28;
            return;
        }
        D_800E6280.unk_110A -= 1;
        D_800E6280.unk_1104.w = sp50 + 1;
        return;
    case 43:
        func_800462C8(0xAU, 0x80162000, 0x433E);
        D_800E6280.unk_1104.w += 1;
        return;
    case 45:
        if ((((*(s32 *)D_80125C04) & 0xFFFF0000) != 0x38000000) || ((*(s32 *)((unsigned char *)D_80125C04 + (4)) & 0xFF010000) != 0x10000)) {
            func_800AE0B0("DEC_BS header Error \n");
            D_800B5924 = 0;
            D_800B5926 = 0;
            D_800E6280.unk_1104.w -= 2;
            return;
        }
        func_8005751C(1);
        dec_bg_show_switch(1);
        D_800E6280.unk_1104.w += 1;
        return;
    case 46:
        sp50 = temp_a3;
        D_800E6280.unk_1104.w = temp_a3;
        func_8013E7B0();
        D_800E6280.unk_110A -= 1;
        D_800E6280.unk_1104.w = temp_a3 + 1;
        return;
    case 48:
        sp50 = temp_a3;
        D_800E6280.unk_1104.w = temp_a3;
        func_8013E980();
        D_800E6280.unk_110A -= 1;
        D_800E6280.unk_1104.w = temp_a3 + 1;
        return;
    case 50:
        sp50 = temp_a3;
        D_800E6280.unk_1104.w = temp_a3;
        func_8013E9BC();
        D_800E6280.unk_110A -= 1;
        D_800E6280.unk_1104.w = temp_a3 + 1;
        return;
    default:
        func_8004284C();
        D_80122D2C = 0;
        func_80044750(0x200);
        hizuke_disp_switch(1);
        break;
    }
}

s32 func_80148924(s16 a0) {
    D_801217D0[4].unk_06 += a0;
    D_801217D0[5].unk_06 += a0;
    D_801217D0[4].unk_0A -= a0;
    D_801217D0[5].unk_0A -= a0;
    D_801217D0[6].unk_0A += a0;
    D_801217D0[7].unk_0A += a0;
    D_801217D0[6].unk_0F -= a0;
    D_801217D0[7].unk_0F -= a0;
}

s32 func_801489D0(void) {
    s16 *temp_a0;
    s16 *temp_v0;
    s16 temp_a1;
    s16 temp_v0_2;
    s16 var_s3;
    s32 remainder;
    u32 temp_s1;
    u32 temp_s1_3;
    u8 temp_s0;
    u8 temp_s0_2;

    switch (D_800E6280.unk_1104.u) {
    case 0:
        D_80122D2C = 1;
        D_8015CFC4 = 0x64;
        func_800AE120((*(s32 *)&D_800E6280.unk_10F8));
        D_8015CF9C = 0;
        hizuke_disp_switch(0);
        func_801491E4(0x43BA);
        D_8015CFA4[0] = -0x50;
        D_8015CFA4[1] = 0;
        D_8015CFA4[2] = 0x64;
        D_8015CFAC[0] = -0x28;
        D_8015CFAC[1] = 0;
        D_8015CFAC[2] = -0x3C;
        D_800E6280.unk_1104.u += 1;
        D_8015CFB4[0] = (func_800AE0D0() % 3) - 1;
        D_8015CFB4[1] = (func_800AE0D0() % 3) - 1;
        D_8015CFB4[2] = (func_800AE0D0() % 3) - 1;
        D_8015CFBC[0] = (func_800AE0D0() % 3) - 1;
        D_8015CFBC[1] = (func_800AE0D0() % 3) - 1;
        D_8015CFBC[2] = (func_800AE0D0() % 3) - 1;
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            D_800E6280.unk_1104.u += 1;
            break;
        }
        break;
    case 2:
        if (((((u32 *)D_80125C04)[0] & 0xFFFF0000) != 0x38000000) || ((((u32 *)D_80125C04)[1] & 0xFF010000) != 0x10000)) {
            func_800AE0B0("DEC_BS header Error \n");
            ((RECT *)D_800B5920)[0].w = 0;
            ((RECT *)D_800B5920)[0].h = 0;
            D_800E6280.unk_1104.u -= 2;
            return 0;
        }
        func_8005751C(1);
        dec_bg_show_switch(1);
        func_80044750(0x203);
        D_800E6280.unk_1104.u += 1;
        break;
    case 3:
        temp_s0 = D_800E6280.unk_110A;
        temp_s1 = D_800E6280.unk_1104.u;
        normal_date_bg_fadein();
        D_800E6280.unk_1104.u = temp_s1;
        if (temp_s0 != D_800E6280.unk_110A) {
            D_800E6280.unk_1104.u = temp_s1 + 1;
        }
        D_800E6280.unk_110A = temp_s0;
        break;
    case 4:
        var_s3 = 0;
        do {
            if (((u32) (*(s32 *)&D_800E6280.unk_10F8) % 30U) == (func_800AE0D0() % 30)) {
                (D_8015CFB4)[var_s3] = (func_800AE0D0() % 3) - 1;
            }

            if (((u32) (*(s32 *)&D_800E6280.unk_10F8) % 30U) == (func_800AE0D0() % 30)) {
                (D_8015CFBC)[var_s3] = (func_800AE0D0() % 3) - 1;
            }

            if ((D_8015CFB4[var_s3] == 0) && (D_8015CFBC[var_s3] == 0)) {
                if ((remainder = func_800AE0D0() % 2)) {
                    D_8015CFB4[var_s3] = ((func_800AE0D0() % 2) * 2) - 1;
                } else {
                    D_8015CFBC[var_s3] = ((func_800AE0D0() % 2) * 2) - 1;
                }
            }
            temp_v0 = &(D_8015CFA4)[var_s3];
            *temp_v0 += D_8015CFB4[var_s3];
            temp_a0 = &(D_8015CFAC)[var_s3];
            *temp_a0 += D_8015CFBC[var_s3];
            temp_a1 = *temp_v0;
            if (temp_a1 < -0x95) {
                D_8015CFB4[var_s3] = 1;
            }
            if (temp_a1 >= 0x96) {
                D_8015CFB4[var_s3] = -1;
            }
            temp_v0_2 = *temp_a0;
            if (temp_v0_2 < -0x6D) {
                D_8015CFBC[var_s3] = 1;
            }
            if (temp_v0_2 >= 0x1E) {
                D_8015CFBC[var_s3] = -1;
            }
            var_s3++;
        } while (var_s3 < 3);
        func_8004A814((s32) D_8015CFA4[0], (s32) D_8015CFAC[0], 0xA, 2, 2, 0x808080, 3, 1);
        func_8004A814((s32) D_8015CFA4[1], (s32) D_8015CFAC[1], 0xA, 2, 2, 0x804040, 3, 1);
        func_8004A814((s32) D_8015CFA4[2], (s32) D_8015CFAC[2], 0xA, 2, 2, 0x800000, 3, 1);
        if (((u32) (*(s32 *)&D_800E6280.unk_10F8) % 50U) == 0) {
            if (!(func_800AE0D0() & 3)) {
                func_80044750(0x504);
            } else if ((func_800AE0D0() % 4) == 1) {
                func_80044750(0x505);
            } else if ((func_800AE0D0() % 4) == 2) {
                func_80044750(0x506);
            } else {
                func_80044750(0x507);
                D_8015CFC4 = 0;
            }
        }
        if (D_8015CFC4++ == 0x14) {
            func_80044750(0x507);
        }
        if (D_8015CFC4 >= 0x65) {
            D_8015CFC4 = 0x64;
        }
        if (D_8015CF9C++ == 0x12C) {
            D_800E6280.unk_1104.u += 1;
            break;
        }
        break;
    case 5:
        temp_s0_2 = D_800E6280.unk_110A;
        temp_s1_3 = D_800E6280.unk_1104.u;
        normal_date_bg_fadeout();
        D_800E6280.unk_1104.u = temp_s1_3;
        if (temp_s0_2 != D_800E6280.unk_110A) {
            D_800E6280.unk_1104.u = temp_s1_3 + 1;
        }
        D_800E6280.unk_110A = temp_s0_2;
        break;
    case 6:
        dec_bg_show_switch(1);
        D_800E6280.unk_1104.u += 1;
        break;
    default:
        func_80044750(0x200);
        D_80122D2C = 0;
        hizuke_disp_switch(1);
        func_8004284C();
        break;
    }
}

void func_801491E4(s32 arg0) {
    s32 temp_v0;

    D_800B3D60 = 0;
    temp_v0 = func_8005742C(arg0, 1);
    if (temp_v0 == D_800B5939) {
        func_800AE0B0("same\n");
        D_800E6280.unk_1104.w += 3;
        return;
    }
    if (temp_v0 == (1 - D_800B5939)) {
        func_800AE0B0("another\n");
        D_800E6280.unk_1104.w += 2;
    }
}

/* FAKE: s32 fall-through keeps the record base in v0 (T-9780). */
s32 func_80149268(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    Rec24 *record;
    s16 i;

    func_80084E90(arg0, arg1, arg2, arg3, arg4, arg5);
    /* FAKE: shared setup/loop line preserves instruction scheduling (T-9780). */
    i = 0; do {
        record = &D_801217D0[i];
        record[4] = record[58];
        D_800E6280.unk_10A5[i + 4] = D_800E6280.unk_10A5[i + 58];
        i++;
    } while (i < 6);
}
