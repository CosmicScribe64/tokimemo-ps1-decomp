void func_8014F88C(void) {
    s32 sp28;
    s32 temp_v0;
    s32 var_a0;

    temp_v0 = D_8015EE00.unk_04;
    D_8015EE00.unk_00 += 1;
    ((DisplayActor44 *)D_8011ECD0)[125].unk_01 = 3;
    ((DisplayActor44 *)D_8011ECD0)[126].unk_01 = 3;
    if ((temp_v0 >= 2) && (temp_v0 < 0x15) && (((DisplayActor44 *)D_8011ECD0)[125].unk_08 == 1)) {
        switch (*(u16 *)&((DisplayActor44 *)D_8011ECD0)[125].unk_18) {
        case 0:
        case 2:
            if (func_8013E9A8(3) == 0) {
                func_8013F15C(0x50C, 1, 9);
            }
            if (func_8013E9A8(2) == 0) {
                func_8013F15C(0x50B, 1, 9);
            }
            break;
        case 4:
        case 8:
        case 12:
            if (func_8013E9A8(2) == 0) {
                func_8013F15C(0x50A, 1, 9);
            }
            break;
        }
    }
    switch (temp_v0) {
    case 0:
        func_8013E54C(5);
        var_a0 = 4;
        do {
            sp28 = var_a0;
            func_8013E97C(var_a0, -0x20, 0);
            var_a0 += 1;
        } while (var_a0 != 0x10);
        func_8013E7C0(0x1D, 6, 1, 1);
        func_8013E7C0(0x1E, 2, 1, 1);
        func_8013E97C(0x1D, -0x20, 0);
        func_8013F15C(0x509, 1, 0);
        func_8014F230();
    default:
        break;
    case 1:
        if (D_8015EE00.unk_00 >= 0x3D) {
            func_8014F230();
        }
        break;
    case 2:
        func_8013E7C0(8, 7, 1, 1);
        func_8014F230();
        break;
    case 3:
        if (D_8015EE00.unk_00 >= 0x4B1) {
            func_8014F230();
        }
        break;
    case 4:
        func_8013E7C0(8, 8, 1, 1);
        func_8014F230();
        break;
    case 5:
        if (D_8015EE00.unk_00 >= 0x961) {
            func_8014F230();
        }
        break;
    case 6:
        func_8013E7C0(8, 9, 1, 1);
        func_8014F230();
        break;
    case 7:
        if (D_8015EE00.unk_00 >= 0xE11) {
            func_8014F230();
        }
        break;
    case 8:
        func_8013E7C0(0x1E, 3, 1, 1);
        func_8013E7C0(8, 0xA, 1, 1);
        func_8014F230();
        break;
    case 9:
        if (D_8015EE00.unk_00 >= 0x12C1) {
            func_8014F230();
        }
        break;
    case 10:
        func_8013E7C0(8, 0xB, 1, 1);
        func_8014F230();
        break;
    case 11:
        if (D_8015EE00.unk_00 >= 0x1771) {
            func_8014F230();
        }
        break;
    case 12:
        func_8013E7C0(0x1E, 4, 1, 1);
        func_8013E7C0(8, 0xC, 1, 1);
        func_8014F230();
        break;
    case 13:
        if (D_8015EE00.unk_00 >= 0x1C21) {
            func_8014F230();
        }
        break;
    case 14:
        func_8013E7C0(8, 0xD, 1, 1);
        func_8014F230();
        break;
    case 15:
        if (D_8015EE00.unk_00 >= 0x20D1) {
            func_8014F230();
        }
        break;
    case 16:
        func_8013E7C0(0x1E, 5, 1, 1);
        func_8013E7C0(8, 0xE, 1, 1);
        func_8014F230();
        break;
    case 17:
        if (D_8015EE00.unk_00 >= 0x2581) {
            func_8014F230();
        }
        break;
    case 18:
        func_8013E7C0(8, 0xF, 1, 1);
        func_8014F230();
        break;
    case 19:
        if (D_8015EE00.unk_00 >= 0x2A31) {
            func_8014F230();
        }
        break;
    case 20:
        if (!(D_8015ED98 & 0xFF) && !(D_8015EDC4 & 0xFF) && !(D_8015EE1C & 0xFF)) {
            func_8014F278();
            func_8013F15C(0x50D, 1, 0);
            D_8015EDF0 = 0x10001001;
        }
        break;
    }
}
