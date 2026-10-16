void func_801447E0(void) {
    s32 v;
    s32 lim;
    s32 k;

    if (D_800E6280.unk_118.unk_02 >= 0x97) {
        lim = 0x96;
    } else {
        lim = D_800E6280.unk_118.unk_02;
    }
    if (D_8015EB98 & 0x80000000) {
        switch (D_8015EDC4 & 0xFFFF) {
        case 1:
        default:
            v = func_8013EA00(D_8015EDA0 / 10, D_8015EDA0 * 5 / 100) + 0x64;
            break;
        case 2:
            k = D_8015EDA0 / 10;
            v = func_8013EA00(k, k) + 0x96;
            break;
        }
        D_8015EBB0 = D_8015EBD8 * (v * (0xE1 - lim) / 150) / 1000;
        if (D_8015ED98 & 0x100) {
            D_8015EBD8 += 0x64;
            D_8015EBB0 = D_8015EBB0 / 2;
        }
    }
    if (D_8015EB98 & 0x100000) {
        k = 0xE1 - lim;
        switch (D_8015EDC4 & 0xFFFF) {
        case 2:
        default:
            v = func_8013E9A8(D_8015EDA0 * 0x19 / 100 - 0x1F4) + 0x1F4;
            if (D_8015ED98 & 0x100) {
                v = v / 3;
            }
            break;
        case 4:
            v = func_8013E9A8(D_8015EDA0 * 0x32 / 100 - 0x3E8) + 0x3E8;
            if (D_8015ED98 & 0x100) {
                v = v / 10;
            }
            break;
        }
        D_8015EBB0 = D_8015EBD8 * (v * k / 150) / 1000;
    }
    if (D_8015EB98 & 0xF0000) {
        if (D_8015ED98 & 0x100) {
            D_8015EBB0 = D_8015ED94 / 100;
        } else {
            D_8015EBB0 = D_8015ED94 / 10;
        }
    }
    if (D_8015EB98 & 0x08000000) {
        if (D_8015ED98 & 0x1000) {
            D_8015EBB0 = D_8015ED94 / 2;
            return;
        }
        if (D_8015ED98 & 0x100) {
            D_8015EBB0 = D_8015ED94 / 50;
        } else {
            D_8015EBB0 = D_8015ED94 / 5;
        }
        if (D_8015ED94 < 0x14) {
            D_8015EBB0 = 0;
        }
    }
}
