extern u8 D_800E62BE;
extern u8 D_800E62BF;
extern u8 D_800E62C0;
extern s32 D_800E7378;
extern u8 D_800E738D;

s32 func_8013AF1C(void) {
    if (D_800E738D++ == 0) {
        if ((D_800E62BF == 3) && (D_800E62C0 == 0x15) && (D_800E62BE != 0x60)) {
            D_8015BF0C = 0;
            goto block_39;
        }
        if (D_800E62BF == 3) {
            if (D_800E62C0 == 0x14) {
                if (D_800E62BE == 0x60) {
                    D_8015BF0C = 0;
                    goto block_39;
                }
                goto block_9;
            }
            goto block_10;
        }
block_9:
block_10:
        if ((D_800E62BE == 0x5F) && (D_800E62BF == 7) && (D_800E62C0 == 0x17)) {
            D_8015BF0C = 1;
            goto block_39;
        }
        if ((D_800E62BE == 0x60) && (D_800E62BF == 7) && (D_800E62C0 == 0x1C)) {
            D_8015BF0C = 1;
            goto block_39;
        }
        if ((D_800E62BE == 0x61) && (D_800E62BF == 7) && (D_800E62C0 == 0x1B)) {
            D_8015BF0C = 1;
            goto block_39;
        }
        if ((D_800E62BF == 9) && (D_800E62C0 == 0x17) && (D_800E62BE != 0x60)) {
            if (D_800E7378 & 1) {
                D_8015BF0C = 2;
            } else {
                D_8015BF0C = 3;
            }
            goto block_39;
        }
        if (D_800E62BF == 9) {
            if (D_800E62C0 == 0x16) {
                if (D_800E62BE == 0x60) {
                    if (D_800E7378 & 1) {
                        D_8015BF0C = 2;
                    } else {
                        D_8015BF0C = 3;
                    }
                    goto block_39;
                }
                goto block_34;
            }
            goto block_35;
        }
block_34:
block_35:
        if ((D_800E62BF == 0xC) && (D_800E62C0 == 0x17)) {
            D_8015BF0C = 4;
            goto block_39;
        }
        func_8004284C();
        return 0;
    }
block_39:
    if ((u8) D_800E738D >= 0xBU) {
        D_800E738D = 0xA;
    }
    D_800CA134 = (u8 *) &D_8015BF0C;
    D_800CA138 = (u8 *) &D_8015BF10;
    D_800CA13C = D_8015BE20;
    D_800CA140 = D_8015BE8C;
    D_800CA144 = D_8015BEF8;
    return func_80082764(D_80122CDC, 1, 0);
}
