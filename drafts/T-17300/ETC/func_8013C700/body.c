s32 func_8013C700(void) {
    switch (*(u8 *)((unsigned char *)(&D_800E6280) + (4365))) {
    case 0:
        func_8013D32C();
        func_80132C3C();
    default:
        break;
    case 1:
        func_8013D32C();
        func_8013D570(0, 0);
        func_8013E89C(5);
        if (D_80150070 >= 3) {
            *(u8 *)((unsigned char *)(&D_800E6280) + (4365)) = (u8) (*(u8 *)((unsigned char *)(&D_800E6280) + (4365)) + 1);
            D_80150070 = 0;
        } else if (D_80150070 == -1) {
            *(u8 *)((unsigned char *)(&D_800E6280) + (4365)) = (u8) (*(u8 *)((unsigned char *)(&D_800E6280) + (4365)) + 1);
        }
        break;
    case 2:
        func_8013D32C();
        func_8013D570(0, 0);
        if ((*(u8 *)((unsigned char *)(&D_800E6280) + (3935)) == 0xC) || (*(u8 *)((unsigned char *)(&D_800E6280) + (3935)) == 0xB)) {
            func_8013F054();
        }
        if (((D_8015006C == 0) || (D_8015006C >= 3)) && ((*(s32 *)((unsigned char *)(&D_800E6280) + (3976)) & 0x20) || (D_801500D8 == 0) || (D_801500D8 == 1))) {
            *(u8 *)((unsigned char *)(&D_800E6280) + (4365)) = (u8) (*(u8 *)((unsigned char *)(&D_800E6280) + (4365)) + 1);
            D_8015006C = 0;
        }
        break;
    case 3:
        func_8013D32C();
        func_8013D570(*(u32 *)((unsigned char *)(&D_800E6280) + (4356)) * 2, 3);
        if ((u32) *(u32 *)((unsigned char *)(&D_800E6280) + (4356)) < 0x28U) {
            *(u32 *)((unsigned char *)(&D_800E6280) + (4356)) = (u32) (*(u32 *)((unsigned char *)(&D_800E6280) + (4356)) + 1);
        } else {
            *(u8 *)((unsigned char *)(&D_800E6280) + (4365)) = (u8) (*(u8 *)((unsigned char *)(&D_800E6280) + (4365)) + 1);
        }
        break;
    case 4:
        func_8013FC08();
        D_801500D4 = 0x800;
        func_8013FA7C();
        func_8004284C();
        break;
    }
}
