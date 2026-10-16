extern s16 D_800EC226;
extern u16 D_800EC22A;
extern ? D_800EC22E;
extern u8 D_800EC234;
extern u8 D_800EC235;
extern u8 D_800EC236;
extern s16 D_800EC24A;
extern u16 D_800EC24E;
extern u8 D_800EC252;
extern u8 D_800EC253;
extern u8 D_800EC258;
extern u8 D_800EC259;
extern u8 D_800EC25A;
extern s16 D_800EC26E;
extern u16 D_800EC272;
extern u8 D_800EC276;
extern u8 D_800EC277;
extern u8 D_800EC27C;
extern u8 D_800EC27D;
extern u8 D_800EC27E;
extern s16 D_800EC292;
extern u16 D_800EC296;
extern u8 D_800EC29A;
extern u8 D_800EC29B;
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

u16 func_8011C750(void) {
    s16 sp60;
    s16 sp5A;
    s16 sp58;
    s16 sp56;
    s16 sp54;
    s32 sp50;
    s32 sp4C;
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
    void *temp_v0;
    void *temp_v0_2;
    void *temp_v0_5;
    void *temp_v0_6;
    void *temp_v0_7;
    void *temp_v0_8;

    switch (D_800B1AF0) {                           /* switch 1 */
    case 0:                                         /* switch 1 */
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
            temp_v0 = D_800EC190 + (var_v1 * 0x24);
            *(s16 *)((unsigned char *)temp_v0 + (152)) = (s16) ((var_v1 << 6) + 0x80);
            temp_t5 = var_v1 << 7;
            temp_t7 = &D_800B0860[var_v1];
            var_v1 += 1;
            *(s32 *)((unsigned char *)temp_v0 + (144)) = 0x82000000;
            *(s16 *)((unsigned char *)temp_v0 + (154)) = 0;
            *(s8 *)((unsigned char *)temp_v0 + (158)) = 0;
            *(s8 *)((unsigned char *)temp_v0 + (159)) = 0xA0;
            *(s16 *)((unsigned char *)temp_v0 + (160)) = 0;
            *(s16 *)((unsigned char *)temp_v0 + (156)) = temp_t9;
            *(s16 *)((unsigned char *)temp_v0 + (162)) = 0x1E0;
            *(s8 *)((unsigned char *)temp_v0 + (164)) = 0;
            *(s8 *)((unsigned char *)temp_v0 + (165)) = 0;
            *(s8 *)((unsigned char *)temp_v0 + (166)) = 0;
            *(s16 *)((unsigned char *)temp_v0 + (168)) = 0;
            *(s16 *)((unsigned char *)temp_v0 + (170)) = 0;
            *(s16 *)((unsigned char *)temp_v0 + (172)) = 0x1000;
            *(s16 *)((unsigned char *)temp_v0 + (174)) = 0x1000;
            *(s32 *)((unsigned char *)temp_v0 + (176)) = 0;
            *(s16 *)((unsigned char *)temp_v0 + (148)) = (s16) (temp_t5 - 0xA0);
            *(s16 *)((unsigned char *)temp_v0 + (150)) = -0x78;
            *(s8 *)((unsigned char *)temp_t7 + (4145)) = 0xB;
        } while (var_v1 < 2);
        var_v1_2 = 0;
        do {
            temp_t9_2 = (var_v1_2 * 2) + 0xA;
            temp_v0_2 = D_800EC190 + (var_v1_2 * 0x24);
            *(s16 *)((unsigned char *)temp_v0_2 + (224)) = (s16) ((var_v1_2 << 6) + 0x80);
            temp_t5_2 = var_v1_2 << 7;
            temp_t7_2 = &D_800B0860[var_v1_2];
            var_v1_2 += 1;
            *(s32 *)((unsigned char *)temp_v0_2 + (216)) = 0x82000000;
            *(s16 *)((unsigned char *)temp_v0_2 + (226)) = 0xA0;
            *(s8 *)((unsigned char *)temp_v0_2 + (230)) = 0;
            *(s8 *)((unsigned char *)temp_v0_2 + (231)) = 0;
            *(s16 *)((unsigned char *)temp_v0_2 + (232)) = 0;
            *(s16 *)((unsigned char *)temp_v0_2 + (228)) = temp_t9_2;
            *(s16 *)((unsigned char *)temp_v0_2 + (234)) = 0x1E0;
            *(s8 *)((unsigned char *)temp_v0_2 + (236)) = 0;
            *(s8 *)((unsigned char *)temp_v0_2 + (237)) = 0;
            *(s8 *)((unsigned char *)temp_v0_2 + (238)) = 0;
            *(s16 *)((unsigned char *)temp_v0_2 + (240)) = 0;
            *(s16 *)((unsigned char *)temp_v0_2 + (242)) = 0;
            *(s16 *)((unsigned char *)temp_v0_2 + (244)) = 0x1000;
            *(s16 *)((unsigned char *)temp_v0_2 + (246)) = 0x1000;
            *(s32 *)((unsigned char *)temp_v0_2 + (248)) = 0;
            *(s16 *)((unsigned char *)temp_v0_2 + (220)) = (s16) (temp_t5_2 - 0xA0);
            *(s16 *)((unsigned char *)temp_v0_2 + (222)) = -0x78;
            *(s8 *)((unsigned char *)temp_t7_2 + (4147)) = 0xB;
        } while (var_v1_2 < 2);
        func_8002746C(0x433B, 1, 0x1E0, D_800B0860);
        D_800B1AF0 += 1;
        break;
    case 1:                                         /* switch 1 */
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
    case 2:                                         /* switch 1 */
        func_8002746C(0x4344, 1);
        D_800B1AF0 += 1;
        break;
    case 3:                                         /* switch 1 */
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
    case 4:                                         /* switch 1 */
        func_8002746C(0x434E, 1);
        D_800B1AF0 += 1;
        break;
    case 5:                                         /* switch 1 */
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
    case 6:                                         /* switch 1 */
        func_8002746C(0x4357, 1);
        D_800B1AF0 += 1;
        break;
    case 7:                                         /* switch 1 */
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
    case 8:                                         /* switch 1 */
        var_v1_3 = 0;
        do {
            temp_lo = var_v1_3 * 0x24;
            var_v1_3 += 1;
            *(s32 *)((unsigned char *)(&D_800EC190[temp_lo]) + (144)) = 0x02000000;
        } while (var_v1_3 < 4);
        D_800B1AF0 += 1;
        break;
    case 9:                                         /* switch 1 */
        temp_v0_3 = (D_800EC234 + 2) & 0xFF;
        D_800EC2A2 = temp_v0_3;
        D_800EC2A1 = temp_v0_3;
        D_800EC2A0 = temp_v0_3;
        D_800EC27E = temp_v0_3;
        D_800EC27D = temp_v0_3;
        D_800EC27C = temp_v0_3;
        D_800EC25A = temp_v0_3;
        D_800EC259 = temp_v0_3;
        D_800EC258 = temp_v0_3;
        D_800EC236 = temp_v0_3;
        D_800EC235 = temp_v0_3;
        D_800EC234 = temp_v0_3;
        if (temp_v0_3 == 0x80) {
            D_800B1AF0 += 1;
        }
        break;
    case 11:                                        /* switch 1 */
        temp_v0_4 = (D_800EC234 - 2) & 0xFF;
        D_800EC2A2 = temp_v0_4;
        D_800EC2A1 = temp_v0_4;
        D_800EC2A0 = temp_v0_4;
        D_800EC27E = temp_v0_4;
        D_800EC27D = temp_v0_4;
        D_800EC27C = temp_v0_4;
        D_800EC25A = temp_v0_4;
        D_800EC259 = temp_v0_4;
        D_800EC258 = temp_v0_4;
        D_800EC236 = temp_v0_4;
        D_800EC235 = temp_v0_4;
        D_800EC234 = temp_v0_4;
        if (temp_v0_4 == 0) {
            var_v1_4 = 0;
            do {
                temp_lo_2 = var_v1_4 * 0x24;
                var_v1_4 += 1;
                *(s32 *)((unsigned char *)(&D_800EC190[temp_lo_2]) + (144)) = 0x82000000;
            } while (var_v1_4 < 4);
            D_800B1AF0 += 1;
        }
        break;
    case 12:                                        /* switch 1 */
        sp50 = (s32) D_800B1AF6;
        sp4C = D_800B1AF0;
        func_8004AFB8();
        D_800B1AF0 = sp4C;
        if (sp50 != D_800B1AF6) {
            D_800B1AF0 = sp4C + 1;
            D_800B1AF6 -= 1;
        }
        break;
    case 13:                                        /* switch 1 */
    case 16:                                        /* switch 1 */
    case 19:                                        /* switch 1 */
    case 21:                                        /* switch 1 */
    case 23:                                        /* switch 1 */
        if (func_80015A44() & 1) {
            D_800B1AF0 += 1;
        }
        break;
    case 14:                                        /* switch 1 */
        sp4C = D_800B1AF0;
        func_8004A9CC();
        D_800B1AF6 -= 1;
        D_800B1AF0 = sp4C + 1;
        break;
    case 15:                                        /* switch 1 */
        switch (D_80125320) {                       /* switch 2 */
        case 0:                                     /* switch 2 */
            func_800469F4(0x42AE);
            break;
        case 1:                                     /* switch 2 */
            func_800469F4(0x42B7);
            break;
        case 2:                                     /* switch 2 */
            func_800469F4(0x42C0);
            break;
        case 3:                                     /* switch 2 */
            func_800469F4(0x42C9);
            break;
        case 4:                                     /* switch 2 */
            func_800469F4(0x42D2);
            break;
        }
        D_800B1AF0 += 1;
        break;
    case 17:                                        /* switch 1 */
        func_8002755C(1);
        func_80027438(1);
        D_800B1AF0 += 1;
        break;
    case 18:                                        /* switch 1 */
        sp4C = D_800B1AF0;
        func_8011A62C();
        D_800B1AF6 -= 1;
        D_800B1AF0 = sp4C + 1;
        break;
    case 20:                                        /* switch 1 */
        sp4C = D_800B1AF0;
        func_8011A7FC();
        D_800B1AF6 -= 1;
        D_800B1AF0 = sp4C + 1;
        break;
    case 22:                                        /* switch 1 */
        sp4C = D_800B1AF0;
        func_8011A838();
        D_800B1AF6 -= 1;
        D_800B1AF0 = sp4C + 1;
        break;
    default:                                        /* switch 1 */
        func_800352B4(1);
        D_800EED08 = 0;
        func_800433D0(0x206);
        func_80011DFC();
        break;
    }
    if (((u32) D_800B1AF0 < 9U) || ((u32) D_800B1AF0 >= 0xCU)) {
        return 0U;
    }
    var_v1_5 = D_800EC22E.unk0 + D_801258E0;
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
    var_a0 = *(u8 *)((unsigned char *)(&D_800EC22E) + (1));
    D_800EC29A = (u8) var_v1_5;
    D_800EC276 = (u8) var_v1_5;
    D_800EC252 = (u8) var_v1_5;
    D_800EC22E.unk0 = (u8) var_v1_5;
    if (var_a0 != 0) {
        D_800EC22A += 0x14;
        D_800EC24E += 0x14;
        var_a0 = (var_a0 - 0x14) & 0xFF;
        D_800EC253 -= 0x14;
    } else {
        D_800EC22A -= 0x14;
        D_800EC226 += 0x14;
        D_800EC24E -= 0x14;
        D_800EC24A += 0x14;
    }
    if (D_800EC277 != 0) {
        D_800EC272 += 0x14;
        D_800EC296 += 0x14;
        D_800EC277 -= 0x14;
        D_800EC29B -= 0x14;
    } else {
        D_800EC272 -= 0x14;
        D_800EC26E += 0x14;
        D_800EC296 -= 0x14;
        D_800EC292 += 0x14;
    }
    *(u8 *)((unsigned char *)(&D_800EC22E) + (1)) = var_a0;
    if (((var_a0 == 0) && ((D_800B1AF9 == 1) || (D_800B1AF9 == 3))) || ((D_800EC277 == 0) && ((D_800B1AF9 == 0) || (D_800B1AF9 == 2)))) {
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
                            temp_v0_5 = D_800EC190 + temp_lo_3;
                            *(s16 *)((unsigned char *)temp_v0_5 + (226)) = 0;
                            *(s8 *)((unsigned char *)temp_v0_5 + (231)) = 0xA0;
                            *(s16 *)((unsigned char *)temp_v0_5 + (222)) = -0x78;
                            *(s16 *)((unsigned char *)temp_v0_5 + (228)) = (s16) (temp_t8 + 0xA);
                        } while (var_v1_9 < 2);
                        func_800137AC(0x280, 0, 0x180, 0xA0, 0x801DD000);
                    }
                } else {
                    do {
                        temp_lo_4 = var_v1_8 * 0x24;
                        temp_t5_3 = var_v1_8 * 2;
                        var_v1_8 += 1;
                        temp_v0_6 = D_800EC190 + temp_lo_4;
                        *(s16 *)((unsigned char *)temp_v0_6 + (154)) = 0;
                        *(s8 *)((unsigned char *)temp_v0_6 + (159)) = 0xA0;
                        *(s16 *)((unsigned char *)temp_v0_6 + (150)) = -0x78;
                        *(s16 *)((unsigned char *)temp_v0_6 + (156)) = (s16) (temp_t5_3 + 0x15);
                    } while (var_v1_8 < 2);
                    func_800137AC(0x140, 0x100, 0x180, 0xA0, 0x801BF000);
                }
            } else {
                do {
                    temp_lo_5 = var_v1_7 * 0x24;
                    temp_t6_2 = var_v1_7 * 2;
                    var_v1_7 += 1;
                    temp_v0_7 = D_800EC190 + temp_lo_5;
                    *(s16 *)((unsigned char *)temp_v0_7 + (226)) = 0;
                    *(s8 *)((unsigned char *)temp_v0_7 + (231)) = 0xA0;
                    *(s16 *)((unsigned char *)temp_v0_7 + (222)) = -0x78;
                    *(s16 *)((unsigned char *)temp_v0_7 + (228)) = (s16) (temp_t6_2 + 0xA);
                } while (var_v1_7 < 2);
                func_800137AC(0x280, 0, 0x180, 0xA0, 0x801A1000);
            }
        } else {
            do {
                temp_lo_6 = var_v1_6 * 0x24;
                temp_t7_3 = var_v1_6 * 2;
                var_v1_6 += 1;
                temp_v0_8 = D_800EC190 + temp_lo_6;
                *(s16 *)((unsigned char *)temp_v0_8 + (154)) = 0;
                *(s8 *)((unsigned char *)temp_v0_8 + (159)) = 0xA0;
                *(s16 *)((unsigned char *)temp_v0_8 + (150)) = -0x78;
                *(s16 *)((unsigned char *)temp_v0_8 + (156)) = (s16) (temp_t7_3 + 0x15);
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
