extern u8 D_800E62BF;
extern u16 D_800E6374;
extern u8 D_800E71DF;
extern ? D_80139A08;
extern ? D_80139A34;
extern ? D_80139A3C;
extern ? D_80139A44;

void func_80132C24(void) {
    s32 sp54;
    s32 sp4C;
    s32 sp40;
    s32 sp38;
    s32 sp34;
    s32 sp2C;

    M2C_MEMCPY_ALIGNED(&sp40, &D_80139A08, 0x2C);
    sp38.unk0 = D_80139A34.unk0;
    *(s32 *)((unsigned char *)(&sp38) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80139A34) + (4));
    sp30[0].unk0 = D_80139A3C.unk0;
    *(s32 *)((unsigned char *)(&sp30[0]) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80139A3C) + (4));
    sp28[0].unk0 = D_80139A44.unk0;
    *(s32 *)((unsigned char *)(&sp28[0]) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80139A44) + (4));
    if (((u8) D_800E62BF < 6U) || ((u8) D_800E62BF >= 0xAU)) {
        sp40 = 0x544A;
        sp4C = 0x544A;
        sp54 = 0x544A;
        sp38 = 0x5D41;
        sp34 = 0x522E;
        sp2C = 0x58AF;
    }
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
    default:
        func_80062CD0(0x6962);
        break;
    }
    func_8004284C();
}
