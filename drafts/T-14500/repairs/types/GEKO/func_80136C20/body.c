/* NON_MATCHING: T-14700, draft-only type reconstruction; see result.json. */
/* NON_MATCHING: T-14700, draft-only type reconstruction; see result.json. */
void func_80136C20(void) {
    RepairWords11 sp40;
    RepairWords2 sp38;
    RepairWords2 sp30;
    RepairWords2 sp28;

    sp40 = D_801451B8;
    sp38 = D_801451E4;
    sp30 = D_801451EC;
    sp28 = D_801451F4;
    if (((u8) D_800E62BF < 6U) || ((u8) D_800E62BF >= 0xAU)) {
        sp40.v[0] = 0x544A;
        sp40.v[3] = 0x544A;
        sp40.v[5] = 0x544A;
        sp38.v[0] = 0x5D41;
        sp30.v[1] = 0x522E;
        sp28.v[1] = 0x58AF;
    }
    if (D_80145088 == 0) {
        if (D_80145090 == 0) {
            read_bustup_uniform();
            return;
        }
        switch (D_800E71DF) {
        case 0:
            func_80062CD0(sp40.v[(u16) D_800E6374 >> 0xC]);
            break;
        case 1:
            func_80062CD0(sp38.v[(u16) D_800E6374 >> 0xC]);
            break;
        case 2:
            func_80062CD0(sp30.v[((u16) D_800E6374 >> 0xC) - 2]);
            break;
        case 3:
            func_80062CD0(sp28.v[((u16) D_800E6374 >> 0xC) - 4]);
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
