void func_80132D04(s16 arg0, s16 arg1) {
    u8 sp33;
    void *sp2C;
    u8 var_v1;
    u8 var_v1_2;
    u8 var_v1_3;
    u8 *temp_a1;
    u8 *var_a1;

    sp33 = get_g_zyotai_s(D_800E71DF) & 0x7F;
    if ((D_80144C50 != 1) && (D_80144C50 != 0x10)) {
        var_a1 = (arg0 << 6) + arg1 + &D_800E6280;
        var_v1 = *(u8 *)((unsigned char *)var_a1 + (101));
        if (var_v1 == 0xC) {
            if (D_800E71DF == 0) {
                if (((u32) ((u16) D_800E6374 >> 0xC) < 6U) || ((u32) ((u8) D_800E644A >> 4) < 6U)) {
                    sp2C = var_a1;
                    func_80132A4C(2);
                    var_v1 = *(u8 *)((unsigned char *)var_a1 + (101));
                }
            } else {
                sp2C = var_a1;
                func_80132A4C(2);
                var_v1 = *(u8 *)((unsigned char *)var_a1 + (101));
            }
        }
        if (var_v1 == 4) {
            sp2C = var_a1;
            func_80132A4C(6);
            var_v1 = *(u8 *)((unsigned char *)var_a1 + (101));
        }
        if (var_v1 == 0xB) {
            if ((sp33 < 2U) || (D_800E71DF == 0xA)) {
                sp2C = var_a1;
                func_80132A4C(7);
            } else {
                sp2C = var_a1;
                func_80132A4C(8);
            }
        }
        if ((D_800E62BE == 0x61) && (D_800E62BF == 9) && ((u8) D_800E62C0 < 0xEU) && (D_800E71DF == 0xA)) {
            sp2C = var_a1;
            func_80132A4C(8);
        }
        var_v1_2 = *(u8 *)((unsigned char *)var_a1 + (101));
        if (var_v1_2 == 6) {
            sp2C = var_a1;
            func_80132A4C(0xB);
            var_v1_2 = *(u8 *)((unsigned char *)var_a1 + (101));
        }
        if (var_v1_2 == 0xE) {
            sp2C = var_a1;
            func_80132A4C(0xC);
            var_v1_2 = *(u8 *)((unsigned char *)var_a1 + (101));
        }
        if (var_v1_2 == 0x14) {
            sp2C = var_a1;
            func_80132A4C(0xD);
            var_v1_2 = *(u8 *)((unsigned char *)var_a1 + (101));
        }
        if ((D_800E62BE == 0x60) && (var_v1_2 == 2)) {
            sp2C = var_a1;
            func_80132A4C(0xE);
            var_v1_2 = *(u8 *)((unsigned char *)var_a1 + (101));
        }
        if ((D_800E62BE == 0x61) && (var_v1_2 == 2)) {
            sp2C = var_a1;
            func_80132A4C(0xF);
            var_v1_2 = *(u8 *)((unsigned char *)var_a1 + (101));
        }
        if (var_v1_2 == 0xF) {
            func_80132A4C(9);
        }
    }
    temp_a1 = (arg0 << 6) + arg1 + &D_800E6280;
    var_v1_3 = *(u8 *)((unsigned char *)temp_a1 + (101));
    if ((var_v1_3 == 5) && (D_80144C50 != 0x10)) {
        D_80144C50 = 0;
        sp2C = temp_a1;
        func_80132A4C(1);
        var_v1_3 = *(u8 *)((unsigned char *)temp_a1 + (101));
    }
    if (var_v1_3 == 0x15) {
        D_80144C50 = 0;
        func_80132A4C(0x10);
    }
}
