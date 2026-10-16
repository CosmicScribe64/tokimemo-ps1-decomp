s32 func_80137DF4(void) {
    ? var_a0;

    if (D_80122CDC == 0) {
        if ((D_8015B678 != 0) || (D_8015B674 != 0)) {
            var_a0 = 0x4359;
            if (D_8015B69C == 0) {
                D_8015B68C = 3;
            }
        } else {
            var_a0 = 0x433E;
            if (D_8015B69C == 0) {
                D_8015B68C = 0;
            }
        }
    } else if (D_80122CDC == 1) {
        D_800CA148 += 5;
        if (D_8015B678 != 0) {
            var_a0 = 0x4362;
            if (D_8015B69C == 0) {
                D_8015B68C = 4;
            }
        } else {
            var_a0 = 0x4347;
            if (D_8015B69C == 0) {
                D_8015B68C = 1;
            }
        }
    } else {
        D_800CA148 += 0xA;
        var_a0 = 0x4350;
        if (D_8015B69C == 0) {
            D_8015B68C = 2;
        }
    }
    D_8015B69C = 1;
    func_8007ED84(var_a0);
    func_80138934();
    if (*(&D_800E6280 + (D_800E6280.unk_F5E * 8)) == 1) {
        D_800E6280.unk_F5E -= 1;
    }
    func_80085B3C(0, D_80122D08);
    func_80136F2C();
    if ((D_800E6280.unk_F5F == 8) && ((u8) D_8015B68C == 3) && (D_800E6280.unk_1BC[8].unk_14[0x18] != 0)) {
        D_80122CDC = 1;
        return 1;
    }
    D_80122CDC = 0;
    return 0;
}
