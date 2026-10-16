extern u8 D_800EC234;
extern u8 D_800EC235;
extern u8 D_800EC236;
extern u8 D_800EC258;
extern u8 D_800EC259;
extern u8 D_800EC25A;
extern u8 D_800EC27C;
extern u8 D_800EC27D;
extern u8 D_800EC27E;
extern u8 D_800EC2A0;
extern u8 D_800EC2A1;
extern u8 D_800EC2A2;
extern s16 D_800F1992;
extern s16 D_800F1994;
extern s16 D_800F1996;
extern s16 D_800F199A;
extern s8 D_800F19A8;
extern s8 D_800F19A9;
extern s8 D_800F19AA;

void func_8011DDB8(void) {
    s32 sp54;
    s32 sp50;
    s16 sp4E;
    s16 sp4C;
    s16 sp4A;
    s16 sp48;
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
    s32 var_a3;
    u16 temp_a1;
    u16 temp_t5;
    u16 temp_t8_3;
    u16 temp_t8_4;
    u8 *temp_t6;
    u8 *temp_t6_2;
    u8 *temp_v0_10;
    u8 *temp_v0_11;
    u8 *temp_v0_12;
    u8 *temp_v0_13;
    u8 *temp_v0_14;
    u8 *temp_v0_15;
    u8 *temp_v0_3;
    u8 *temp_v0_4;
    u8 *temp_v0_5;
    u8 *temp_v0_6;
    u8 *temp_v0_7;
    u8 *temp_v0_8;
    u8 *temp_v0_9;
    u8 temp_v1;
    u8 temp_v1_2;
    u8 temp_v1_3;
    void *temp_v0;
    void *temp_v0_2;

    var_a3 = D_800B1AF0;
    switch (var_a3) {                               /* switch 1 */
    case 0:                                         /* switch 1 */
        D_800EED08 = 1;
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
        D_801258DC = 0;
        var_v1 = 0;
        do {
            temp_t8 = (var_v1 * 2) + 0x15;
            temp_v0 = D_800EC190 + (var_v1 * 0x24);
            *(s16 *)((unsigned char *)temp_v0 + (152)) = (s16) ((var_v1 << 6) + 0x80);
            temp_t9 = var_v1 << 7;
            temp_t6 = &D_800B0860[var_v1];
            var_v1 += 1;
            *(s32 *)((unsigned char *)temp_v0 + (144)) = 0x82000000;
            *(s16 *)((unsigned char *)temp_v0 + (154)) = 0xA0;
            *(s8 *)((unsigned char *)temp_v0 + (158)) = 0;
            *(s8 *)((unsigned char *)temp_v0 + (159)) = 0;
            *(s16 *)((unsigned char *)temp_v0 + (160)) = 0;
            *(s16 *)((unsigned char *)temp_v0 + (156)) = temp_t8;
            *(s16 *)((unsigned char *)temp_v0 + (162)) = 0x1E0;
            *(s8 *)((unsigned char *)temp_v0 + (164)) = 0;
            *(s8 *)((unsigned char *)temp_v0 + (165)) = 0;
            *(s8 *)((unsigned char *)temp_v0 + (166)) = 0;
            *(s16 *)((unsigned char *)temp_v0 + (168)) = 0;
            *(s16 *)((unsigned char *)temp_v0 + (170)) = 0;
            *(s16 *)((unsigned char *)temp_v0 + (172)) = 0x1000;
            *(s16 *)((unsigned char *)temp_v0 + (174)) = 0x1000;
            *(s32 *)((unsigned char *)temp_v0 + (176)) = 0;
            *(s16 *)((unsigned char *)temp_v0 + (148)) = (s16) (temp_t9 - 0xA0);
            *(s16 *)((unsigned char *)temp_v0 + (150)) = -0x78;
            *(s8 *)((unsigned char *)temp_t6 + (4145)) = 0xB;
        } while (var_v1 < 2);
        var_v1_2 = 0;
        do {
            temp_t8_2 = (var_v1_2 * 2) + 0xA;
            temp_v0_2 = D_800EC190 + (var_v1_2 * 0x24);
            *(s16 *)((unsigned char *)temp_v0_2 + (224)) = (s16) ((var_v1_2 << 6) + 0x80);
            temp_t9_2 = var_v1_2 << 7;
            temp_t6_2 = &D_800B0860[var_v1_2];
            var_v1_2 += 1;
            *(s32 *)((unsigned char *)temp_v0_2 + (216)) = 0x82000000;
            *(s16 *)((unsigned char *)temp_v0_2 + (226)) = 0;
            *(s8 *)((unsigned char *)temp_v0_2 + (230)) = 0;
            *(s8 *)((unsigned char *)temp_v0_2 + (231)) = 0xA0;
            *(s16 *)((unsigned char *)temp_v0_2 + (232)) = 0;
            *(s16 *)((unsigned char *)temp_v0_2 + (228)) = temp_t8_2;
            *(s16 *)((unsigned char *)temp_v0_2 + (234)) = 0x1E0;
            *(s8 *)((unsigned char *)temp_v0_2 + (236)) = 0;
            *(s8 *)((unsigned char *)temp_v0_2 + (237)) = 0;
            *(s8 *)((unsigned char *)temp_v0_2 + (238)) = 0;
            *(s16 *)((unsigned char *)temp_v0_2 + (240)) = 0;
            *(s16 *)((unsigned char *)temp_v0_2 + (242)) = 0;
            *(s16 *)((unsigned char *)temp_v0_2 + (244)) = 0x1000;
            *(s16 *)((unsigned char *)temp_v0_2 + (246)) = 0x1000;
            *(s32 *)((unsigned char *)temp_v0_2 + (248)) = 0;
            *(s16 *)((unsigned char *)temp_v0_2 + (220)) = (s16) (temp_t9_2 - 0xA0);
            *(s16 *)((unsigned char *)temp_v0_2 + (222)) = -0x78;
            *(s8 *)((unsigned char *)temp_t6_2 + (4147)) = 0xB;
        } while (var_v1_2 < 2);
        func_8002746C(0x4321, 1, 0xA0, -0x78);
        D_800B1AF0 += 1;
        return;
    case 1:                                         /* switch 1 */
    case 4:                                         /* switch 1 */
    case 7:                                         /* switch 1 */
    case 10:                                        /* switch 1 */
    case 13:                                        /* switch 1 */
    case 16:                                        /* switch 1 */
    case 19:                                        /* switch 1 */
    case 27:                                        /* switch 1 */
    case 41:                                        /* switch 1 */
    case 44:                                        /* switch 1 */
    case 47:                                        /* switch 1 */
    case 49:                                        /* switch 1 */
        if (func_80015A44() & 1) {
            D_800B1AF0 += 1;
            return;
        }
        return;
    case 2:                                         /* switch 1 */
        func_8002469C(0x140, 0x100, 0x140, 0xA0);
        sp48 = 0x140;
        sp4A = 0x100;
        sp4C = 0x140;
        sp4E = 0xA0;
        func_800651F8(&sp48, 0x80183000);
        D_800B1AF0 += 1;
        return;
    case 3:                                         /* switch 1 */
        func_8002746C(0x431A, 1);
        D_800B1AF0 += 1;
        return;
    case 5:                                         /* switch 1 */
        func_8002469C(0x140, 0x100, 0x140, 0xA0);
        sp48 = 0x140;
        sp4A = 0x100;
        sp4C = 0x140;
        sp4E = 0xA0;
        func_800651F8(&sp48, 0x8019C000);
        D_800B1AF0 += 1;
        return;
    case 6:                                         /* switch 1 */
        func_8002746C(0x4313, 1);
        D_800B1AF0 += 1;
        return;
    case 8:                                         /* switch 1 */
        func_8002469C(0x140, 0x100, 0x140, 0xA0);
        sp48 = 0x140;
        sp4A = 0x100;
        sp4C = 0x140;
        sp4E = 0xA0;
        func_800651F8(&sp48, 0x801B5000);
        D_800B1AF0 += 1;
        return;
    case 9:                                         /* switch 1 */
        func_8002746C(0x430C, 1);
        D_800B1AF0 += 1;
        return;
    case 11:                                        /* switch 1 */
        func_8002469C(0x140, 0x100, 0x140, 0xA0);
        sp48 = 0x140;
        sp4A = 0x100;
        sp4C = 0x140;
        sp4E = 0xA0;
        func_800651F8(&sp48, 0x801CE000);
        D_800B1AF0 += 1;
        return;
    case 12:                                        /* switch 1 */
        func_8002746C(0x4305, 1);
        D_800B1AF0 += 1;
        return;
    case 14:                                        /* switch 1 */
        func_8002469C(0x140, 0, 0x140, 0xA0);
        D_800B1AF0 += 1;
        return;
    case 15:                                        /* switch 1 */
        func_8002746C(0x42ED, 1);
        D_800B1AF0 += 1;
        return;
    case 17:                                        /* switch 1 */
        func_8002469C(0x140, 0x100, 0x140, 0xA0);
        D_800B1AF0 += 1;
        return;
    case 18:                                        /* switch 1 */
        func_8002746C(0x42F5, 1);
        D_800B1AF0 += 1;
        return;
    case 20:                                        /* switch 1 */
        func_8002469C(0x280, 0, 0x140, 0xA0);
        D_800B1AF0 += 1;
        return;
    case 21:                                        /* switch 1 */
        var_v1_3 = 0;
        do {
            temp_lo = var_v1_3 * 0x24;
            var_v1_3 += 1;
            temp_v0_3 = &D_800EC190[temp_lo];
            *(s32 *)((unsigned char *)temp_v0_3 + (144)) = (s32) (*(s32 *)((unsigned char *)temp_v0_3 + (144)) & 0x7FFFFFFF);
        } while (var_v1_3 < 4);
        D_800B1AF0 = var_a3 + 1;
        return;
    case 22:                                        /* switch 1 */
        temp_v1 = (D_800EC234 + 2) & 0xFF;
        D_800EC2A2 = temp_v1;
        D_800EC2A1 = temp_v1;
        D_800EC2A0 = temp_v1;
        D_800EC27E = temp_v1;
        D_800EC27D = temp_v1;
        D_800EC27C = temp_v1;
        D_800EC25A = temp_v1;
        D_800EC259 = temp_v1;
        D_800EC258 = temp_v1;
        D_800EC236 = temp_v1;
        D_800EC235 = temp_v1;
        D_800EC234 = temp_v1;
        D_800B1AF0 = var_a3;
        if (temp_v1 == 0x80) {
            D_800B1AF0 = var_a3 + 1;
            func_800433D0(0x500);
            return;
        }
        break;
    case 23:                                        /* switch 1 */
        temp_a1 = D_801258DC;
        D_800B1AF0 = var_a3;
        if (!(temp_a1 & 7)) {
            func_8011ED50(4);
        }
        temp_t8_3 = (temp_a1 + 1) & 0xFFFF;
        D_801258DC = temp_t8_3;
        if (temp_t8_3 == 0x140) {
            D_801258DC = 0;
            D_800B1AF0 += 1;
            return;
        }
        break;
    case 24:                                        /* switch 1 */
        D_800B1AF0 = var_a3 + 1;
        return;
    case 25:                                        /* switch 1 */
        func_80012D40(0);
        var_v1_4 = 0;
        do {
            temp_lo_2 = var_v1_4 * 0x24;
            var_v1_4 += 1;
            temp_v0_4 = &D_800EC190[temp_lo_2];
            *(s32 *)((unsigned char *)temp_v0_4 + (144)) = (s32) (*(s32 *)((unsigned char *)temp_v0_4 + (144)) | 0x80000000);
        } while (var_v1_4 < 4);
        D_800B1AF0 += 1;
        return;
    case 26:                                        /* switch 1 */
        func_8002746C(0x42FD, 1);
        D_800B1AF0 += 1;
        return;
    case 28:                                        /* switch 1 */
        func_8002469C(0x140, 0x100, 0x140, 0xA0);
        sp48 = 0x140;
        sp4A = 0;
        sp4C = 0x140;
        sp4E = 0xA0;
        func_80065234(&sp48, 0x280, 0);
        D_800B1AF0 += 1;
        func_80012D40(1);
        return;
    case 29:                                        /* switch 1 */
        var_v1_5 = 0;
        do {
            temp_lo_3 = var_v1_5 * 0x24;
            var_v1_5 += 1;
            temp_v0_5 = &D_800EC190[temp_lo_3];
            *(s32 *)((unsigned char *)temp_v0_5 + (144)) = 0x02000000;
            *(s16 *)((unsigned char *)temp_v0_5 + (154)) = 0xA0;
            *(s8 *)((unsigned char *)temp_v0_5 + (159)) = 0;
            *(s16 *)((unsigned char *)temp_v0_5 + (150)) = -0x78;
        } while (var_v1_5 < 2);
        var_v1_6 = 0;
        do {
            temp_lo_4 = var_v1_6 * 0x24;
            var_v1_6 += 1;
            temp_v0_6 = &D_800EC190[temp_lo_4];
            *(s32 *)((unsigned char *)temp_v0_6 + (216)) = 0x02000000;
            *(s16 *)((unsigned char *)temp_v0_6 + (226)) = 0;
            *(s8 *)((unsigned char *)temp_v0_6 + (231)) = 0xA0;
            *(s16 *)((unsigned char *)temp_v0_6 + (222)) = -0x78;
        } while (var_v1_6 < 2);
        D_801258DC = 0;
        func_800433D0(0x501);
        D_800B1AF0 += 1;
        return;
    case 30:                                        /* switch 1 */
    case 32:                                        /* switch 1 */
    case 34:                                        /* switch 1 */
        func_8011ED50(8);
        temp_t8_4 = (D_801258DC + 1) & 0xFFFF;
        D_801258DC = temp_t8_4;
        if (temp_t8_4 == 0x14) {
            D_800B1AF0 += 1;
            return;
        }
        break;
    case 31:                                        /* switch 1 */
        sp48 = 0x280;
        sp4A = 0;
        sp4C = 0x140;
        sp4E = 0xA0;
        func_80065234(&sp48, 0x140, 0x100, var_a3);
        func_800137AC(0x280, 0, 0x140, 0xA0, 0x801CE000);
        var_v1_7 = 0;
        do {
            temp_lo_5 = var_v1_7 * 0x24;
            var_v1_7 += 1;
            temp_v0_7 = &D_800EC190[temp_lo_5];
            *(s16 *)((unsigned char *)temp_v0_7 + (154)) = 0xA0;
            *(s8 *)((unsigned char *)temp_v0_7 + (159)) = 0;
            *(s16 *)((unsigned char *)temp_v0_7 + (150)) = -0x78;
        } while (var_v1_7 < 2);
        var_v1_8 = 0;
        do {
            temp_lo_6 = var_v1_8 * 0x24;
            var_v1_8 += 1;
            temp_v0_8 = &D_800EC190[temp_lo_6];
            *(s16 *)((unsigned char *)temp_v0_8 + (226)) = 0;
            *(s8 *)((unsigned char *)temp_v0_8 + (231)) = 0xA0;
            *(s16 *)((unsigned char *)temp_v0_8 + (222)) = -0x78;
        } while (var_v1_8 < 2);
        D_801258DC = 0;
        D_800B1AF0 += 1;
        return;
    case 33:                                        /* switch 1 */
        sp48 = 0x280;
        sp4A = 0;
        sp4C = 0x140;
        sp4E = 0xA0;
        func_80065234(&sp48, 0x140, 0x100, var_a3);
        func_800137AC(0x280, 0, 0x140, 0xA0, 0x801B5000);
        var_v1_9 = 0;
        do {
            temp_lo_7 = var_v1_9 * 0x24;
            var_v1_9 += 1;
            temp_v0_9 = &D_800EC190[temp_lo_7];
            *(s16 *)((unsigned char *)temp_v0_9 + (154)) = 0xA0;
            *(s8 *)((unsigned char *)temp_v0_9 + (159)) = 0;
            *(s16 *)((unsigned char *)temp_v0_9 + (150)) = -0x78;
        } while (var_v1_9 < 2);
        var_v1_10 = 0;
        do {
            temp_lo_8 = var_v1_10 * 0x24;
            var_v1_10 += 1;
            temp_v0_10 = &D_800EC190[temp_lo_8];
            *(s16 *)((unsigned char *)temp_v0_10 + (226)) = 0;
            *(s8 *)((unsigned char *)temp_v0_10 + (231)) = 0xA0;
            *(s16 *)((unsigned char *)temp_v0_10 + (222)) = -0x78;
        } while (var_v1_10 < 2);
        D_801258DC = 0;
        D_800B1AF0 += 1;
        return;
    case 35:                                        /* switch 1 */
        sp48 = 0x280;
        sp4A = 0;
        sp4C = 0x140;
        sp4E = 0xA0;
        func_80065234(&sp48, 0x140, 0x100, var_a3);
        func_800137AC(0x280, 0, 0x140, 0xA0, 0x8019C000);
        var_v1_11 = 0;
        do {
            temp_lo_9 = var_v1_11 * 0x24;
            var_v1_11 += 1;
            temp_v0_11 = &D_800EC190[temp_lo_9];
            *(s16 *)((unsigned char *)temp_v0_11 + (154)) = 0xA0;
            *(s8 *)((unsigned char *)temp_v0_11 + (159)) = 0;
            *(s16 *)((unsigned char *)temp_v0_11 + (150)) = -0x78;
        } while (var_v1_11 < 2);
        var_v1_12 = 0;
        do {
            temp_lo_10 = var_v1_12 * 0x24;
            var_v1_12 += 1;
            temp_v0_12 = &D_800EC190[temp_lo_10];
            *(s16 *)((unsigned char *)temp_v0_12 + (226)) = 0;
            *(s8 *)((unsigned char *)temp_v0_12 + (231)) = 0xA0;
            *(s16 *)((unsigned char *)temp_v0_12 + (222)) = -0x78;
        } while (var_v1_12 < 2);
        D_801258DC = 0;
        D_800B1AF0 += 1;
        return;
    case 36:                                        /* switch 1 */
        func_8011ED50(8);
        if ((u16) D_801258DC >= 8U) {
            temp_v1_2 = D_800EC234 - 4;
            D_800EC2A2 = temp_v1_2;
            D_800EC2A1 = temp_v1_2;
            D_800EC2A0 = temp_v1_2;
            D_800EC27E = temp_v1_2;
            D_800EC27D = temp_v1_2;
            D_800EC27C = temp_v1_2;
            D_800EC25A = temp_v1_2;
            D_800EC259 = temp_v1_2;
            D_800EC258 = temp_v1_2;
            D_800EC236 = temp_v1_2;
            D_800EC235 = temp_v1_2;
            D_800EC234 = temp_v1_2;
        }
        temp_t5 = (D_801258DC + 1) & 0xFFFF;
        D_801258DC = temp_t5;
        if (temp_t5 == 0x14) {
            D_800B1AF0 += 1;
            return;
        }
        break;
    case 37:                                        /* switch 1 */
        sp48 = 0x280;
        sp4A = 0;
        sp4C = 0x140;
        sp4E = 0xA0;
        func_80065234(&sp48, 0x140, 0x100, var_a3);
        func_800137AC(0x280, 0, 0x140, 0xA0, 0x80183000);
        var_v1_13 = 0;
        do {
            temp_lo_11 = var_v1_13 * 0x24;
            var_v1_13 += 1;
            temp_v0_13 = &D_800EC190[temp_lo_11];
            *(s16 *)((unsigned char *)temp_v0_13 + (154)) = 0xA0;
            *(s8 *)((unsigned char *)temp_v0_13 + (159)) = 0;
            *(s16 *)((unsigned char *)temp_v0_13 + (150)) = -0x78;
        } while (var_v1_13 < 2);
        var_v1_14 = 0;
        do {
            temp_lo_12 = var_v1_14 * 0x24;
            var_v1_14 += 1;
            temp_v0_14 = &D_800EC190[temp_lo_12];
            *(s16 *)((unsigned char *)temp_v0_14 + (226)) = 0;
            *(s8 *)((unsigned char *)temp_v0_14 + (231)) = 0xA0;
            *(s16 *)((unsigned char *)temp_v0_14 + (222)) = -0x78;
        } while (var_v1_14 < 2);
        D_801258DC = 0;
        D_800B1AF0 += 1;
        return;
    case 38:                                        /* switch 1 */
        func_8011ED50(8);
        temp_v1_3 = (D_800EC234 - 4) & 0xFF;
        D_800EC2A2 = temp_v1_3;
        D_800EC2A1 = temp_v1_3;
        D_800EC2A0 = temp_v1_3;
        D_800EC27E = temp_v1_3;
        D_800EC27D = temp_v1_3;
        D_800EC27C = temp_v1_3;
        D_800EC25A = temp_v1_3;
        D_800EC259 = temp_v1_3;
        D_800EC258 = temp_v1_3;
        D_800EC236 = temp_v1_3;
        D_800EC235 = temp_v1_3;
        D_800EC234 = temp_v1_3;
        if (temp_v1_3 < 4U) {
            D_800B1AF0 += 1;
            return;
        }
        break;
    case 39:                                        /* switch 1 */
        var_v1_15 = 0;
        do {
            temp_lo_13 = var_v1_15 * 0x24;
            var_v1_15 += 1;
            temp_v0_15 = &D_800EC190[temp_lo_13];
            *(s32 *)((unsigned char *)temp_v0_15 + (144)) = (s32) (*(s32 *)((unsigned char *)temp_v0_15 + (144)) | 0x80000000);
        } while (var_v1_15 < 4);
        D_800B1AF0 = var_a3 + 1;
        return;
    case 40:                                        /* switch 1 */
        sp50 = var_a3;
        D_800B1AF0 = var_a3;
        sp54 = (s32) D_800B1AF6;
        func_8004AFB8();
        D_800B1AF0 = var_a3;
        if (sp54 != D_800B1AF6) {
            D_800B1AF6 -= 1;
            D_800B1AF0 = var_a3 + 1;
            return;
        }
        break;
    case 42:                                        /* switch 1 */
        sp50 = var_a3;
        D_800B1AF0 = var_a3;
        func_8004A9CC();
        D_800B1AF6 -= 1;
        D_800B1AF0 = var_a3 + 1;
        return;
    case 43:                                        /* switch 1 */
        D_800B1AF0 = var_a3;
        switch (D_80125320) {                       /* switch 2 */
        case 0:                                     /* switch 2 */
            func_800469F4(0x42AE);
            var_a3 = D_800B1AF0;
            break;
        case 1:                                     /* switch 2 */
            func_800469F4(0x42B7);
            var_a3 = D_800B1AF0;
            break;
        case 2:                                     /* switch 2 */
            func_800469F4(0x42C0);
            var_a3 = D_800B1AF0;
            break;
        case 3:                                     /* switch 2 */
            func_800469F4(0x42C9);
            var_a3 = D_800B1AF0;
            break;
        case 4:                                     /* switch 2 */
            func_800469F4(0x42D2);
            var_a3 = D_800B1AF0;
            break;
        }
        D_800B1AF0 = var_a3 + 1;
        return;
    case 45:                                        /* switch 1 */
        func_8002755C(1);
        func_80027438(1);
        D_800B1AF0 += 1;
        return;
    case 46:                                        /* switch 1 */
        sp50 = var_a3;
        D_800B1AF0 = var_a3;
        func_8011A62C();
        D_800B1AF6 -= 1;
        D_800B1AF0 = var_a3 + 1;
        return;
    case 48:                                        /* switch 1 */
        sp50 = var_a3;
        D_800B1AF0 = var_a3;
        func_8011A7FC();
        D_800B1AF6 -= 1;
        D_800B1AF0 = sp50 + 1;
        return;
    case 50:                                        /* switch 1 */
        sp50 = var_a3;
        D_800B1AF0 = var_a3;
        func_8011A838();
        D_800B1AF6 -= 1;
        D_800B1AF0 = sp50 + 1;
        return;
    default:                                        /* switch 1 */
        func_80011DFC();
        D_800EED08 = 0;
        func_800433D0(0x206);
        func_800352B4(1);
        break;
    }
}
