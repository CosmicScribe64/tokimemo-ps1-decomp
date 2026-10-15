void func_8013388C(void) {
    u8 sp2B;
    s16 temp_a0;
    s16 temp_a0_2;
    s16 var_a0;
    u32 temp_v0;
    u32 temp_v0_2;

    sp2B = get_g_zyotai_s(D_800E71DF) & 0x7F;
    func_800AE120(D_800E7378);
    switch (D_800E71DF) {
    case 0:
        temp_v0 = D_800E71E8 & 0xF;
        if (temp_v0 < 2U) {
            D_80134CCC = 0;
            D_800CA148 = 0;
        } else {
            var_a0 = 2;
            if (temp_v0 == 2) {
                D_80134CCC = 0;
                goto block_12;
            }
            var_a0 = 4;
            if (temp_v0 == 3) {
                D_80134CCC = 0;
                goto block_12;
            }
            if (temp_v0 == 4) {
                temp_a0 = (((u16) D_800E6374 >> 0xC) * 2) + 6;
                D_80134CCC = 0;
                D_800CA148 = temp_a0;
                if (((u16) D_800E6374 >> 0xC) == 0xA) {
                    D_800CA148 = temp_a0 - 4;
                }
            } else {
                D_80134CCC = 3;
                var_a0 = 0x1A;
block_12:
                D_800CA148 = var_a0;
            }
        }
        if ((func_800AE0D0() % 2) != 0) {
            temp_a0_2 = D_800CA148 + 1;
            D_800CA148 = temp_a0_2;
            if (temp_a0_2 == 5) {
                D_80134CCC = 2;
            }
        }
        break;
    case 1:
        if ((u32) (D_800E71E8 & 0xF) < 2U) {
            D_80134CCC = 0;
            D_800CA148 = 0;
        } else {
            D_80134CCC = 0;
            D_800CA148 = (((u16) D_800E6374 >> 0xC) * 2) + 2;
        }
        if ((func_800AE0D0() % 2) != 0) {
            D_800CA148 += 1;
        }
        break;
    case 2:
        if ((u32) (D_800E71E8 & 0xF) < 2U) {
            D_80134CCC = 0;
            D_800CA148 = 0;
        } else {
            D_80134CCC = 4;
            D_800CA148 = (((u16) D_800E6374 >> 0xC) * 2) - 2;
        }
        if ((func_800AE0D0() % 2) != 0) {
            D_800CA148 += 1;
        }
        break;
    case 3:
        if ((u32) (D_800E71E8 & 0xF) < 3U) {
            D_80134CCC = 0;
            D_800CA148 = 0;
        } else {
            D_80134CCC = 0;
            D_800CA148 = (((u16) D_800E6374 >> 0xC) * 2) - 6;
        }
        if ((func_800AE0D0() % 2) != 0) {
            D_800CA148 += 1;
        }
        break;
    case 4:
        if ((u32) (D_800E71E8 & 0xF) < 4U) {
            D_80134CCC = 0;
            D_800CA148 = 0;
            if ((func_800AE0D0() % 2) != 0) {
                D_800CA148 += 1;
            }
        } else if (((func_800AE0D0() % 2) != 0) || ((temp_v0_2 = (u16) D_800E6374 >> 0xC, (temp_v0_2 == 6)) && ((D_800E62BE == 0x62) || ((D_800E62BE == 0x61) && ((u8) D_800E62BF >= 9U)))) || ((temp_v0_2 == 7) && (D_800E62BE == 0x62))) {
            if (sp2B < 2U) {
                D_80134CCC = 6;
                D_800CA148 = 4;
            } else {
                D_80134CCC = 5;
                D_800CA148 = 5;
            }
        } else {
            D_80134CCC = 0;
            D_800CA148 = ((u16) D_800E6374 >> 0xC) - 4;
        }
        break;
    case 5:
        if ((u32) (D_800E71E8 & 0xF) < 4U) {
            D_80134CCC = 0;
            D_800CA148 = 0;
        } else {
            D_80134CCC = 0;
            D_800CA148 = 2;
        }
        if ((func_800AE0D0() % 2) != 0) {
            D_800CA148 += 1;
        }
        break;
    case 6:
        if ((u32) (D_800E71E8 & 0xF) < 4U) {
            D_80134CCC = 0;
            D_800CA148 = 0;
            if ((func_800AE0D0() % 2) != 0) {
                D_80134CCC = 2;
                D_800CA148 += 1;
            }
        } else {
            D_80134CCC = 0;
            D_800CA148 = 4;
            if ((func_800AE0D0() % 2) != 0) {
                D_800CA148 += 1;
            }
        }
        if (sp2B < 2U) {
            D_800CA148 += 2;
        }
        break;
    case 7:
        D_80134CCC = 3;
        D_800CA148 = 0;
        break;
    case 8:
        D_80134CCC = 7;
        D_800CA148 = 0;
        if ((func_800AE0D0() % 2) != 0) {
            D_800CA148 += 1;
        }
        break;
    case 9:
        D_80134CCC = (u8) (func_800AE0D0() % 2);
        D_800CA148 = 0;
        if ((func_800AE0D0() % 2) != 0) {
            D_800CA148 += 1;
        }
        break;
    case 10:
        if ((u32) (D_800E71E8 & 0xF) < 4U) {
            D_800CA148 = 0;
            if ((func_800AE0D0() % 2) != 0) {
                D_800CA148 += 1;
            }
            if (D_800CA148 == 1) {
                D_80134CCC = 2;
            } else {
                D_80134CCC = 0;
            }
        } else {
            D_80134CCC = 0;
            D_800CA148 = 2;
            if ((func_800AE0D0() % 2) != 0) {
                D_800CA148 += 1;
            }
        }
        break;
    default:
        D_800CA148 = (s16) (func_800AE0D0() % 8);
        D_80134CCC = (func_800AE0D0() % 2) + 8;
        if ((D_800E71E8 & 0xF) == 3) {
            D_80134CCC = 9;
        }
        break;
    }
    D_800CA14C = 0;
    func_801340E4();
    func_8004284C();
}
