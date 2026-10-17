void func_80076FDC(void) {
    s32 sp28;
    s32 temp_v0_3;
    s32 var_v1;
    u32 temp_t1;
    u32 temp_t3;
    u8 temp_v0;
    u8 temp_v0_2;
    u8 var_a0;
    u8 var_a0_2;

    D_801230D0 = 0xFF;
    if (func_80066A84() == 1) {
        temp_v0 = *(&D_800E691D + (D_800E699C * 4));
        if (temp_v0 == 4) {
            var_a0 = 0xFF;
        } else {
            var_a0 = D_800B69D0[temp_v0];
        }
    } else {
        temp_v0_2 = *(&D_800E691D + (D_800E699C * 4));
        if (temp_v0_2 == 4) {
            var_a0 = 0xFF;
        } else {
            var_a0 = D_800B69D8[temp_v0_2];
        }
    }
    D_801230D0 = (s32) var_a0;
    if (var_a0 == 0xFF) {
        temp_v0_3 = func_8006F524(4);
        var_v1 = temp_v0_3;
        if (temp_v0_3 != 0) {
            var_v1 = 1;
        }
        sp28 = var_v1;
        if (func_80066A84() == 1) {
            temp_t1 = (u16) D_800E6374 >> 0xC;
            switch (temp_t1) {
            case 0:
                var_a0_2 = (*D_800B6890)[var_v1];
                goto block_37;
            case 1:
                var_a0_2 = *(&D_800B6892 + var_v1);
                goto block_37;
            case 2:
                var_a0_2 = *(&D_800B6894 + var_v1);
                goto block_37;
            case 3:
                var_a0_2 = *(&D_800B6896 + var_v1);
                goto block_37;
            case 4:
                var_a0_2 = *(&D_800B6898 + var_v1);
                goto block_37;
            case 5:
                var_a0_2 = *(&D_800B689A + var_v1);
                goto block_37;
            case 6:
                var_a0_2 = *(&D_800B689C + var_v1);
                goto block_37;
            case 7:
                var_a0_2 = *(&D_800B689E + var_v1);
                goto block_37;
            case 8:
                var_a0_2 = *(&D_800B68A0 + var_v1);
                goto block_37;
            case 9:
                var_a0_2 = *(&D_800B68A2 + var_v1);
                goto block_37;
            case 10:
                var_a0_2 = *(&D_800B68A4 + var_v1);
                goto block_37;
            }
        } else {
            temp_t3 = (u16) D_800E6374 >> 0xC;
            switch (temp_t3) {
            case 0:
                var_a0_2 = (*D_800B68A8)[var_v1];
block_37:
                D_801230D0 = (s32) var_a0_2;
                break;
            case 1:
                var_a0_2 = *(&D_800B68AA + var_v1);
                goto block_37;
            case 2:
                var_a0_2 = *(&D_800B68AC + var_v1);
                goto block_37;
            case 3:
                var_a0_2 = *(&D_800B68AE + var_v1);
                goto block_37;
            case 4:
                var_a0_2 = *(&D_800B68B0 + var_v1);
                goto block_37;
            case 5:
                var_a0_2 = *(&D_800B68B2 + var_v1);
                goto block_37;
            case 6:
                var_a0_2 = *(&D_800B68B4 + var_v1);
                goto block_37;
            case 7:
                var_a0_2 = *(&D_800B68B6 + var_v1);
                goto block_37;
            case 8:
                var_a0_2 = *(&D_800B68B8 + var_v1);
                goto block_37;
            case 9:
                var_a0_2 = *(&D_800B68BA + var_v1);
                goto block_37;
            case 10:
                var_a0_2 = *(&D_800B68BC + var_v1);
                goto block_37;
            }
        }
    }
}
