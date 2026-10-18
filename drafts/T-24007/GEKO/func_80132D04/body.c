void func_80132D04(s16 arg0, s16 arg1) {
    u8 sp33;
    u8 var_v1;
    u8 *var_a1;

    sp33 = get_g_zyotai_s(D_800E6280.unk_F5F) & 0x7F;
    if ((D_80144C50 != 1) && (D_80144C50 != 0x10)) {
        var_a1 = (u8 *)&D_800E6280 + (arg0 << 6) + arg1;
        var_v1 = *(u8 *)((unsigned char *)var_a1 + (101));
        if (var_v1 == 0xC) {
            if (D_800E6280.unk_F5F == 0) {
                if (((u32) ((u16) D_800E6280.unk_0F4.h >> 0xC) < 6U) || ((u32) (D_800E6280.unk_1BC[0].unk_0C.b[2] >> 4) < 6U)) {
                    func_80132A4C(2);
                    var_v1 = *(u8 *)((unsigned char *)var_a1 + (101));
                }
            } else {
                func_80132A4C(2);
                var_v1 = *(u8 *)((unsigned char *)var_a1 + (101));
            }
        }
        if (var_v1 == 4) {
            func_80132A4C(6);
            var_v1 = *(u8 *)((unsigned char *)var_a1 + (101));
        }
        if (var_v1 == 0xB) {
            if ((sp33 < 2U) || (D_800E6280.unk_F5F == 0xA)) {
                func_80132A4C(7);
            } else {
                func_80132A4C(8);
            }
        }
        if ((D_800E6280.unk_03E == 0x61) && (D_800E6280.unk_03F == 9) && ((u8) D_800E6280.unk_040 < 0xEU) && (D_800E6280.unk_F5F == 0xA)) {
            func_80132A4C(8);
        }
        var_v1 = *(u8 *)((unsigned char *)var_a1 + (101));
        if (var_v1 == 6) {
            func_80132A4C(0xB);
            var_v1 = *(u8 *)((unsigned char *)var_a1 + (101));
        }
        if (var_v1 == 0xE) {
            func_80132A4C(0xC);
            var_v1 = *(u8 *)((unsigned char *)var_a1 + (101));
        }
        if (var_v1 == 0x14) {
            func_80132A4C(0xD);
            var_v1 = *(u8 *)((unsigned char *)var_a1 + (101));
        }
        if ((D_800E6280.unk_03E == 0x60) && (var_v1 == 2)) {
            func_80132A4C(0xE);
            var_v1 = *(u8 *)((unsigned char *)var_a1 + (101));
        }
        if ((D_800E6280.unk_03E == 0x61) && (var_v1 == 2)) {
            func_80132A4C(0xF);
            var_v1 = *(u8 *)((unsigned char *)var_a1 + (101));
        }
        if (var_v1 == 0xF) {
            func_80132A4C(9);
        }
    }
    var_a1 = (u8 *)&D_800E6280 + (arg0 << 6) + arg1;
    var_v1 = *(u8 *)((unsigned char *)var_a1 + (101));
    if ((var_v1 == 5) && (D_80144C50 != 0x10)) {
        D_80144C50 = 0;
        func_80132A4C(1);
        var_v1 = *(u8 *)((unsigned char *)var_a1 + (101));
    }
    if (var_v1 == 0x15) {
        D_80144C50 = 0;
        func_80132A4C(0x10);
    }
}
