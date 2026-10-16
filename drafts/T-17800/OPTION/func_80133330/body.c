s32 func_80133330(void) {
    Arrs4x8 sp2C;
    s8 temp_v1;

    menu_check(0, ((OptActor44 *)D_8011ECD0)->unk_26, ((OptActor44 *)D_8011ECD0)->unk_2A);
    func_8004FC10(0);
    temp_v1 = D_800E6280.unk_1093;
    if (temp_v1 == -1) {
        ((OptActor44 *)D_8011ECD0)[97].unk_03 = (u8) (((OptActor44 *)D_8011ECD0)[97].unk_03 & 0xFF7F);
    } else {
        func_801325A0(&sp2C);
        ((OptActor44 *)D_8011ECD0)[97].unk_03 = (u8) (((OptActor44 *)D_8011ECD0)[97].unk_03 | 0x80);
        ((OptActor44 *)D_8011ECD0)[97].unk_26 = (s16) (sp2C.a[D_800E6280.unk_1093] + 0x30);
        ((OptActor44 *)D_8011ECD0)[97].unk_2A = (s16) (sp2C.b[D_800E6280.unk_1093] + 6);
    }
    if (D_800E6280.unk_F88 & 0x20) {
        switch (temp_v1) {
        case 0:
            func_80044750(0x501);
            func_80042908(1);
            break;
        case 1:
            func_80044750(0x501);
            func_80042908(2);
            break;
        case 2:
            if (D_800E6280.unk_F74 == 0) {
                func_80044750(0x501);
                func_80042908(3);
            }
            break;
        case 3:
            if (D_800E6280.unk_F74 == 1) {
                func_80044750(0x501);
                func_80042908(4);
            }
            break;
        case 4:
            func_80044750(0x501);
            func_80042908(5);
            break;
        case 5:
            func_80044750(0x501);
            func_80042908(6);
            break;
        case 6:
            func_80044750(0x501);
            func_80042908(7);
            break;
        case 7:
            func_80044750(0x501);
            func_80042908(8);
            break;
        }
    }
    if ((D_800B3220 == 1) && ((D_800E6280.unk_F80 & 0xF) == 0xF)) {
        main_state_set(0x21);
    }
    if ((D_800B3220 == 1) && ((D_800E6280.unk_F80 & 0xF0000) == 0xF0000)) {
        main_state_set(0x22);
    }
    if (D_800E6280.unk_F88 & 0x40) {
        D_800E6280.unk_1093 = 7;
    }
}
