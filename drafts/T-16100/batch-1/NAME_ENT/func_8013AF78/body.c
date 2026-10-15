u8 func_8013AF78(s32 arg0) {
    s32 flags[3];
    s32 *var_v1;
    u8 temp_t6;
    u8 temp_t7;
    u8 temp_t8;
    u8 temp_t9;
    u8 var_v0;
    u8 var_v1_2;

    var_v1 = &flags[0];
    do {
        var_v1 += 4;
        *(s32 *)((unsigned char *)var_v1 + (-4)) = 0;
    } while ((u32) var_v1 < (u32) &flags[3]);
    temp_t8 = D_80120BE7 & 0x7F;
    temp_t6 = D_80120B5F & 0x7F;
    temp_t7 = D_80120BA3 & 0x7F;
    temp_t9 = D_80120C2B & 0x7F;
    var_v0 = temp_t9;
    var_v1_2 = temp_t7;
    if (arg0 == 0) {
        D_80120BE7 = temp_t8;
        D_80120B5F = temp_t6;
        D_80120BA3 = temp_t7;
        D_80120C2B = temp_t9;
        return 0U;
    }
    *(s32 *)((unsigned char *)(sp + (((s32) D_8014D0D4 % 3) * 4)) + (28)) = 1;
    D_80120BE7 = temp_t8;
    if (flags[0] != 0) {
        var_v1_2 = (var_v1_2 | 0x80) & 0xFF;
        D_80120BE7 = temp_t8 | 0x80;
    }
    D_80120B5F = temp_t6;
    if (flags[1] != 0) {
        D_80120B5F = temp_t6 | 0x80;
    }
    D_80120C2B = var_v0;
    D_80120BA3 = var_v1_2;
    if (flags[2] != 0) {
        var_v0 = (var_v0 | 0x80) & 0xFF;
        D_80120C2B = var_v0;
        D_80120BA3 = var_v1_2 | 0x80;
    }
    return var_v0;
}
