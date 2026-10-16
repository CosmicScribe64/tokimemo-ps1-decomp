void func_80134628(void) {
    s32 sp28[16];
    s32 sp30[16];
    typedef struct { s32 w[11]; } M2cBlock2C; /* m2c memcpy of 44 bytes */
    s32 sp54;
    s32 sp4C;
    s32 sp40;
    s32 sp38;
    s32 sp34;
    s32 sp2C;

    *(M2cBlock2C *)(&sp40) = *(M2cBlock2C *)(&D_80134D38);
    sp38 = D_80134D64;
    *(s32 *)((unsigned char *)(&sp38) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80134D64) + (4));
    sp30[0] = D_80134D6C;
    *(s32 *)((unsigned char *)(&sp30[0]) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80134D6C) + (4));
    sp28[0] = D_80134D74;
    *(s32 *)((unsigned char *)(&sp28[0]) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80134D74) + (4));
    if (((u8) D_800E62BF < 6U) || ((u8) D_800E62BF >= 0xAU)) {
        sp40 = 0x544A;
        sp4C = 0x544A;
        sp54 = 0x544A;
        sp38 = 0x5D41;
        sp34 = 0x522E;
        sp2C = 0x58AF;
    }
    if ((D_800E71E8 & 0xF) == 4) {
        switch (D_800E71DF) {
        case 0:
            func_80062CD0((&sp40)[(u16) D_800E6374 >> 0xC]);
            break;
        case 1:
            func_80062CD0((&sp38)[(u16) D_800E6374 >> 0xC]);
            break;
        case 2:
            func_80062CD0(*(s32 *)((unsigned char *)(&(&sp30[0])[(u16) D_800E6374 >> 0xC]) + (-8)));
            break;
        case 3:
            func_80062CD0(*(s32 *)((unsigned char *)(&(&sp28[0])[(u16) D_800E6374 >> 0xC]) + (-16)));
            break;
        case 4:
            func_80062CD0(0x66EC);
            break;
        case 5:
            func_80062CD0(0x5FE4);
            break;
        case 6:
            func_80062CD0(0x625A);
            break;
        case 10:
            func_80062CD0(0x6962);
            break;
        case 9:
            func_80062CD0(0x64D0);
            break;
        default:
            if (((u8) D_800E62BF >= 6U) && ((u8) D_800E62BF < 0xAU)) {
                func_80062CD0(0x57FB);
            } else {
                func_80062CD0(0x5828);
            }
            break;
        }
        func_8004284C();
        return;
    }
    read_bustup_uniform(&sp38, (s32 (*)[]) &sp30[0], (s32 (*)[]) &sp28[0]);
}
