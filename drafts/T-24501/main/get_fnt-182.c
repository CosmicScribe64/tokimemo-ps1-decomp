void get_fnt(u8 *arg0, u8 *arg1, s32 arg2, s32 arg3, s32 arg4, u8 arg5) {
    u8 sp8C[256];
    s32 temp_lo;
    s32 var_s0;
    s32 var_s1;
    s32 vertical_row;

    var_s0 = 0;
    if (arg4 > 0) {
        temp_lo = 0x80 / arg4;
        var_s1 = 0;
        do {
            memcpy((var_s1 / arg3) + arg0, ((s32) ((arg2 % temp_lo) * arg4) / arg3) + arg1 + ((s32) ((((arg2 / temp_lo) * arg4) + var_s0) << 7) / arg3), arg4 / arg3);
            var_s0 += 1;
            var_s1 += arg4;
        } while (var_s0 != arg4);
    }
    if (arg5 & 1) {
        memcpy(sp8C, arg0, 0x100);
        for (vertical_row = 0; vertical_row < arg4; vertical_row++) {
            memcpy(arg0 + (arg4 - vertical_row - 1) * arg4, sp8C + vertical_row * arg4, arg4);
        }
    }
    if (arg5 & 2) {
        memcpy(sp8C, arg0, 0x100);
        for (var_s0 = 0; var_s0 < arg4; var_s0++) {
            arg0[var_s0 * arg4 + 4] = sp8C[var_s0 * arg4 + 3];
            arg0[var_s0 * arg4 + 5] = sp8C[var_s0 * arg4 + 2];
            arg0[var_s0 * arg4 + 6] = sp8C[var_s0 * arg4 + 1];
            arg0[var_s0 * arg4 + 7] = sp8C[var_s0 * arg4];
            arg0[var_s0 * arg4] = sp8C[var_s0 * arg4 + 7];
            arg0[var_s0 * arg4 + 1] = sp8C[var_s0 * arg4 + 6];
            arg0[var_s0 * arg4 + 2] = sp8C[var_s0 * arg4 + 5];
            arg0[var_s0 * arg4 + 3] = sp8C[var_s0 * arg4 + 4];
        }
    }
}
