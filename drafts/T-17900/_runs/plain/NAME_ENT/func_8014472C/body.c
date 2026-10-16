void func_8014472C(void) {
    s16 var_v1;

    if (func_800460EC() & 4) {
        var_v1 = getCDlevel();
    } else {
        var_v1 = 0;
        if (((u32) D_800E7200 >> 0x10) != 0) {
            var_v1 = D_800E7380 & 0x3F;
        }
    }
    D_80125C80[D_800E7378 & 3] = var_v1;
    if (func_800460EC() & 4) {
        if (!((u8) D_801206DA & 1) || !(D_801206DB & 0x80)) {
            func_80048F64(0x62);
            D_801206D9 = 3;
            D_801206DA = 0x11;
            D_80120710 = 0x41000000;
            D_801206DB = 0xAC;
            D_801206E4 = 0x801B27B0;
            D_801206E8 = 0x801B27D8;
            D_8012070C = 0x801B27AC;
            D_801206EE = 1;
            D_801206DC = 8;
            D_801206DD = 0x80;
            D_801206FE = -0xA0;
            D_80120702 = -0x78;
            load_tpage_buf_lock(5, *(s32 *)0x801B27AC, 1);
        }
    } else {
        func_80048F64(0x62);
        D_801206DB = 0;
        D_801206DA = 0;
        D_801206F0 = 0;
        D_801206E0 = 0;
    }
}
