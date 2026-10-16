s32 func_80052648(s16);                             /* extern */
s32 func_80052C88(s16);                             /* extern */

s32 func_80134808(void) {
    s16 var_fp;
    s16 var_s0;
    s16 var_s2;
    s32 temp_s3;
    s32 temp_s4;
    s32 temp_s5;
    s32 temp_v0;
    s32 temp_v1;
    u32 temp_a0;
    u32 var_s6;
    u8 temp_a0_2;

    var_fp = 0xFF;
    var_s6 = 4;
    var_s0 = 0;
    do {
        if (!(*(&D_800E6280.unk_1BC[0].unk_0C + (var_s0 * 0x38)) & 2)) {
            var_s2 = var_s0 + 1;
        } else if ((!(D_800E6280.unk_71E & 0x80) || (var_s2 = var_s0 + 1, (var_s2 != D_80122D40))) && ((var_s2 = var_s0 + 1, temp_v1 = func_80051A68(var_s0) & 0x7F, temp_a0 = temp_v1 & 0xFF, ((temp_a0 < var_s6) != 0)) || ((temp_a0 == var_s6) && (temp_a0 == var_s6) && (temp_s3 = func_80052C88(var_s0) & 0xFF, temp_s4 = func_80052648(var_s0) & 0xFF, temp_s5 = func_80052648(var_fp) & 0xFF, (((u32) (func_80052C88(var_fp) + temp_s5) < (u32) (temp_s4 + temp_s3)) != 0))))) {
            var_fp = var_s0 & 0xFF;
            var_s6 = temp_v1 & 0xFF & 0xFF;
        }
        var_s0 = var_s2;
    } while (var_s2 < 0xB);
    temp_v0 = func_80051A68(var_fp);
    D_80122D40 = 0;
    if (((u32) (temp_v0 & 0x7F) >= 2U) || (var_fp == 0xFF) || (temp_a0_2 = var_fp & 0xFF, ((D_800E6280.unk_71E & 8) != 0))) {
        func_80042940(0x17);
        return 0;
    }
    D_800E6280.unk_F5F = temp_a0_2;
    func_800847B8(temp_a0_2);
    func_80134F84();
    func_80134A14();
    return func_8004284C();
}
