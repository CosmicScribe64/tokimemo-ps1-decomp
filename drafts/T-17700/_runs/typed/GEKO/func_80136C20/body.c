void func_80136C20(void) {
    GekoWords2 sp28;
    GekoWords2 sp30;
    GekoWords11 sp40;
    GekoWords2 sp38;
    u8 temp_t6;
    u8 temp_v0;

    temp_v0 = D_800E6280.unk_03F;
    sp40 = D_801451B8;
    sp38 = D_801451E4;
    sp30 = D_801451EC;
    sp28 = D_801451F4;
    if ((temp_v0 < 6U) || (temp_v0 >= 0xAU)) {
        sp40.v[0] = 0x544A;
        sp40.v[3] = 0x544A;
        sp40.v[5] = 0x544A;
        sp38.v[0] = 0x5D41;
        sp30.v[1] = 0x522E;
        sp28.v[1] = 0x58AF;
    }
    if (D_80145088 == 0) {
        if (D_80145090 == 0) {
            read_bustup_uniform(sp38.v, (s32 (*)[]) &sp30[0], (s32 (*)[]) &sp28[0]);
            return;
        }
        temp_t6 = D_800E6280.unk_F5F;
        switch (temp_t6) {
        case 0:
            func_80062CD0(sp40.v[(u16) D_800E6280.unk_0F4.h >> 0xC]);
            break;
        case 1:
            func_80062CD0(sp38.v[(u16) D_800E6280.unk_0F4.h >> 0xC]);
            break;
        case 2:
            func_80062CD0(sp30.v[(u16) D_800E6280.unk_0F4.h >> 0xC - 2]);
            break;
        case 3:
            func_80062CD0(sp28.v[(u16) D_800E6280.unk_0F4.h >> 0xC - 4]);
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
    read_bustup(sp38.v, (s32 (*)[]) &sp30[0], (s32 (*)[]) &sp28[0]);
}
