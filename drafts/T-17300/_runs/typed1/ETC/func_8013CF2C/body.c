void func_8013CF2C(void) {
    u8 *temp_v0;

    if ((D_80150064 == 0) || (D_801500E0 == 1)) {
        *(u8 *)((unsigned char *)(&D_800E6280) + (4364)) = 5U;
    }
    switch (*(u8 *)((unsigned char *)(&D_800E6280) + (4364))) {
    case 0:
        if ((u32) *(u32 *)((unsigned char *)(&D_800E6280) + (4352)) >= 0x79U) {
            if (*(u8 *)((unsigned char *)(&D_800E6280) + (3935)) == 0) {
                func_8013F320(0x14);
                if (D_8015006C >= 3) {
                    *(u8 *)((unsigned char *)(&D_800E6280) + (4364)) = (u8) (*(u8 *)((unsigned char *)(&D_800E6280) + (4364)) + 1);
                    D_8015006C = 0;
                }
            } else if (*(u8 *)((unsigned char *)(&D_800E6280) + (3935)) == 2) {
                func_8013F320(8);
                if (D_8015006C == 2) {
                    *(u32 *)((unsigned char *)(&D_800E6280) + (4352)) = 0U;
                }
                if (D_8015006C >= 3) {
                    if ((u32) D_800E6280.unk0 >= 0x3DU) {
                        *(u8 *)((unsigned char *)(&D_800E6280) + (4364)) = (u8) (*(u8 *)((unsigned char *)(&D_800E6280) + (4364)) + 1);
                        D_8015006C = 0;
                    } else {
                        *(u32 *)((unsigned char *)(&D_800E6280) + (4352)) = (u32) (D_800E6280.unk0 + 1);
                    }
                }
            } else if (*(u8 *)((unsigned char *)(&D_800E6280) + (3935)) == 4) {
                func_8013F320(0xD);
                if (D_8015006C >= 3) {
                    *(u8 *)((unsigned char *)(&D_800E6280) + (4364)) = (u8) (*(u8 *)((unsigned char *)(&D_800E6280) + (4364)) + 1);
                    D_8015006C = 0;
                }
            } else if (*(u8 *)((unsigned char *)(&D_800E6280) + (3935)) == 0xC) {
                func_8013F320(7);
                if (D_8015006C >= 3) {
                    *(u8 *)((unsigned char *)(&D_800E6280) + (4364)) = (u8) (*(u8 *)((unsigned char *)(&D_800E6280) + (4364)) + 1);
                    D_8015006C = 0;
                }
            } else {
                *(u8 *)((unsigned char *)(&D_800E6280) + (4364)) = (u8) ((u8) D_800E6280.unk0 + 1);
            default: ;
            }
        }
        break;
    case 1:
        temp_v0 = (*(u8 *)((unsigned char *)(&D_800E6280) + (3935)) * 8) + &D_8014DBF8;
        func_80046290((*(s32 *)temp_v0), *(s32 *)((unsigned char *)temp_v0 + (4)), *(u8 *)((unsigned char *)(&D_800E6280) + (3935)));
        func_80044750(0x300);
        *(u8 *)((unsigned char *)(&D_800E6280) + (4364)) = (u8) (*(u8 *)((unsigned char *)(&D_800E6280) + (4364)) + 1);
        break;
    case 2:
        if (func_800460EC() & 4) {
            *(u8 *)((unsigned char *)(&D_800E6280) + (4364)) = (u8) (*(u8 *)((unsigned char *)(&D_800E6280) + (4364)) + 1);
        }
        break;
    case 3:
        if (func_800460EC() & 2) {
            *(u32 *)((unsigned char *)(&D_800E6280) + (4352)) = 0U;
            *(u8 *)((unsigned char *)(&D_800E6280) + (4364)) = (u8) (*(u8 *)((unsigned char *)(&D_800E6280) + (4364)) + 1);
        }
        break;
    case 4:
        if ((u32) *(u32 *)((unsigned char *)(&D_800E6280) + (4352)) >= 0x79U) {
            *(u32 *)((unsigned char *)(&D_800E6280) + (4352)) = 0U;
            *(u8 *)((unsigned char *)(&D_800E6280) + (4364)) = (u8) (*(u8 *)((unsigned char *)(&D_800E6280) + (4364)) + 1);
        }
        break;
    case 5:
        func_8013E1DC(*(u32 *)((unsigned char *)(&D_800E6280) + (4352)));
        if ((u32) *(u32 *)((unsigned char *)(&D_800E6280) + (4352)) >= 0x12DU) {
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
