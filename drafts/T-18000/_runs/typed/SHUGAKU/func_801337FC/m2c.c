s32 func_80052648(s16);                             /* extern */
s32 func_80052C88(s16);                             /* extern */

s32 func_801337FC(void) {
    s16 var_s0;
    s16 var_s7;
    s32 temp_s2;
    s32 temp_s3;
    s32 temp_s4;
    s32 temp_v0;
    s32 temp_v1;
    u32 temp_a0;
    u32 temp_t2;
    u32 var_s5;
    u8 temp_a0_2;

    var_s7 = 0xFF;
    var_s5 = 4;
    var_s0 = 0;
    do {
        if ((D_800CA2B0[var_s0] != 0) && ((temp_v1 = func_80051A68(var_s0) & 0x7F, temp_a0 = temp_v1 & 0xFF, ((temp_a0 < var_s5) != 0)) || ((temp_a0 == var_s5) && (temp_a0 == var_s5) && (temp_s2 = func_80052C88(var_s0) & 0xFF, temp_s3 = func_80052648(var_s0) & 0xFF, temp_s4 = func_80052648(var_s7) & 0xFF, (((u32) (func_80052C88(var_s7) + temp_s4) < (u32) (temp_s3 + temp_s2)) != 0))))) {
            var_s7 = var_s0 & 0xFF;
            var_s5 = temp_v1 & 0xFF & 0xFF;
        }
        var_s0 += 1;
    } while (var_s0 < 0xA);
    temp_v0 = func_80051A68(var_s7);
    temp_t2 = temp_v0 & 0x7F;
    if ((var_s7 == 4) && (temp_t2 < 2U) && (D_800E6280.unk_721 == 5)) {
        if (D_800E6280.unk_56C[0x20] != 0) {
            D_800E6280.unk_71E = D_800E6280.unk_71E | 8;
        }
    }
    if ((var_s7 != 0xFF) && ((u32) (temp_v0 & 0x7F) < 2U)) {
        temp_a0_2 = var_s7 & 0xFF;
        if (!(D_800E6280.unk_71E & 8)) {
            D_800E6280.unk_F5F = temp_a0_2;
            func_800847B8(temp_a0_2);
            func_80133A68();
            if (D_800E6280.unk_720 != 6) {
                func_80085B3C(0xD, 0x26);
            } else {
                switch (D_800CA2CC) {               /* irregular */
                case 0:
                    func_80085B3C(0xD, 0x27);
                    break;
                case 1:
                    func_80085B3C(0xD, 0x28);
                    break;
                default:
                    func_80085B3C(0xD, 0x29);
                    break;
                }
            }
            return func_8004284C();
        }
    }
    D_8013BF0C = D_8013BE9C;
    D_8013BF10 = D_8013BED0;
    D_8013BF14 = D_8013BF04;
    D_8013BF18 = 0;
    D_8013BF1C = 0;
    D_800E6280.unk_110A = D_800E6280.unk_110A + 0x20;
    return 0;
}
