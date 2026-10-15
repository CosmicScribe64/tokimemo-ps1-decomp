s32 func_8014C20C(void) {
    u8 temp_v0;
    u8 var_v0;

    var_v0 = D_800E738D;
    if (var_v0 == 0) {
        func_800674B0();
        var_v0 = (D_800E738D + 1) & 0xFF;
    }
    D_800E738D = var_v0;
    if (var_v0 == 1) {
        D_800E7384 += 1;
        if ((u32) D_800E7384 >= 0x400U) {
            func_800452C4();
            func_8004482C();
            D_800E738D = 0;
        }
        if (func_80044E8C() == 1) {
            D_800E738D += 1;
            goto block_8;
        }
        return 0;
    }
block_8:
    if ((D_800E6396 >= 0x23) || ((D_800E6396 >= 0x1E) && (D_800E71DF != 7))) {
        if (D_80122D08 >= 0) {
            D_800E6280.unk_72C[D_800E71DF] = (u8) D_80122D08;
            D_800E6280.unk_73C[D_800E71DF] = 0xC;
        }
        if (D_80122D08 == 8) {
            D_800E6280.unk_72C[D_800E71DF] = 7;
        }
        if (D_80122D08 == 6) {
            D_800E6280.unk_72C[D_800E71DF] = 5;
        }
    }
    if (((D_80122CD4 != 7) || (D_80122CD0 != 6)) && (D_80122CD4 != 0xB)) {
        temp_v0 = *(u8 *)((unsigned char *)(&D_800E6280 + (D_800E71DE * 8)) + (1880));
        if ((temp_v0 == 1) || (temp_v0 == 0)) {
            D_800E71DE -= 1;
        }
        if ((D_80122CD4 == 7) && (D_80122CD0 == 2)) {
            D_80122D48 = (u8) D_8015E204 + 1;
        }
        func_80085B3C(2, (s32) D_800CA368, (s32) &D_800E6280, 5);
    }
    D_800E6280.unk_1BC[D_800E71DF].unk_0A -= 0x28;
    if ((D_800E62BF == 5) && ((D_800E62C0 == 4) || (D_800E62C0 == 5)) && (D_800E6280.unk_1BC[D_800E71DF].unk_0C.w & 0x10)) {
        D_800E6280.unk_1BC[D_800E71DF].unk_02 += 2;
    }
    if ((D_800E62BF == 5) && ((D_800E62C0 == 3) || (D_800E62C0 == 4))) {
        D_800E6280.unk_1BC[D_800E71DF].unk_0C.b[0] |= 0x10;
    }
    check_para_limit();
    D_800E69A4 = D_800CA2F8;
    D_800E69AB = D_800E69AA;
    D_800E69AA = D_800E69A9;
    D_800E69A9 = D_800E69A8;
    D_800E69A8 = D_800CA2F8;
    D_80122D44 = 0;
    return func_80072B5C(1);
}
