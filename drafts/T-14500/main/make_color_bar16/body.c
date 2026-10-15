void make_color_bar16(void *arg0, u32 arg1, u32 arg2, s32 arg3, s32 arg4) {
    s32 spAC;
    s32 spA8;
    s32 spA4;
    s32 spA0;
    s32 sp9C;
    s32 sp98;
    s32 sp74;
    s32 sp70;
    s32 sp6C;
    s32 sp68;
    s32 sp64;
    s32 sp60;
    s32 sp5C;
    s32 sp58;
    s32 sp54;
    s32 sp48;
    s32 sp44;
    s32 sp40;
    s32 temp_a1;
    s32 temp_lo;
    s32 temp_lo_2;
    s32 temp_lo_3;
    s32 temp_ra;
    s32 temp_s5;
    s32 temp_s7;
    s32 temp_t6;
    s32 temp_t6_2;
    s32 temp_t8;
    s32 temp_t8_2;
    s32 temp_t8_3;
    s32 temp_t9;
    s32 temp_t9_2;
    s32 var_a0;
    s32 var_a1;
    s32 var_a2;
    s32 var_s0;
    s32 var_s1;
    s32 var_s2;
    s32 var_t0;
    s32 var_t0_2;
    s32 var_t1;
    s32 var_t2;
    s32 var_t3;
    s32 var_t4;
    s32 var_t5;
    s32 var_v1;
    s32 var_v1_2;
    s32 var_v1_3;
    s8 temp_t6_3;
    s8 temp_t7;
    u8 temp_t6_4;
    u8 temp_t8_4;
    u8 temp_t8_5;
    u8 *var_v0;
    u8 *var_v0_2;
    u8 *var_v0_3;
    u8 *var_v0_4;
    u8 *var_v0_5;

    temp_t6 = (arg1 >> 0x10) & 0xFF;
    temp_t8 = (arg1 >> 8) & 0xFF;
    sp9C = arg1 & 0xFF;
    temp_t9 = (arg2 >> 0x10) & 0xFF;
    spAC = temp_t6;
    sp40 = temp_t6;
    temp_t6_2 = (arg2 >> 8) & 0xFF;
    spA0 = temp_t6_2;
    spA4 = temp_t8;
    spA8 = temp_t9;
    sp98 = arg2 & 0xFF;
    var_v1 = 0;
    var_t0 = arg3 / 2;
    if (var_t0 > 0) {
        sp44 = temp_t8;
        temp_t8_2 = arg3 - 1;
        sp48 = temp_t6_2;
        sp54 = temp_t8_2;
        var_t2 = sp44;
        var_s0 = sp9C;
        var_a1 = sp40;
        sp5C = var_t0;
        var_t0_2 = 0;
        var_a0 = temp_t8_2 * temp_t9;
        sp58 = arg3;
        var_t4 = 0;
        temp_t9_2 = sp48;
        sp48 = temp_t8_2;
        var_s2 = 0;
        var_v0 = arg0;
        temp_ra = var_t2 * 2;
        temp_s5 = var_a1 * 2;
        temp_s7 = var_a1 * 2;
        var_a2 = arg3 * temp_t9;
        sp74 = temp_t9_2 * -2;
        sp70 = var_t2 * 2;
        sp6C = sp98 * -2;
        sp68 = var_s0 * 2;
        var_t1 = temp_t8_2 * temp_t9_2;
        sp60 = sp9C * 2;
        var_t3 = arg3 * temp_t9_2;
        sp64 = sp98 * -2;
        var_t5 = sp48 * sp98;
        var_s1 = arg3 * sp98;
        do {
            temp_lo = (s32) (var_a0 + var_a1) / arg3;
            var_v1 += 1;
            var_a1 += temp_s5;
            var_a0 += temp_t9 * -2;
            var_v0 += 4;
            temp_lo_2 = (s32) (var_a2 + var_t0_2) / arg3;
            *(u8 *)((unsigned char *)var_v0 + (-2)) = (u8) (((temp_lo / 8) & 0x1F) | (*(u8 *)((unsigned char *)var_v0 + (-2)) & 0xFFE0));
            var_t0_2 += temp_s7;
            var_a2 += temp_t9 * -2;
            temp_lo_3 = (s32) (var_t1 + var_t2) / arg3;
            *(u8 *)((unsigned char *)var_v0 + (-4)) = (u8) (((temp_lo_2 / 8) & 0x1F) | (*(u8 *)((unsigned char *)var_v0 + (-4)) & 0xFFE0));
            var_t2 += temp_ra;
            var_t1 += temp_t9_2 * -2;
            *(u8 *)((unsigned char *)var_v0 + (-2)) = (s16) ((((temp_lo_3 / 8) << 5) & 0x3E0) | ((u16) *(u8 *)((unsigned char *)var_v0 + (-2)) & 0xFC1F));
            *(u8 *)((unsigned char *)var_v0 + (-4)) = (s16) (((((s32) ((s32) (var_t3 + var_t4) / arg3) / 8) << 5) & 0x3E0) | ((u16) *(u8 *)((unsigned char *)var_v0 + (-4)) & 0xFC1F));
            *(u8 *)((unsigned char *)var_v0 + (-1)) = (u8) (((((s32) ((s32) (var_t5 + var_s0) / arg3) / 8) * 4) & 0x7C) | (*(u8 *)((unsigned char *)var_v0 + (-1)) & 0xFF83));
            *(u8 *)((unsigned char *)var_v0 + (-3)) = (u8) (((((s32) ((s32) (var_s1 + var_s2) / arg3) / 8) * 4) & 0x7C) | (*(u8 *)((unsigned char *)var_v0 + (-3)) & 0xFF83));
            var_t5 += sp6C;
            var_t4 += sp70;
            var_t3 += sp74;
            var_s1 += sp64;
            var_s2 += sp60;
            var_s0 += sp68;
        } while (var_v1 != sp5C);
        var_t0 = sp5C;
    }
    if (arg4 != 0) {
        var_v1_2 = 1;
        if (var_t0 >= 2) {
            temp_t8_3 = (var_t0 - 1) & 3;
            if (temp_t8_3 != 0) {
                var_v0_2 = ((u8 *)arg0) + (1 * 4);
                do {
                    var_v1_2 += 1;
                    *(u8 *)((unsigned char *)var_v0_2 + (1)) = (u8) (*(u8 *)((unsigned char *)var_v0_2 + (1)) | 0x80);
                    *(u8 *)((unsigned char *)var_v0_2 + (3)) = (u8) (*(u8 *)((unsigned char *)var_v0_2 + (3)) | 0x80);
                    var_v0_2 += 4;
                } while ((temp_t8_3 + 1) != var_v1_2);
                if (var_v1_2 != var_t0) {
                    goto block_10;
                }
            } else {
block_10:
                var_v0_3 = ((u8 *)arg0) + (var_v1_2 * 4);
                do {
                    *(u8 *)((unsigned char *)var_v0_3 + (3)) = (u8) (*(u8 *)((unsigned char *)var_v0_3 + (3)) | 0x80);
                    *(u8 *)((unsigned char *)var_v0_3 + (1)) = (u8) (*(u8 *)((unsigned char *)var_v0_3 + (1)) | 0x80);
                    *(u8 *)((unsigned char *)var_v0_3 + (7)) = (u8) (*(u8 *)((unsigned char *)var_v0_3 + (7)) | 0x80);
                    *(u8 *)((unsigned char *)var_v0_3 + (5)) = (u8) (*(u8 *)((unsigned char *)var_v0_3 + (5)) | 0x80);
                    *(u8 *)((unsigned char *)var_v0_3 + (11)) = (u8) (*(u8 *)((unsigned char *)var_v0_3 + (11)) | 0x80);
                    temp_t8_4 = *(u8 *)((unsigned char *)var_v0_3 + (13));
                    temp_t7 = *(u8 *)((unsigned char *)var_v0_3 + (9)) | 0x80;
                    temp_t6_3 = *(u8 *)((unsigned char *)var_v0_3 + (15)) | 0x80;
                    var_v0_3 += 0x10;
                    *(s8 *)((unsigned char *)var_v0_3 + (-3)) = (s8) (temp_t8_4 | 0x80);
                    *(s8 *)((unsigned char *)var_v0_3 + (-1)) = temp_t6_3;
                    *(s8 *)((unsigned char *)var_v0_3 + (-7)) = temp_t7;
                } while (var_v0_3 != ((var_t0 * 4) + ((u8 *)arg0)));
            }
        }
        *(u8 *)((unsigned char *)arg0 + (3)) = (u8) (*(u8 *)((unsigned char *)arg0 + (3)) | 0x80);
    } else {
        var_v1_3 = 0;
        if (var_t0 > 0) {
            temp_a1 = var_t0 & 3;
            if (temp_a1 != 0) {
                var_v0_4 = ((u8 *)arg0) + (0 * 4);
                do {
                    var_v1_3 += 1;
                    *(u8 *)((unsigned char *)var_v0_4 + (1)) = (u8) (*(u8 *)((unsigned char *)var_v0_4 + (1)) & 0xFF7F);
                    *(u8 *)((unsigned char *)var_v0_4 + (3)) = (u8) (*(u8 *)((unsigned char *)var_v0_4 + (3)) & 0xFF7F);
                    var_v0_4 += 4;
                } while (temp_a1 != var_v1_3);
                if (var_v1_3 != var_t0) {
                    goto block_18;
                }
            } else {
block_18:
                var_v0_5 = ((u8 *)arg0) + (var_v1_3 * 4);
                do {
                    *(u8 *)((unsigned char *)var_v0_5 + (3)) = (u8) (*(u8 *)((unsigned char *)var_v0_5 + (3)) & 0xFF7F);
                    *(u8 *)((unsigned char *)var_v0_5 + (1)) = (u8) (*(u8 *)((unsigned char *)var_v0_5 + (1)) & 0xFF7F);
                    *(u8 *)((unsigned char *)var_v0_5 + (7)) = (u8) (*(u8 *)((unsigned char *)var_v0_5 + (7)) & 0xFF7F);
                    *(u8 *)((unsigned char *)var_v0_5 + (5)) = (u8) (*(u8 *)((unsigned char *)var_v0_5 + (5)) & 0xFF7F);
                    temp_t6_4 = *(u8 *)((unsigned char *)var_v0_5 + (13));
                    temp_t8_5 = *(u8 *)((unsigned char *)var_v0_5 + (15));
                    *(u8 *)((unsigned char *)var_v0_5 + (9)) = (u8) (*(u8 *)((unsigned char *)var_v0_5 + (9)) & 0xFF7F);
                    *(u8 *)((unsigned char *)var_v0_5 + (11)) = (u8) (*(u8 *)((unsigned char *)var_v0_5 + (11)) & 0xFF7F);
                    var_v0_5 += 0x10;
                    *(s8 *)((unsigned char *)var_v0_5 + (-1)) = (s8) (temp_t8_5 & 0xFF7F);
                    *(s8 *)((unsigned char *)var_v0_5 + (-3)) = (s8) (temp_t6_4 & 0xFF7F);
                } while (var_v0_5 != ((var_t0 * 4) + ((u8 *)arg0)));
            }
        }
    }
    *(u8 *)((unsigned char *)arg0 + (1)) = (u8) (*(u8 *)((unsigned char *)arg0 + (1)) & 0xFF7F);
    (*(s32 *)arg0) = (u8) ((*(s32 *)arg0) & 0xFFE0);
    (*(s32 *)arg0) = (s16) ((u16) (*(s32 *)arg0) & 0xFC1F);
    *(u8 *)((unsigned char *)arg0 + (1)) = (u8) (*(u8 *)((unsigned char *)arg0 + (1)) & 0xFF83);
}
