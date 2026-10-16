s32 func_801496C4(void) {
    s32 var_t9;

    if (!(*(&D_800E6448 + (D_800E71DF * 0x38)) & 0x40)) {
        D_800E738A += 0x11;
        return 0;
    }
    if (D_800E71DF == 0) {
        D_800E699E |= 1;
    } else {
        D_800E699E |= 2;
    }
    if (((D_800CA2F8 == 0) || (D_800CA2F8 == 0x12)) && ((u8) D_800E71DE < 0xFEU)) {
        D_800E71DE -= 1;
    }
    func_80085B3C(0xA, D_80122D08);
    D_8015D094 = (u8) D_80122CDC;
    D_8015D084 = 0x1D;
    D_8015D088 = 0;
    func_800847B8(D_800E71DF);
    switch (D_800E71DF) {
    case 0:
        D_8015D06C = D_8015CFD4;
        D_8015D070 = D_8015D008;
        D_8015D074 = D_8015D03C;
        break;
    case 1:
        D_8015D06C = D_8015CFE4;
        D_8015D070 = D_8015D018;
        D_8015D074 = D_8015D04C;
        break;
    case 2:
        D_8015D06C = D_8015CFD8;
        D_8015D070 = D_8015D00C;
        D_8015D074 = D_8015D040;
        break;
    case 3:
        D_8015D06C = D_8015CFDC;
        D_8015D070 = D_8015D010;
        D_8015D074 = D_8015D044;
        break;
    case 4:
        D_8015D06C = D_8015CFF8;
        D_8015D070 = D_8015D02C;
        D_8015D074 = D_8015D060;
        break;
    case 5:
        D_8015D06C = D_8015CFE8;
        var_t9 = D_8015D050;
        D_8015D070 = D_8015D01C;
block_23:
        D_8015D074 = var_t9;
        break;
    case 6:
        D_8015D06C = D_8015CFF0;
        D_8015D070 = D_8015D024;
        D_8015D074 = D_8015D058;
        if ((u32) (get_g_zyotai_s(6) & 0x7F) < 2U) {
            D_8015D06C = D_8015CFEC;
            D_8015D070 = D_8015D020;
            D_8015D074 = D_8015D054;
        }
        break;
    case 7:
        D_8015D06C = D_8015CFE0;
        D_8015D070 = D_8015D014;
        D_8015D074 = D_8015D048;
        break;
    case 8:
        D_8015D06C = D_8015CFD0;
        D_8015D070 = D_8015D004;
        D_8015D074 = D_8015D038;
        break;
    case 9:
        D_8015D06C = D_8015CFF4;
        D_8015D070 = D_8015D028;
        D_8015D074 = D_8015D05C;
        break;
    case 10:
        D_8015D06C = D_8015D000;
        var_t9 = D_8015D068;
        D_8015D070 = D_8015D034;
        goto block_23;
    }
    if ((u8) D_800E62BF >= 4U) {
        D_8015D090 = D_800E62BE - 0x5F;
    } else {
        D_8015D090 = D_800E62BE - 0x60;
    }
    return func_8004284C();
}
