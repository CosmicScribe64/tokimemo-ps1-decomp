void func_801352B0(void) {
    s32 sp30;
    s32 sp2C;
    s32 temp_v1;

    if (D_800E62BC == 2) {
        D_800E62BC = 1;
    }
    temp_v1 = D_800E7384;
    sp30 = (s32) D_800E738A;
    if (!(D_800E7208 & 0x600060) || (D_801206FE < -0xBF)) {
        sp2C = temp_v1;
        func_801343B0();
    }
    D_800E7384 = temp_v1 + 1;
    if (D_801206FE >= -0xBF) {
        if (!(D_800E7384 & 1) && (check_end_k() != 0) && ((D_800E62BC == 0) || (func_80046094() == 9))) {
            D_801206FE -= 1;
        }
        D_8011ED5B &= 0x7F;
    }
    if (sp30 != D_800E7384) {
        if (D_801206FE >= -0xBF) {
            D_800E738A = (u8) sp30;
            D_800CA2E4 = 9;
            D_800CA2E8 = 0;
        } else {
            D_800CA2E4 = 9;
            D_800CA2E8 = 1;
        }
    }
}
