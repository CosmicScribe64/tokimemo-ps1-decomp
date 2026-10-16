s32 func_80137DF4(void) {
    s32 var_a0;

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
    bg_read_sub2(var_a0);
    func_80138934();
    if (*(&D_800E69D8 + (D_800E71DE * 8)) == 1) {
        D_800E71DE -= 1;
    }
    func_80085B3C(0, D_80122D08);
    func_80136F2C();
    if ((D_800E71DF == 8) && ((u8) D_8015B68C == 3) && (D_800E6628 != 0)) {
        D_80122CDC = 1;
        return 1;
    }
    D_80122CDC = 0;
    return 0;
}
