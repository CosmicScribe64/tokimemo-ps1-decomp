void func_801454F0(void) {
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;
    s32 temp_v1;
    s32 temp_v1_2;

    if (D_8015EDF0 == 0x40000000) {
        switch (D_8015EDAC) {
        case 6:
            func_80150DE0();
            func_8013E97C(0x1D, D_8015EC60 + 0x60, D_8015EC64);
            func_8013E97C(0x32, D_8015EC68 + D_8015EC60 + 0x60, D_8015EC6C + D_8015EC64);
            func_8013E810(0x1D, 0, 1, 1);
            func_8013E810(0x32, 7, 5, 1);
            D_801213B0 = 1;
            D_80120E05 = 4;
            D_80121399 = 3;
            func_8014EBA8();
            break;
        case 7:
            func_8013E97C(0x1D, (D_8015EC60 - D_8015EDB8) + 0x60, D_8015EC64);
            func_8013E97C(0x32, ((D_8015EC68 + D_8015EC60) - D_8015EDB8) + 0x60, D_8015EC6C + D_8015EC64);
            temp_v1 = D_8015EDB8 + 2;
            D_8015EDB8 = temp_v1;
            if (temp_v1 >= 0x61) {
                func_8014EBA8();
            }
            break;
        case 8:
            func_800AE0F0(D_8015E74C, "美樹原「いいかげんに…");
            func_800AE0F0(D_8015E774, "い、いえ…やっぱりいいです…");
            func_8014B738((s32) &D_8015E814, 2, 0xA);
            func_8013F1C4(0x86, 0x1300008B, 0);
            D_8015EE48 = 3;
            func_8013E810(0x1D, 8, 1, 1);
            func_8014EBA8();
            break;
        case 9:
            func_8014ED44(0x14);
            break;
        case 10:
            func_8013E810(0x32, 0xD, 1, 1);
            func_8014EBA8();
            break;
        case 11:
            temp_v0 = D_8015EDB4 + 1;
            D_8015EDB4 = temp_v0;
            if (temp_v0 >= 0x12D) {
                func_8013E810(0x32, 0xD, 1, 0);
                D_8015EE48 = 1;
                func_8014EBA8();
            }
            if (func_800460EC() == 4) {
                D_8015ED38 = 1;
            }
            if (D_8015ED38 == 1) {
                if (func_800460EC() == 2) {
                    func_8013E810(0x32, 0xD, 1, 0);
                    D_8015EE48 = 1;
                    D_8015ED38 = 0;
                    func_8014EBA8();
                }
            }
            break;
        case 12:
            func_8013F1C4(0x8C, 0x14000099, 1);
            D_8015EE48 = 3;
            func_8014EBA8();
            break;
        case 13:
            func_8014ED44(5);
            break;
        case 14:
            temp_v0_2 = D_8015EDB4 + 1;
            D_8015EDB4 = temp_v0_2;
            if (temp_v0_2 >= 0x12D) {
                D_8015EE48 = 1;
                D_8015EE4C = D_8015E744 + 1;
                func_8013E810(0x1D, 0, 1, 1);
                func_8014EBA8();
            }
            if (func_800460EC() == 4) {
                D_8015ED38 = 1;
            }
            if (D_8015ED38 == 1) {
                if (func_800460EC() == 2) {
                    D_8015EE48 = 1;
                    D_8015EE4C = D_8015E744 + 1;
                    func_8013E810(0x1D, 0, 1, 1);
                    D_8015ED38 = 0;
                    func_8014EBA8();
                }
            }
            break;
        case 15:
            func_8014ED44(0x1E);
            break;
        case 16:
            func_8013E97C(0x1D, D_8015EC60 + D_8015EDB8, D_8015EC64);
            func_8013E97C(0x32, D_8015EC68 + D_8015EC60 + D_8015EDB8, D_8015EC6C + D_8015EC64);
            temp_v1_2 = D_8015EDB8 + 2;
            D_8015EDB8 = temp_v1_2;
            if (temp_v1_2 >= 0x61) {
                func_8014EBA8();
            }
            break;
        case 17:
            D_8015EDAC = 0x64;
            break;
        }
    } else {
        switch (D_8015EDAC) {
        case 6:
            func_801516E4();
        default:
            break;
        case 7:
            func_800AE0F0(D_8015E74C, "美樹原「いいかげんに…");
            func_800AE0F0(D_8015E774, "い、いえ…やっぱりいいです…");
            func_8014B738((s32) &D_8015E814, 2, 0xA);
            func_8013F1C4(0x86, 0x1300008B, 0);
            D_8015EE48 = 3;
            func_8013E810(0x1D, 8, 1, 1);
            func_8014EBA8();
            break;
        case 8:
            func_8014ED44(0x14);
            break;
        case 9:
            func_8013E810(0x32, 0xD, 1, 1);
            func_8014EBA8();
            break;
        case 10:
            temp_v0_3 = D_8015EDB4 + 1;
            D_8015EDB4 = temp_v0_3;
            if (temp_v0_3 >= 0x12D) {
                func_8013E810(0x32, 0xD, 1, 0);
                D_8015EE48 = 1;
                func_8014EBA8();
            }
            if (func_800460EC() == 4) {
                D_8015ED38 = 1;
            }
            if (D_8015ED38 == 1) {
                if (func_800460EC() == 2) {
                    func_8013E810(0x32, 0xD, 1, 0);
                    D_8015EE48 = 1;
                    D_8015ED38 = 0;
                    func_8014EBA8();
                }
            }
            break;
        case 11:
            func_8013F1C4(0x8C, 0x14000099, 1);
            D_8015EE48 = 3;
            func_8014EBA8();
            break;
        case 12:
            func_8014ED44(5);
            break;
        case 13:
            temp_v0_4 = D_8015EDB4 + 1;
            D_8015EDB4 = temp_v0_4;
            if (temp_v0_4 >= 0x12D) {
                D_8015EE48 = 1;
                D_8015EE4C = D_8015E744 + 1;
                func_8013E810(0x1D, 0, 1, 1);
                func_8014EBA8();
            }
            if (func_800460EC() == 4) {
                D_8015ED38 = 1;
            }
            if (D_8015ED38 == 1) {
                if (func_800460EC() == 2) {
                    D_8015EE48 = 1;
                    D_8015EE4C = D_8015E744 + 1;
                    func_8013E810(0x1D, 0, 1, 1);
                    D_8015ED38 = 0;
                    func_8014EBA8();
                }
            }
            break;
        case 14:
            func_8014ED44(0x1E);
            break;
        case 15:
            func_80151984();
            break;
        case 16:
            D_8015EDAC = 0x64;
            break;
        }
    }
}
