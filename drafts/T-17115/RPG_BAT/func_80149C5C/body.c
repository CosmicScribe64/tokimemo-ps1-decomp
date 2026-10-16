/* T-17115: natural C of the compare chains, 252 differing rows, all register choice (original: D_8015EDB4 in a1, chain value in v1; draft v1 and v0). */
void func_80149C5C(void) {
    s32 x;
    s32 y;

    x = (D_8015EC28 - 0xE0) * D_8015EDB0.unk_04 / 1000 + 0xE0;
    y = (D_8015EC2C - 0x58) * D_8015EDB0.unk_04 / 1000 + 0x58;
    if (D_8015EBAC == 0) {
        if (D_8015EDB0.unk_04 * 0x1800 / 1000 > 0) {
            func_8013F0F4(0x504, 0, 2);
        }
        if (D_8015EDB0.unk_04 * 0x1800 / 1000 > 0x400) {
            func_8013F0F4(0x504, 0, 3);
        }
        if (D_8015EDB0.unk_04 * 0x1800 / 1000 > 0x800) {
            func_8013F0F4(0x504, 0, 4);
        }
        if (D_8015EDB0.unk_04 * 0x1800 / 1000 > 0xC00) {
            func_8013F0F4(0x504, 0, 5);
        }
        if (D_8015EDB0.unk_04 * 0x1800 / 1000 > 0x1000) {
            func_8013F0F4(0x504, 0, 6);
        }
        if (D_8015EDB0.unk_04 * 0x1800 / 1000 > 0x1400) {
            func_8013F0F4(0x504, 0, 7);
        }
    } else {
        if (D_8015EDB0.unk_04 * 0x2000 / 1000 > 0) {
            func_8013F0F4(0x504, 0, 2);
        }
        if (D_8015EDB0.unk_04 * 0x2000 / 1000 > 0x400) {
            func_8013F0F4(0x504, 0, 3);
        }
        if (D_8015EDB0.unk_04 * 0x2000 / 1000 > 0x800) {
            func_8013F0F4(0x504, 0, 4);
        }
        if (D_8015EDB0.unk_04 * 0x2000 / 1000 > 0xC00) {
            func_8013F0F4(0x504, 0, 5);
        }
        if (D_8015EDB0.unk_04 * 0x2000 / 1000 > 0x1000) {
            func_8013F0F4(0x504, 0, 6);
        }
        if (D_8015EDB0.unk_04 * 0x2000 / 1000 > 0x1400) {
            func_8013F0F4(0x504, 0, 7);
        }
        if (D_8015EDB0.unk_04 * 0x2000 / 1000 > 0x1800) {
            func_8013F0F4(0x504, 0, 8);
        }
        if (D_8015EDB0.unk_04 * 0x2000 / 1000 > 0x1C00) {
            func_8013F0F4(0x504, 0, 9);
        }
    }
    if (D_8015EBAC == 0) {
        func_8013E97C(0x29, x, (func_800A0070(D_8015EDB0.unk_04 * 0x1800 / 1000) >> 8) + y);
        func_8013E97C(0x28, x, (func_800A0070(D_8015EDB0.unk_04 * 0x1800 / 1000) >> 8) + y);
    } else {
        func_8013E97C(0x29, x, (func_800A0070(D_8015EDB0.unk_04 * 0x2000 / 1000) >> 8) + y);
        func_8013E97C(0x28, x, (func_800A0070(D_8015EDB0.unk_04 * 0x2000 / 1000) >> 8) + y);
    }
    if (D_8015EBAC == 0) {
        D_8015EDB0.unk_04 += 0xC;
    } else {
        D_8015EDB0.unk_04 += 8;
    }
    if (D_8015EDB0.unk_04 > 0x3E8) {
        func_8014EBA8();
    }
}
