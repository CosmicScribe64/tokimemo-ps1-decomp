s32 func_80076000(void) {
    s32 sp2C;
    s32 temp_v0;
    s32 temp_v0_2;

    if (((u8) D_800E6280.unk_F6C == 2) && (D_800E6280.unk_F80 & 1)) {
        return 1;
    }
    sp2C = 0;
    if (D_800E6280.unk_F88 & 0x20) {
        if (D_800E6280.unk_1093 >= 0) {
            sp2C = 1;
            func_80045414(9, 0, NULL);
            func_80044750(3);
            if (func_8004480C() == 0) {
                func_8004500C(0, 0);
            }
        }
        switch (D_800E6280.unk_1093) {
        case 4:
            if (get_icon_can_use((s32) D_800E6280.unk_1093) != 0) {
                func_80044750(0x500);
                if (D_800E6280.unk_041 == 6) {
                    if (((u16) D_800E6280.unk_0F4.h >> 0xC) == 0xB) {
                        D_800E6280.unk_F68.b[0] = (D_800E6280.unk_1093 * 0x10) | (D_800E6280.unk_F68.b[0] & 0xFF0F);
                        *(&D_800E6280.unk_69C[0].unk_01 + (D_800E6280.unk_71C * 4)) = D_800E6280.unk_1093;
                        *(&D_800E6280.unk_69C[0].unk_02 + (D_800E6280.unk_71C * 4)) = func_8006BDA8(1);
                        func_80042908(7);
                    } else {
                        *(&D_800E6280.unk_69C[0].unk_01 + (D_800E6280.unk_71C * 4)) = D_800E6280.unk0;
                        *(&D_800E6280.unk_69C[0].unk_02 + (D_800E6280.unk_71C * 4)) = func_8006BDA8(1);
                        D_800E6280.unk_F68.b[0] = (D_800E6280.unk_1093 * 0x10) | (D_800E6280.unk_F68.b[0] & 0xFF0F);
                        temp_v0 = func_80053564((s32) ((u16) D_800E6280.unk_0F4.h >> 0xC));
                        if ((temp_v0 != -1) && (D_800E6280.unk_1BC[temp_v0].unk_0C.w & 4)) {
                            D_800E6280.unk_1BC[temp_v0].unk_06 += 1;
                        }
                        func_80042908(6);
                    }
                } else {
                    temp_v0_2 = func_80053564((s32) ((u16) D_800E6280.unk_000 >> 0xC));
                    if ((temp_v0_2 != -1) && (D_800E6280.unk_1BC[temp_v0_2].unk_0C.w & 4)) {
                        D_800E6280.unk_1BC[temp_v0_2].unk_06 += 1;
                    }
                    *(&D_800E6280.unk_69C[0].unk_01 + (D_800E6280.unk_71C * 4)) = D_800E6280.unk0;
                    *(&D_800E6280.unk_69C[0].unk_02 + (D_800E6280.unk_71C * 4)) = func_8006BDA8(1);
                    D_800E6280.unk_F68.b[0] = (D_800E6280.unk_1093 * 0x10) | (D_800E6280.unk_F68.b[0] & 0xFF0F);
                    func_80042908(5);
                }
            }
            break;
        case 0:
        case 1:
        case 2:
        case 3:
        case 5:
        case 6:
        case 7:
            if (get_icon_can_use((s32) D_800E6280.unk_1093) != 0) {
                D_800E6280.unk_F68.b[0] = (D_800E6280.unk_1093 * 0x10) | (D_800E6280.unk_F68.b[0] & 0xFF0F);
                func_80044750(0x500);
                *(&D_800E6280.unk_69C[0].unk_01 + (D_800E6280.unk_71C * 4)) = D_800E6280.unk_1093;
                *(&D_800E6280.unk_69C[0].unk_02 + (D_800E6280.unk_71C * 4)) = func_8006BDA8(1);
                func_80042908(5);
            }
            break;
        case 9:
            if ((get_icon_can_use((s32) D_800E6280.unk_1093) != 0) && (appointment_find_today((s32) D_800E6280.unk_03E, (s32) D_800E6280.unk_03F, (s32) D_800E6280.unk_040) > 0)) {
                D_800E6280.unk_F68.b[0] = (D_800E6280.unk_1093 * 0x10) | (D_800E6280.unk_F68.b[0] & 0xFF0F);
                func_80044750(0x500);
                func_80042908(0xFE);
            }
            break;
        case 8:
            if (get_icon_can_use((s32) D_800E6280.unk_1093) != 0) {
                D_800E6280.unk_F68.b[0] = (D_800E6280.unk_1093 * 0x10) | (D_800E6280.unk_F68.b[0] & 0xFF0F);
                func_80044750(0x50D);
                func_80042908(0x10);
            }
            break;
        case 10:
            if (get_icon_can_use((s32) D_800E6280.unk_1093) != 0) {
                func_80044750(0x50D);
                func_80042908(0x20);
            }
            break;
        case 12:
            func_80044750(0x50D);
            func_80042908(0x30);
            break;
        case 11:
            func_80044750(0x50D);
            func_80042908(0x40);
            break;
        }
    } else {
    default:
    }
    return sp2C;
}
