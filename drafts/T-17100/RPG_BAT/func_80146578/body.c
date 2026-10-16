void func_80146578(void) {
    s32 sp44;
    s32 sp38;
    DisplayActor44 *var_s0;
    s32 temp_lo;
    s32 temp_lo_2;
    s32 temp_s0;
    s32 temp_s0_2;
    s32 temp_s0_3;
    s32 temp_s0_4;
    s32 temp_s1;
    s32 temp_s2;
    s32 temp_v1;
    s32 var_a0;
    s32 var_a0_2;
    s32 var_s1;
    s32 var_s1_2;
    s32 var_s1_3;

    func_8013F15C(0x508, 0, 1);
    var_s1 = 0;
    switch (D_8015EDDC) {
    case 0:
        var_s0 = ((DisplayActor44 *)D_8011ECD0);
        do {
            *(s32 *)&var_s0[148].unk_0C = (s32) D_8015EEA8;
            *(s32 *)&var_s0[148].unk_10 = (s32) D_8015EEC0;
            temp_s2 = var_s1 + 0x34;
            var_s0[148].unk_38 = 0x41000000;
            var_s0[148].unk_07 = 0x80;
            var_s0[148].unk_43 = 0x13;
            *(s32 *)&var_s0[148].unk_34 = (s32) D_8015EE90;
            func_8013E97C(temp_s2, 0, 0);
            func_8013E7C0(temp_s2, var_s1 + 5, 1, 1);
            var_s1 += 1;
            var_s0 += 1;
        } while (var_s1 != 0xC);
        func_8014EF6C();
        break;
    case 1:
        temp_s1 = D_8015EDE0 + 1;
        D_8015EDE0 = temp_s1;
        temp_lo = (s32) (temp_s1 * -0x30) / 100;
        temp_v1 = func_800A0070((s32) (temp_s1 << 0xB) / 100) >> 8;
        sp44 = temp_v1;
        func_8013E97C(0x34, temp_lo, temp_lo - temp_v1);
        temp_s0 = temp_lo - sp44;
        sp38 = 0;
        func_8013E97C(0x35, 0, temp_s0);
        temp_lo_2 = (s32) (temp_s1 * 0x30) / 100;
        func_8013E97C(0x36, temp_lo_2, temp_s0);
        temp_s0_2 = ((s32) (temp_s1 * -0x10) / 100) - sp44;
        func_8013E97C(0x37, temp_lo, temp_s0_2);
        func_8013E97C(0x38, sp38, temp_s0_2);
        func_8013E97C(0x39, temp_lo_2, temp_s0_2);
        temp_s0_3 = ((s32) (temp_s1 * 0x10) / 100) - sp44;
        func_8013E97C(0x3A, temp_lo, temp_s0_3);
        func_8013E97C(0x3B, sp38, temp_s0_3);
        func_8013E97C(0x3C, temp_lo_2, temp_s0_3);
        func_8013E97C(0x3D, (s32) (temp_s1 * 0x60) / 100, temp_s0_3);
        temp_s0_4 = temp_lo_2 - sp44;
        func_8013E97C(0x3E, sp38, temp_s0_4);
        func_8013E97C(0x3F, temp_lo_2, temp_s0_4);
        if (D_8015EDE0 >= 0x51) {
            D_8015EDE0 = D_8015EDE0;
            if (!(D_8015EDE0 & 1)) {
                var_s1_2 = 0;
                var_a0 = 0x34;
                do {
                    func_8013E7C0(var_a0, var_s1_2 + 5, 1, 1);
                    var_s1_2 += 1;
                    var_a0 = var_s1_2 + 0x34;
                } while (var_s1_2 != 0xC);
            } else {
                var_s1_3 = 0;
                var_a0_2 = 0x34;
                do {
                    func_8013E7C0(var_a0_2, var_s1_3 + 5, 1, 0);
                    var_s1_3 += 1;
                    var_a0_2 = var_s1_3 + 0x34;
                } while (var_s1_3 != 0xC);
            }
        }
        if (D_8015EDE0 >= 0x65) {
            D_8015EE18 ^= 0x10000000;
            func_8014EF3C();
        }
        break;
    }
}
