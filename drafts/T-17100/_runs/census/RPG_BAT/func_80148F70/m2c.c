extern s32 D_800E68EC;
void func_80148F70(void) {
    s32 sp2C;
    s32 temp_a0;
    s32 temp_t2;
    s32 temp_v0_2;
    s32 var_a2_2;
    s32 var_a3;
    s32 var_v0;
    u16 temp_v0;
    u16 var_a2;

    temp_v0 = *(&D_800E68EC + (D_8015EBA0 * 4));
    if (temp_v0 < 0x32U) {
        var_a2 = 0x32;
    } else {
        var_a2 = temp_v0;
        if (temp_v0 >= 0x227U) {
            var_a2 = 0x226;
        }
    }
    var_a2_2 = (var_a2 * 0xA) - 0x1F4;
    if (D_8015EB98 & 0xF0000) {
        sp2C = var_a2_2;
        func_801495DC();
    }
    if (D_8015EB98 & 0x80000000) {
        sp2C = var_a2_2;
        func_801495DC();
    }
    temp_v0_2 = D_8015EDC4 & 0x100000;
    if ((temp_v0_2 != 0) || (D_8015EDC4 & 0x600000) || (D_8015EDC4 & 0x08000000)) {
        if (D_8015ED98 & 2) {
            switch (D_8015EBA0) {
            case 2:
                var_a3 = 0x270F;
                if (temp_v0_2 != 0) {
                    D_8015EBB0 = func_8013EA00(0x9C4, 0x9C4) + 0x9C4;
                    D_8015EBB4 = func_8013EA00(0x9C4, 0x9C4) + 0x9C4;
                    D_8015EBB8 = func_8013EA00(0x9C4, 0x9C4) + 0x9C4;
                    if (D_8015EBAC == 0) {
                        D_8015EBB0 = 0x270F;
                    }
                    var_a3 = D_8015EBB0;
                    if (D_8015EBAC == 1) {
                        D_8015EBB4 = 0x270F;
                    }
                    if (D_8015EBAC == 2) {
                        D_8015EBB8 = 0x270F;
                    }
                }
block_64:
                D_8015EBB0 = var_a3;
                break;
            case 5:
                if (temp_v0_2 != 0) {
                    D_8015EBB0 = func_8013EA00(0x9C4, 0x3E8) + 0x7D0;
                    D_8015EBB4 = func_8013EA00(0x9C4, 0x3E8) + 0x7D0;
                    D_8015EBB8 = func_8013EA00(0x9C4, 0x3E8) + 0x7D0;
                    if (D_8015EBAC == 0) {
                        D_8015EBB0 += 0x7D0;
                    }
                    var_a3 = D_8015EBB0;
                    if (D_8015EBAC == 1) {
                        D_8015EBB4 += 0x7D0;
                    }
                    if (D_8015EBAC == 2) {
                        D_8015EBB8 += 0x7D0;
                    }
                } else {
                    var_a3 = func_8013EA00(0x9C4, 0x3E8) + 0xFA0;
                }
                goto block_64;
            case 6:
                if (temp_v0_2 != 0) {
                    D_8015EBB0 = (func_8013E9A8(0x32) * 0x64) + 0x7D0;
                    D_8015EBB4 = (func_8013E9A8(0x32) * 0x64) + 0x3E8;
                    D_8015EBB8 = (func_8013E9A8(0x32) * 0x64) + 0x3E8;
                } else {
                    D_8015EBB0 = (func_8013E9A8(0x32) * 0x64) + 0xBB8;
                }
                break;
            case 1:
                if (temp_v0_2 != 0) {
                    sp2C = var_a2_2;
                    D_8015EBB0 = func_8013EA00(0x1770, var_a2_2);
                    D_8015EBB4 = func_8013EA00(0x1770, sp2C);
                    D_8015EBB8 = func_8013EA00(0x1770, sp2C);
                } else {
                    D_8015EBB0 = func_8013EA00(0x1770, var_a2_2);
                }
                break;
            case 4:
                if (temp_v0_2 != 0) {
                    sp2C = var_a2_2;
                    if (func_8013E9A8(0x64) < 0x14) {
                        var_a3 = 0x3E8;
                    } else {
                        var_v0 = func_8013EA00(var_a2_2 / 2, 0xDAC);
                        goto block_44;
                    }
                } else {
                    var_v0 = func_8013EA00(var_a2_2 / 2, 0xDAC);
block_44:
                    var_a3 = var_v0 + 0x9C4;
                }
                goto block_64;
            case 0:
                var_a3 = (func_8013E9A8((var_a2_2 / 1000) + 1) * 0x3E8) + 0x1388;
                goto block_64;
            case 9:
                temp_a0 = var_a2_2 / 2;
                var_a3 = func_8013EA00(temp_a0, temp_a0) + 0x1770;
                goto block_64;
            case 3:
                var_a3 = func_8013EA00(0x5DC, var_a2_2) + 0xBB8;
                goto block_64;
            case 7:
                var_a3 = func_8013EA00(var_a2_2, var_a2_2 + 0x7D0) + 0x3E8;
                goto block_64;
            case 10:
                var_a3 = func_8013EA00(var_a2_2, var_a2_2 + 0x3E8) + 0x7D0;
                goto block_64;
            case 8:
                temp_t2 = (s32) (var_a2_2 + 0x7D0) / 2;
                var_a3 = func_8013EA00(temp_t2, temp_t2) + 0xFA0;
                goto block_64;
            case 11:
                switch (D_8015EC34) {
                case 0:
                case 1:
                    D_8015EC34 = 0;
                    var_a3 = func_8013EA00(0x1F4, 0x1F4);
                    break;
                case 2:
                case 3:
                    D_8015EC34 = 1;
                    var_a3 = func_8013EA00(0x3E8, 0x3E8);
                    break;
                case 4:
                case 5:
                    D_8015EC34 = 2;
                    var_a3 = func_8013EA00(0x5DC, 0x5DC);
                    break;
                case 6:
                case 7:
                    D_8015EC34 = 3;
                    var_a3 = func_8013EA00(0x7D0, 0x7D0);
                    break;
                case 8:
                case 9:
                case 10:
                case 11:
                case 12:
                case 13:
                case 14:
                case 15:
                    D_8015EC34 = 4;
                    var_a3 = func_8013EA00(0x9C4, 0x9C4);
                    break;
                default:
                    D_8015EC34 = 5;
                    if (temp_v0_2 != 0) {

                    } else {
                        D_8015EDC0 -= 0x7530;
                    }
                    var_a3 = 0x270F;
                    break;
                }
                goto block_64;
            }
        } else {
            func_801495DC();
        }
    }
    if (D_8015EBB0 >= 0x2710) {
        D_8015EBB0 = 0x270F;
    }
    if (D_8015EBB4 >= 0x2710) {
        D_8015EBB4 = 0x270F;
    }
    if (D_8015EBB8 >= 0x2710) {
        D_8015EBB8 = 0x270F;
    }
}
