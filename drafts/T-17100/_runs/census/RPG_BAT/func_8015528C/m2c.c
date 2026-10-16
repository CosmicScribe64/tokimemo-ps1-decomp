
void func_8015528C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8) {
    s32 *temp_fp;
    s32 *temp_s7;
    s32 temp_a1;
    s32 temp_a2;
    s32 temp_lo;
    s32 temp_lo_2;
    s32 temp_t6;
    s32 temp_v0;
    s32 var_a0;
    s32 var_a3;
    s32 var_s1;
    s32 var_v1;

    temp_fp = &(&D_8015EBAC)[arg6];
    temp_v0 = *temp_fp;
    var_s1 = 0;
    if (arg3 < (temp_v0 / 10)) {
        temp_s7 = &(&D_8015EBAC)[arg7];
        *temp_fp = temp_v0 - (arg3 * 0xA);
        temp_t6 = *temp_s7 - 1;
        *temp_s7 = temp_t6;
        if (temp_t6 < 0) {
            *temp_s7 = 2;
        }
    }
    do {
        temp_lo = (s32) *temp_fp / 10;
        temp_a2 = (&D_8015EBAC)[arg7] + var_s1;
        temp_a1 = temp_lo + (var_s1 * arg3);
        temp_lo_2 = (var_s1 - 1) * arg3;
        if (temp_a1 < arg0) {
            var_a0 = temp_lo + temp_lo_2;
            var_a3 = temp_a1;
            var_v1 = temp_a2 % 3;
            if (var_a0 < 0) {
                goto block_9;
            }
        } else {
            var_a0 = temp_lo + temp_lo_2;
            var_a3 = arg0;
            var_s1 = 0x64;
            var_v1 = temp_a2 % 3;
            if (var_a0 < 0) {
block_9:
                var_a0 = 0;
            }
        }
        switch (var_v1) {
        case 0:
            func_80155868(var_a0, (s32) (arg1 * var_a0) / arg0, (s32) (arg2 * var_a0) / arg0, var_a3, (s32) (arg1 * var_a3) / arg0, (s32) (arg2 * var_a3) / arg0, arg8 << 0x10, arg8 << 8, 4);
            break;
        case 1:
            func_80155868(var_a0, (s32) (arg1 * var_a0) / arg0, (s32) (arg2 * var_a0) / arg0, var_a3, (s32) (arg1 * var_a3) / arg0, (s32) (arg2 * var_a3) / arg0, arg8 << 8, arg8, 4);
            break;
        case 2:
            func_80155868(var_a0, (s32) (arg1 * var_a0) / arg0, (s32) (arg2 * var_a0) / arg0, var_a3, (s32) (arg1 * var_a3) / arg0, (s32) (arg2 * var_a3) / arg0, arg8, arg8 << 0x10, 4);
            break;
        }
        var_s1 += 1;
    } while (var_s1 < 0x32);
    dtd_on_tpage(0, 0, 4, arg5 - 4, 0);
    *temp_fp += arg4;
}
