void func_8013FC5C(void) {
    s32 sp64[3];
    s32 *var_s4;
    s32 temp_s1;
    s32 var_s2;
    s32 var_s5;
    s32 var_v1;
    u16 *temp_v0;
    u16 *temp_v0_2;
    u16 temp_t5;
    u8 *var_s3;
    u8 *var_v0;

    (*(s32 *)&sp64) = (s32) D_80149F80;
    *(s32 *)((unsigned char *)(&sp64) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80149F80) + (4));
    var_s4 = &D_801499B4;
    var_s3 = &D_801499CC;
    var_s2 = 0;
    var_s5 = 0;
    *(s32 *)((unsigned char *)(&sp64) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_80149F80) + (8));
    do {
        temp_s1 = var_s2 + 1;
        if (*(s16 *)((unsigned char *)var_s4 + (2)) < 0xE00) {
            if (*var_s3 == 0) {
                (*(s32 *)var_s4) = (s32) ((*(s32 *)var_s4) + *(&sp64 + var_s5));
            }
        } else {
            temp_v0 = &D_801499C4 + (var_s2 * 2);
            if (*temp_v0 != 0xFFFF) {
                *temp_v0 = 0xFFFF;
                var_v1 = 0;
                if (D_801499D4 != 0xFF) {
                    var_v0 = &D_801499D4;
                    do {
                        var_v1 += 1;
                        var_v0 += 1;
                    } while (*(u8 *)((unsigned char *)var_v0 + (1)) != 0xFF);
                }
                (&D_801499D4)[var_v1] = var_s2 + 1;
            }
        }
        if (((func_8013FB14(temp_s1, &sp64) == 1) || (func_8013FB14(temp_s1) == 2) || (func_8013FB14(temp_s1) == 4) || (func_8013FB14(temp_s1) == 5)) && !(func_800AE0D0() & 0xFFF)) {
            *var_s3 = 1;
        }
        var_s5 += 4;
        var_s4 += 4;
        if (*var_s3 == 1) {
            temp_v0_2 = &D_801499C4 + (var_s2 * 2);
            temp_t5 = *temp_v0_2 + 1;
            *temp_v0_2 = temp_t5;
            if ((u32) (temp_t5 & 0xFFFF) >= 0xF1U) {
                *var_s3 = 0;
                *temp_v0_2 = 0;
            }
        }
        var_s2 = temp_s1;
        var_s3 += 1;
    } while (temp_s1 != 3);
}
