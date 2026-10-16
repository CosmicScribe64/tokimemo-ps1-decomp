void func_8013282C(void) {
    u8 temp_t6;

    D_8013B084 = 0;
    D_8013B088 = 0;
    func_800847B8(D_800E6280.unk_F5F);
    temp_t6 = D_800E6280.unk_F5F;
    switch (temp_t6) {
    case 0:
        D_8013B06C = D_8013AFD4;
        D_8013B070 = D_8013B008;
        D_8013B074 = D_8013B03C;
        break;
    case 1:
        D_8013B06C = D_8013AFE4;
        D_8013B070 = D_8013B018;
        D_8013B074 = D_8013B04C;
        break;
    case 2:
        D_8013B06C = D_8013AFD8;
        D_8013B070 = D_8013B00C;
        D_8013B074 = D_8013B040;
        break;
    case 3:
        D_8013B06C = D_8013AFDC;
        D_8013B070 = D_8013B010;
        D_8013B074 = D_8013B044;
        break;
    case 4:
        D_8013B06C = D_8013AFF8;
        D_8013B070 = D_8013B02C;
        D_8013B074 = D_8013B060;
        break;
    case 5:
        D_8013B06C = D_8013AFE8;
        D_8013B070 = D_8013B01C;
        D_8013B074 = D_8013B050;
        break;
    case 6:
        D_8013B06C = D_8013AFF0;
        D_8013B070 = D_8013B024;
        D_8013B074 = D_8013B058;
        if ((u32) (get_g_zyotai_s(6) & 0x7F) < 2U) {
            D_8013B06C = D_8013AFEC;
            D_8013B070 = D_8013B020;
            D_8013B074 = D_8013B054;
        }
        break;
    case 7:
        D_8013B06C = D_8013AFE0;
        D_8013B070 = D_8013B014;
        D_8013B074 = D_8013B048;
        break;
    case 8:
        D_8013B06C = D_8013AFD0;
        D_8013B070 = D_8013B004;
        D_8013B074 = D_8013B038;
        break;
    case 9:
        D_8013B06C = D_8013AFF4;
        D_8013B070 = D_8013B028;
        D_8013B074 = D_8013B05C;
        break;
    case 10:
        D_8013B06C = D_8013B000;
        D_8013B070 = D_8013B034;
        D_8013B074 = D_8013B068;
        break;
    }
    if ((u8) D_800E6280.unk_03F >= 4U) {
        D_8013B090 = D_800E6280.unk_03E - 0x5F;
    } else {
        D_8013B090 = D_800E6280.unk_03E - 0x60;
    }
    func_8004284C();
}
