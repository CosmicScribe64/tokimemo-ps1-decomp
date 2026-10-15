s32 func_80138E10(void) {
    u8 var_v0;
    u8 *temp_v0;
    u8 *temp_v0_2;

    var_v0 = D_800E738D;
    if (var_v0 == 0) {
        func_80044750(0xB2);
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
        if (func_80044E8C() != 1) {
            return 0;
        }
        goto block_7;
    }
block_7:
    func_80044750(0x22);
    func_80044750(0x21);
    func_80044750(0x2F);
    D_800E71DF = (s8) D_80122CD4;
    if (D_800CA368 != 0xFF) {
        func_80085B3C(2, (s32) D_800CA368);
        if (D_800CA368 == 0x14) {
            temp_v0 = &D_800E6280 + (D_800E71DE * 8);
            temp_v0_2 = temp_v0 - 8;
            if ((*(u8 *)((unsigned char *)temp_v0 + (1880)) == 2) && (D_800CA368 == *(u8 *)((unsigned char *)temp_v0_2 + (1887)))) {
                *(s8 *)((unsigned char *)temp_v0_2 + (1889)) = 4;
            } else {
                D_800E6280.unk_75E[D_800E71DE].unk_03 = 4;
            }
        }
    }
    D_80122D44 = 0;
    return func_80072B5C(1);
}
