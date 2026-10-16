s32 func_8013243C(void) {
    s16 var_s0;
    s16 var_s7;
    s32 temp_s2;
    s32 temp_s3;
    s32 temp_s4;
    s32 temp_v0;
    s32 temp_v1;
    u32 temp_a0;
    u32 var_s5;
    u8 *var_t1;

    temp_v0 = D_800E6280.unk_0F8;
    var_s7 = 0xFF;
    var_s5 = 4;
    if (((temp_v0 & 0xF) != D_800E6280.unk_03F) || (var_s0 = 0, (((u32) (temp_v0 << 0x17) >> 0x1B) != D_800E6280.unk_040))) {
        func_8004284C();
        return 0;
    }
    var_t1 = D_800CA2B0;
    do {
        if ((*var_t1 != 0) && ((temp_v1 = get_g_zyotai_s(var_s0) & 0x7F, temp_a0 = temp_v1 & 0xFF, ((temp_a0 < var_s5) != 0)) || ((temp_a0 == var_s5) && (temp_a0 == var_s5) && (temp_s2 = get_h_yuukou(var_s0) & 0xFF, temp_s3 = get_h_tokimeki(var_s0) & 0xFF, temp_s4 = get_h_tokimeki(var_s7) & 0xFF, (((u32) (get_h_yuukou(var_s7) + temp_s4) < (u32) (temp_s3 + temp_s2)) != 0))))) {
            var_s7 = var_s0 & 0xFF;
            var_s5 = temp_v1 & 0xFF & 0xFF;
        }
        var_s0 += 1;
        var_t1 = &D_800CA2B0[var_s0];
    } while (var_s0 < 0xA);
    if (((u32) (get_g_zyotai_s(var_s7) & 0x7F) < 2U) && (var_s7 == 0)) {
        func_80042940(0x32);
        return 0;
    }
    return func_8004284C();
}
