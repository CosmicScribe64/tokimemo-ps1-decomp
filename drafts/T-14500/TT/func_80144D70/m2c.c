void func_80144D70(void) {
    u8 *sp4C;
    u8 *sp44;
    s32 *sp40;
    u8 *sp38;
    u8 *sp34;
    s32 sp30;
    s32 temp_a0;
    s32 temp_v0;
    s32 temp_v1_3;
    s32 var_v1;
    u16 temp_t0;
    u16 temp_v1;
    u16 var_v0;
    u8 *temp_a3;
    u8 *temp_a3_2;
    u8 *temp_a3_3;
    u8 *temp_s0;
    u8 *var_a3;
    u8 temp_t0_2;
    u8 temp_v1_2;

    temp_s0 = D_80158A64;
    sp34 = D_80158AA4 + 0xC80;
    sp4C = D_80158A60;
    sp40 = D_80158A70;
    temp_v1 = *(u16 *)((unsigned char *)temp_s0 + (2));
    temp_a3 = D_80158AA8 + 0x140;
    switch (temp_v1) {                              /* switch 1; irregular */
    case 0x0:                                       /* switch 1 */
        sp44 = D_80158A6C;
        sp38 = temp_a3;
        func_80044750(0x22);
        func_80044750(0x2F);
        func_8014AF3C();
        func_80132F4C();
        func_80133B70();
        *(s8 *)((unsigned char *)temp_s0 + (17)) = 0;
        if ((u8) *(u8 *)((unsigned char *)sp44 + (11)) < 3U) {
            sp38 = temp_a3;
            func_80044750(4);
            func_80044750(0x202);
            temp_v1_2 = *(u8 *)((unsigned char *)sp44 + (11));
            switch (temp_v1_2) {                    /* switch 2; irregular */
            case 0:                                 /* switch 2 */
                *(s16 *)((unsigned char *)sp4C + (164)) = 1;
                sp38 = temp_a3;
                func_8014304C(0x80016000, 0xC869, 0x2420);
                func_8014304C(0x8002E800, 0xC817, 0x69E4);
                func_8014304C(0x8019A000, 0xC81A, 0x27D60);
                break;
            case 1:                                 /* switch 2 */
                *(s16 *)((unsigned char *)sp4C + (164)) = 2;
                sp38 = temp_a3;
                func_8014304C(0x80016000, 0xC8BD, 0x2420);
                func_8014304C(0x8002E800, 0xC86D, 0x69E4);
                func_8014304C(0x8019A000, 0xC871, 0x27D60);
                break;
            case 2:                                 /* switch 2 */
                *(s16 *)((unsigned char *)sp4C + (164)) = 3;
                sp38 = temp_a3;
                func_8014304C(0x80016000, 0xC916, 0x2420);
                func_8014304C(0x8002E800, 0xC8C1, 0x69E4);
                func_8014304C(0x8019A000, 0xC8C6, 0x27D60);
                break;
            }
            sp38 = temp_a3;
            func_8014304C(0x8001A000, 0xBFD1, 0xE20);
            func_8014304C(0x801C2000, 0xBF9C, 0x1A680);
            var_a3 = temp_a3;
            *(s16 *)((unsigned char *)var_a3 + (10)) = -0x38;
        } else {
            sp38 = temp_a3;
            func_80044750(4);
            func_80044750(0x200);
            var_a3 = temp_a3;
            *(s16 *)((unsigned char *)var_a3 + (10)) = -0x20;
        }
        *(s16 *)((unsigned char *)var_a3 + (12)) = 0x74;
        *(s16 *)((unsigned char *)var_a3 + (14)) = 0xB;
        *(s8 *)((unsigned char *)var_a3 + (2)) = 4;
        var_a3->unk0 = 1;
        *(s16 *)((unsigned char *)var_a3 + (6)) = 0;
        *(s16 *)((unsigned char *)var_a3 + (16)) = 0;
        temp_a3_2 = var_a3 + 0x14;
        if ((u8) *(u8 *)((unsigned char *)sp44 + (11)) < 3U) {
            *(s16 *)((unsigned char *)temp_a3_2 + (10)) = 8;
        } else {
            *(s16 *)((unsigned char *)temp_a3_2 + (10)) = 0x10;
        }
        *(s16 *)((unsigned char *)temp_a3_2 + (12)) = 0x74;
        *(s16 *)((unsigned char *)temp_a3_2 + (14)) = 0xC;
        *(s8 *)((unsigned char *)temp_a3_2 + (2)) = 4;
        *(s8 *)((unsigned char *)var_a3 + (20)) = 1;
        *(s16 *)((unsigned char *)temp_a3_2 + (6)) = 0;
        *(s16 *)((unsigned char *)temp_a3_2 + (16)) = 0;
        temp_a3_3 = temp_a3_2 + 0x14;
        if ((u8) *(u8 *)((unsigned char *)sp44 + (11)) < 3U) {
            sp34->unk0 = 1;
            *(s8 *)((unsigned char *)sp34 + (1)) = 0;
            *(s16 *)((unsigned char *)sp34 + (68)) = 0;
            *(s16 *)((unsigned char *)sp34 + (20)) = -0xC;
            *(s16 *)((unsigned char *)sp34 + (22)) = 0x6C;
            *(s16 *)((unsigned char *)sp34 + (60)) = 0x10;
            *(s16 *)((unsigned char *)sp34 + (62)) = 0x10;
            *(s8 *)((unsigned char *)sp34 + (70)) = (s8) ((*(u8 *)((unsigned char *)sp44 + (11)) * 0x10) + 0x10);
            *(s8 *)((unsigned char *)sp34 + (71)) = 0x20;
            *(s16 *)((unsigned char *)sp34 + (64)) = 0xA;
            *(s16 *)((unsigned char *)sp34 + (66)) = 0x3C08;
            *(s8 *)((unsigned char *)sp34 + (59)) = 0x65;
            *(s16 *)((unsigned char *)sp34 + (16)) = 0;
        } else {
            *(s16 *)((unsigned char *)temp_a3_3 + (10)) = -0x40;
            *(s16 *)((unsigned char *)temp_a3_3 + (12)) = 0x74;
            *(s16 *)((unsigned char *)temp_a3_3 + (14)) = 0xA;
            *(s8 *)((unsigned char *)temp_a3_3 + (2)) = 4;
            *(s8 *)((unsigned char *)temp_a3_2 + (20)) = 1;
            *(s16 *)((unsigned char *)temp_a3_3 + (6)) = 0;
            *(s16 *)((unsigned char *)temp_a3_3 + (16)) = 0;
        }
        *(u16 *)((unsigned char *)temp_s0 + (46)) = 0x80U;
        *(s8 *)((unsigned char *)temp_s0 + (29)) = 0x80;
        *(s8 *)((unsigned char *)temp_s0 + (30)) = 0x80;
        *(s8 *)((unsigned char *)temp_s0 + (31)) = 0x80;
        *(s8 *)((unsigned char *)temp_s0 + (28)) = 3;
        *(s32 *)((unsigned char *)sp44 + (4)) = 0;
        func_80148DB0();
        *(s16 *)((unsigned char *)temp_s0 + (2)) = 0x40;
        break;
    case 0x40:                                      /* switch 1 */
        temp_t0 = *(u16 *)((unsigned char *)temp_s0 + (46)) - 4;
        var_v0 = temp_t0 & 0xFFFF;
        *(u16 *)((unsigned char *)temp_s0 + (46)) = temp_t0;
        if (var_v0 == 0) {
            D_80158A6C->unk0 = 0;
            var_v0 = *(u16 *)((unsigned char *)temp_s0 + (46));
            *(u16 *)((unsigned char *)temp_s0 + (38)) = 0U;
            *(s16 *)((unsigned char *)temp_s0 + (2)) = 0x50;
        }
        *(s8 *)((unsigned char *)temp_s0 + (29)) = (s8) var_v0;
        *(s8 *)((unsigned char *)temp_s0 + (30)) = (s8) var_v0;
        *(s8 *)((unsigned char *)temp_s0 + (31)) = (s8) var_v0;
        temp_v1_3 = D_80158A6C->unk0;
        if (*(s32 *)((unsigned char *)D_80158A6C + (4)) < temp_v1_3) {
            D_80158A6C->unk0 = (s32) (temp_v1_3 - 0x400);
        } else {
            D_80158A6C->unk0 = (s32) (temp_v1_3 + 0x200);
        }
        temp_a0 = *(s32 *)((unsigned char *)D_80158A6C + (4));
        temp_v0 = D_80158A6C->unk0 - temp_a0;
        var_v1 = temp_v0;
        if (temp_v0 < 0) {
            var_v1 = -temp_v0;
        }
        if (var_v1 < 0x1000) {
            D_80158A6C->unk0 = temp_a0;
        }
        break;
    case 0x50:                                      /* switch 1 */
        if ((u8) *(u8 *)((unsigned char *)D_80158A6C + (11)) < 3U) {
            if ((u16) *(u16 *)((unsigned char *)D_80158A64 + (38)) >= 0x191U) {
                *(s16 *)((unsigned char *)temp_s0 + (2)) = 0xA0;
            } else {
                *(u16 *)((unsigned char *)temp_s0 + (38)) = (u16) (*(u16 *)((unsigned char *)temp_s0 + (38)) + 1);
            }
        } else if ((u16) *(u16 *)((unsigned char *)D_80158A64 + (38)) >= 0x1AFU) {
            *(s16 *)((unsigned char *)temp_s0 + (2)) = 0xA0;
        } else {
            *(u16 *)((unsigned char *)temp_s0 + (38)) = (u16) (*(u16 *)((unsigned char *)temp_s0 + (38)) + 1);
        }
        if ((*(u8 *)((unsigned char *)temp_s0 + (34)) == 0) && (*(u16 *)((unsigned char *)(D_80158A60 + 0x10) + (4)) & 0x820)) {
            *(s16 *)((unsigned char *)temp_s0 + (2)) = 0xA0;
        }
        break;
    case 0xA0:                                      /* switch 1 */
        sp44 = D_80158A6C;
        if (((u8) *(u8 *)((unsigned char *)D_80158A6C + (11)) >= 3U) || (sp44 = D_80158A6C, sp30 = func_800460DC(), (((((sp30 - func_801430C4()) + 0x100) & 0xFF) < 5) == 0))) {
            *(s8 *)((unsigned char *)D_80158AA8 + (320)) = 0;
            *(s8 *)((unsigned char *)D_80158AA8 + (340)) = 0;
            sp34->unk0 = 0;
            func_800AE080((u8 *) sp40, 0x20);
            temp_t0_2 = *(u8 *)((unsigned char *)sp44 + (11)) + 1;
            *(u8 *)((unsigned char *)sp44 + (11)) = temp_t0_2;
            if ((u32) (temp_t0_2 & 0xFF) < 4U) {
                temp_s0->unk0 = 2;
                *(s16 *)((unsigned char *)temp_s0 + (2)) = 1;
            } else {
                *(s8 *)((unsigned char *)sp44 + (15)) = 1;
                if (*(u8 *)((unsigned char *)temp_s0 + (34)) != 0) {
                    temp_s0->unk0 = 8;
                } else {
                    temp_s0->unk0 = 6;
                }
                *(s16 *)((unsigned char *)temp_s0 + (6)) = 0;
                *(s16 *)((unsigned char *)temp_s0 + (4)) = 0;
                *(s16 *)((unsigned char *)temp_s0 + (2)) = 0;
                *(s8 *)((unsigned char *)temp_s0 + (11)) = 0;
                *(s8 *)((unsigned char *)temp_s0 + (10)) = 0;
                *(s8 *)((unsigned char *)temp_s0 + (9)) = 0;
            }
        }
        break;
    }
}
