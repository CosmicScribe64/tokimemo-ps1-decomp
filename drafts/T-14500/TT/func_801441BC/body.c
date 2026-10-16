void func_801441BC(void) {
    u8 *sp38;
    u8 *sp2C;
    u8 *sp28;
    s32 var_a1;
    s32 var_a1_2;
    u16 temp_v0;
    u16 temp_v0_2;
    u8 *temp_a1;
    u8 *temp_v0_11;
    u8 *temp_v0_12;
    u8 *temp_v0_13;
    u8 *temp_v0_7;
    u8 *temp_v0_8;
    u8 *temp_v0_9;
    u8 *var_v0;
    u8 *var_v0_2;
    u8 *var_v1;
    u8 temp_t0;
    u8 temp_t0_2;
    u8 temp_t3;
    u8 temp_t7;
    u8 temp_v0_10;
    u8 temp_v0_3;
    u8 temp_v0_4;
    u8 temp_v0_5;
    u8 temp_v0_6;

    var_v1 = D_80158A74;
    sp28 = D_80158A7C;
    temp_v0 = *(u16 *)((unsigned char *)var_v1 + (4));
    temp_a1 = D_80158A64;
    switch (temp_v0) {
    case 0x20:
        sp2C = var_v1;
        func_801437F0((s32) D_80158A6C, (s32) temp_a1);
        if (*(u8 *)((unsigned char *)var_v1 + (13)) != 0) {
            *(u8 *)((unsigned char *)var_v1 + (13)) = 0U;
            *(s16 *)((unsigned char *)var_v1 + (98)) = 0;
            sp2C = var_v1;
            func_80146BE0();
            func_80148DB0();
            *(s16 *)((unsigned char *)var_v1 + (4)) = 0x30;
            *(s16 *)((unsigned char *)var_v1 + (10)) = 0;
            *(s16 *)((unsigned char *)var_v1 + (8)) = 0;
            *(s16 *)((unsigned char *)var_v1 + (6)) = 0;
            *(s8 *)((unsigned char *)var_v1 + (15)) = 0;
            *(s8 *)((unsigned char *)var_v1 + (14)) = 0;
            *(s8 *)((unsigned char *)var_v1 + (13)) = 0;
        }
        break;
    case 0x30:
        sp2C = var_v1;
        func_80143A44();
        temp_v0_2 = *(u16 *)((unsigned char *)var_v1 + (98));
        if (temp_v0_2 >= 0x5AU) {
            *(s8 *)((unsigned char *)var_v1 + (82)) = 0;
            *(u16 *)((unsigned char *)var_v1 + (80)) = (u16) (*(u16 *)((unsigned char *)var_v1 + (80)) & 0xFFFD);
            *(s16 *)((unsigned char *)var_v1 + (4)) = 0x40;
            *(s16 *)((unsigned char *)var_v1 + (10)) = 0;
            *(s16 *)((unsigned char *)var_v1 + (8)) = 0;
            *(s16 *)((unsigned char *)var_v1 + (6)) = 0;
            *(s8 *)((unsigned char *)var_v1 + (15)) = 0;
            *(s8 *)((unsigned char *)var_v1 + (14)) = 0;
            *(s8 *)((unsigned char *)var_v1 + (13)) = 0;
        } else {
            *(u16 *)((unsigned char *)var_v1 + (98)) = (u16) (temp_v0_2 + 1);
        }
        break;
    case 0x40:
        sp2C = var_v1;
        func_80143A44();
        break;
    case 0x60:
        *(s32 *)((unsigned char *)var_v1 + (40)) = 0;
        *(s32 *)((unsigned char *)var_v1 + (44)) = 0x10000;
        sp2C = var_v1;
        func_801478D0((s32) (var_v1 + 0x38));
        func_801478D0((s32) (sp28 + 0x38));
        if (*(u8 *)((unsigned char *)var_v1 + (64)) & 1) {
            *(s32 *)((unsigned char *)var_v1 + (40)) = 0;
            *(s32 *)((unsigned char *)var_v1 + (44)) = 0;
            *(s16 *)((unsigned char *)var_v1 + (4)) = 0x61;
        }
        break;
    case 0x61:
        if (*(u8 *)((unsigned char *)D_80158A6C + (12)) == 0) {
            *(s8 *)((unsigned char *)temp_a1 + (18)) = 0;
            (*(s32 *)temp_a1) = 5;
            *(s16 *)((unsigned char *)temp_a1 + (4)) = 0;
        } else {
            *(u8 *)((unsigned char *)D_80158A6C + (12)) = (u8) (*(u8 *)((unsigned char *)D_80158A6C + (12)) - 1);
            sp2C = var_v1;
            func_80144CA8();
        }
        sp2C = var_v1;
        func_801436BC();
        (*(s32 *)var_v1) = 0;
        *(s32 *)((unsigned char *)var_v1 + (32)) = 0;
        *(s32 *)((unsigned char *)var_v1 + (36)) = 0xF80000;
        *(s32 *)((unsigned char *)var_v1 + (40)) = 0;
        *(s32 *)((unsigned char *)var_v1 + (44)) = 0;
        *(s16 *)((unsigned char *)var_v1 + (20)) = 0;
        *(s16 *)((unsigned char *)var_v1 + (22)) = 0xF8;
        *(s16 *)((unsigned char *)var_v1 + (4)) = 0x20;
        *(s16 *)((unsigned char *)var_v1 + (10)) = 0;
        *(s16 *)((unsigned char *)var_v1 + (8)) = 0;
        *(s16 *)((unsigned char *)var_v1 + (6)) = 0;
        *(s8 *)((unsigned char *)var_v1 + (15)) = 0;
        *(s8 *)((unsigned char *)var_v1 + (14)) = 0;
        *(s8 *)((unsigned char *)var_v1 + (13)) = 0;
        break;
    case 0xA0:
        sp2C = var_v1;
        sp38 = temp_a1;
        func_80143978();
        *(s8 *)((unsigned char *)var_v1 + (62)) = 0;
        *(s8 *)((unsigned char *)sp28 + (62)) = 5;
        *(s8 *)((unsigned char *)sp28 + (158)) = 0xA;
        if (*(u8 *)((unsigned char *)var_v1 + (13)) != 0) {
            (*(s32 *)temp_a1) = 4;
            *(s16 *)((unsigned char *)temp_a1 + (6)) = 0;
            *(s16 *)((unsigned char *)temp_a1 + (4)) = 0;
            *(s16 *)((unsigned char *)temp_a1 + (2)) = 0;
            *(s8 *)((unsigned char *)temp_a1 + (11)) = 0;
            *(s8 *)((unsigned char *)temp_a1 + (10)) = 0;
            *(s8 *)((unsigned char *)temp_a1 + (9)) = 0;
        }
        break;
    }
    sp2C = var_v1;
    func_80143FE8();
    if ((u16) *(u16 *)((unsigned char *)var_v1 + (4)) < 0x60U) {
        temp_v0_3 = *(u8 *)((unsigned char *)var_v1 + (88));
        if ((temp_v0_3 == 0) && (*(u8 *)((unsigned char *)var_v1 + (89)) == 0)) {
            *(u8 *)((unsigned char *)var_v1 + (87)) = 0U;
        } else if (temp_v0_3 >= 7U) {
            *(u8 *)((unsigned char *)var_v1 + (87)) = 2U;
        } else if (temp_v0_3 >= 3U) {
            *(u8 *)((unsigned char *)var_v1 + (87)) = 1U;
        } else {
            temp_v0_4 = *(u8 *)((unsigned char *)var_v1 + (89));
            if (temp_v0_4 >= 7U) {
                *(u8 *)((unsigned char *)var_v1 + (87)) = 4U;
            } else if (temp_v0_4 >= 3U) {
                *(u8 *)((unsigned char *)var_v1 + (87)) = 3U;
            }
        }
        temp_v0_5 = *(u8 *)((unsigned char *)var_v1 + (87));
        *(u8 *)((unsigned char *)var_v1 + (62)) = temp_v0_5;
        *(s8 *)((unsigned char *)sp28 + (62)) = (s8) ((temp_v0_5 * 0xA) + *(u8 *)((unsigned char *)var_v1 + (90)) + 5);
        *(s8 *)((unsigned char *)sp28 + (158)) = (s8) ((*(u8 *)((unsigned char *)var_v1 + (87)) * 0xA) + *(u8 *)((unsigned char *)var_v1 + (91)) + 0xA);
        if (*(u8 *)((unsigned char *)var_v1 + (92)) != 0) {
            temp_v0_6 = *(u8 *)((unsigned char *)var_v1 + (90));
            temp_t7 = *(u8 *)((unsigned char *)var_v1 + (94)) + 1;
            *(u8 *)((unsigned char *)var_v1 + (94)) = temp_t7;
            temp_t0 = temp_v0_6 + 1;
            if ((u8) *(&D_801567B0 + temp_v0_6) < (u32) (temp_t7 & 0xFF)) {
                *(u8 *)((unsigned char *)var_v1 + (94)) = 0U;
                *(u8 *)((unsigned char *)var_v1 + (90)) = temp_t0;
                if ((u32) (temp_t0 & 0xFF) >= 5U) {
                    *(u8 *)((unsigned char *)var_v1 + (90)) = 0U;
                    *(u8 *)((unsigned char *)var_v1 + (92)) = 0U;
                }
            }
            var_v0 = D_80158A84;
            var_a1 = 0;
            do {
                var_a1 += 4;
                if (((*(s32 *)var_v0) != 0) && (*(u8 *)((unsigned char *)var_v0 + (61)) == 0) && (*(u8 *)((unsigned char *)var_v1 + (90)) == 4) && (*(u8 *)((unsigned char *)var_v1 + (94)) == 1)) {
                    *(s8 *)((unsigned char *)var_v0 + (1)) = 0;
                    *(s16 *)((unsigned char *)var_v0 + (62)) = 0;
                }
                temp_v0_7 = var_v0 + 0x58;
                if ((*(u8 *)((unsigned char *)var_v0 + (88)) != 0) && (*(u8 *)((unsigned char *)temp_v0_7 + (61)) == 0) && (*(u8 *)((unsigned char *)var_v1 + (90)) == 4) && (*(u8 *)((unsigned char *)var_v1 + (94)) == 1)) {
                    *(s8 *)((unsigned char *)temp_v0_7 + (1)) = 0;
                    *(s16 *)((unsigned char *)temp_v0_7 + (62)) = 0;
                }
                temp_v0_8 = temp_v0_7 + 0x58;
                if ((*(u8 *)((unsigned char *)temp_v0_7 + (88)) != 0) && (*(u8 *)((unsigned char *)temp_v0_8 + (61)) == 0) && (*(u8 *)((unsigned char *)var_v1 + (90)) == 4) && (*(u8 *)((unsigned char *)var_v1 + (94)) == 1)) {
                    *(s8 *)((unsigned char *)temp_v0_8 + (1)) = 0;
                    *(s16 *)((unsigned char *)temp_v0_8 + (62)) = 0;
                }
                temp_v0_9 = temp_v0_8 + 0x58;
                if ((*(u8 *)((unsigned char *)temp_v0_8 + (88)) != 0) && (*(u8 *)((unsigned char *)temp_v0_9 + (61)) == 0) && (*(u8 *)((unsigned char *)var_v1 + (90)) == 4) && (*(u8 *)((unsigned char *)var_v1 + (94)) == 1)) {
                    *(s8 *)((unsigned char *)temp_v0_9 + (1)) = 0;
                    *(s16 *)((unsigned char *)temp_v0_9 + (62)) = 0;
                }
                var_v0 = temp_v0_9 + 0x58;
            } while (var_a1 != 8);
        }
        if (*(u8 *)((unsigned char *)var_v1 + (93)) != 0) {
            temp_v0_10 = *(u8 *)((unsigned char *)var_v1 + (91));
            temp_t0_2 = *(u8 *)((unsigned char *)var_v1 + (95)) + 1;
            *(u8 *)((unsigned char *)var_v1 + (95)) = temp_t0_2;
            temp_t3 = temp_v0_10 + 1;
            if ((u8) *(&D_801567B0 + temp_v0_10) < (u32) (temp_t0_2 & 0xFF)) {
                *(u8 *)((unsigned char *)var_v1 + (95)) = 0U;
                *(u8 *)((unsigned char *)var_v1 + (91)) = temp_t3;
                if ((u32) (temp_t3 & 0xFF) >= 5U) {
                    *(u8 *)((unsigned char *)var_v1 + (91)) = 0U;
                    *(u8 *)((unsigned char *)var_v1 + (93)) = 0U;
                }
            }
            var_v0_2 = D_80158A84;
            var_a1_2 = 0;
            do {
                var_a1_2 += 4;
                if (((*(s32 *)var_v0_2) != 0) && (*(u8 *)((unsigned char *)var_v0_2 + (61)) == 1) && (*(u8 *)((unsigned char *)var_v1 + (91)) == 4) && (*(u8 *)((unsigned char *)var_v1 + (95)) == 1)) {
                    *(s8 *)((unsigned char *)var_v0_2 + (1)) = 0;
                    *(s16 *)((unsigned char *)var_v0_2 + (62)) = 0;
                }
                temp_v0_11 = var_v0_2 + 0x58;
                if ((*(u8 *)((unsigned char *)var_v0_2 + (88)) != 0) && (*(u8 *)((unsigned char *)temp_v0_11 + (61)) == 1) && (*(u8 *)((unsigned char *)var_v1 + (91)) == 4) && (*(u8 *)((unsigned char *)var_v1 + (95)) == 1)) {
                    *(s8 *)((unsigned char *)temp_v0_11 + (1)) = 0;
                    *(s16 *)((unsigned char *)temp_v0_11 + (62)) = 0;
                }
                temp_v0_12 = temp_v0_11 + 0x58;
                if ((*(u8 *)((unsigned char *)temp_v0_11 + (88)) != 0) && (*(u8 *)((unsigned char *)temp_v0_12 + (61)) == 1) && (*(u8 *)((unsigned char *)var_v1 + (91)) == 4) && (*(u8 *)((unsigned char *)var_v1 + (95)) == 1)) {
                    *(s8 *)((unsigned char *)temp_v0_12 + (1)) = 0;
                    *(s16 *)((unsigned char *)temp_v0_12 + (62)) = 0;
                }
                temp_v0_13 = temp_v0_12 + 0x58;
                if ((*(u8 *)((unsigned char *)temp_v0_12 + (88)) != 0) && (*(u8 *)((unsigned char *)temp_v0_13 + (61)) == 1) && (*(u8 *)((unsigned char *)var_v1 + (91)) == 4) && (*(u8 *)((unsigned char *)var_v1 + (95)) == 1)) {
                    *(s8 *)((unsigned char *)temp_v0_13 + (1)) = 0;
                    *(s16 *)((unsigned char *)temp_v0_13 + (62)) = 0;
                }
                var_v0_2 = temp_v0_13 + 0x58;
            } while (var_a1_2 != 8);
        }
    }
}
