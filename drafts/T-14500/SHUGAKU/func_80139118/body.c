void func_80139118(void) {
    s16 var_s0;
    s16 var_s7;
    s32 temp_s2;
    s32 temp_s3;
    s32 temp_s4;
    s32 temp_v1;
    u32 temp_a0;
    u32 temp_t3;
    u32 var_s6;
    u8 *var_t6;
    u8 temp_a0_2;

    D_8013C978 = 0;
    var_s7 = 0xFF;
    var_s6 = 4;
    D_8013C980 = 0;
    var_s0 = 0;
    var_t6 = D_800CA2B0;
    do {
        if ((*var_t6 != 0) && ((temp_v1 = get_g_zyotai_s(var_s0) & 0x7F, temp_a0 = temp_v1 & 0xFF, ((temp_a0 < var_s6) != 0)) || ((temp_a0 == var_s6) && (temp_s2 = get_h_yuukou(var_s0) & 0xFF, temp_s3 = get_h_tokimeki(var_s0) & 0xFF, temp_s4 = get_h_tokimeki(var_s7) & 0xFF, (((u32) (get_h_yuukou(var_s7) + temp_s4) < (u32) (temp_s3 + temp_s2)) != 0))))) {
            var_s7 = var_s0 & 0xFF;
            var_s6 = temp_v1 & 0xFF & 0xFF;
        }
        var_s0 += 1;
        var_t6 = &D_800CA2B0[var_s0];
    } while (var_s0 < 0xA);
    temp_t3 = get_g_zyotai_s(var_s7) & 0x7F;
    if ((var_s7 != 0xFF) && (temp_t3 < 2U)) {
        D_8013C980 = var_s7 + 0x32;
        temp_a0_2 = var_s7 & 0xFF;
        D_800E71DF = temp_a0_2;
        func_800847B8(temp_a0_2);
        func_801399F4();
    } else {
        D_800CA2DC += 4;
        D_800E738A += 0x1E;
        D_800E71DF = 0xFF;
    }
    func_8004284C();
}
