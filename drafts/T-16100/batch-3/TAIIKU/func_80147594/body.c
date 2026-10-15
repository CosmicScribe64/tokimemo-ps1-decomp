void func_80147594(void) {
    s32 sp68[3];
    s32 temp_s5;
    s32 temp_v1;
    s32 var_s4;
    u16 *var_s3;
    u16 *var_s3_2;
    u16 *var_s3_3;
    u8 *var_s0;
    u8 *var_s2;
    u8 *temp_s1;

    sp68[0] = D_8014A614;
    var_s0 = D_8014A454;
    *(s32 *)((unsigned char *)(&sp68[0]) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_8014A614) + (8));
    *(s32 *)((unsigned char *)(&sp68[0]) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_8014A614) + (4));
    do {
        if ((u16) *(u16 *)((unsigned char *)var_s0 + (48)) >= 0x65U) {
            temp_v1 = func_800AE0D0() & 0x1F;
            if (temp_v1 < 0xA) {
                *(s32 *)((unsigned char *)var_s0 + (36)) = -0x64;
            } else if (temp_v1 < 0x14) {
                *(s32 *)((unsigned char *)var_s0 + (36)) = 0x64;
            } else {
                *(s32 *)((unsigned char *)var_s0 + (36)) = 0;
            }
            *(u16 *)((unsigned char *)var_s0 + (48)) = 0U;
        }
        var_s0 += 0x1C;
    } while (var_s0 != &D_8014A4A8);
    var_s2 = D_8014A454;
    var_s4 = 0;
    do {
        if ((u16) *(u16 *)((unsigned char *)var_s2 + (52)) >= 0xBU) {
            var_s3 = D_8014A4D8;
            temp_s1 = (var_s4 * 0x44) + D_8011ECD0;
            temp_s5 = (var_s4 * 0x13) + 0x13;
            do {
                if (*(s16 *)((unsigned char *)temp_s1 + (6620)) == (*var_s3 + temp_s5)) {
                    if (((*(s16 *)((unsigned char *)temp_s1 + (6634)) + 0x4F) < *(s16 *)((unsigned char *)temp_s1 + (6906))) && ((func_800AE0D0() & 0xFF) < (&sp68[0])[var_s4])) {
                        *(s32 *)((unsigned char *)var_s2 + (36)) = (s32) (*(s32 *)((unsigned char *)var_s2 + (36)) - 0x22);
                    }
                    if ((*(s16 *)((unsigned char *)temp_s1 + (6906)) < (*(s16 *)((unsigned char *)temp_s1 + (6634)) + 0x40)) && ((func_800AE0D0() & 0xFF) < (&sp68[0])[var_s4])) {
                        *(s32 *)((unsigned char *)var_s2 + (36)) = (s32) (*(s32 *)((unsigned char *)var_s2 + (36)) + 0x22);
                    }
                }
                var_s3 += 2;
            } while ((u32) var_s3 < (u32) D_8014A4E4);
            var_s3_2 = &D_8014A4E8;
            do {
                if (*(s16 *)((unsigned char *)temp_s1 + (6620)) == (*var_s3_2 + temp_s5)) {
                    if (((*(s16 *)((unsigned char *)temp_s1 + (6634)) + 0x2D) < *(s16 *)((unsigned char *)temp_s1 + (6906))) && ((func_800AE0D0() & 0xFF) < (&sp68[0])[var_s4])) {
                        *(s32 *)((unsigned char *)var_s2 + (36)) = (s32) (*(s32 *)((unsigned char *)var_s2 + (36)) - 0x22);
                    }
                    if ((*(s16 *)((unsigned char *)temp_s1 + (6906)) < (*(s16 *)((unsigned char *)temp_s1 + (6634)) + 0x28)) && ((func_800AE0D0() & 0xFF) < (&sp68[0])[var_s4])) {
                        *(s32 *)((unsigned char *)var_s2 + (36)) = (s32) (*(s32 *)((unsigned char *)var_s2 + (36)) + 0x22);
                    }
                }
                var_s3_2 += 2;
            } while ((u32) var_s3_2 < (u32) &D_8014A4F4);
            var_s3_3 = D_8014A4C4;
            do {
                if (*(s16 *)((unsigned char *)temp_s1 + (6620)) == (*var_s3_3 + temp_s5)) {
                    if (((*(s16 *)((unsigned char *)temp_s1 + (6634)) + 0x2D) < *(s16 *)((unsigned char *)temp_s1 + (6906))) && ((func_800AE0D0() & 0xFF) < (&sp68[0])[var_s4])) {
                        *(s32 *)((unsigned char *)var_s2 + (36)) = (s32) (*(s32 *)((unsigned char *)var_s2 + (36)) - 0x22);
                    }
                    if ((*(s16 *)((unsigned char *)temp_s1 + (6906)) < (*(s16 *)((unsigned char *)temp_s1 + (6634)) + 0x28)) && ((func_800AE0D0() & 0xFF) < (&sp68[0])[var_s4])) {
                        *(s32 *)((unsigned char *)var_s2 + (36)) = (s32) (*(s32 *)((unsigned char *)var_s2 + (36)) + 0x22);
                    }
                }
                var_s3_3 += 2;
            } while (var_s3_3 != &D_8014A4D4);
            *(u16 *)((unsigned char *)var_s2 + (52)) = 0U;
        }
        var_s4 += 1;
        var_s2 += 0x1C;
    } while (var_s4 != 3);
}
