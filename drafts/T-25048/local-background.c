void func_80076FDC(void) {
    s32 col;
    u8 background;

    background = 0xFF;
    D_801230D0 = background;
    if (func_80066A84() == 1) {
        if (D_800E6280.unk_69C[D_800E6280.unk_71C].unk_01 == 4) {
            background = 0xFF;
        } else {
            background = D_800B69D0[D_800E6280.unk_69C[D_800E6280.unk_71C].unk_01];
        }
    } else {
        if (D_800E6280.unk_69C[D_800E6280.unk_71C].unk_01 == 4) {
            background = 0xFF;
        } else {
            background = D_800B69D8[D_800E6280.unk_69C[D_800E6280.unk_71C].unk_01];
        }
    }
    D_801230D0 = background;
    if (background == 0xFF) {
        col = func_8006F524(4);
        if (col != 0) {
            col = 1;
        }
        if (func_80066A84() == 1) {
            switch (D_800E6280.unk_0F4.h >> 12) {
            case 0:
                D_801230D0 = D_800B6890[0][col];
                break;
            case 1:
                D_801230D0 = D_800B6890[1][col];
                break;
            case 2:
                D_801230D0 = D_800B6890[2][col];
                break;
            case 3:
                D_801230D0 = D_800B6890[3][col];
                break;
            case 4:
                D_801230D0 = D_800B6890[4][col];
                break;
            case 5:
                D_801230D0 = D_800B6890[5][col];
                break;
            case 6:
                D_801230D0 = D_800B6890[6][col];
                break;
            case 7:
                D_801230D0 = D_800B6890[7][col];
                break;
            case 8:
                D_801230D0 = D_800B6890[8][col];
                break;
            case 9:
                D_801230D0 = D_800B6890[9][col];
                break;
            case 10:
                D_801230D0 = D_800B6890[10][col];
                break;
            }
        } else {
            switch (D_800E6280.unk_0F4.h >> 12) {
            case 0:
                D_801230D0 = D_800B68A8[0][col];
                break;
            case 1:
                D_801230D0 = D_800B68A8[1][col];
                break;
            case 2:
                D_801230D0 = D_800B68A8[2][col];
                break;
            case 3:
                D_801230D0 = D_800B68A8[3][col];
                break;
            case 4:
                D_801230D0 = D_800B68A8[4][col];
                break;
            case 5:
                D_801230D0 = D_800B68A8[5][col];
                break;
            case 6:
                D_801230D0 = D_800B68A8[6][col];
                break;
            case 7:
                D_801230D0 = D_800B68A8[7][col];
                break;
            case 8:
                D_801230D0 = D_800B68A8[8][col];
                break;
            case 9:
                D_801230D0 = D_800B68A8[9][col];
                break;
            case 10:
                D_801230D0 = D_800B68A8[10][col];
                break;
            }
        }
    }
}
