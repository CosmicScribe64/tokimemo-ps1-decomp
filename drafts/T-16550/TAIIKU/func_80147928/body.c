void func_80147928(void) {
    s32 temp_a0;
    s32 temp_v0;
    s32 temp_v1;
    s32 var_a0;
    s32 var_a0_2;
    s32 var_v1;
    u32 temp_t1;

    if ((D_8014A3F8 > 0) || (D_8014A3E0 != 0)) {
        if (((u16) D_8014A468 >= 0x65U) && (D_8014A45C == 0)) {
            temp_v0 = func_800AE0D0();
            var_v1 = 0;
            if (D_8014A3F8 == D_8014A3D4) {
                var_v1 = 0xF;
            }
            temp_t1 = (temp_v0 % 100) & 0xFFFF;
            if (temp_t1 < (u32) (var_v1 + 0x19)) {
                D_8014A468 = 0;
                var_a0 = -0x64;
                goto block_10;
            }
            var_a0 = 0x64;
            if (temp_t1 < (u32) (var_v1 + 0x32)) {
                D_8014A468 = 0;
block_10:
                D_8014A45C = var_a0;
            }
        }
        var_a0_2 = D_8014A45C;
        temp_v1 = D_800E7200 & 0xA;
        if ((temp_v1 != 0) && !(D_800E7200 & 5)) {
            var_a0_2 -= 0xB;
            if (var_a0_2 == 0) {
                D_8014A468 = 0;
            }
        }
        D_8014A45C = var_a0_2;
        if (D_800E7200 & 5) {
            D_8014A45C = var_a0_2;
            if (temp_v1 == 0) {
                temp_a0 = var_a0_2 + 0xB;
                D_8014A45C = temp_a0;
                if (temp_a0 == 0) {
                    D_8014A468 = 0;
                }
            }
        }
    }
}
