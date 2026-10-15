s32 func_8005B43C(void) {
    s32 sp28;
    s32 temp_v0;
    u32 temp_v1;
    u8 temp_v0_2;

    if ((D_800E71EC == 2) && (D_800E7200 & 1)) {
        return 1;
    }
    sp28 = 0;
    if (D_800E7208 & 0x20) {
        if (D_800E7313 >= 0) {
            sp28 = 1;
            func_80045414(9, 0, NULL);
            func_80044750(3);
            if (func_8004480C() == 0) {
                func_8004500C(0, 0);
            }
        }
        switch (D_800E7313) {
        case 4:
            if (get_icon_can_use((s32) D_800E7313) != 0) {
                func_80044750(0x500);
                temp_v1 = (u16) D_800E6374 >> 0xC;
                if (temp_v1 == 0xB) {
                    D_800E71E8 = (D_800E7313 & 0xF) | (D_800E71E8 & 0xFFF0);
                    func_80042908(5);
                } else {
                    temp_v0 = func_80053564((s32) temp_v1);
                    if ((temp_v0 != -1) && (D_800E6280.unk_1BC[temp_v0].unk_0C.w & 4)) {
                        D_800E6280.unk_1BC[temp_v0].unk_06 += 1;
                    }
                    main_state_set(0x30);
                    D_800E71E8 = (D_800E7313 & 0xF) | (D_800E71E8 & 0xFFF0);
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
        case 8:
        case 9:
        case 10:
            if (get_icon_can_use((s32) D_800E7313) != 0) {
                if ((D_800E7313 != 8) && (D_800E7313 != 0xA)) {
                    func_80044750(0x500);
                } else {
                    func_80044750(0x50D);
                }
                main_state_set(0x30);
                D_800E71E8 = (D_800E7313 & 0xF) | (D_800E71E8 & 0xFFF0);
            }
            break;
        case 12:
            func_80044750(0x50D);
            func_80042908(2);
            break;
        case 13:
            if (get_icon_can_use((s32) D_800E7313) != 0) {
                if (D_800E739B == 0) {
                    func_8005C4CC(0);
                    func_80044750(0x50D);
                    func_80042908(3);
                }
            }
            break;
        case 11:
            func_80044750(0x50D);
            func_80042908(4);
            break;
        case 14:
            if (get_icon_can_use((s32) D_800E7313) != 0) {
                D_800E71E8 = (D_800E71E8 & 0xFFF0) | 0xE;
                temp_v0_2 = *(&D_800E62E5 + D_800E62C0);
                if (temp_v0_2 == 5) {
                    func_80044750(0x500);
                    main_state_set(0x30);
                    sndisp((s32) "明日からテストだ）", 0, 0x1F);
                } else if (temp_v0_2 == 0xB) {
                    func_80044750(0x500);
                    main_state_set(0x30);
                    sndisp((s32) "明日から修学旅行だ）", 0, 0x1F);
                } else {
                    main_state_set(0x30);
                    D_800E71E8 = (D_800E71E8 & 0xFFF0) | 7;
                }
            }
            break;
        }
    }
    return sp28;
}
