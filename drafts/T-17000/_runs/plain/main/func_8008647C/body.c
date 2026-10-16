s32 func_8008647C(void) {
    s32 sp6C[16];
    s32 sp70;
    GameState *var_v0;
    s32 *var_s1;
    s32 temp_a2;
    s32 temp_s0;
    s32 temp_s0_2;
    s32 temp_s3;
    s32 var_s2;
    s32 var_s4;
    s32 var_s5;
    u32 temp_s0_3;
    u32 temp_s0_4;
    u32 var_s2_2;
    u32 var_s6;

    var_s6 = 0;
    var_v0 = &D_800E6280;
    var_s2 = 0;
    do {
        if (((u32) (var_v0->unk_1BC[0].unk_0C.w << 0x1E) >> 0x1F) == 1) {
            (&sp6C[0])[var_s6] = var_s2;
            var_s6 = (var_s6 + 1) & 0xFF;
        }
        var_s2 += 1;
        var_v0 += 0x38;
    } while (var_s2 < 0xB);
    temp_s0 = get_h_yuukou(0) & 0xFF;
    var_s5 = get_h_tokimeki(0) + temp_s0;
    var_s4 = 0;
    var_s2_2 = 1;
    if (var_s6 >= 2U) {
        var_s1 = &sp70;
        do {
            temp_s0_2 = get_h_tokimeki(*var_s1) & 0xFF;
            temp_s3 = get_h_yuukou(*var_s1) + temp_s0_2;
            if ((var_s5 < temp_s3) && (temp_s0_3 = func_8008667C(var_s4) & 0xFF, ((temp_s0_3 < func_8008667C(*var_s1)) == 0))) {
                var_s4 = *var_s1;
                var_s5 = temp_s3;
            } else if (var_s5 == temp_s3) {
                temp_s0_4 = func_8008667C(var_s4) & 0xFF;
                if (temp_s0_4 >= func_8008667C(*var_s1)) {
                    temp_a2 = *var_s1;
                    if ((D_800E6280.unk_1BC[var_s4].unk_06 + D_800E6280.unk_1BC[var_s4].unk_02) < (D_800E6280.unk_1BC[temp_a2].unk_06 + D_800E6280.unk_1BC[temp_a2].unk_02)) {
                        var_s5 = temp_s3;
                        var_s4 = temp_a2;
                    }
                }
            }
            var_s2_2 += 1;
            var_s1 += 4;
        } while (var_s2_2 < var_s6);
    }
    return var_s4;
}
