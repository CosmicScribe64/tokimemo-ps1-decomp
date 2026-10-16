s32 func_80134A18(void) {
    s32 i;
    s32 frame;
    u8 r;
    u8 g;
    u8 b;

    frame = D_8015F3E8;
    if (frame < 0x20) {
        if (D_8012276C < 0xC0) {
            D_8012276C = D_8012276D = D_8012276E = D_8012276E + 6;
            D_8012277C = D_8012277D = D_8012277E = D_8012277E + 1;
            D_8012278C = D_8012278D = D_8012278E = D_8012278E + 1;
            func_8009AD70(0, (s32) &D_80122760);
            func_8009AD70(1, (s32) &D_80122770);
            func_8009AD70(2, (s32) &D_80122780);
        }
        if (frame < 0xF) {
            D_800E6280.unk_038 += 1;
        }
    } else if (frame >= 0x20 && frame < 0x54) {
        for (i = 0; i < 2; i++) {
            func_80147488(i + 1, D_8015F3E0[i], 1);
        }
    } else if (frame >= 0x54 && frame < 0x68) {
        for (i = 0; i < 2; i++) {
            func_80147488(i + 1, D_8015F3E0[i], 2);
        }
    } else if (frame >= 0x68 && frame < 0x72) {
        for (i = 0; i < 2; i++) {
            func_80147488(i + 1, D_8015F3E0[i], 3);
        }
    } else if (frame >= 0x72 && frame < 0x7C) {
        for (i = 0; i < 2; i++) {
            func_80147488(i + 1, D_8015F3E0[i], 4);
        }
    } else if (frame >= 0x7C && frame < 0x9A) {
        for (i = 0; i < 2; i++) {
            func_80147488(i + 1, D_8015F3E0[i], 5);
        }
    } else if (frame >= 0x9A && frame < 0xAE) {
        for (i = 0; i < 2; i++) {
            func_80147488(i + 1, D_8015F3E0[i], 6);
        }
    } else if (frame >= 0xAE && frame < 0xEA) {
        for (i = 0; i < 2; i++) {
            func_80147488(i + 1, D_8015F3E0[i], 7);
        }
    } else if (frame >= 0xF0) {
        return 1;
    }
    D_8015F3E8 = D_8015F3E8 + 1;
    return 0;
}
