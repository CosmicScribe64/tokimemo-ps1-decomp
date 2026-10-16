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
        temp_v0 = *(&D_800E6280.unk_69C[0].unk_01 + (D_800E6280.unk_71C * 4));
        if (temp_v0 == 4) {
            var_a0 = 0xFF;
        } else {
            var_a0 = *(&D_800B69D0 + temp_v0);
        }
    } else {
        temp_v0_2 = *(&D_800E6280.unk_69C[0].unk_01 + (D_800E6280.unk0 * 4));
        if (temp_v0_2 == 4) {
            var_a0 = 0xFF;
        } else {
            var_a0 = *(&D_800B69D8 + temp_v0_2);
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
            temp_t1 = (u16) D_800E6280.unk_0F4.h >> 0xC;
            switch (temp_t1) {
            case 0:
                var_a0_2 = (*D_800B6890)[var_v1];
                goto block_37;
            case 1:
                var_a0_2 = D_800B6890[1][var_v1];
                goto block_37;
            case 2:
                var_a0_2 = D_800B6890[2][var_v1];
                goto block_37;
            case 3:
                var_a0_2 = D_800B6890[3][var_v1];
                goto block_37;
            case 4:
                var_a0_2 = D_800B6890[4][var_v1];
                goto block_37;
            case 5:
                var_a0_2 = D_800B6890[5][var_v1];
                goto block_37;
            case 6:
                var_a0_2 = D_800B6890[6][var_v1];
                goto block_37;
            case 7:
                var_a0_2 = D_800B6890[7][var_v1];
                goto block_37;
            case 8:
                var_a0_2 = D_800B6890[8][var_v1];
                goto block_37;
            case 9:
                var_a0_2 = D_800B6890[9][var_v1];
                goto block_37;
            case 10:
                var_a0_2 = D_800B6890[0xA][var_v1];
                goto block_37;
            }
        } else {
            temp_t3 = (u16) D_800E6280.unk_000 >> 0xC;
            switch (temp_t3) {
            case 0:
                var_a0_2 = (*D_800B68A8)[var_v1];
block_37:
                D_801230D0 = (s32) var_a0_2;
                break;
            case 1:
                var_a0_2 = D_800B68A8[1][var_v1];
                goto block_37;
            case 2:
                var_a0_2 = D_800B68A8[2][var_v1];
                goto block_37;
            case 3:
                var_a0_2 = D_800B68A8[3][var_v1];
                goto block_37;
            case 4:
                var_a0_2 = D_800B68A8[4][var_v1];
                goto block_37;
            case 5:
                var_a0_2 = D_800B68A8[5][var_v1];
                goto block_37;
            case 6:
                var_a0_2 = D_800B68A8[6][var_v1];
                goto block_37;
            case 7:
                var_a0_2 = D_800B68A8[7][var_v1];
                goto block_37;
            case 8:
                var_a0_2 = D_800B68A8[8][var_v1];
                goto block_37;
            case 9:
                var_a0_2 = D_800B68A8[9][var_v1];
                goto block_37;
            case 10:
                var_a0_2 = D_800B68A8[0xA][var_v1];
                goto block_37;
            }
        }
    }
}
