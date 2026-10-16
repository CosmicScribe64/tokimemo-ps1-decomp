void func_80144270(void) {
    u8 sp2C[21];
    s32 var_at;

    (*(s32 *)&sp2C) = (s32) D_80147C6C;
    *(s32 *)((unsigned char *)(&sp2C) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_80147C6C) + (8));
    *(s32 *)((unsigned char *)(&sp2C) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80147C6C) + (4));
    *(s32 *)((unsigned char *)(&sp2C) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_80147C6C) + (16));
    *(u8 *)((unsigned char *)(&sp2C) + (20)) = (u8) *(u8 *)((unsigned char *)(&D_80147C6C) + (20));
    *(s32 *)((unsigned char *)(&sp2C) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_80147C6C) + (12));
    if ((D_800E71DF == 1) && (D_800E62BE == 0x60) && ((D_800E62BF == 0xC) || (D_800E62BF == 1) || ((D_800E62BF == 2) && ((u8) D_800E62C0 < 0x15U))) && (D_800E67F8 == 0)) {
        D_801476E0 = sp2C[14];
        D_800E63AE = 1;
    }
    if ((D_800E71DF == 3) && (D_800E62BE == 0x61) && ((D_800E62BF == 9) || (D_800E62BF == 0xA) || ((D_800E62BF == 0xB) && ((u8) D_800E62C0 < 0x15U))) && (D_800E6804 == 0)) {
        D_801476E0 = sp2C[7];
        D_800E63A7 = 1;
    }
    if ((D_800E71DF == 4) && ((var_at = (u8) D_800E62BE < 0x60U, (D_800E62BF == 9)) || ((D_800E62BF == 0xA) && (var_at = (u8) D_800E62BE < 0x60U, (((u8) D_800E62C0 < 0x15U) != 0)))) && (var_at == 0) && (D_800E680A == 0)) {
        D_801476E0 = sp2C[13];
        D_800E63AD = 1;
    }
    if ((D_800E71DF == 7) && ((D_800E62BF == 3) || (D_800E62BF == 4) || ((D_800E62BF == 5) && ((u8) D_800E62C0 < 0x15U))) && (D_800E681B == 0)) {
        D_801476E0 = sp2C[2];
        D_800E63A2 = 1;
    }
    if (D_800E71DF == 0xA) {
        if ((u8) D_800E62BE >= 0x60U) {
            if ((D_800E62BF == 0xA) && ((u8) D_800E62C0 < 0x15U) && (D_800E6833 == 0)) {
                D_801476E0 = sp2C[13];
                D_800E63AD = 1;
            }
        }
    }
    if (D_800E71DF == 8) {
        if ((((D_800E62BE == 0x5F) && (D_800E62BF == 0xC)) || ((D_800E62BE == 0x60) && (D_800E62BF == 1)) || ((D_800E62BE == 0x60) && (D_800E62BF == 2) && ((u8) D_800E62C0 < 0x15U))) && (D_800E6616 == 0)) {
            D_801476E0 = sp2C[3];
            D_800E63A3 = 1;
        }
        if ((D_800E62BE == 0x60) && (D_800E6619 == 0)) {
            if (D_800E62BF != 6) {
                if (D_800E62BF == 7) {
                    goto block_52;
                }
            } else {
block_52:
                D_801476E0 = sp2C[5];
                D_800E63A5 = 1;
            }
        }
        if ((D_800E62BE == 0x60) && (D_800E6622 == 0)) {
            if (D_800E62BF != 9) {
                if ((D_800E62BF == 0xA) || ((D_800E62BF == 0xB) && ((u8) D_800E62C0 < 0x15U))) {
                    goto block_60;
                }
            } else {
block_60:
                D_801476E0 = sp2C[10];
                D_800E63AA = 1;
            }
        }
        if ((D_800E62BE == 0x61) && (D_800E6623 == 0)) {
            if (D_800E62BF != 3) {
                if ((D_800E62BF == 4) || ((D_800E62BF == 5) && ((u8) D_800E62C0 < 0x15U))) {
                    goto block_68;
                }
            } else {
block_68:
                D_801476E0 = sp2C[11];
                D_800E63AB = 1;
            }
        }
        if ((D_800E62BE == 0x61) && (D_800E6825 == 0)) {
            if (D_800E62BF != 9) {
                if ((D_800E62BF == 0xA) || ((D_800E62BF == 0xB) && ((u8) D_800E62C0 < 0x15U))) {
                    goto block_76;
                }
            } else {
block_76:
                D_801476E0 = sp2C[15];
                D_800E63AF = 1;
            }
        }
        if ((D_800E62BE == 0x61) && ((D_800E62BF == 0xC) || (D_800E62BF == 1) || ((D_800E62BF == 2) && ((u8) D_800E62C0 < 0xBU)))) {
            D_801476E0 = sp2C[12];
            D_800E63AC = 1;
        }
        if (((func_80144198(0) != 0x5F) || (func_80144198(1) != 0xC) || (func_80144198(2) != 0xA)) && ((func_80144198(0) != 0x60) || (func_80144198(1) != 0xC) || (func_80144198(2) != 8))) {
            if ((func_80144198(0) == 0x61) && (func_80144198(1) == 0xC) && (func_80144198(2) == 7)) {
                goto block_93;
            }
        } else {
block_93:
            D_801476E0 = sp2C[8];
            D_800E63A8 = 1;
        }
    }
}
