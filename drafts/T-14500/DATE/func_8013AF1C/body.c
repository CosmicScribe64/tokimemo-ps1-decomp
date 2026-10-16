s32 func_8013AF1C(void) {
    if (D_800E6280.unk_110D++ == 0) {
        if ((D_800E6280.unk_03F == 3) && (D_800E6280.unk_040 == 0x15) && (D_800E6280.unk_03E != 0x60)) {
            D_8015BF0C = 0;
        } else if ((D_800E6280.unk_03F == 3) && (D_800E6280.unk_040 == 0x14) && (D_800E6280.unk_03E == 0x60)) {
            D_8015BF0C = 0;
        } else if ((D_800E6280.unk_03E == 0x5F) && (D_800E6280.unk_03F == 7) && (D_800E6280.unk_040 == 0x17)) {
            D_8015BF0C = 1;
        } else if ((D_800E6280.unk_03E == 0x60) && (D_800E6280.unk_03F == 7) && (D_800E6280.unk_040 == 0x1C)) {
            D_8015BF0C = 1;
        } else if ((D_800E6280.unk_03E == 0x61) && (D_800E6280.unk_03F == 7) && (D_800E6280.unk_040 == 0x1B)) {
            D_8015BF0C = 1;
        } else if ((D_800E6280.unk_03F == 9) && (D_800E6280.unk_040 == 0x17) && (D_800E6280.unk_03E != 0x60)) {
            if (D_800E6280.unk_10F8 & 1) {
                D_8015BF0C = 2;
            } else {
                D_8015BF0C = 3;
            }
        } else if ((D_800E6280.unk_03F == 9) && (D_800E6280.unk_040 == 0x16) && (D_800E6280.unk_03E == 0x60)) {
            if (D_800E6280.unk_10F8 & 1) {
                D_8015BF0C = 2;
            } else {
                D_8015BF0C = 3;
            }
        } else if ((D_800E6280.unk_03F == 0xC) && (D_800E6280.unk_040 == 0x17)) {
            D_8015BF0C = 4;
        } else {
            func_8004284C();
            return 0;
        }
    }
    if (D_800E6280.unk_110D >= 0xBU) {
        D_800E6280.unk_110D = 0xA;
    }
    D_800CA134 = (u8 *) &D_8015BF0C;
    D_800CA138 = (u8 *) &D_8015BF10;
    D_800CA13C = D_8015BE20;
    D_800CA140 = D_8015BE8C;
    D_800CA144 = D_8015BEF8;
    return func_80082764(D_80122CDC, 1, 0);
}
