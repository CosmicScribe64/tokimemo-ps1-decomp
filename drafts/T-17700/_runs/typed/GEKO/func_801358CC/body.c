s32 func_801358CC(void) {
    s32 temp_s2;
    s32 temp_s3;
    s32 temp_s4;
    s32 temp_v0;
    s32 temp_v1;
    s32 temp_v1_2;
    s32 var_t6;
    u32 temp_a0;
    u32 var_s6;
    u8 temp_v1_3;
    u8 var_s0;
    u8 var_s7;

    var_s7 = 0xFF;
    var_s6 = 4;
    var_s0 = 0;
    var_t6 = 0 * 8;
    do {
        if ((*(s32 *)((unsigned char *)(&D_800E6280 + ((var_t6 - var_s0) * 8)) + (456)) & 2) && ((temp_v1 = get_g_zyotai_s(var_s0) & 0x7F, temp_a0 = temp_v1 & 0xFF, ((temp_a0 < var_s6) != 0)) || ((temp_a0 == var_s6) && (temp_s2 = get_h_yuukou(var_s0) & 0xFF, temp_s3 = get_h_tokimeki(var_s0) & 0xFF, temp_s4 = get_h_tokimeki(var_s7) & 0xFF, (((u32) (get_h_yuukou(var_s7) + temp_s4) < (u32) (temp_s3 + temp_s2)) != 0))))) {
            var_s7 = var_s0 & 0xFF;
            var_s6 = temp_v1 & 0xFF & 0xFF;
        }
        var_s0 = (u8) (s16) (var_s0 + 1);
        var_t6 = var_s0 * 8;
    } while ((s32) var_s0 < 0xB);
    temp_v0 = get_g_zyotai_s(D_800E6280.unk_F5F);
    temp_v1_2 = D_800E6280.unk_0F8;
    if ((D_800E6280.unk_03F == (temp_v1_2 & 0xF)) && (D_800E6280.unk_040 == ((u32) (temp_v1_2 << 0x17) >> 0x1B))) {
        temp_v1_3 = D_800E6280.unk_71E;
        if (!(temp_v1_3 & 4) && ((u32) (temp_v0 & 0x7F) < 2U) && (var_s7 == D_800E6280.unk_F5F)) {
            D_800E6280.unk_71E = temp_v1_3 | 0x44;
            func_80042940(0x34);
            return 0;
        }
    }
    return func_8004284C();
}
