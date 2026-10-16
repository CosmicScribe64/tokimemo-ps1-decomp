void func_8004A814(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7) {
    s32 spAC;
    u32 spA0;
    u32 sp9C;
    s32 sp98;
    s32 sp84;
    s32 sp80;
    s32 sp7C;
    s32 sp74;
    s32 sp6C;
    s32 sp58;
    s32 temp_lo;
    s32 temp_s1;
    s32 temp_s1_2;
    s32 temp_s1_3;
    s32 temp_s2;
    s32 temp_s5;
    s32 temp_s7;
    s32 temp_t2;
    s32 temp_t4;
    s32 temp_t9;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 var_a2;
    s32 var_at;
    s32 var_at_2;
    s32 var_s3;
    s32 var_s4;
    s32 var_s6;

    temp_t2 = arg3 & 0x7F;
    spA0 = (u32) (arg5 & 0xFF0000) >> 0x10;
    sp9C = (u32) (arg5 & 0xFF00) >> 8;
    sp98 = arg5 & 0xFF;
    arg3 = temp_t2;
    arg4 &= 0x7F;
    if (temp_t2 < 2) {
        arg3 = 2;
    }
    if (arg4 < 2) {
        arg4 = 2;
    }
    temp_v0 = arg0 - arg2;
    temp_t9 = arg0 + arg2;
    sp84 = temp_t9;
    spAC = temp_v0;
    if (temp_v0 < temp_t9) {
        sp7C = arg1 + arg2;
        sp80 = arg1 - arg2;
        do {
            var_s6 = sp80;
            if (var_s6 < sp7C) {
                temp_s7 = arg0 - spAC;
                sp74 = temp_s7 * temp_s7;
                sp6C = arg2 * arg2;
                do {
                    temp_s2 = arg1 - var_s6;
                    temp_s5 = arg2 * 2;
                    if ((sp74 + (temp_s2 * temp_s2)) < sp6C) {
                        sp58 = arg7 * 4;
                        temp_v0_2 = GetWorkBase(0x10, D_8011ECA0);
                        func_8009F0F4(temp_v0_2);
                        SetSemiTrans(temp_v0_2, 1);
                        *(s16 *)((unsigned char *)temp_v0_2 + (8)) = (s16) ((D_800E6280.unk_014[D_8011ECA0] + (s16) spAC) - (arg3 / 2));
                        *(s16 *)((unsigned char *)temp_v0_2 + (14)) = (s16) arg3;
                        *(s16 *)((unsigned char *)temp_v0_2 + (10)) = (s16) ((D_800E6280.unk_018[D_8011ECA0] + var_s6) - (arg4 / 2));
                        *(s16 *)((unsigned char *)temp_v0_2 + (12)) = (s16) arg3;
                        temp_s1 = abs(temp_s7);
                        var_s4 = (s32) (((temp_s5 - abs(temp_s2)) - temp_s1) * spA0) / arg2;
                        temp_s1_2 = abs(temp_s7);
                        var_s3 = (s32) (((temp_s5 - abs(temp_s2)) - temp_s1_2) * sp9C) / arg2;
                        temp_s1_3 = abs(temp_s7);
                        temp_lo = (s32) (((temp_s5 - abs(temp_s2)) - temp_s1_3) * sp98) / arg2;
                        var_a2 = temp_lo;
                        var_at = var_s3 < 0x100;
                        if (var_s4 >= 0x100) {
                            var_s4 = 0xFF;
                            var_at = var_s3 < 0x100;
                        }
                        var_at_2 = temp_lo < 0x100;
                        if (var_at == 0) {
                            var_s3 = 0xFF;
                            var_at_2 = temp_lo < 0x100;
                        }
                        if (var_at_2 == 0) {
                            var_a2 = 0xFF;
                        }
                        *(s8 *)((unsigned char *)temp_v0_2 + (4)) = (s8) var_s4;
                        *(s8 *)((unsigned char *)temp_v0_2 + (5)) = (s8) var_s3;
                        *(s8 *)((unsigned char *)temp_v0_2 + (6)) = (s8) var_a2;
                        AddPrim(&D_800E8CA0[(D_8011ECA0 << 0xA) + sp58], temp_v0_2);
                    }
                    var_s6 += arg4;
                } while (var_s6 < sp7C);
            }
            temp_t4 = spAC + arg3;
            spAC = temp_t4;
        } while (temp_t4 < sp84);
    }
    dtd_on_tpage(0, 0, arg7, arg6 & 3, 0);
}
