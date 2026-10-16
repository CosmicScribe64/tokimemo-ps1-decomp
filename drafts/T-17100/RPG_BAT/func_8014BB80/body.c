void func_8014BB80(void) {
    RpgHalfTbl12 sp34;
    s32 var_v1;

    var_v1 = D_8015EE50.unk_00;
    sp34 = D_8015E720;
    if ((var_v1 != 3) && !(D_800E6280.unk_1092 & 1)) {
        func_8006BA40();
        var_v1 = D_8015EE50.unk_00;
    }
    if (!(D_8015ED98 & 0x10000) && (var_v1 != 4) && (var_v1 != 5)) {
        var_v1 = 6;
    }
    D_8015EE50.unk_00 = var_v1;
    switch (var_v1) {
    case 0:
        func_8013E7C0(0x25, 0x31, 5, 1);
        if (D_8015EBA8 == 1) {
            menu_set(0, 3, sp34.v, &sp34.v[3], &sp34.v[6], &sp34.v[9]);
        } else {
            menu_set(0, 2, sp34.v, &sp34.v[3], &sp34.v[6], &sp34.v[9]);
        }
        func_8014EB58(1);
    default:
        break;
    case 1:
        if (func_8013E90C(0x25) != 0) {
            func_8014EB58(1);
        }
        break;
    case 2:
        func_8013E7C0(0x25, 2, 1, 1);
        if (D_8015EBA8 == 1) {
            func_8013E7C0(0x23, 0xFF, 1, 1);
        } else {
            func_8013E7C0(0x23, 0xFF, 1, 0);
        }
        func_8014EB58(1);
        break;
    case 3:
        menu_check(0, D_8011ECF6, D_8011ECFA);
        func_8004FC10(0);
        func_8013E7C0(0x22, 0xFF, 1, 1);
        switch (D_800E6280.unk_1093) {
        case 0:
            func_8013E7C0(0x22, 5, 1, 1);
            break;
        case 1:
            func_8013E7C0(0x22, 6, 1, 1);
            break;
        case 2:
            func_8013E7C0(0x22, 7, 1, 1);
            break;
        case -1:
            func_8013E7C0(0x22, 0xFF, 1, 0);
            break;
        }
        if ((D_800E6280.unk_1093 != -1) && (D_800E6280.unk_F88 & 0x20)) {
            func_8013E7C0(0x22, 0xFF, 1, 0);
            func_8013E7C0(0x23, 0xFF, 1, 0);
            switch (D_800E6280.unk_1093) {
            case 0:
                D_8015ED98 ^= 0x01000000;
                break;
            case 1:
                D_8015ED98 ^= 0x02000000;
                break;
            case 2:
                D_8015ED98 ^= 0x04000000;
                break;
            }
            func_8014EB58(1);
        }
        break;
    case 4:
        func_8013E7C0(0x25, 0x31, 0x21, 1);
        D_8012103C = 6;
        func_8013E7C0(0x22, 0xFF, 1, 0);
        func_8013E7C0(0x23, 0xFF, 1, 0);
        func_8014EB58(1);
        break;
    case 5:
        if (func_8013E90C(0x25) != 0) {
            func_8013E7C0(0x25, 0xFF, 1, 0);
            func_8014EB58(1);
        }
        break;
    case 6:
        D_8015ED98 &= 0xFFFEFFFF;
        func_8013E7C0(0x22, 0xFF, 1, 0);
        func_8013E7C0(0x23, 0xFF, 1, 0);
        func_8013E7C0(0x25, 0xFF, 1, 0);
        D_8015EE50.unk_00 = 0;
        break;
    }
}
