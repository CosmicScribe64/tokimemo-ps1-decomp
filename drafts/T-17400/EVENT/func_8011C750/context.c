#include "common.h"
#include "ovl/EVENT.h"

void func_8011C650(void) {
    D_801258C0 = 0x801C6000;
    D_801258C4 = 0x801AE000;
    D_801258C8 = 0x801B2000;
    D_801258CC = 0x801B6000;
    D_801258D0 = 0x801BA000;
    D_801258D4 = 0x801BE000;
    D_801258D8 = 0x801C2000;
}

void func_8011C6C4(void) {
    switch (*(u32 *)&D_80125320) {
    case 0:
        func_8011DDB8();
        return;
    case 1:
        func_8011EDFC();
        return;
    case 2:
        func_8011D5C4();
        return;
    case 3:
        func_8011C750();
        return;
    case 4:
        func_8011C750();
        return;
    }
}

/* bulk_m2c declarations (T-11190) */
extern s16 D_800F1992;
extern s16 D_800F1994;
extern s16 D_800F1996;
extern s16 D_800F199A;
extern s8 D_800F19A8;
extern s8 D_800F19A9;
extern s8 D_800F19AA;
/* end bulk_m2c declarations */
u16 func_8011C750(void) {
    s16 sp60;
    s16 sp5A;
    s16 sp58;
    s16 sp56;
    s16 sp54;
    s32 sp50;
    s32 sp4C;
    Rec24 *temp_v0;
    Rec24 *temp_v0_2;
    s16 temp_t9;
    s16 temp_t9_2;
    s16 var_v1;
    s16 var_v1_2;
    s16 var_v1_3;
    s16 var_v1_4;
    s16 var_v1_5;
    s16 var_v1_6;
    s16 var_v1_7;
    s16 var_v1_8;
    s16 var_v1_9;
    s32 temp_lo;
    s32 temp_lo_2;
    s32 temp_lo_3;
    s32 temp_lo_4;
    s32 temp_lo_5;
    s32 temp_lo_6;
    s32 temp_t5;
    s32 temp_t5_2;
    s32 temp_t5_3;
    s32 temp_t6_2;
    s32 temp_t7_3;
    s32 temp_t8;
    s32 var_at;
    u16 temp_t6;
    u8 *temp_t7;
    u8 *temp_t7_2;
    u8 temp_t9_3;
    u8 temp_v0_3;
    u8 temp_v0_4;
    u8 var_a0;
    Rec24 *temp_v0_5;
    Rec24 *temp_v0_6;
    Rec24 *temp_v0_7;
    Rec24 *temp_v0_8;

    switch (D_800B1AF0) {
    case 0:
        D_800EED08 = 1;
        func_8011F530(D_800B1AE4);
        func_800433D0(0x207);
        D_800F1996 = 0;
        D_800F1992 = 0;
        D_800F1994 = 0;
        *D_800F1990 = 0;
        D_800F19A9 = 0;
        D_800F19A8 = 0;
        D_80080710 = -1;
        D_800F199A = 0;
        D_800F19AA = 0;
        func_800352B4(0);
        D_801258E0 = 2;
        D_800B1AF9 = 0;
        D_801258DC = 0;
        var_v1 = 0;
        do {
            temp_t9 = (var_v1 * 2) + 0x15;
            temp_v0 = &((Rec24 *)D_800EC190)[var_v1];
            temp_v0[4].unk_08 = (s16) ((var_v1 << 6) + 0x80);
            temp_t5 = var_v1 << 7;
            temp_t7 = &D_800B0860[var_v1];
            var_v1 += 1;
            temp_v0[4].unk_00 = 0x82000000;
            temp_v0[4].unk_0A = 0;
            temp_v0[4].unk_0E = 0;
            temp_v0[4].unk_0F = 0xA0;
            temp_v0[4].unk_10 = 0;
            temp_v0[4].unk_0C = temp_t9;
            temp_v0[4].unk_12 = 0x1E0;
            temp_v0[4].unk_14 = 0;
            temp_v0[4].unk_15 = 0;
            temp_v0[4].unk_16 = 0;
            temp_v0[4].unk_18 = 0;
            temp_v0[4].unk_1A = 0;
            temp_v0[4].unk_1C = 0x1000;
            temp_v0[4].unk_1E = 0x1000;
            temp_v0[4].unk_20 = 0;
            temp_v0[4].unk_04 = (s16) (temp_t5 - 0xA0);
            temp_v0[4].unk_06 = -0x78;
            *(s8 *)((unsigned char *)temp_t7 + (4145)) = 0xB;
        } while (var_v1 < 2);
        var_v1_2 = 0;
        do {
            temp_t9_2 = (var_v1_2 * 2) + 0xA;
            temp_v0_2 = &((Rec24 *)D_800EC190)[var_v1_2];
            temp_v0_2[6].unk_08 = (s16) ((var_v1_2 << 6) + 0x80);
            temp_t5_2 = var_v1_2 << 7;
            temp_t7_2 = &D_800B0860[var_v1_2];
            var_v1_2 += 1;
            temp_v0_2[6].unk_00 = 0x82000000;
            temp_v0_2[6].unk_0A = 0xA0;
            temp_v0_2[6].unk_0E = 0;
            temp_v0_2[6].unk_0F = 0;
            temp_v0_2[6].unk_10 = 0;
            temp_v0_2[6].unk_0C = temp_t9_2;
            temp_v0_2[6].unk_12 = 0x1E0;
            temp_v0_2[6].unk_14 = 0;
            temp_v0_2[6].unk_15 = 0;
            temp_v0_2[6].unk_16 = 0;
            temp_v0_2[6].unk_18 = 0;
            temp_v0_2[6].unk_1A = 0;
            temp_v0_2[6].unk_1C = 0x1000;
            temp_v0_2[6].unk_1E = 0x1000;
            temp_v0_2[6].unk_20 = 0;
            temp_v0_2[6].unk_04 = (s16) (temp_t5_2 - 0xA0);
            temp_v0_2[6].unk_06 = -0x78;
            *(s8 *)((unsigned char *)temp_t7_2 + (4147)) = 0xB;
        } while (var_v1_2 < 2);
        func_8002746C(0x433B, 1, 0x1E0, D_800B0860);
        D_800B1AF0 += 1;
        break;
    case 1:
        if (func_80015A44() & 1) {
            func_8002469C(0x140, 0x100, 0x180, 0xA0);
            sp54 = 0x140;
            sp56 = 0x100;
            sp58 = 0x180;
            sp5A = 0xA0;
            func_800651F8(&sp54, 0x80183000);
            D_800B1AF0 += 1;
        }
        break;
    case 2:
        func_8002746C(0x4344, 1);
        D_800B1AF0 += 1;
        break;
    case 3:
        if (func_80015A44() & 1) {
            func_8002469C(0x280, 0, 0x180, 0xA0);
            sp54 = 0x280;
            sp56 = 0;
            sp58 = 0x180;
            sp5A = 0xA0;
            func_800651F8(&sp54, 0x801A1000);
            D_800B1AF0 += 1;
        }
        break;
    case 4:
        func_8002746C(0x434E, 1);
        D_800B1AF0 += 1;
        break;
    case 5:
        if (func_80015A44() & 1) {
            func_8002469C(0x140, 0x100, 0x180, 0xA0);
            sp54 = 0x140;
            sp56 = 0x100;
            sp58 = 0x180;
            sp5A = 0xA0;
            func_800651F8(&sp54, 0x801BF000);
            D_800B1AF0 += 1;
        }
        break;
    case 6:
        func_8002746C(0x4357, 1);
        D_800B1AF0 += 1;
        break;
    case 7:
        if (func_80015A44() & 1) {
            func_8002469C(0x280, 0, 0x180, 0xA0);
            sp54 = 0x280;
            sp56 = 0;
            sp58 = 0x180;
            sp5A = 0xA0;
            func_800651F8(&sp54, 0x801DD000);
            D_800B1AF0 += 1;
        }
        break;
    case 8:
        var_v1_3 = 0;
        do {
            temp_lo = var_v1_3 * 0x24;
            var_v1_3 += 1;
            *(s32 *)((unsigned char *)((Rec24 *)((u8 *)D_800EC190 + temp_lo)) + (144)) = 0x02000000;
        } while (var_v1_3 < 4);
        D_800B1AF0 += 1;
        break;
    case 9:
        temp_v0_3 = (*(u8 *)((unsigned char *)D_800EC190 + (164)) + 2) & 0xFF;
        *(u8 *)((unsigned char *)D_800EC190 + (274)) = temp_v0_3;
        *(u8 *)((unsigned char *)D_800EC190 + (273)) = temp_v0_3;
        *(u8 *)((unsigned char *)D_800EC190 + (272)) = temp_v0_3;
        *(u8 *)((unsigned char *)D_800EC190 + (238)) = temp_v0_3;
        *(u8 *)((unsigned char *)D_800EC190 + (237)) = temp_v0_3;
        *(u8 *)((unsigned char *)D_800EC190 + (236)) = temp_v0_3;
        *(u8 *)((unsigned char *)D_800EC190 + (202)) = temp_v0_3;
        *(u8 *)((unsigned char *)D_800EC190 + (201)) = temp_v0_3;
        *(u8 *)((unsigned char *)D_800EC190 + (200)) = temp_v0_3;
        *(u8 *)((unsigned char *)D_800EC190 + (166)) = temp_v0_3;
        *(u8 *)((unsigned char *)D_800EC190 + (165)) = temp_v0_3;
        *(u8 *)((unsigned char *)D_800EC190 + (164)) = temp_v0_3;
        if (temp_v0_3 == 0x80) {
            D_800B1AF0 += 1;
        }
        break;
    case 11:
        temp_v0_4 = (*(u8 *)((unsigned char *)D_800EC190 + (164)) - 2) & 0xFF;
        *(u8 *)((unsigned char *)D_800EC190 + (274)) = temp_v0_4;
        *(u8 *)((unsigned char *)D_800EC190 + (273)) = temp_v0_4;
        *(u8 *)((unsigned char *)D_800EC190 + (272)) = temp_v0_4;
        *(u8 *)((unsigned char *)D_800EC190 + (238)) = temp_v0_4;
        *(u8 *)((unsigned char *)D_800EC190 + (237)) = temp_v0_4;
        *(u8 *)((unsigned char *)D_800EC190 + (236)) = temp_v0_4;
        *(u8 *)((unsigned char *)D_800EC190 + (202)) = temp_v0_4;
        *(u8 *)((unsigned char *)D_800EC190 + (201)) = temp_v0_4;
        *(u8 *)((unsigned char *)D_800EC190 + (200)) = temp_v0_4;
        *(u8 *)((unsigned char *)D_800EC190 + (166)) = temp_v0_4;
        *(u8 *)((unsigned char *)D_800EC190 + (165)) = temp_v0_4;
        *(u8 *)((unsigned char *)D_800EC190 + (164)) = temp_v0_4;
        if (temp_v0_4 == 0) {
            var_v1_4 = 0;
            do {
                temp_lo_2 = var_v1_4 * 0x24;
                var_v1_4 += 1;
                *(s32 *)((unsigned char *)((Rec24 *)((u8 *)D_800EC190 + temp_lo_2)) + (144)) = 0x82000000;
            } while (var_v1_4 < 4);
            D_800B1AF0 += 1;
        }
        break;
    case 12:
        sp50 = (s32) D_800B1AF6;
        sp4C = D_800B1AF0;
        func_8004AFB8();
        D_800B1AF0 = sp4C;
        if (sp50 != D_800B1AF6) {
            D_800B1AF0 = sp4C + 1;
            D_800B1AF6 -= 1;
        }
        break;
    case 13:
    case 16:
    case 19:
    case 21:
    case 23:
        if (func_80015A44() & 1) {
            D_800B1AF0 += 1;
        }
        break;
    case 14:
        sp4C = D_800B1AF0;
        func_8004A9CC();
        D_800B1AF6 -= 1;
        D_800B1AF0 = sp4C + 1;
        break;
    case 15:
        switch (D_80125320) {
        case 0:
            func_800469F4(0x42AE);
            break;
        case 1:
            func_800469F4(0x42B7);
            break;
        case 2:
            func_800469F4(0x42C0);
            break;
        case 3:
            func_800469F4(0x42C9);
            break;
        case 4:
            func_800469F4(0x42D2);
            break;
        }
        D_800B1AF0 += 1;
        break;
    case 17:
        func_8002755C(1);
        func_80027438(1);
        D_800B1AF0 += 1;
        break;
    case 18:
        sp4C = D_800B1AF0;
        func_8011A62C();
        D_800B1AF6 -= 1;
        D_800B1AF0 = sp4C + 1;
        break;
    case 20:
        sp4C = D_800B1AF0;
        func_8011A7FC();
        D_800B1AF6 -= 1;
        D_800B1AF0 = sp4C + 1;
        break;
    case 22:
        sp4C = D_800B1AF0;
        func_8011A838();
        D_800B1AF6 -= 1;
        D_800B1AF0 = sp4C + 1;
        break;
    default:
        func_800352B4(1);
        D_800EED08 = 0;
        func_800433D0(0x206);
        func_80011DFC();
        break;
    }
    if (((u32) D_800B1AF0 < 9U) || ((u32) D_800B1AF0 >= 0xCU)) {
        return 0U;
    }
    var_v1_5 = ((Rec24 *)D_800EC190)->unk0 + D_801258E0;
    var_at = var_v1_5 < 0x40;
    if (var_v1_5 < 0) {
        sp60 = 0;
        var_v1_5 = sp60;
        D_801258E0 = (func_800789A0() % 4) + 2;
        var_at = var_v1_5 < 0x40;
    }
    if (var_at == 0) {
        sp60 = 0x3F;
        var_v1_5 = 0x3F;
        D_801258E0 = -2 - (func_800789A0() % 4);
    }
    var_a0 = *(u8 *)((unsigned char *)D_800EC190 + (159));
    *(s8 *)((unsigned char *)D_800EC190 + (266)) = (s8) var_v1_5;
    *(s8 *)((unsigned char *)D_800EC190 + (230)) = (s8) var_v1_5;
    *(s8 *)((unsigned char *)D_800EC190 + (194)) = (s8) var_v1_5;
    *(s8 *)((unsigned char *)D_800EC190 + (158)) = (s8) var_v1_5;
    if (var_a0 != 0) {
        *(u16 *)((unsigned char *)D_800EC190 + (154)) = (u16) (*(u16 *)((unsigned char *)D_800EC190 + (154)) + 0x14);
        *(u16 *)((unsigned char *)D_800EC190 + (190)) = (u16) (*(u16 *)((unsigned char *)D_800EC190 + (190)) + 0x14);
        var_a0 = (var_a0 - 0x14) & 0xFF;
        *(u8 *)((unsigned char *)D_800EC190 + (195)) = (u8) (*(u8 *)((unsigned char *)D_800EC190 + (195)) - 0x14);
    } else {
        *(u16 *)((unsigned char *)D_800EC190 + (154)) = (u16) (*(u16 *)((unsigned char *)D_800EC190 + (154)) - 0x14);
        *(s16 *)((unsigned char *)D_800EC190 + (150)) = (s16) (*(s16 *)((unsigned char *)D_800EC190 + (150)) + 0x14);
        *(u16 *)((unsigned char *)D_800EC190 + (190)) = (u16) (*(u16 *)((unsigned char *)D_800EC190 + (190)) - 0x14);
        *(s16 *)((unsigned char *)D_800EC190 + (186)) = (s16) (*(s16 *)((unsigned char *)D_800EC190 + (186)) + 0x14);
    }
    if (*(u8 *)((unsigned char *)D_800EC190 + (231)) != 0) {
        *(u16 *)((unsigned char *)D_800EC190 + (226)) = (u16) (*(u16 *)((unsigned char *)D_800EC190 + (226)) + 0x14);
        *(u16 *)((unsigned char *)D_800EC190 + (262)) = (u16) (*(u16 *)((unsigned char *)D_800EC190 + (262)) + 0x14);
        *(u8 *)((unsigned char *)D_800EC190 + (231)) = (u8) (*(u8 *)((unsigned char *)D_800EC190 + (231)) - 0x14);
        *(u8 *)((unsigned char *)D_800EC190 + (267)) = (u8) (*(u8 *)((unsigned char *)D_800EC190 + (267)) - 0x14);
    } else {
        ((Rec24 *)D_800EC190)->unk0 = (s16) ((u16) ((Rec24 *)D_800EC190)->unk0 - 0x14);
        *(s16 *)((unsigned char *)D_800EC190 + (222)) = (s16) (*(s16 *)((unsigned char *)D_800EC190 + (222)) + 0x14);
        *(u16 *)((unsigned char *)D_800EC190 + (262)) = (u16) (*(u16 *)((unsigned char *)D_800EC190 + (262)) - 0x14);
        *(s16 *)((unsigned char *)D_800EC190 + (258)) = (s16) (*(s16 *)((unsigned char *)D_800EC190 + (258)) + 0x14);
    }
    *(u8 *)((unsigned char *)D_800EC190 + (159)) = var_a0;
    if (((var_a0 == 0) && ((D_800B1AF9 == 1) || (D_800B1AF9 == 3))) || ((*(u8 *)((unsigned char *)D_800EC190 + (231)) == 0) && ((D_800B1AF9 == 0) || (D_800B1AF9 == 2)))) {
        var_v1_6 = 0;
        if (D_800B1AF9 != 0) {
            var_v1_7 = 0;
            if (D_800B1AF9 != 1) {
                var_v1_8 = 0;
                if (D_800B1AF9 != 2) {
                    var_v1_9 = 0;
                    if (D_800B1AF9 != 3) {

                    } else {
                        do {
                            temp_lo_3 = var_v1_9 * 0x24;
                            temp_t8 = var_v1_9 * 2;
                            var_v1_9 += 1;
                            temp_v0_5 = (Rec24 *)((u8 *)D_800EC190 + temp_lo_3);
                            temp_v0_5[6].unk_0A = 0;
                            temp_v0_5[6].unk_0F = 0xA0;
                            temp_v0_5[6].unk_06 = -0x78;
                            temp_v0_5[6].unk_0C = (s16) (temp_t8 + 0xA);
                        } while (var_v1_9 < 2);
                        func_800137AC(0x280, 0, 0x180, 0xA0, 0x801DD000);
                    }
                } else {
                    do {
                        temp_lo_4 = var_v1_8 * 0x24;
                        temp_t5_3 = var_v1_8 * 2;
                        var_v1_8 += 1;
                        temp_v0_6 = (Rec24 *)((u8 *)D_800EC190 + temp_lo_4);
                        temp_v0_6[4].unk_0A = 0;
                        temp_v0_6[4].unk_0F = 0xA0;
                        temp_v0_6[4].unk_06 = -0x78;
                        temp_v0_6[4].unk_0C = (s16) (temp_t5_3 + 0x15);
                    } while (var_v1_8 < 2);
                    func_800137AC(0x140, 0x100, 0x180, 0xA0, 0x801BF000);
                }
            } else {
                do {
                    temp_lo_5 = var_v1_7 * 0x24;
                    temp_t6_2 = var_v1_7 * 2;
                    var_v1_7 += 1;
                    temp_v0_7 = (Rec24 *)((u8 *)D_800EC190 + temp_lo_5);
                    temp_v0_7[6].unk_0A = 0;
                    temp_v0_7[6].unk_0F = 0xA0;
                    temp_v0_7[6].unk_06 = -0x78;
                    temp_v0_7[6].unk_0C = (s16) (temp_t6_2 + 0xA);
                } while (var_v1_7 < 2);
                func_800137AC(0x280, 0, 0x180, 0xA0, 0x801A1000);
            }
        } else {
            do {
                temp_lo_6 = var_v1_6 * 0x24;
                temp_t7_3 = var_v1_6 * 2;
                var_v1_6 += 1;
                temp_v0_8 = (Rec24 *)((u8 *)D_800EC190 + temp_lo_6);
                temp_v0_8[4].unk_0A = 0;
                temp_v0_8[4].unk_0F = 0xA0;
                temp_v0_8[4].unk_06 = -0x78;
                temp_v0_8[4].unk_0C = (s16) (temp_t7_3 + 0x15);
            } while (var_v1_6 < 2);
            func_800137AC(0x140, 0x100, 0x180, 0xA0, 0x80183000);
        }
        temp_t9_3 = D_800B1AF9 + 1;
        D_800B1AF9 = temp_t9_3;
        if ((u32) (temp_t9_3 & 0xFF) >= 4U) {
            D_800B1AF9 = 0;
        }
    }
    temp_t6 = (D_801258DC + 1) & 0xFFFF;
    D_801258DC = temp_t6;
    if (temp_t6 == 0x12C) {
        D_800B1AF0 += 1;
    }
    return temp_t6;
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/8011C650", func_8011D5C4);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/8011C650", func_8011DDB8);

