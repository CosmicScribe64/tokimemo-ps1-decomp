void func_80135A48(s32 arg0, s32 arg1) {
    s16 *temp_a0;
    s32 *temp_v1;
    s32 *var_v1;
    s32 temp_a2;
    s32 var_v0;

    temp_v1 = &D_80139278[D_801392A0];
    temp_a2 = *temp_v1;
    if ((temp_a2 <= 0) || (temp_a2 >= 0xA)) {
        var_v0 = D_801392A0 + 1;
        var_v1 = temp_v1 + 4;
        if (var_v0 >= 0xA) {
            var_v1 = D_80139278;
            var_v0 = 0;
        }
        temp_a0 = D_80139228[var_v0];
        temp_a0[1] = arg0 + 0xA0;
        temp_a0[3] = arg1 + 0x78;
        *var_v1 = 1;
        D_801392A0 = var_v0;
    }
}
