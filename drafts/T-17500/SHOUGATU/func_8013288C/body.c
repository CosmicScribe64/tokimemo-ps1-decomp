void func_8013288C(void) {
    GameState *var_s6;
    s32 temp_s2;
    s32 temp_s3;
    s32 temp_s4;
    s32 temp_v1;
    u32 temp_a0;
    u32 temp_t2;
    u32 var_s5;
    u8 temp_a0_2;
    u8 var_fp;
    u8 var_s0;

    var_fp = 0xFF;
    var_s5 = 4;
    D_80122D40 = 0;
    var_s6 = &D_800E6280;
    var_s0 = 0;
    do {
        if ((var_s6->unk_1BC[0].unk_0C.w & 2) && ((temp_v1 = get_g_zyotai_s(var_s0) & 0x7F, temp_a0 = temp_v1 & 0xFF, ((temp_a0 < var_s5) != 0)) || ((temp_a0 == var_s5) && (temp_a0 == var_s5) && (temp_s2 = get_h_yuukou(var_s0) & 0xFF, temp_s3 = get_h_tokimeki(var_s0) & 0xFF, temp_s4 = get_h_tokimeki(var_fp) & 0xFF, (((u32) (get_h_yuukou(var_fp) + temp_s4) < (u32) (temp_s3 + temp_s2)) != 0))))) {
            var_fp = var_s0 & 0xFF;
            var_s5 = temp_v1 & 0xFF & 0xFF;
        }
        var_s0 += 1;
        var_s6 += 0x38;
    } while (var_s0 != 0xB);
    temp_t2 = get_g_zyotai_s(var_fp) & 0x7F;
    if (var_fp != 0xFF) {
        temp_a0_2 = var_fp & 0xFF;
        if (temp_t2 < 2U) {
            D_80143B1C = var_fp;
            D_800E6280.unk_F5F = temp_a0_2;
            func_800847B8(temp_a0_2);
            func_80132A34();
            D_80143B00 = 0;
            D_80143B04 = 0;
            D_80143B14 = 1;
            D_80122D40 = var_fp + 1;
            D_800E6280.unk_71E |= 0x80;
            func_80042908(1);
            return;
        }
    }
    D_80143B14 = 0;
    func_80042908(2);
}