/* T-10040: the implicit result view matches DATE func_80148924. */
s32 func_8011ED50(s16 arg0) {
    ((Rec24 *)D_800EC190)[4].unk_06 += arg0;
    ((Rec24 *)D_800EC190)[5].unk_06 += arg0;
    ((Rec24 *)D_800EC190)[4].unk_0A -= arg0;
    ((Rec24 *)D_800EC190)[5].unk_0A -= arg0;
    ((Rec24 *)D_800EC190)[6].unk_0A += arg0;
    ((Rec24 *)D_800EC190)[7].unk_0A += arg0;
    ((Rec24 *)D_800EC190)[6].unk_0F -= arg0;
    ((Rec24 *)D_800EC190)[7].unk_0F -= arg0;
}

/* NON_MATCHING: T-10040, 13-line initialization/data-ownership residual. */
#ifdef NON_MATCHING

void func_8011EDFC(void) {
    s16 *temp_v0;
    s16 *var_a0;
    s16 temp_v0_2;
    s16 var_a1;
    s16 var_s3;
    s32 remainder;
    s32 temp_s1;
    s32 temp_s1_3;
    u8 temp_s0;
    u8 temp_s0_2;

    switch (((EventTransitionState *)&D_800B1AF0)->unk_00) {
    case 0:
        D_800EED08 = 1;
        func_8011F530(D_800B1AE4);
        func_800433D0(0x202);
        D_801258DC = 0;
        func_800352B4(0);
        func_800469F4(0x432A);
        D_801258E4[0] = -0x50;
        D_801258E4[1] = 0;
        D_801258E4[2] = 0x64;
        D_801258EC[0] = -0x28;
        D_801258EC[1] = 0;
        D_801258EC[2] = -0x3C;
        ((EventTransitionState *)&D_800B1AF0)->unk_00 += 1;
        D_801258F4[0] = (func_800789A0() % 3) - 1;
        D_801258F4[1] = (func_800789A0() % 3) - 1;
        D_801258F4[2] = (func_800789A0() % 3) - 1;
        D_801258FC[0] = (func_800789A0() % 3) - 1;
        D_801258FC[1] = (func_800789A0() % 3) - 1;
        D_801258FC[2] = (func_800789A0() % 3) - 1;
        break;
    case 1:
        if (func_80015A44() & 1) {
            ((EventTransitionState *)&D_800B1AF0)->unk_00 += 1;
            break;
        }
        break;
    case 2:
        func_8002755C(1);
        func_80027438(1);
        ((EventTransitionState *)&D_800B1AF0)->unk_00 += 1;
        break;
    case 3:
        temp_s0 = ((EventTransitionState *)&D_800B1AF0)->unk_06;
        temp_s1 = ((EventTransitionState *)&D_800B1AF0)->unk_00;
        func_80046480();
        ((EventTransitionState *)&D_800B1AF0)->unk_00 = temp_s1;
        if (temp_s0 != ((EventTransitionState *)&D_800B1AF0)->unk_06) {
            ((EventTransitionState *)&D_800B1AF0)->unk_00 = temp_s1 + 1;
            func_800433D0(0x201);
        }
        ((EventTransitionState *)&D_800B1AF0)->unk_06 = temp_s0;
        break;
    case 4:
        var_s3 = 0;
        do {
            if (((u32) D_800B1AE4 % 30U) == (func_800789A0() % 30)) {
                (D_801258F4)[var_s3] = (func_800789A0() % 3) - 1;
            }

            if (((u32) D_800B1AE4 % 30U) == (func_800789A0() % 30)) {
                (D_801258FC)[var_s3] = (func_800789A0() % 3) - 1;
            }

            if ((D_801258F4[var_s3] == 0) && (D_801258FC[var_s3] == 0)) {
                if ((remainder = func_800789A0() % 2)) {
                    D_801258F4[var_s3] = ((func_800789A0() % 2) * 2) - 1;
                } else {
                    D_801258FC[var_s3] = ((func_800789A0() % 2) * 2) - 1;
                }
            }
            temp_v0 = &(D_801258E4)[var_s3];
            *temp_v0 += D_801258F4[var_s3];
            var_a0 = &(D_801258EC)[var_s3];
            *var_a0 += D_801258FC[var_s3];
            var_a1 = *temp_v0;
            if (var_a1 < -0x95) {
                D_801258F4[var_s3] = 1;
            }
            if (var_a1 >= 0x96) {
                D_801258F4[var_s3] = -1;
            }
            temp_v0_2 = *var_a0;
            if (temp_v0_2 < -0x6D) {
                D_801258FC[var_s3] = 1;
            }

            if (temp_v0_2 >= 0x1E) {
                D_801258FC[var_s3] = -1;

            }
            var_s3++;
        } while (var_s3 < 3);
        func_8001BB10(D_801258E4[0], D_801258EC[0], 0xA, 2, 2, 0x808080, 1, 1);
        func_8001BB10(D_801258E4[1], D_801258EC[1], 0xA, 2, 2, 0x804040, 1, 1);
        func_8001BB10(D_801258E4[2], D_801258EC[2], 0xA, 2, 2, 0x800000, 1, 1);
        if (D_801258DC++ == 0x12C) {
            ((EventTransitionState *)&D_800B1AF0)->unk_00 += 1;
            func_800433D0(0x202);
            break;
        }
        break;
    case 5:
        temp_s0_2 = ((EventTransitionState *)&D_800B1AF0)->unk_06;
        temp_s1_3 = ((EventTransitionState *)&D_800B1AF0)->unk_00;
        func_80046598();
        ((EventTransitionState *)&D_800B1AF0)->unk_00 = temp_s1_3;
        if (temp_s0_2 != ((EventTransitionState *)&D_800B1AF0)->unk_06) {
            ((EventTransitionState *)&D_800B1AF0)->unk_00 = temp_s1_3 + 1;
        }
        ((EventTransitionState *)&D_800B1AF0)->unk_06 = temp_s0_2;
        break;
    case 6:
        func_80027438(1);
        ((EventTransitionState *)&D_800B1AF0)->unk_00 += 1;
        break;
    default:
        func_800433D0(0x206);
        D_800EED08 = 0;
        func_800352B4(1);
        func_80011DFC();
        break;
    }
}
#else
INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/8011C650", func_8011EDFC);
#endif
