extern u8 D_800E71DF;
extern u32 D_800E7380;
extern u8 D_800E738C;
extern ? D_8014DBF8;
extern s32 D_801500D8;
extern s32 D_801500DC;

void func_8013CF2C(void) {
    void *temp_v0;

    if ((D_80150064 == 0) || (D_801500E0 == 1)) {
        D_800E738C = 5;
    }
    switch (D_800E738C) {
    case 0:
        if ((u32) D_800E7380 >= 0x79U) {
            if (D_800E71DF == 0) {
                func_8013F320(0x14);
                if (D_8015006C >= 3) {
                    D_800E738C += 1;
                    D_8015006C = 0;
                }
            } else if (D_800E71DF == 2) {
                func_8013F320(8);
                if (D_8015006C == 2) {
                    D_800E7380 = 0;
                }
                if (D_8015006C >= 3) {
                    if ((u32) D_800E7380 >= 0x3DU) {
                        D_800E738C += 1;
                        D_8015006C = 0;
                    } else {
                        D_800E7380 += 1;
                    }
                }
            } else if (D_800E71DF == 4) {
                func_8013F320(0xD);
                if (D_8015006C >= 3) {
                    D_800E738C += 1;
                    D_8015006C = 0;
                }
            } else if (D_800E71DF == 0xC) {
                func_8013F320(7);
                if (D_8015006C >= 3) {
                    D_800E738C += 1;
                    D_8015006C = 0;
                }
            } else {
                D_800E738C += 1;
            default:
            }
        }
        break;
    case 1:
        temp_v0 = (D_800E71DF * 8) + &D_8014DBF8;
        func_80046290(temp_v0->unk0, *(s32 *)((unsigned char *)temp_v0 + (4)), (s32) D_800E71DF);
        func_80044750(0x300);
        D_800E738C += 1;
        break;
    case 2:
        if (func_800460EC() & 4) {
            D_800E738C += 1;
        }
        break;
    case 3:
        if (func_800460EC() & 2) {
            D_800E7380 = 0;
            D_800E738C += 1;
        }
        break;
    case 4:
        if ((u32) D_800E7380 >= 0x79U) {
            D_800E7380 = 0;
            D_800E738C += 1;
        }
        break;
    case 5:
        func_8013E1DC((s32) D_800E7380);
        if ((u32) D_800E7380 >= 0x12DU) {
            D_801500E0 = 0;
            D_80150070 = 0;
            D_80150064 = 0;
            D_801500D8 = 0;
            D_801500DC = 0;
            D_801220D0 = 0x80000000;
            D_801220F4 = 0x80000000;
            D_801500D4 = 0xC00;
            func_8013FA7C();
            func_80042908(0xFF);
        }
        break;
    }
}
