void func_8014B294(void) {
    void *sp4C;                                     /* compiler-managed */
    s32 temp_lo;
    s32 temp_lo_2;
    s32 temp_lo_3;
    s32 temp_lo_4;
    s32 temp_v0_2;
    s32 var_s0;
    s32 var_s1;
    u8 *temp_v0;

    var_s1 = 1;
    var_s0 = 0;
    do {
        k_disp_switch(D_8015EDBC, 1);
        temp_lo = (s32) D_8015ED94 / var_s1;
        if (temp_lo != 0) {
            func_8013E7C0(0x13 - var_s0, (temp_lo % 10) + 9, 1, 1);
        } else {
            func_8013E7C0(0x13 - var_s0, 0xFF, 1, 0);
        }
        if (D_8015EDEC >= 0x10000001) {
            temp_v0 = -(var_s0 * 0x44) + &D_80120C6C;
            if (D_8015EB9C != 0xE) {
                sp4C = temp_v0;
                k_disp_switch(D_8015EE14, 0);
            }
            D_80120F17 = 0;
            *(s8 *)((unsigned char *)temp_v0 + (3)) = 0;
        } else {
            k_disp_switch(D_8015EE14, 1);
            temp_lo_2 = (s32) D_8015EDEC / var_s1;
            D_80120F17 = 0x80;
            if (temp_lo_2 != 0) {
                func_8013E7C0(0x17 - var_s0, (temp_lo_2 % 10) + 9, 1, 1);
            } else {
                func_8013E7C0(0x17 - var_s0, 0xFF, 1, 0);
            }
        }
        if (D_8015EE18 >= 0x10000001) {
            temp_v0_2 = var_s0 * 0x44;
            if (D_8015EB98 & 0xF0000) {
                sp4C = temp_v0_2;
                k_disp_switch(D_8015EE40, 0);
            }
            if (D_8015EB9C != 0xE) {
                D_80120ED3 = 0;
                *(&D_80120D7F + -temp_v0_2) = 0;
            } else {
                D_80120F17 = 0;
                (&D_80120C6F)[-temp_v0_2] = 0;
            }
        } else {
            k_disp_switch(D_8015EE40, 1);
            if (D_8015EB9C != 0xE) {
                temp_lo_3 = (s32) D_8015EE18 / var_s1;
                D_80120ED3 = 0x80;
                if (temp_lo_3 != 0) {
                    func_8013E7C0(0x1B - var_s0, (temp_lo_3 % 10) + 9, 1, 1);
                } else {
                    func_8013E7C0(0x1B - var_s0, 0xFF, 1, 0);
                }
            } else {
                temp_lo_4 = (s32) D_8015EE18 / var_s1;
                D_80120F17 = 0x80;
                if (temp_lo_4 != 0) {
                    func_8013E7C0(0x17 - var_s0, (temp_lo_4 % 10) + 9, 1, 1);
                } else {
                    func_8013E7C0(0x1B - var_s0, 0xFF, 1, 0);
                }
            }
        }
        var_s0 += 1;
        var_s1 *= 0xA;
    } while (var_s0 != 4);
    if (D_8015ED94 == 0) {
        k_disp_switch(D_8015EDBC, 1);
        func_8013E7C0(0x13, 9, 1, 1);
    }
}
