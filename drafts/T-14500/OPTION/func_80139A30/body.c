void func_80139A30(void) {
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 var_v0_2;
    u16 var_v1;
    u8 var_v0;

    if (D_800E7208 & 0x08000000) {
        D_8013D3F0 = 1 - D_8013D3F0;
    }
    if ((D_8013D3F0 != 0) && (func_800460EC() & 2)) {
        D_800E7208 |= 0x20;
        D_800E7200 |= 4;
    }
    if (D_800E7200 & 5) {
        if (D_800E7208 & 0x8000) {
            D_800E71DF += 1;
        }
        var_v0 = D_800E71DF;
        if (D_800E7208 & 0x2000) {
            var_v0 = (var_v0 - 1) & 0xFF;
        }
        if (var_v0 == 0xFF) {
            var_v0 = 0xF;
        }
        D_800E71DF = var_v0;
        if (var_v0 >= 0x11U) {
            D_800E71DF = 0;
        }
    }
    if ((D_8013D3B0 | D_8013D3B4) != 0) {
        func_8013A060();
    } else if (D_800E7208 & 0x860) {
        switch (D_800E7313) {
        case 0:
            func_80139E3C(0);
            break;
        case 1:
            func_80042940(3);
            break;
        case 2:
            func_80042940(4);
            break;
        case 3:
            func_8013A010();
            break;
        case 4:
            func_8013A038();
            break;
        case 5:
            if (D_800E7208 & 0x800) {
                func_80042940(5);
            } else {
                func_80139F04();
            }
            break;
        case 6:
            func_80139F8C();
            break;
        }
    }
    if (D_800E7200 & 0x50000) {
        temp_v0 = D_800E7200 & 0xF00000;
        switch (temp_v0) {
        case 0x200000:
            D_8013D3B8 += 1;
            break;
        case 0x400000:
            D_8013D3B8 += 0xA;
            break;
        case 0x800000:
            D_8013D3B8 += 0x64;
            break;
        case 0x100000:
            D_8013D3B8 += 0x3E8;
            break;
        }
        var_v1 = D_8013D3B8;
    } else {
        temp_v0_2 = D_800E7208 & 0xF00000;
        switch (temp_v0_2) {
        default:
            var_v1 = D_8013D3B8;
            break;
        case 0x200000:
            var_v1 = (D_8013D3B8 + 1) & 0xFFFF;
            break;
        case 0x400000:
            var_v1 = (D_8013D3B8 + 0xA) & 0xFFFF;
            break;
        case 0x800000:
            var_v1 = (D_8013D3B8 + 0x64) & 0xFFFF;
            break;
        case 0x100000:
            var_v1 = (D_8013D3B8 + 0x3E8) & 0xFFFF;
            break;
        }
    }
    temp_v0_3 = D_800E7200 & 0xF00000;
    if (D_800E7200 & 0xA0000) {
        switch (temp_v0_3) {
        default:
            var_v0_2 = var_v1 - 0x64;
            if (temp_v0_3 != 0x800000) {

            } else if (var_v0_2 != 0) {
block_61:
                var_v1 = var_v0_2 & 0xFFFF;
            }
            break;
        case 0x200000:
            var_v0_2 = var_v1 - 1;
            goto block_61;
        case 0x400000:
            var_v0_2 = var_v1 - 0xA;
            if (var_v0_2 != 0) {
                goto block_61;
            }
            break;
        case 0x100000:
            var_v0_2 = var_v1 - 0x3E8;
            if (var_v0_2 != 0) {
                goto block_61;
            }
            break;
        }
    }
    D_8013D3B8 = var_v1;
    if (var_v1 >= 0xB54U) {
        D_8013D3B8 = 0;
    }
    func_8013A334();
}
