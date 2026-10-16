s32 func_80146754(void) {
    s32 sp3C;
    s32 temp_a1;
    s32 temp_s0;

    temp_s0 = D_800E6280.unk_1104.w;
    if (D_800E6280.unk_1104.w == 1) {
        func_80144DF8(2);
        func_80147488(2, D_8015F830, 9);
        D_80127080[2].unk0 = 0;
        D_80122754 = 0;
        D_80122750 = 0;
        D_8012274C = 0;
        func_80099540(&D_80122740);
    } else if ((D_800E6280.unk_1104.w >= 2) && (D_800E6280.unk_1104.w < 0x64)) {
        func_80147488(2, D_8015F830, 0xA);
    } else if ((D_800E6280.unk_1104.w >= 0x65) && (D_800E6280.unk_1104.w < 0x78)) {
        func_80147488(2, D_8015F830, 0xB);
    } else if ((D_800E6280.unk_1104.w >= 0x79) && (D_800E6280.unk_1104.w < 0x8C)) {
        func_80147488(2, D_8015F830, 0xC);
    } else if ((D_800E6280.unk_1104.w >= 0x8D) && (D_800E6280.unk_1104.w < 0xA0)) {
        func_80147488(2, D_8015F830, 0xD);
    } else if ((D_800E6280.unk_1104.w >= 0xA1) && (D_800E6280.unk_1104.w < 0xB4)) {
        func_80147488(2, D_8015F830, 0xE);
    } else if ((D_800E6280.unk_1104.w >= 0xB5) && (D_800E6280.unk_1104.w < 0xC8)) {
        func_80147488(2, D_8015F830, 0xF);
    } else if ((D_800E6280.unk_1104.w >= 0xC9) && (D_800E6280.unk_1104.w < 0xDC)) {
        func_80147488(2, D_8015F830, 0x10);
    } else if ((D_800E6280.unk_1104.w >= 0xDD) && (D_800E6280.unk_1104.w < 0xFA)) {
        func_80147488(2, D_8015F830, 0x11);
    } else if ((D_800E6280.unk_1104.w >= 0xFB) && (D_800E6280.unk_1104.w < 0x122)) {
        func_80147488(2, D_8015F830, 0x12);
    } else if ((D_800E6280.unk_1104.w >= 0x123) && (D_800E6280.unk_1104.w < 0x15E)) {
        func_80147488(2, D_8015F830, 0x13);
    } else if ((D_800E6280.unk_1104.w >= 0x15F) && (D_800E6280.unk_1104.w < 0x186)) {
        func_80147488(2, D_8015F830, 0x14);
    } else if ((D_800E6280.unk_1104.w >= 0x187) && (D_800E6280.unk_1104.w < 0x1A4)) {
        func_80147488(2, D_8015F830, 0x15);
    } else if ((D_800E6280.unk_1104.w >= 0x1A5) && (D_800E6280.unk_1104.w < 0x1F4)) {
        func_80147488(2, D_8015F830, 0x16);
    }
    if ((temp_s0 >= 0x32) && (temp_s0 < 0x96)) {
        func_8014559C(0x14, -0xAA, 0x1E, 0x14, temp_s0 - 0x1E, 0x3C);
    }
    if ((temp_s0 >= 0x69) && (temp_s0 < 0xC8)) {
        func_8014559C(0xAA, -0xAA, 0x50, 8, temp_s0 - 0x50, 0x3C);
    }
    if ((temp_s0 >= 0x78) && (temp_s0 < 0xDC)) {
        func_8014559C(-0x1E, -0xAA, 0x50, 0xA, temp_s0 - 0x78, 0x3C);
    }
    if ((temp_s0 >= 0xB4) && (temp_s0 < 0x118)) {
        func_8014559C(0xC8, -0xAA, 0x32, 0xF, temp_s0 - 0xAA, 0x3C);
    }
    if ((temp_s0 >= 0xDC) && (temp_s0 < 0x140)) {
        func_8014559C(0x5A, -0xAA, 0x3C, 0xA, temp_s0 - 0xD2, 0x3C);
    }
    if ((temp_s0 >= 0x122) && (temp_s0 < 0x136)) {
        func_8014559C(0xF, -0x96, 0x2D, 0xA, temp_s0 - 0x118, 0x3C);
    }
    if ((temp_s0 >= 0x136) && (temp_s0 < 0x186)) {
        func_8014559C((temp_s0 * 2) - 0x25D, -0x96, 0x2D, 0xA, temp_s0 - 0x118, ((s32) (temp_s0 - 0x136) / 3) + 0x3C);
    }
    if ((temp_s0 >= 0x190) && (temp_s0 < 0x1F4)) {
        func_8014559C(0x15E, -0x96, 0x50, 0xA, temp_s0 - 0x186, 0x78);
    }
    if ((temp_s0 >= 0x1A4) && (temp_s0 < 0x1F4)) {
        func_8014559C(0xDC, -0x96, 0x3C, 0x14, temp_s0 - 0x19A, 0x78);
    }
    if ((temp_s0 >= 0x1B8) && (temp_s0 < 0x21C)) {
        func_8014559C(0x96, -0x96, 0x28, 0x14, temp_s0 - 0x1AE, 0x78);
    }
    if (temp_s0 == 0x64) {
        func_801335A0(0, 0x507);
    }
    if (temp_s0 == 0xE6) {
        func_801335A0(0, 0x505);
    }
    if (temp_s0 == 0x12C) {
        func_801335A0(0, 0x506);
    }
    if (temp_s0 == 0x190) {
        func_801335A0(0, 0x505);
    }
    if (temp_s0 < 0x12C) {
        func_801449D8((s32) &D_800E6280.unk_163C[0], -5, -0xA);
        func_801449D8((s32) &D_800E7ABC, -5, -8);
    } else if ((temp_s0 >= 0x12C) && (temp_s0 < 0x190)) {
        temp_a1 = ((s32) (temp_s0 - 0x12C) / 10) - 5;
        sp3C = temp_a1;
        func_801449D8((s32) &D_800E6280.unk_163C[0], temp_a1, -0xA);
        func_801449D8((s32) &D_800E7ABC, temp_a1, -8);
    } else if (temp_s0 >= 0x190) {
        func_801449D8((s32) &D_800E6280.unk_163C[0], 5, -0xA);
        func_801449D8((s32) &D_800E7ABC, 5, -8);
    }
    if (temp_s0 == 0x226) {
        func_80042940(4);
    }
    D_800E6280.unk_1104.w += 1;
}
