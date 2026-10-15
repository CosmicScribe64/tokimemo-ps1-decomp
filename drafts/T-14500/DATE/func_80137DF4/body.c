s32 func_80137DF4(void) {
    s32 text;

    if (D_80122CDC == 0) {
        if ((D_8015B678 != 0) || (D_8015B674 != 0)) {
            text = 0x4359;
            if (D_8015B69C == 0) {
                D_8015B68C = 3;
            }
        } else {
            text = 0x433E;
            if (D_8015B69C == 0) {
                D_8015B68C = 0;
            }
        }
    } else if (D_80122CDC == 1) {
        D_800CA148 += 5;
        if (D_8015B678 != 0) {
            text = 0x4362;
            if (D_8015B69C == 0) {
                D_8015B68C = 4;
            }
        } else {
            text = 0x4347;
            if (D_8015B69C == 0) {
                D_8015B68C = 1;
            }
        }
    } else {
        D_800CA148 += 0xA;
        text = 0x4350;
        if (D_8015B69C == 0) {
            D_8015B68C = 2;
        }
    }
    D_8015B69C = 1;
    func_8007ED84(text);
    func_80138934();
    if ((D_800E6280.unk_75E - 1)[D_800E6280.unk_F5E].unk_02 == 1) {
        D_800E6280.unk_F5E -= 1;
    }
    func_80085B3C(0, D_80122D08);
    func_80136F2C();
    if ((D_800E6280.unk_F5F == 8) && ((u8)D_8015B68C == 3) && (D_800E6280.unk_1BC[8].unk_14[0x18] != 0)) {
        (&D_80122CDC)[0] = 1;
        return (&D_80122CDC)[0];
    }
    D_80122CDC = 0;
    return 0;
}
