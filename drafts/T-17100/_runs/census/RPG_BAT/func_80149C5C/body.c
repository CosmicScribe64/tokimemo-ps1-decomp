void func_80149C5C(void) {
    s32 sp2C;
    s32 sp28;
    s32 temp_a1;
    s32 var_a1;
    s32 var_at;
    s32 var_at_2;
    s32 var_v1;
    s32 var_v1_2;

    temp_a1 = D_8015EDB0.unk_04;
    sp2C = ((s32) ((D_8015EBFC.unk_2C - 0xE0) * temp_a1) / 1000) + 0xE0;
    sp28 = ((s32) ((D_8015EBFC.unk_30 - 0x58) * temp_a1) / 1000) + 0x58;
    if (D_8015EBAC == 0) {
        var_v1 = (s32) (temp_a1 * 0x1800) / 1000;
        var_at = var_v1 < 0x401;
        if (var_v1 > 0) {
            func_8013F0F4(0x504, 0, 2);
            var_v1 = (s32) (D_8015EDB0.unk_04 * 0x1800) / 1000;
            var_at = var_v1 < 0x401;
        }
        if (var_at == 0) {
            func_8013F0F4(0x504, 0, 3);
            var_v1 = (s32) (D_8015EDB0.unk_04 * 0x1800) / 1000;
        }
        if (var_v1 >= 0x801) {
            func_8013F0F4(0x504, 0, 4);
            var_v1 = (s32) (D_8015EDB0.unk_04 * 0x1800) / 1000;
        }
        if (var_v1 >= 0xC01) {
            func_8013F0F4(0x504, 0, 5);
            var_v1 = (s32) (D_8015EDB0.unk_04 * 0x1800) / 1000;
        }
        if (var_v1 >= 0x1001) {
            func_8013F0F4(0x504, 0, 6);
            var_v1 = (s32) (D_8015EDB0.unk_04 * 0x1800) / 1000;
        }
        if (var_v1 >= 0x1401) {
            func_8013F0F4(0x504, 0, 7);
        }
    } else {
        var_v1_2 = (s32) (temp_a1 << 0xD) / 1000;
        var_at_2 = var_v1_2 < 0x401;
        if (var_v1_2 > 0) {
            func_8013F0F4(0x504, 0, 2);
            var_v1_2 = (s32) (D_8015EDB0.unk_04 << 0xD) / 1000;
            var_at_2 = var_v1_2 < 0x401;
        }
        if (var_at_2 == 0) {
            func_8013F0F4(0x504, 0, 3);
            var_v1_2 = (s32) (D_8015EDB0.unk_04 << 0xD) / 1000;
        }
        if (var_v1_2 >= 0x801) {
            func_8013F0F4(0x504, 0, 4);
            var_v1_2 = (s32) (D_8015EDB0.unk_04 << 0xD) / 1000;
        }
        if (var_v1_2 >= 0xC01) {
            func_8013F0F4(0x504, 0, 5);
            var_v1_2 = (s32) (D_8015EDB0.unk_04 << 0xD) / 1000;
        }
        if (var_v1_2 >= 0x1001) {
            func_8013F0F4(0x504, 0, 6);
            var_v1_2 = (s32) (D_8015EDB0.unk_04 << 0xD) / 1000;
        }
        if (var_v1_2 >= 0x1401) {
            func_8013F0F4(0x504, 0, 7);
            var_v1_2 = (s32) (D_8015EDB0.unk_04 << 0xD) / 1000;
        }
        if (var_v1_2 >= 0x1801) {
            func_8013F0F4(0x504, 0, 8);
            var_v1_2 = (s32) (D_8015EDB0.unk_04 << 0xD) / 1000;
        }
        if (var_v1_2 >= 0x1C01) {
            func_8013F0F4(0x504, 0, 9);
        }
    }
    if (D_8015EBAC == 0) {
        D_8015EDB0.unk_04 = temp_a1;
        func_8013E97C(0x29, sp2C, (func_800A0070((s32) (temp_a1 * 0x1800) / 1000) >> 8) + sp28);
        func_8013E97C(0x28, sp2C, (func_800A0070((s32) (D_8015EDB0.unk_04 * 0x1800) / 1000) >> 8) + sp28);
    } else {
        D_8015EDB0.unk_04 = temp_a1;
        func_8013E97C(0x29, sp2C, (func_800A0070((s32) (temp_a1 << 0xD) / 1000) >> 8) + sp28);
        func_8013E97C(0x28, sp2C, (func_800A0070((s32) (D_8015EDB0.unk_04 << 0xD) / 1000) >> 8) + sp28);
    }
    if (D_8015EBAC == 0) {
        var_a1 = D_8015EDB0.unk_04 + 0xC;
    } else {
        var_a1 = D_8015EDB0.unk_04 + 8;
    }
    D_8015EDB0.unk_04 = var_a1;
    if (var_a1 >= 0x3E9) {
        func_8014EBA8();
    }
}
