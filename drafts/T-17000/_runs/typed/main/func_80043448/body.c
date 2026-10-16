void func_80043448(void) {
    u8 temp_t1;
    u8 temp_t3;
    u8 temp_t7;

    if (D_800B3CFC != 0) {
        temp_t7 = (D_800B3D00 + 1) & 0xFF;
        D_800B3D00 = temp_t7;
        if (temp_t7 == 0) {
            D_800B3CFC = 0;
            D_800B3CF8 = 0;
        }
    } else {
        temp_t1 = (D_800B3CF8 + 1) & 0xFF;
        if (D_800E6280 == *(&D_800B3CA8 + (D_800B3CF8 * 4))) {
            D_800B3CF8 = temp_t1;
            if (temp_t1 >= 0x12U) {
                D_800B3CFC = 1;
            }
            D_800B3D00 = 0x10;
        }
        temp_t3 = (D_800B3D00 - 1) & 0xFF;
        D_800B3D00 = temp_t3;
        if (temp_t3 == 0) {
            D_800B3CF8 = 0;
        }
    }
}
