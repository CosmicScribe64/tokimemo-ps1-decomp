void func_80137654(void) {
    u8 *sp90;
    s16 sp6E;
    s16 sp6C;
    s32 sp64;
    s32 temp_t8;
    s32 var_s2;
    u8 *temp_s3;
    u8 *var_s0;
    u8 *var_s5;
    u8 temp_t4;
    u8 *temp_s1;
    u8 *temp_s4;

    var_s5 = D_80158A8C;
    temp_s3 = D_80158A64;
    sp64 = 0;
    sp90 = var_s5;
    do {
        if (*(u8 *)((unsigned char *)var_s5 + (851)) != 0) {
            temp_s4 = sp90 + (sp64 * 0x14);
            *(s8 *)((unsigned char *)temp_s4 + (106)) = 0;
            temp_s1 = sp90 + (sp64 * 2);
            var_s0 = D_80158A80;
            sp6C = *(s16 *)((unsigned char *)temp_s1 + (588)) + *(s16 *)((unsigned char *)sp90 + (20));
            var_s2 = 0;
            sp6E = *(s16 *)((unsigned char *)temp_s1 + (636)) + *(s16 *)((unsigned char *)sp90 + (22));
            do {
                if (((*(s32 *)var_s0) != 0) && (*(u16 *)((unsigned char *)var_s0 + (4)) == 0x40) && (func_80145508((s32) (var_s0 + 0x14), (s32) &D_80155B36, (s32) &sp6C, (s32) &D_80155C6A) != 0)) {
                    temp_t4 = *(u8 *)((unsigned char *)temp_s3 + (37)) + 1;
                    *(u8 *)((unsigned char *)temp_s3 + (37)) = temp_t4;
                    if ((*(u8 *)((unsigned char *)temp_s3 + (36)) != 0) && !(temp_t4 & 3)) {
                        func_80133D14((s32) *(s16 *)((unsigned char *)var_s0 + (20)), (s32) *(s16 *)((unsigned char *)var_s0 + (22)), 0x20, 0xA0);
                    }
                    func_80133AD0(0x507);
                    if (*(u8 *)((unsigned char *)var_s5 + (851)) != 0) {
                        if (var_s2 >= 6) {
                            *(s16 *)((unsigned char *)temp_s1 + (728)) = (s16) (*(s16 *)((unsigned char *)temp_s1 + (728)) - 1);
                        } else if (*(u16 *)((unsigned char *)var_s0 + (2)) == 9) {
                            *(s16 *)((unsigned char *)temp_s1 + (728)) = (s16) (*(s16 *)((unsigned char *)temp_s1 + (728)) - 4);
                        } else {
                            *(s16 *)((unsigned char *)temp_s1 + (728)) = (s16) (*(s16 *)((unsigned char *)temp_s1 + (728)) - 2);
                        }
                        if (*(s16 *)((unsigned char *)temp_s1 + (728)) < 0) {
                            func_80133AD0(0x505);
                            *(u8 *)((unsigned char *)var_s5 + (851)) = 0U;
                            *(s8 *)((unsigned char *)temp_s4 + (105)) = 0;
                        }
                    }
                    *(s8 *)((unsigned char *)temp_s4 + (106)) = 1;
                    *(u16 *)((unsigned char *)var_s0 + (4)) = 0x80U;
                    *(s8 *)((unsigned char *)var_s0 + (13)) = 0;
                    *(s8 *)((unsigned char *)var_s0 + (14)) = 0;
                    *(s8 *)((unsigned char *)var_s0 + (15)) = 0;
                    *(s16 *)((unsigned char *)var_s0 + (6)) = 0;
                    *(s16 *)((unsigned char *)var_s0 + (8)) = 0;
                    *(s16 *)((unsigned char *)var_s0 + (10)) = 0;
                }
                var_s2 += 1;
                var_s0 += 0x54;
            } while (var_s2 != 0xC);
        }
        temp_t8 = sp64 + 1;
        sp64 = temp_t8;
        var_s5 += 1;
    } while (temp_t8 != 4);
}
