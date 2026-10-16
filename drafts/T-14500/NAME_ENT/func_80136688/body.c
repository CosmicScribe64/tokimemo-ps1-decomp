void func_80136688(void) {
    u8 *var_v0;
    u8 *var_v0_2;

    if (D_8014CC70 == 0) {
        func_8013664C(0x61);
        func_8013664C(0x62);
        D_80120734 = 0xD;
        D_8012071E = 0x40;
        D_80120778 = 0xD;
        D_80120762 = 0x40;
        D_801207BC = 0xD;
        D_801207A6 = 0x40;
        D_80120800 = 0xD;
        D_801207EA = 0x40;
        return;
    }
    if (D_8014CC70 == 1) {
        func_8013664C(0x63);
        func_8013664C(0x64);
        var_v0 = D_8011ECD0;
        do {
            var_v0 += 0x44;
            *(s16 *)((unsigned char *)var_v0 + (6552)) = 0xD;
            *(s8 *)((unsigned char *)var_v0 + (6530)) = 0x40;
        } while ((u32) var_v0 < (u32) D_8011ED58);
        var_v0_2 = D_8011EDE0;
        do {
            var_v0_2 += 0x44;
            *(s16 *)((unsigned char *)var_v0_2 + (6552)) = 0xD;
            *(s8 *)((unsigned char *)var_v0_2 + (6530)) = 0x40;
        } while (var_v0_2 != &D_8011EE68);
        return;
    }
    if (D_8014CC70 == 2) {
        D_801206AC = 0xD;
        D_80120696 = 0x40;
        D_801206F0 = 0xD;
        D_801206DA = 0x40;
        D_80120734 = 0xD;
        D_8012071E = 0x40;
        D_80120778 = 0xD;
        D_80120762 = 0x40;
        func_8013664C(0x65);
        func_8013664C(0x66);
    }
}
