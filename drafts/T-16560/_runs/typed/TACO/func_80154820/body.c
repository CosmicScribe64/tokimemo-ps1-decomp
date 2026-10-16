void func_80154820(s32 arg0, s32 arg1) {
    s32 *temp_s2;
    s32 temp_v0;
    s32 var_a0;
    s32 var_a1;
    s32 var_s0;
    s32 var_v1;
    u32 temp_v0_2;

    temp_s2 = &D_8015F290[arg0];
    var_s0 = (*(s32 *)temp_s2);
loop_1:
    if (func_8015431C(var_s0) != 0) {
        temp_v0 = (*(s32 *)var_s0);
        if ((temp_v0 == 0x21010001) || (temp_v0 == 0x21010002) || (temp_v0 == 0x21010304)) {
            temp_v0_2 = *(u32 *)((unsigned char *)var_s0 + (4));
            var_s0 += 4;
            var_v1 = ((temp_v0_2 >> 0x10) & 0xFF) + arg1;
            var_a0 = ((temp_v0_2 >> 8) & 0xFF) + arg1;
            var_a1 = (temp_v0_2 & 0xFF) + arg1;
            if (var_v1 >= 0x100) {
                var_v1 = 0xFF;
            }
            if (var_a0 >= 0x100) {
                var_a0 = 0xFF;
            }
            if (var_a1 >= 0x100) {
                var_a1 = 0xFF;
            }
            *(s32 *)var_s0 = (s32) ((temp_v0_2 & 0xFF000000) + ((var_v1 & 0xFF) << 0x10) + ((var_a0 & 0xFF) << 8) + (var_a1 & 0xFF));
        }
        var_s0 += 4;
        if ((u32) var_s0 >= (u32) *(u32 *)((unsigned char *)temp_s2 + (4))) {

        } else {
            goto loop_1;
        }
    }
}
