void func_80136A9C(void) {
    s16 temp_v1;
    s32 temp_v0;
    s32 var_v0;

    switch (D_8015EDB0.unk_00) {
    case 0:
        if (D_8015EDC4 & 0x100000) {
            var_v0 = D_8015EBFC.unk_30 - 0xA0;
        } else {
            var_v0 = D_8015EBFC.unk_30 - 0x78;
        }
        D_8015EC58.unk_14 = var_v0;
        switch (D_8015EBAC) {
        case 0:
            D_80121685 = 4;
            break;
        case 1:
            D_801215FD = 4;
            D_80121685 = 4;
            break;
        }
        func_8013E97C(0x2B, D_8015EBFC.unk_2C, -0xC4);
        func_8013E810(0x2B, 4, 1, 1);
        func_8013F0F4(0x503, 0, 3);
        func_8014EBD0();
        return;
    case 1:
        temp_v1 = D_801211E6 + 4;
        temp_v0 = D_8015EC58.unk_14;
        D_801211E6 = temp_v1;
        if (temp_v0 < temp_v1) {
            D_8015EC58.unk_14 = temp_v0;
            D_801211E6 = (s16) temp_v0;
            func_8014EBA8();
        }
        return;
    }
}
