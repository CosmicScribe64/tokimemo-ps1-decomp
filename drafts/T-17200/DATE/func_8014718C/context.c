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

/* bulk_m2c declarations (T-11190) */
extern s16 D_800B5922;
extern s16 D_800B5924;
extern s16 D_800B5926;
extern s16 D_800B592A;
extern s8 D_800B593A;
extern u8 D_80121875;
extern u8 D_80121876;
/* end bulk_m2c declarations */
void func_8014718C(void) {
    s32 sp30;
    Rec24 *temp_v0;
    Rec24 *temp_v0_6;
    s16 temp_a0;
    s16 temp_t6_2;
    s16 temp_t8;
    s16 var_a0;
    s16 var_a0_2;
    s16 var_a0_3;
    s16 var_a0_4;
    s16 var_a0_5;
    s16 var_a0_6;
    s16 var_a0_7;
    s16 var_a0_8;
    s16 var_a0_9;
    s32 temp_at;
    s32 temp_lo;
    s32 temp_lo_2;
    s32 temp_lo_3;
    s32 temp_lo_4;
    s32 temp_lo_5;
    s32 temp_lo_6;
    s32 temp_lo_7;
    s32 temp_lo_8;
    s32 temp_t9;
    s32 var_a3;
    u16 temp_t8_2;
    u16 temp_v1;
    u8 temp_a1;
    u8 temp_a1_2;
    u8 temp_a1_3;
    u8 *temp_t6;
    Rec24 *temp_v0_10;
    Rec24 *temp_v0_2;
    Rec24 *temp_v0_3;
    Rec24 *temp_v0_4;
    Rec24 *temp_v0_5;
    Rec24 *temp_v0_7;
    Rec24 *temp_v0_8;
    Rec24 *temp_v0_9;

    var_a3 = D_800E6280.unk_1104.w;
    D_800B3D60 = 0;
    switch (var_a3) {
    case 0:
        func_80044750(0x201);
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
        var_a0 = 0;
        do {
            temp_t8 = (var_a0 * 2) + 0x15;
            temp_v0 = &D_801217D0[var_a0];
            temp_v0[10].unk_08 = (s16) ((var_a0 << 6) + 0x80);
            temp_t9 = var_a0 << 7;
            temp_t6 = &D_800E6280 + var_a0;
            var_a0 += 1;
            temp_v0[10].unk_00 = 0x82000000;
            temp_v0[10].unk_0A = 0xA0;
            temp_v0[10].unk_0E = 0;
            temp_v0[10].unk_0F = 0;
            temp_v0[10].unk_10 = 0;
            temp_v0[10].unk_0C = temp_t8;
            temp_v0[10].unk_12 = 0x1E0;
            temp_v0[10].unk_14 = 0;
            temp_v0[10].unk_15 = 0;
            temp_v0[10].unk_16 = 0;
            temp_v0[10].unk_18 = 0;
            temp_v0[10].unk_1A = 0;
            temp_v0[10].unk_1C = 0x1000;
            temp_v0[10].unk_1E = 0x1000;
            temp_v0[10].unk_20 = 0;
            temp_v0[10].unk_04 = (s16) (temp_t9 - 0xA0);
            temp_v0[10].unk_06 = -0x96;
            *(s8 *)((unsigned char *)temp_t6 + (4271)) = 0xC;
        } while (var_a0 < 2);
        dec_bg_cd_read(0x43C2, 1);
        D_800E6280.unk_1104.w += 1;
        return;
    case 1:
        if (cd_read_sync_status() & 1) {
            func_800536AC(0x140, 0x100, 0x140, 0xA0);
            D_800E6280.unk_1104.w += 1;
        }
        return;
    case 2:
        cd_read_sectors(0x31U, 0x801AE000, 0xBCD1);
        func_801461B0();
        D_800E6280.unk_1104.w += 1;
        return;
    case 3:
    case 11:
    case 14:
    case 16:
    case 18:
        if (cd_read_sync_status() & 1) {
            D_800E6280.unk_1104.w += 1;
        }
        return;
    case 4:
        load_palette(D_8015CF80, 0x11, 1, 0, 0);
        func_80149268(D_8015CF84, D_8015CF88, D_8015CF8C, D_8015CF90, D_8015CF94, D_8015CF98);
        D_800E6280.unk_1104.w += 1;
        return;
    case 5:
        var_a0_2 = 0;
        do {
            temp_lo = var_a0_2 * 0x24;
            var_a0_2 += 1;
            temp_v0_2 = &D_801217D0[var_a0_2];
            temp_v0_2[4].unk_00 = (s32) (temp_v0_2[4].unk_00 & 0x7FFFFFFF);
        } while (var_a0_2 < 8);
        D_800E6280.unk_1104.w = var_a3 + 1;
        return;
    case 6:
        temp_a1 = D_80121874 + 1;
        D_80121874 = temp_a1;
        D_80121876 = temp_a1;
        D_80121875 = temp_a1;
        var_a0_3 = 1;
        do {
            temp_lo_2 = var_a0_3 * 0x24;
            var_a0_3 += 1;
            temp_v0_3 = &D_801217D0[var_a0_3];
            temp_v0_3[4].unk_16 = (u8) D_80121874;
            temp_v0_3[4].unk_15 = (u8) D_80121874;
            temp_v0_3[4].unk_14 = (u8) D_80121874;
        } while (var_a0_3 < 8);
        if (D_80121874 == 0x40) {
            D_800E6280.unk_1104.w = var_a3 + 1;
        }
        return;
    case 7:
        temp_a1_2 = D_80121874 + 1;
        D_80121874 = temp_a1_2;
        D_80121876 = temp_a1_2;
        D_80121875 = temp_a1_2;
        var_a0_4 = 1;
        do {
            temp_lo_3 = var_a0_4 * 0x24;
            var_a0_4 += 1;
            temp_v0_4 = &D_801217D0[var_a0_4];
            temp_v0_4[4].unk_16 = (u8) D_80121874;
            temp_v0_4[4].unk_15 = (u8) D_80121874;
            temp_v0_4[4].unk_14 = (u8) D_80121874;
        } while (var_a0_4 < 8);
        if (D_80121874 == 0x80) {
            var_a3 += 1;
        }
block_40:
        temp_t8_2 = (D_8015CF9C + 1) & 0xFFFF;
        var_a0_5 = 0;
        if ((temp_t8_2 % 19U) == 0x12) {
            do {
                temp_lo_4 = var_a0_5 * 0x24;
                temp_a0 = var_a0_5 + 1;
                temp_t6_2 = temp_a0;
                temp_at = temp_t6_2 < 2;
                var_a0_5 = temp_t6_2;
                temp_v0_5 = &D_801217D0[var_a0_5];
                temp_v0_5[10].unk_06 = (s16) (temp_v0_5[10].unk_06 + 1);
            } while (temp_at != 0);
            temp_v0_6 = &D_801217D0[temp_t6_2];
            temp_v0_6[10].unk_0A = (u16) (temp_v0_6[10].unk_0A - 1);
        }
        var_a0_6 = 0;
        if ((temp_t8_2 % 5U) == 4) {
            do {
                temp_lo_5 = var_a0_6 * 0x24;
                var_a0_6 += 1;
                temp_v0_7 = &D_801217D0[var_a0_6];
                temp_v0_7[4].unk_06 = (s16) (temp_v0_7[4].unk_06 + 1);
            } while (var_a0_6 < 6);
            var_a0_7 = 3;
            do {
                temp_lo_6 = var_a0_7 * 0x24;
                var_a0_7 += 1;
                temp_v0_8 = &D_801217D0[var_a0_7];
                temp_v1 = temp_v0_8[4].unk_0A;
                if (temp_v1 != 0) {
                    temp_v0_8[4].unk_0A = (u16) (temp_v1 - 1);
                } else {
                    temp_v0_8[1].unk_0A = (u16) (temp_v0_8[1].unk_0A - 1);
                }
            } while (var_a0_7 < 6);
        }
        D_800E6280.unk_1104.w = var_a3;
        D_8015CF9C = temp_t8_2;
        if (temp_t8_2 == 0x12C) {
            D_800E6280.unk_1104.w = var_a3 + 1;
        }
        return;
    case 8:
        goto block_40;
    case 9:
        temp_a1_3 = D_80121874 - 2;
        D_80121874 = temp_a1_3;
        D_80121876 = temp_a1_3;
        D_80121875 = temp_a1_3;
        var_a0_8 = 1;
        do {
            temp_lo_7 = var_a0_8 * 0x24;
            var_a0_8 += 1;
            temp_v0_9 = &D_801217D0[var_a0_8];
            temp_v0_9[4].unk_16 = (u8) D_80121874;
            temp_v0_9[4].unk_15 = (u8) D_80121874;
            temp_v0_9[4].unk_14 = (u8) D_80121874;
        } while (var_a0_8 < 8);
        var_a0_9 = 0;
        if (D_80121874 == 0) {
            do {
                temp_lo_8 = var_a0_9 * 0x24;
                var_a0_9 += 1;
                temp_v0_10 = &D_801217D0[var_a0_9];
                temp_v0_10[4].unk_00 = (s32) (temp_v0_10[4].unk_00 | 0x80000000);
            } while (var_a0_9 < 8);
            var_a3 += 1;
        }
        goto block_40;
    case 10:
        func_800462C8(0xAU, 0x80162000, 0x4350);
        D_800E6280.unk_1104.w += 1;
        return;
    case 12:
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
    case 13:
        sp30 = var_a3;
        D_800E6280.unk_1104.w = var_a3;
        func_8013E7B0();
        D_800E6280.unk_110A -= 1;
        D_800E6280.unk_1104.w = var_a3 + 1;
        return;
    case 15:
        sp30 = var_a3;
        D_800E6280.unk_1104.w = var_a3;
        func_8013E980();
        D_800E6280.unk_110A -= 1;
        D_800E6280.unk_1104.w = var_a3 + 1;
        return;
    case 17:
        sp30 = var_a3;
        D_800E6280.unk_1104.w = var_a3;
        func_8013E9BC();
        D_800E6280.unk_110A -= 1;
        D_800E6280.unk_1104.w = var_a3 + 1;
        return;
    default:
        func_8004284C();
        D_80122D2C = 0;
        func_80044750(0x200);
        hizuke_disp_switch(1);
        return;
    }
}

