extern s16 D_800E6442;
extern s16 D_800E6446;
extern u8 D_800E69DD;
extern u32 D_800E7384;
extern u8 D_800E738D;

s32 func_80136860(void) {
    s32 temp_t5;
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
    temp_t5 = (((D_8013A4C4 & 1) != 0) + ((D_8013A4C4 & 2) != 0) + ((D_8013A4C4 & 4) != 0)) & 0xFF;
    if (temp_t5 == 0) {
        D_800E6442 += 0xC;
        D_800E6280.unk_1BC[D_800E69DD].unk_0A += 7;
    } else if (temp_t5 == 1) {
        D_800E6442 += 3;
        D_800E6280.unk_1BC[D_800E69DD].unk_0A += 2;
    } else if (temp_t5 == 2) {
        D_800E6280.unk_1BC[D_800E69DD].unk_06 += 3;
        D_800E6446 += 2;
    } else {
        D_800E6280.unk_1BC[D_800E69DD].unk_06 += 0xC;
        D_800E6446 += 7;
    }
    func_80084D3C();
    return func_80072B5C(1);
}
