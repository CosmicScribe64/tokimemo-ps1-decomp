void func_8014BFE4(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 sp58;
    s32 sp50;
    s16 var_s1;
    s32 temp_a1;
    s32 temp_hi;
    s32 temp_lo;
    s32 temp_s0;
    s32 temp_s3;
    s32 temp_t1;
    s32 temp_t7;
    s32 var_a1;
    s32 var_s2;
    s32 var_s4;
    s32 var_s6;
    u8 *temp_v0;
    u8 *temp_v0_2;
    u8 *var_s0;

    temp_a1 = arg1;
    temp_t7 = temp_a1 - 0xA0;
    arg1 = temp_t7;
    var_s1 = arg2 - 0x78;
    if (arg3 >= 0xA) {
        if (arg3 < 0x64) {
            arg1 = temp_t7 + 4;
        } else if (arg3 < 0x3E8) {
            arg1 += 9;
        } else {
            arg1 += 0xD;
        }
    }
    var_a1 = 1;
    switch (arg0) {
    case 0:
        D_8015EE5C = 0;
        D_8015EE60 = 1;
        D_8015EE64 = 0;
        break;
    case 1:
        D_8015EE68 = 0;
        D_8015EE6C = 1;
        D_8015EE70 = 0;
        break;
    case 2:
        D_8015EE74 = 0;
        D_8015EE78 = 1;
        D_8015EE7C = 0;
        break;
    case 3:
        D_8015EE80 = 0;
        D_8015EE84 = 1;
        D_8015EE88 = 0;
        break;
    }
    temp_t1 = arg0 * 0xC;
    var_s4 = arg1;
    var_s0 = (arg0 * 0xCC * 4) + D_8011ECD0;
    sp50 = temp_t1;
    var_s2 = temp_t1 + 0x40;
    var_s6 = 0;
    do {
        temp_lo = arg3 / var_a1;
        *(s16 *)((unsigned char *)var_s0 + (4390)) = (s16) var_s4;
        *(s16 *)((unsigned char *)var_s0 + (4394)) = var_s1;
        *(s16 *)((unsigned char *)var_s0 + (4662)) = (s16) var_s4;
        *(s16 *)((unsigned char *)var_s0 + (4666)) = var_s1;
        *(s16 *)((unsigned char *)var_s0 + (4934)) = (s16) var_s4;
        *(s16 *)((unsigned char *)var_s0 + (4938)) = var_s1;
        if (temp_lo != 0) {
            temp_hi = temp_lo % 10;
            arg2 = (s32) var_s1;
            sp58 = var_a1;
            temp_s3 = temp_hi + 0x27;
            func_8013E864(var_s2 + 4, temp_s3, 0);
            func_8013E864(var_s2 + 8, temp_s3, 0);
            if (arg4 == 0) {
                func_8013E864(var_s2, temp_hi + 0x1D, 0);
            } else {
                func_8013E864(var_s2, temp_hi + 0x13, 0);
            }
            var_s1 = (s16) arg2;
            var_a1 = sp58;
        } else {
            temp_v0 = (var_s2 * 0x44) + D_8011ECD0;
            *(s16 *)((unsigned char *)temp_v0 + (38)) = 0xA0;
            *(s16 *)((unsigned char *)temp_v0 + (42)) = 0x78;
            *(s16 *)((unsigned char *)temp_v0 + (310)) = 0xA0;
            *(s16 *)((unsigned char *)temp_v0 + (314)) = 0x78;
            *(s16 *)((unsigned char *)temp_v0 + (582)) = 0xA0;
            *(s16 *)((unsigned char *)temp_v0 + (586)) = 0x78;
        }
        var_s6 += 9;
        var_s2 += 1;
        var_s0 += 0x44;
        var_s4 -= 9;
        var_a1 *= 0xA;
    } while (var_s6 != 0x24);
    if (arg3 == 0) {
        temp_v0_2 = (sp50 * 0x44) + D_8011ECD0;
        *(s16 *)((unsigned char *)temp_v0_2 + (4394)) = var_s1;
        *(s16 *)((unsigned char *)temp_v0_2 + (4666)) = var_s1;
        *(s16 *)((unsigned char *)temp_v0_2 + (4938)) = var_s1;
        *(s16 *)((unsigned char *)temp_v0_2 + (4390)) = (s16) arg1;
        *(s16 *)((unsigned char *)temp_v0_2 + (4662)) = (s16) arg1;
        *(s16 *)((unsigned char *)temp_v0_2 + (4934)) = (s16) arg1;
        func_8013E864(sp50 + 0x44, 0x27, 0);
        temp_s0 = sp50 + 0x40;
        func_8013E864(temp_s0 + 8, 0x27, 0);
        if (arg4 == 0) {
            func_8013E864(temp_s0, 0x1D, 0);
            return;
        }
        func_8013E864(temp_s0, 0x13, 0);
    }
}
