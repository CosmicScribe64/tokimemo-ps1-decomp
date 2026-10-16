extern s32 D_80122D48;

s32 func_8014C20C(void) {
    u8 temp_v0;
    u8 var_v0;

    var_v0 = D_800E6280.unk_110D;
    if (var_v0 == 0) {
        func_800674B0();
        var_v0 = (D_800E6280.unk_110D + 1) & 0xFF;
    }
    D_800E6280.unk_110D = var_v0;
    if (var_v0 == 1) {
        D_800E6280.unk_1104.w += 1;
        if ((u32) D_800E6280.unk_1104.w >= 0x400U) {
            func_800452C4();
            func_8004482C();
            D_800E6280.unk_110D = 0;
        }
        if (func_80044E8C() == 1) {
            D_800E6280.unk_110D += 1;
            goto block_8;
        }
        return 0;
    }
block_8:
    if ((D_800E6280.unk_114.unk_02 >= 0x23) || ((D_800E6280.unk_114.unk_02 >= 0x1E) && (D_800E6280.unk_F5F != 7))) {
        if (D_80122D08 >= 0) {
            D_800E6280.unk_72C[D_800E6280.unk_F5F] = (u8) D_80122D08;
            D_800E6280.unk_73C[D_800E6280.unk_F5F] = 0xC;
        }
        if (D_80122D08 == 8) {
            D_800E6280.unk_72C[D_800E6280.unk_F5F] = 7;
        }
        if (D_80122D08 == 6) {
            D_800E6280.unk_72C[D_800E6280.unk_F5F] = 5;
        }
    }
    if (((D_80122CD4 != 7) || (D_80122CD0 != 6)) && (D_80122CD4 != 0xB)) {
        temp_v0 = *(u8 *)((unsigned char *)(&D_800E6280 + (D_800E6280.unk_F5E * 8)) + (1880));
        if ((temp_v0 == 1) || (temp_v0 == 0)) {
            D_800E6280.unk_F5E = D_800E6280.unk0 - 1;
        }
        if ((D_80122CD4 == 7) && (D_80122CD0 == 2)) {
            D_80122D48 = (u8) D_8015E204 + 1;
        }
        func_80085B3C(2, (s32) D_800CA368, (s32) &D_800E6280, 5);
    }
    D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0A -= 0x28;
    if ((D_800E6280.unk_03F == 5) && ((D_800E6280.unk_040 == 4) || (D_800E6280.unk_040 == 5)) && (D_800E6280.unk_1BC[D_800E6280.unk0].unk_0C.w & 0x10)) {
        D_800E6280.unk_1BC[D_800E6280.unk0].unk_02 += 2;
    }
    if ((D_800E6280.unk0 == 5) && ((D_800E6280.unk_040 == 3) || (D_800E6280.unk_040 == 4))) {
        D_800E6280.unk_1BC[D_800E6280.unk0].unk_0C.b[0] |= 0x10;
    }
    func_80084D3C();
    D_800E6280.unk_724 = D_800CA2F8;
    D_800E6280.unk_728[3] = D_800E6280.unk_728[2];
    D_800E6280.unk_728[2] = D_800E6280.unk_728[1];
    D_800E6280.unk_728[1] = D_800E6280.unk_728[0];
    D_800E6280.unk_728[0] = D_800CA2F8;
    D_80122D44 = 0;
    return func_80072B5C(1);
}
