void func_801347D4(void) {
    GyoziWork *var_s1;
    s32 var_s0;
    s32 var_s3;
    u32 temp_t8;
    u32 temp_t9;
    u32 var_s2;
    u8 temp_a0;

    var_s3 = 0xFF;
    var_s2 = 4;
    var_s1 = &D_800F53A0;
    var_s0 = 0;
    do {
        if ((*(s32 *)((unsigned char *)var_s1 + (440)) & 2) && ((temp_t9 = func_8005E0E0(var_s0) & 0x7F, ((temp_t9 < var_s2) != 0)) || ((temp_t9 == var_s2) && ((D_800F53A0.girl[var_s3].unk_0A + D_800F53A0.girl[var_s3].unk_06) < (var_s1->girl[0].unk_06 + var_s1->girl[0].unk_0A))))) {
            var_s3 = var_s0 & 0xFF;
            var_s2 = temp_t9 & 0xFF;
        }
        var_s0 += 1;
        var_s1 += 0x38;
    } while (var_s0 != 0xB);
    temp_t8 = func_8005E0E0(var_s3) & 0x7F;
    if (var_s3 != 0xFF) {
        temp_a0 = var_s3 & 0xFF;
        if (temp_t8 < 2U) {
            D_800F62CF = temp_a0;
            func_8008F618(temp_a0);
            func_80134938();
            D_80145EB4 = 0;
            D_80145EB8 = 0;
            D_80145EC4 = 1;
            func_8004DEAC(1);
            return;
        }
    }
    D_80145EC4 = 0;
    func_8004DEAC(2);
}
