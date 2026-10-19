s32 func_801358CC(void) {
    s32 temp_s2;
    s32 temp_s3;
    s32 temp_s4;
    s32 temp_v0;
    s32 temp_v1;
    s32 var_t6;
    u32 temp_a0;
    u32 var_s6;
    u8 var_s0;
    u8 var_s7;

    var_s7 = 0xFF;
    var_s6 = 4;
    var_s0 = 0;
    var_t6 = 0 * 8;
    do {
        if ((*(s32 *)((unsigned char *)(&D_800E6280 + ((var_t6 - var_s0) * 8)) + (456)) & 2) && ((temp_v1 = func_80051A68(var_s0) & 0x7F, temp_a0 = temp_v1 & 0xFF, ((temp_a0 < var_s6) != 0)) || ((temp_a0 == var_s6) && (temp_s2 = func_80052C88(var_s0) & 0xFF, temp_s3 = func_80052648(var_s0) & 0xFF, temp_s4 = func_80052648(var_s7) & 0xFF, (((u32) (func_80052C88(var_s7) + temp_s4) < (u32) (temp_s3 + temp_s2)) != 0))))) {
            var_s7 = var_s0 & 0xFF;
            var_s6 = temp_v1 & 0xFF & 0xFF;
        }
        var_s0 = (u8) (s16) (var_s0 + 1);
        var_t6 = var_s0 * 8;
    } while ((s32) var_s0 < 0xB);
    temp_v0 = func_80051A68(D_800E71DF);
    if ((D_800E62BF == (D_800E6378 & 0xF)) && (D_800E62C0 == ((u32) (D_800E6378 << 0x17) >> 0x1B)) && !(D_800E699E & 4) && ((u32) (temp_v0 & 0x7F) < 2U) && (var_s7 == D_800E71DF)) {
        D_800E699E |= 0x44;
        func_80042940(0x34);
        return 0;
    }
    return func_8004284C();
}
