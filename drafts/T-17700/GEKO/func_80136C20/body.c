void func_80136C20(void) {
    u8 girl;
    GekoWords11 all;
    GekoWords2 first;
    GekoWords2 second;
    GekoWords2 third;

    all = D_801451B8;
    first = D_801451E4;
    second = D_801451EC;
    third = D_801451F4;
    if (D_800E6280.unk_03F < 6 || D_800E6280.unk_03F >= 10) {
        all.v[0] = 0x544A;
        all.v[3] = 0x544A;
        all.v[5] = 0x544A;
        first.v[0] = 0x5D41;
        second.v[1] = 0x522E;
        third.v[1] = 0x58AF;
    }
    if (D_80145088 == 0) {
        girl = D_800E6280.unk_F5F;
        if (D_80145090 == 0) {
            read_bustup_uniform();
            return;
        }
        switch (girl) {
        case 0:
            func_80062CD0(all.v[D_800E6280.unk_0F4.h >> 12]);
            break;
        case 1:
            func_80062CD0(first.v[D_800E6280.unk_0F4.h >> 12]);
            break;
        case 2:
            func_80062CD0(second.v[(D_800E6280.unk_0F4.h >> 12) - 2]);
            break;
        case 3:
            func_80062CD0(third.v[(D_800E6280.unk_0F4.h >> 12) - 4]);
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
    read_bustup();
}
