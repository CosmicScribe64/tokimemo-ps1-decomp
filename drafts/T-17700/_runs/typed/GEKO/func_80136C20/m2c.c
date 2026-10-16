? func_80084150(s32 *, s32 (*)[], s32 (*)[]);       /* extern */

void func_80136C20(void) {
    s32 sp54;
    s32 sp4C;
    s32 sp40;
    s32 sp38;
    s32 sp34;
    s32 sp2C;
    u8 temp_t6;
    u8 temp_v0;

    temp_v0 = D_800E6280.unk_03F;
    M2C_MEMCPY_ALIGNED(&sp40, &D_801451B8, 0x2C);
    sp38.unk0 = D_801451E4.v[0];
    *(s32 *)((unsigned char *)(&sp38) + (4)) = (s32) D_801451E4.v[1];
    sp30[0].unk0 = D_801451EC.v[0];
    *(s32 *)((unsigned char *)(&sp30[0]) + (4)) = (s32) D_801451EC.v[1];
    sp28[0].unk0 = D_801451F4.v[0];
    *(s32 *)((unsigned char *)(&sp28[0]) + (4)) = (s32) D_801451F4.v[1];
    if ((temp_v0 < 6U) || (temp_v0 >= 0xAU)) {
        sp40 = 0x544A;
        sp4C = 0x544A;
        sp54 = 0x544A;
        sp38 = 0x5D41;
        sp34 = 0x522E;
        sp2C = 0x58AF;
    }
    if (D_80145088 == 0) {
        if (D_80145090 == 0) {
            func_80084150(&sp38, (s32 (*)[]) &sp30[0], (s32 (*)[]) &sp28[0]);
            return;
        }
        temp_t6 = D_800E6280.unk_F5F;
        switch (temp_t6) {
        case 0:
            func_80062CD0((&sp40)[(u16) D_800E6280.unk_0F4.h >> 0xC]);
            break;
        case 1:
            func_80062CD0((&sp38)[(u16) D_800E6280.unk_0F4.h >> 0xC]);
            break;
        case 2:
            func_80062CD0(*(s32 *)((unsigned char *)(&(&sp30[0])[(u16) D_800E6280.unk_0F4.h >> 0xC]) + (-8)));
            break;
        case 3:
            func_80062CD0(*(s32 *)((unsigned char *)(&(&sp28[0])[(u16) D_800E6280.unk_0F4.h >> 0xC]) + (-16)));
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
        return;
    }
    func_80083B24(&sp38, (s32 (*)[]) &sp30[0], (s32 (*)[]) &sp28[0]);
}
