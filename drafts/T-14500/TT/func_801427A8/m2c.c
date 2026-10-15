u8 func_801427A8(void) {
    u8 *sp78;
    u8 *sp70;
    s32 sp68;
    u8 sp5F;
    s32 temp_a3;
    s32 temp_a3_2;
    s32 temp_a3_3;
    s32 temp_t9;
    s32 var_s0;
    s32 var_s3;
    s32 var_s3_2;
    s32 var_v1;
    u8 *temp_s5;
    u8 *temp_s6;
    u8 *var_s1;
    u8 *var_s1_2;
    u8 *var_s4;
    u8 *var_s4_2;
    u8 *var_v0;
    u8 temp_v0;

    temp_s6 = D_80158A64;
    temp_s5 = D_80158A8C;
    var_s1 = D_80158A80;
    sp5F = 0;
    sp68 = 0;
    sp78 = D_80158A6C;
    sp70 = D_80158A74;
    do {
        if (var_s1->unk0 != 0) {
            var_s3 = 0;
            if (*(u16 *)((unsigned char *)var_s1 + (4)) == 0x40) {
                var_s4 = temp_s5;
                do {
                    temp_a3 = *(s32 *)((unsigned char *)var_s4 + (536));
                    if ((temp_a3 != 0) && (func_80145508((s32) (var_s1 + 0x14), (s32) &D_80155B36, (s32) (temp_s5 + 0x14), temp_a3) != 0)) {
                        *(u8 *)((unsigned char *)temp_s6 + (37)) = (u8) (*(u8 *)((unsigned char *)temp_s6 + (37)) + 1);
                        sp5F = 1;
                        var_v1 = 0;
                        if (*(u8 *)((unsigned char *)temp_s6 + (36)) != 0) {
                            temp_v0 = *(u8 *)((unsigned char *)temp_s6 + (37));
                            var_v1 = 0;
                            if (!(temp_v0 & 3)) {
                                if ((u8) *(u8 *)((unsigned char *)sp78 + (11)) < 2U) {
                                    if (!(temp_v0 & 7)) {
                                        func_801342CC((s32) *(s16 *)((unsigned char *)var_s1 + (20)), (s32) *(s16 *)((unsigned char *)var_s1 + (22)), 0x100, 0x20, 0xA0);
                                    } else {
                                        func_80133FB0((s32) *(s16 *)((unsigned char *)var_s1 + (20)), (s32) *(s16 *)((unsigned char *)var_s1 + (22)), 0x100, 0x20, 0xA0);
                                    }
                                } else {
                                    func_80133D14((s32) *(s16 *)((unsigned char *)var_s1 + (20)), (s32) *(s16 *)((unsigned char *)var_s1 + (22)), 0x20, 0xA0);
                                }
                                var_v1 = 0;
                            }
                        }
                        var_v0 = temp_s5;
                        do {
                            var_v1 += 4;
                            *(s8 *)((unsigned char *)var_v0 + (86)) = 1;
                            *(s8 *)((unsigned char *)var_v0 + (106)) = 1;
                            *(s8 *)((unsigned char *)var_v0 + (126)) = 1;
                            var_v0 += 0x50;
                            *(s8 *)((unsigned char *)var_v0 + (-14)) = 1;
                        } while (var_v1 != 0x18);
                        if (sp68 >= 6) {
                            *(s16 *)((unsigned char *)temp_s5 + (780)) = (s16) (*(s16 *)((unsigned char *)temp_s5 + (780)) - 1);
                        } else if (*(u16 *)((unsigned char *)var_s1 + (2)) == 9) {
                            *(s16 *)((unsigned char *)temp_s5 + (780)) = (s16) (*(s16 *)((unsigned char *)temp_s5 + (780)) - 4);
                        } else {
                            *(s16 *)((unsigned char *)temp_s5 + (780)) = (s16) (*(s16 *)((unsigned char *)temp_s5 + (780)) - 2);
                        }
                        *(u16 *)((unsigned char *)var_s1 + (4)) = 0x80U;
                        *(s16 *)((unsigned char *)var_s1 + (10)) = 0;
                        *(s16 *)((unsigned char *)var_s1 + (8)) = 0;
                        *(s16 *)((unsigned char *)var_s1 + (6)) = 0;
                        *(s8 *)((unsigned char *)var_s1 + (15)) = 0;
                        *(s8 *)((unsigned char *)var_s1 + (14)) = 0;
                        *(s8 *)((unsigned char *)var_s1 + (13)) = 0;
                    }
                    var_s3 += 4;
                    var_s4 += 4;
                } while (var_s3 < 0x10);
                var_s3_2 = 0x10;
                var_s4_2 = temp_s5 + 0x10;
                do {
                    temp_a3_2 = *(s32 *)((unsigned char *)var_s4_2 + (536));
                    if ((temp_a3_2 != 0) && (func_80145508((s32) (var_s1 + 0x14), (s32) &D_80155B36, (s32) (temp_s5 + 0x14), temp_a3_2) != 0)) {
                        *(u16 *)((unsigned char *)var_s1 + (4)) = 0x80U;
                        *(s16 *)((unsigned char *)var_s1 + (10)) = 0;
                        *(s16 *)((unsigned char *)var_s1 + (8)) = 0;
                        *(s16 *)((unsigned char *)var_s1 + (6)) = 0;
                        *(s8 *)((unsigned char *)var_s1 + (15)) = 0;
                        *(s8 *)((unsigned char *)var_s1 + (14)) = 0;
                        *(s8 *)((unsigned char *)var_s1 + (13)) = 0;
                    }
                    var_s3_2 += 4;
                    var_s4_2 += 4;
                } while (var_s3_2 != 0x20);
            }
        }
        var_s1 += 0x54;
        temp_t9 = sp68 + 1;
        sp68 = temp_t9;
    } while (temp_t9 < 0xC);
    sp68 = 4;
    var_s0 = 0x10;
    var_s1_2 = temp_s5 + 0x10;
    do {
        temp_a3_3 = *(s32 *)((unsigned char *)var_s1_2 + (536));
        if ((temp_a3_3 != 0) && (*(u8 *)((unsigned char *)sp70 + (82)) == 0) && (sp70->unk0 != 0) && (func_80145508((s32) (sp70 + 0x14), (s32) D_80155B30, (s32) (temp_s5 + 0x14), temp_a3_3) != 0)) {
            func_80144094();
            *(s16 *)((unsigned char *)sp70 + (4)) = 0x60;
            *(s16 *)((unsigned char *)sp70 + (10)) = 0;
            *(s16 *)((unsigned char *)sp70 + (8)) = 0;
            *(s16 *)((unsigned char *)sp70 + (6)) = 0;
            *(s8 *)((unsigned char *)sp70 + (15)) = 0;
            *(s8 *)((unsigned char *)sp70 + (14)) = 0;
            *(s8 *)((unsigned char *)sp70 + (13)) = 0;
        }
        var_s0 += 4;
        var_s1_2 += 4;
    } while (var_s0 != 0x30);
    return sp5F;
}
