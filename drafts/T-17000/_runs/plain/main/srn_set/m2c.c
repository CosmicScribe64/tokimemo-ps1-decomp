extern u8 D_8012175C;

s32 srn_set(s32 arg0) {
    s32 spAC;
    s32 sp94;
    s32 sp88;
    s32 sp7C;
    s32 sp78;
    s32 sp64;
    s32 sp60;
    s16 temp_s7;
    s16 temp_t1;
    s16 var_s3;
    s32 *var_s1;
    s32 temp_a2;
    s32 temp_lo;
    s32 temp_lo_2;
    s32 temp_t3;
    s32 temp_t6;
    s32 var_at;
    s32 var_s0;
    s32 var_s2;
    u8 temp_a1;
    u8 temp_s4;
    u8 temp_s5;
    u8 temp_s6;
    void *var_v0;

    var_v0 = (arg0 * 0x28) + D_80121750;
    temp_a1 = *(u8 *)((unsigned char *)var_v0 + (15));
    temp_s4 = *(u8 *)((unsigned char *)var_v0 + (14));
    temp_s5 = *(u8 *)((unsigned char *)var_v0 + (16));
    temp_s6 = *(u8 *)((unsigned char *)var_v0 + (18));
    temp_s7 = *(s16 *)((unsigned char *)var_v0 + (2));
    temp_t1 = *(s16 *)((unsigned char *)var_v0 + (6));
    var_s1 = *(s32 **)((unsigned char *)var_v0 + (8));
    spAC = (s32) *(u8 *)((unsigned char *)var_v0 + (17));
    if ((temp_a1 == 0) || (temp_s4 == 0)) {
        var_v0 = (void *)-1;
    } else {
        var_s2 = *(s32 *)((unsigned char *)var_v0 + (28));
        sp88 = 0;
        sp7C = (s32) *(u16 *)((unsigned char *)var_v0 + (22));
        sp78 = (s32) *(u16 *)((unsigned char *)var_v0 + (24));
        temp_lo = (s32) (*(u8 *)((unsigned char *)var_v0 + (20)) * temp_s5) / (s32) temp_s4;
        sp64 = temp_lo;
        temp_lo_2 = (s32) (*(u8 *)((unsigned char *)var_v0 + (21)) * spAC) / (s32) temp_a1;
        sp94 = temp_lo_2;
        if (temp_lo_2 > 0) {
            var_s3 = temp_t1;
            sp60 = temp_t1 + (temp_a1 * *(u8 *)((unsigned char *)var_v0 + (19)));
loop_5:
            if ((sp60 >= -0x77) && (var_s0 = 0, (sp64 > 0))) {
                var_at = var_s3 < 0x79;
loop_8:
                if (var_at == 0) {
                    var_v0 = NULL;
                } else {
                    temp_a2 = (temp_s5 * var_s0) + temp_s7;
                    if ((temp_a2 + (temp_s6 * temp_s4)) >= -0x9F) {
                        temp_t6 = *var_s1 & 0xFFFF;
                        var_v0 = (void *) temp_t6;
                        if (temp_t6 != 0) {
                            var_v0 = mod_set_gpu(0, temp_t6, temp_a2, (s32) var_s3, sp7C, sp78, (s32) D_8012175C);
                        }
                    }
                    var_s2 -= 1;
                    var_s1 += 4;
                    if (var_s2 == 0) {
                        var_v0 = NULL;
                    } else {
                        var_s0 += 1;
                        var_at = var_s3 < 0x79;
                        if (var_s0 == temp_lo) {
                            goto block_16;
                        }
                        goto loop_8;
                    }
                }
            } else {
block_16:
                temp_t3 = sp88 + 1;
                sp60 += spAC;
                sp88 = temp_t3;
                var_s3 += spAC;
                if (temp_t3 < sp94) {
                    goto loop_5;
                }
            }
        }
    }
    return (s32) var_v0;
}
