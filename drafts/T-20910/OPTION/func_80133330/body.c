s32 func_80133330(void) {
    Arrs4x8 sp2C;
    s8 var_v1;

    menu_check(0, ((DisplayActor44 *)D_8011ECD0)->unk_26, ((DisplayActor44 *)D_8011ECD0)->unk_2A);
    func_8004FC10(0);
    var_v1 = D_800E6280.unk_1093;
    if (var_v1 == -1) {
        ((DisplayActor44 *)D_8011ECD0)[97].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)[97].unk_03 & 0xFF7F);
    } else {
        func_801325A0(&sp2C);
        var_v1 = D_800E6280.unk_1093;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)[97].unk_03 | 0x80);
        ((DisplayActor44 *)D_8011ECD0)[97].unk_26 = (s16) (sp2C.a[var_v1] + 0x30);
        ((DisplayActor44 *)D_8011ECD0)[97].unk_2A = (s16) (sp2C.b[var_v1] + 6);
    }
    if (D_800E6280.unk_F88 & 0x20) {
        switch (var_v1) {
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
