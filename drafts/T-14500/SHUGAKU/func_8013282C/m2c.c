extern u8 D_800E62BE;
extern u8 D_800E62BF;
extern u8 D_800E71DF;

void func_8013282C(void) {
    s32 var_t4;

    D_8013B084 = 0;
    D_8013B088 = 0;
    func_800847B8(D_800E71DF);
    switch (D_800E71DF) {
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
        var_t4 = D_8013B050;
        D_8013B070 = D_8013B01C;
block_14:
        D_8013B074 = var_t4;
        break;
    case 6:
        D_8013B06C = D_8013AFF0;
        D_8013B070 = D_8013B024;
        D_8013B074 = D_8013B058;
        if ((u32) (func_80051A68(6) & 0x7F) < 2U) {
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
        var_t4 = D_8013B068;
        D_8013B070 = D_8013B034;
        goto block_14;
    }
    if ((u8) D_800E62BF >= 4U) {
        D_8013B090 = D_800E62BE - 0x5F;
    } else {
        D_8013B090 = D_800E62BE - 0x60;
    }
    func_8004284C();
}