/* NON_MATCHING: T-12100, four fade addiu/andi register rows remain. */
#ifdef NON_MATCHING
/* FAKE: s32 with normal bare returns preserves the original v0 liveness;
 * only the decoder-error path returns zero. Real return type unknown. T-12100. */
s32 func_80147958(void) {
    GameState *g;
    s32 current;
    s32 sp54;
    u32 sp50;
    RECT rect;
    DateMovieWork *movie;
    /* FAKE: unused view reserves the observed frame slot. T-12100. */
    DateMovieHeader8 *header;
    u8 value;
    u32 color;
    s16 var_v1;

    g = &D_800E6280;
    movie = (DateMovieWork *)D_800B5920;
    D_800B3D60 = 0;
    switch (g->unk_1104.u) {
    case 0:
        func_80044750(0x7F);
        D_80122D2C = 1;
        movie->unk_06 = 0;
        movie->unk_02 = 0;
        movie->unk_04 = 0;
        movie->unk_00 = 0;
        movie->unk_19 = 0;
        movie->unk_18 = 0;
        movie->unk_28 = -1;
        movie->unk_0A = 0;
        movie->unk_1A = 0;
        func_8006492C(0);
        D_8015CF9C = 0;
        for (var_v1 = 0; var_v1 < 2; var_v1++) {
            D_801217D0[var_v1 + 4].unk_08 = (var_v1 << 6) + 0x80;
            D_801217D0[var_v1 + 4].unk_00 = 0x82000000;
            D_801217D0[var_v1 + 4].unk_0A = 0xa0;
            D_801217D0[var_v1 + 4].unk_0E = 0;
            D_801217D0[var_v1 + 4].unk_0F = 0x0;
            D_801217D0[var_v1 + 4].unk_10 = 0;
            D_801217D0[var_v1 + 4].unk_0C = (var_v1 * 2) + 0x15;
            D_801217D0[var_v1 + 4].unk_12 = 0x1E0;
            D_801217D0[var_v1 + 4].unk_14 = 0;
            D_801217D0[var_v1 + 4].unk_15 = 0;
            D_801217D0[var_v1 + 4].unk_16 = 0;
            D_801217D0[var_v1 + 4].unk_18 = 0;
            D_801217D0[var_v1 + 4].unk_1A = 0;
            D_801217D0[var_v1 + 4].unk_1C = 0x1000;
            D_801217D0[var_v1 + 4].unk_1E = 0x1000;
            D_801217D0[var_v1 + 4].unk_20 = 0;
            D_801217D0[var_v1 + 4].unk_04 = (var_v1 << 7) - 0xA0;
            D_801217D0[var_v1 + 4].unk_06 = -0x78;
            D_800E6280.unk_10A5[var_v1 + 4] = 0xb;
        }
        for (var_v1 = 0; var_v1 < 2; var_v1++) {
            D_801217D0[var_v1 + 6].unk_08 = (var_v1 << 6) + 0x80;
            D_801217D0[var_v1 + 6].unk_00 = 0x82000000;
            D_801217D0[var_v1 + 6].unk_0A = 0x0;
            D_801217D0[var_v1 + 6].unk_0E = 0;
            D_801217D0[var_v1 + 6].unk_0F = 0xa0;
            D_801217D0[var_v1 + 6].unk_10 = 0;
            D_801217D0[var_v1 + 6].unk_0C = (var_v1 * 2) + 0xa;
            D_801217D0[var_v1 + 6].unk_12 = 0x1E0;
            D_801217D0[var_v1 + 6].unk_14 = 0;
            D_801217D0[var_v1 + 6].unk_15 = 0;
            D_801217D0[var_v1 + 6].unk_16 = 0;
            D_801217D0[var_v1 + 6].unk_18 = 0;
            D_801217D0[var_v1 + 6].unk_1A = 0;
            D_801217D0[var_v1 + 6].unk_1C = 0x1000;
            D_801217D0[var_v1 + 6].unk_1E = 0x1000;
            D_801217D0[var_v1 + 6].unk_20 = 0;
            D_801217D0[var_v1 + 6].unk_04 = (var_v1 << 7) - 0xA0;
            D_801217D0[var_v1 + 6].unk_06 = -0x78;
            D_800E6280.unk_10A5[var_v1 + 6] = 0xb;
        }
        func_8005742C(0x43B1, 1);
        g->unk_1104.u += 1;
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
            g->unk_1104.u += 1;
            return;
        }
        return;
    case 2:
        func_800536AC(0x140, 0x100, 0x140, 0xA0);
        rect.x = 0x140;
        rect.y = 0x100;
        rect.w = 0x140;
        rect.h = 0xA0;
        func_8009C8E0(&rect, (void *)0x80183000);
        g->unk_1104.u += 1;
        return;
    case 3:
        func_8005742C(0x43AA, 1);
        g->unk_1104.u += 1;
        return;
    case 5:
        func_800536AC(0x140, 0x100, 0x140, 0xA0);
        rect.x = 0x140;
        rect.y = 0x100;
        rect.w = 0x140;
        rect.h = 0xA0;
        func_8009C8E0(&rect, (void *)0x8019C000);
        g->unk_1104.u += 1;
        return;
    case 6:
        func_8005742C(0x43A3, 1);
        g->unk_1104.u += 1;
        return;
    case 8:
        func_800536AC(0x140, 0x100, 0x140, 0xA0);
        rect.x = 0x140;
        rect.y = 0x100;
        rect.w = 0x140;
        rect.h = 0xA0;
        func_8009C8E0(&rect, (void *)0x801B5000);
        g->unk_1104.u += 1;
        return;
    case 9:
        func_8005742C(0x439C, 1);
        g->unk_1104.u += 1;
        return;
    case 11:
        func_800536AC(0x140, 0x100, 0x140, 0xA0);
        rect.x = 0x140;
        rect.y = 0x100;
        rect.w = 0x140;
        rect.h = 0xA0;
        func_8009C8E0(&rect, (void *)0x801CE000);
        g->unk_1104.u += 1;
        return;
    case 12:
        func_8005742C(0x4395, 1);
        g->unk_1104.u += 1;
        return;
    case 14:
        func_800536AC(0x140, 0, 0x140, 0xA0);
        g->unk_1104.u += 1;
        return;
    case 15:
        func_8005742C(0x437D, 1);
        g->unk_1104.u += 1;
        return;
    case 17:
        func_800536AC(0x140, 0x100, 0x140, 0xA0);
        g->unk_1104.u += 1;
        return;
    case 18:
        func_8005742C(0x4385, 1);
        g->unk_1104.u += 1;
        return;
    case 20:
        func_800536AC(0x280, 0, 0x140, 0xA0);
        g->unk_1104.u += 1;
        return;
    case 21:
        for (var_v1 = 0; var_v1 < 4; var_v1++) {
            D_801217D0[var_v1 + 4].unk_00 = (s32) (D_801217D0[var_v1 + 4].unk_00 & 0x7FFFFFFF);
        }
        g->unk_1104.u++;
        return;
    case 22:
        value = D_801217D0[4].unk_14;
        value += 2;
        color = (value & 0xFF);
        D_801217D0[7].unk_16 = color;
        D_801217D0[7].unk_15 = color;
        D_801217D0[7].unk_14 = color;
        D_801217D0[6].unk_16 = color;
        D_801217D0[6].unk_15 = color;
        D_801217D0[6].unk_14 = color;
        D_801217D0[5].unk_16 = color;
        D_801217D0[5].unk_15 = color;
        D_801217D0[5].unk_14 = color;
        D_801217D0[4].unk_16 = color;
        D_801217D0[4].unk_15 = color;
        D_801217D0[4].unk_14 = color;

        if (color == 0x80) {
            g->unk_1104.u++;
            return;
        }
        break;
    case 23:
        if (!(D_8015CF9C & 7)) {
            func_80148924(4);
                if (((u16) D_8015CF9C % 24U) == 0) {
                func_80044750(0x503);
                    }
        }
        if (++D_8015CF9C == 0x140) {
            D_8015CF9C = 0;
            D_800E6280.unk_1104.u += 1;
        }
        break;
    case 24:
        g->unk_1104.u++;
        return;
    case 25:
        func_800438F0(0);
        for (var_v1 = 0; var_v1 < 4; var_v1++) {
            D_801217D0[var_v1 + 4].unk_00 = (s32) (D_801217D0[var_v1 + 4].unk_00 | 0x80000000);
        }
        g->unk_1104.u += 1;
        return;
    case 26:
        func_8005742C(0x438D, 1);
        g->unk_1104.u += 1;
        return;
    case 28:
        func_800536AC(0x140, 0x100, 0x140, 0xA0);
        rect.x = 0x140;
        rect.y = 0;
        rect.w = 0x140;
        rect.h = 0xA0;
        func_8009C93C(&rect, 0x280, 0);
        g->unk_1104.u += 1;
        func_800438F0(1);
        return;
    case 29:
        for (var_v1 = 0; var_v1 < 2; var_v1++) {
            D_801217D0[var_v1 + 4].unk_00 = 0x02000000;
            D_801217D0[var_v1 + 4].unk_0A = 0xA0;
            D_801217D0[var_v1 + 4].unk_0F = 0;
            D_801217D0[var_v1 + 4].unk_06 = -0x78;
        }
        for (var_v1 = 0; var_v1 < 2; var_v1++) {
            D_801217D0[var_v1 + 6].unk_00 = 0x02000000;
            D_801217D0[var_v1 + 6].unk_0A = 0;
            D_801217D0[var_v1 + 6].unk_0F = 0xA0;
            D_801217D0[var_v1 + 6].unk_06 = -0x78;
        }
        D_8015CF9C = 0;
        func_80044750(0x500);
        g->unk_1104.u += 1;
        return;
    case 30:
    case 32:
    case 34:
        func_80148924(8);
        if (++D_8015CF9C == 0x14) {
            g->unk_1104.u += 1;
            return;
        }
        break;
    case 31:
        rect.x = 0x280;
        rect.y = 0;
        rect.w = 0x140;
        rect.h = 0xA0;
        func_8009C93C(&rect, 0x140, 0x100);
        func_8004435C(0x280U, 0U, 0x140U, 0xA0U, 0x801CE000);
        for (var_v1 = 0; var_v1 < 2; var_v1++) {
            D_801217D0[var_v1 + 4].unk_0A = 0xA0;
            D_801217D0[var_v1 + 4].unk_0F = 0;
            D_801217D0[var_v1 + 4].unk_06 = -0x78;
        }
        for (var_v1 = 0; var_v1 < 2; var_v1++) {
            D_801217D0[var_v1 + 6].unk_0A = 0;
            D_801217D0[var_v1 + 6].unk_0F = 0xA0;
            D_801217D0[var_v1 + 6].unk_06 = -0x78;
        }
        D_8015CF9C = 0;
        g->unk_1104.u += 1;
        return;
    case 33:
        rect.x = 0x280;
        rect.y = 0;
        rect.w = 0x140;
        rect.h = 0xA0;
        func_8009C93C(&rect, 0x140, 0x100);
        func_8004435C(0x280U, 0U, 0x140U, 0xA0U, 0x801B5000);
        for (var_v1 = 0; var_v1 < 2; var_v1++) {
            D_801217D0[var_v1 + 4].unk_0A = 0xA0;
            D_801217D0[var_v1 + 4].unk_0F = 0;
            D_801217D0[var_v1 + 4].unk_06 = -0x78;
        }
        for (var_v1 = 0; var_v1 < 2; var_v1++) {
            D_801217D0[var_v1 + 6].unk_0A = 0;
            D_801217D0[var_v1 + 6].unk_0F = 0xA0;
            D_801217D0[var_v1 + 6].unk_06 = -0x78;
        }
        D_8015CF9C = 0;
        g->unk_1104.u += 1;
        return;
    case 35:
        rect.x = 0x280;
        rect.y = 0;
        rect.w = 0x140;
        rect.h = 0xA0;
        func_8009C93C(&rect, 0x140, 0x100);
        func_8004435C(0x280U, 0U, 0x140U, 0xA0U, 0x8019C000);
        for (var_v1 = 0; var_v1 < 2; var_v1++) {
            D_801217D0[var_v1 + 4].unk_0A = 0xA0;
            D_801217D0[var_v1 + 4].unk_0F = 0;
            D_801217D0[var_v1 + 4].unk_06 = -0x78;
        }
        for (var_v1 = 0; var_v1 < 2; var_v1++) {
            D_801217D0[var_v1 + 6].unk_0A = 0;
            D_801217D0[var_v1 + 6].unk_0F = 0xA0;
            D_801217D0[var_v1 + 6].unk_06 = -0x78;
        }
        D_8015CF9C = 0;
        g->unk_1104.u += 1;
        return;
    case 36:
        func_80148924(8);
        if ((u16) D_8015CF9C >= 8U) {
            value = D_801217D0[4].unk_14;
        value -= 4;
        color = (value & 0xFF);
            D_801217D0[7].unk_16 = color;
            D_801217D0[7].unk_15 = color;
            D_801217D0[7].unk_14 = color;
            D_801217D0[6].unk_16 = color;
            D_801217D0[6].unk_15 = color;
            D_801217D0[6].unk_14 = color;
            D_801217D0[5].unk_16 = color;
            D_801217D0[5].unk_15 = color;
            D_801217D0[5].unk_14 = color;
            D_801217D0[4].unk_16 = color;
            D_801217D0[4].unk_15 = color;
        D_801217D0[4].unk_14 = color;
        }
        if (++D_8015CF9C == 0x14) {
            g->unk_1104.u += 1;
            return;
        }
        break;
    case 37:
        rect.x = 0x280;
        rect.y = 0;
        rect.w = 0x140;
        rect.h = 0xA0;
        func_8009C93C(&rect, 0x140, 0x100);
        func_8004435C(0x280U, 0U, 0x140U, 0xA0U, 0x80183000);
        for (var_v1 = 0; var_v1 < 2; var_v1++) {
            D_801217D0[var_v1 + 4].unk_0A = 0xA0;
            D_801217D0[var_v1 + 4].unk_0F = 0;
            D_801217D0[var_v1 + 4].unk_06 = -0x78;
        }
        for (var_v1 = 0; var_v1 < 2; var_v1++) {
            D_801217D0[var_v1 + 6].unk_0A = 0;
            D_801217D0[var_v1 + 6].unk_0F = 0xA0;
            D_801217D0[var_v1 + 6].unk_06 = -0x78;
        }
        D_8015CF9C = 0;
        g->unk_1104.u += 1;
        return;
    case 38:
        func_80148924(8);
        value = D_801217D0[4].unk_14;
        value -= 4;
        color = (value & 0xFF);
        D_801217D0[7].unk_16 = color;
        D_801217D0[7].unk_15 = color;
        D_801217D0[7].unk_14 = color;
        D_801217D0[6].unk_16 = color;
        D_801217D0[6].unk_15 = color;
        D_801217D0[6].unk_14 = color;
        D_801217D0[5].unk_16 = color;
        D_801217D0[5].unk_15 = color;
        D_801217D0[5].unk_14 = color;
        D_801217D0[4].unk_16 = color;
        D_801217D0[4].unk_15 = color;
        D_801217D0[4].unk_14 = color;

        if (color < 4U) {
            g->unk_1104.u += 1;
            return;
        }
        break;
    case 39:
        for (var_v1 = 0; var_v1 < 4; var_v1++) {
            D_801217D0[var_v1 + 4].unk_00 = (s32) (D_801217D0[var_v1 + 4].unk_00 | 0x80000000);
        }
        g->unk_1104.u++;
        return;
    case 40:
        sp50 = g->unk_1104.u;

        sp54 = (s32) D_800E6280.unk_110A;
        /* FAKE: one source line preserves as1 callback load scheduling. T-12100. */
        func_80083B24(D_800E6280.unk_110A); g->unk_1104.u = sp50; if (sp54 != D_800E6280.unk_110A) { D_800E6280.unk_110A -= 1; g->unk_1104.u++; } break;
    case 42:
        sp54 = D_800E6280.unk_110A;
        sp50 = g->unk_1104.u;

        func_80083474();
        current = sp54 + 1;
        if (D_800E6280.unk_110A != current) {
            D_800E6280.unk_110A = sp54;
            g->unk_1104.u = 0x28;
            return;
        }
        D_800E6280.unk_110A -= 1;
        g->unk_1104.u = sp50;
        g->unk_1104.u++;
        return;
    case 43:
        func_800462C8(0xAU, 0x80162000, 0x433E);
        g->unk_1104.u += 1;
        return;
    case 45:
        if (((((DateMovieHeader8 *)D_80125C04)->unk_00 & 0xFFFF0000) != 0x38000000) || ((((DateMovieHeader8 *)D_80125C04)->unk_04 & 0xFF010000) != 0x10000)) {
            func_800AE0B0("DEC_BS header Error \n");
            movie->unk_04 = 0;
            movie->unk_06 = 0;
            g->unk_1104.u -= 2;
            return 0;
        }
        func_8005751C(1);
        func_800573F8(1);
        g->unk_1104.u += 1;
        return;
    case 46:
        sp50 = g->unk_1104.u;

        func_8013E7B0();
        D_800E6280.unk_110A -= 1;
        g->unk_1104.u = sp50;
        g->unk_1104.u++;
        return;
    case 48:
        sp50 = g->unk_1104.u;

        func_8013E980();
        D_800E6280.unk_110A -= 1;
        g->unk_1104.u = sp50;
        g->unk_1104.u++;
        return;
    case 50:
        sp50 = g->unk_1104.u;

        func_8013E9BC();
        D_800E6280.unk_110A -= 1;
        g->unk_1104.u = sp50;
        g->unk_1104.u++;
        return;
    default:
        func_8004284C();
        D_80122D2C = 0;
        func_80044750(0x200);
        func_8006492C(1);
        break;
    }
    return;
}
#else
INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/801461B0", func_80147958);
#endif

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
