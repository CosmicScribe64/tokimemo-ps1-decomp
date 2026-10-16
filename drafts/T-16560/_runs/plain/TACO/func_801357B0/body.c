void func_801357B0(u8 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    s32 var_a0;
    s32 var_s1;
    s32 var_s2;
    u8 *var_s0;
    u8 temp_t6;

    temp_t6 = *arg0;
    if (temp_t6 != 0) {
        var_s0 = arg0;
        var_s2 = arg5;
        var_a0 = temp_t6 & 0xFF;
        var_s1 = 0;
loop_2:
        func_80139424(var_a0, arg3, 3, 0x80, var_s2, arg6, arg1, arg2, arg4);
        var_s1 += 0xD;
        var_s0 += 1;
        var_s2 += 0xD;
        if (var_s1 < 0x111) {
            var_a0 = (s32) *var_s0;
            if (var_a0 != 0) {
                goto loop_2;
            }
        }
    }
}
