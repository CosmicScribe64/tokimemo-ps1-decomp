void func_8013B488(void) {
    s16 sp3A;
    u8 sp39;
    u32 temp_t3;
    u32 temp_v0;
    u8 temp_v1;

    temp_v1 = func_80051A68(D_800E6280.unk_F5F) & 0x7F;
    sp39 = temp_v1;
    if ((u8) D_800E6280.unk_110D >= 0xBU) {
        D_800E6280.unk_110D = 0xA;
    }
    D_800E6280.unk_110D += 1;
    if (D_800E6280.unk_110D == 0) {
        temp_v0 = temp_v1 & 0xFF;
        D_800CA158 = ((temp_v0 >= 2U) + (temp_v0 >= 3U) + (temp_v0 >= 4U)) * 3;
        if (temp_v0 == 4) {
            func_80083440(4);
            D_800CA234[D_80122CDC] = 0;
        }
    }
    sp3A = D_800CA234[D_80122CDC] + D_800CA158;
    if (D_800E6280.unk_75C != 0) {
        sp3A = 0xA;
    }
    if ((D_800E6280.unk_75C != 0) && (sp39 < 2U) && (D_800E6280.unk_F5F == 6)) {
        sp3A = 0x12;
    }
    if (D_80122D08 >= 0) {
        if (D_80122D08 == D_800E6280.unk_72C[D_800E6280.unk_F5F]) {
            if ((D_800E6280.unk_73C[D_800E6280.unk_F5F] != 0) && (D_800E6280.unk_1108 == 0x50)) {
                if ((u8) D_8015B680 == 0) {
                    if (D_800E6280.unk_75C == 0) {
                        if (D_800CA2F8 != 0) {
                            if (D_800CA2F8 != 0x12) {
                                sp3A = 0xB;
                                if (D_800E6280.unk_110D == 1) {
                                    func_80083440(3);
                                    D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_02 -= 1;
                                    D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_06 -= 2;
                                    D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0A += 5;
                                    func_80084D3C(0x38, &D_800E6280);
                                }
                                if (sp39 < 2U) {
                                    if (D_800E6280.unk_F5F == 6) {
                                        sp3A = 0x13;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    D_800CA134 = (u8 *) &sp3A;
    D_800CA138 = &D_800CA15C;
    temp_t3 = func_80051A68(D_800E6280.unk_F5F) & 0x7F;
    if (((D_800E6280.unk_F5F == 1) || (D_800E6280.unk_F5F == 5)) && (temp_t3 < 2U) && (D_800CA15C == 3)) {
        func_80083440(1);
    }
    D_800CA13C = D_8015BE18;
    D_800CA140 = D_8015BE84;
    D_800CA144 = D_8015BEF0;
    func_80082764(D_80122CDC, 1, 0);
}
