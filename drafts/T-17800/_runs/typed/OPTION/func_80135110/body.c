void func_80135110(void) {
    OptActor44 *temp_a0;
    OptActor44 *var_v0;
    s32 var_v1;
    u32 temp_t9;
    u32 var_v0_2;

    menu_check(1, ((OptActor44 *)D_8011ECD0)->unk_26, ((OptActor44 *)D_8011ECD0)->unk_2A);
    func_8004FC10(1);
    func_801320C0();
    var_v0 = &((OptActor44 *)D_8011ECD0)[96];
    temp_a0 = &var_v0[D_800E6280.unk_1094];
    var_v1 = 0;
    do {
        if (var_v0 == temp_a0) {
            var_v0[18].unk_18 = (s16) (var_v1 + 0x29);
        } else {
            var_v0[18].unk_18 = (s16) (var_v1 + 0x2A);
        }
        var_v0 += 1;
        var_v1 += 2;
    } while (var_v0 != (&((OptActor44 *)D_8011ECD0)[98]));
    if (((D_800E6280.unk_F75 != 0) && (D_800E6280.unk_F77 & 4)) || ((D_800E6280.unk_F75 == 0) && (D_800E6280.unk_F77 & 8))) {
        switch (D_800E6280.unk_1094) {
        case 0:
            D_800E6280.unk_1124 = D_800E6280.unk0 | 0x20;
            D_800E6280.unk_F73 = 0x20;
            func_80044750(0x501);
            func_8004284C();
            break;
        case 1:
            if ((u8) D_8013D180 != 0) {
                D_800E6280.unk_1124 |= 0x20;
                temp_t9 = (0xFF - (u8) D_8013D180) & 0xFF;
                var_v0_2 = temp_t9;
                if (temp_t9 < 0x10U) {
                    var_v0_2 = 0x10;
                }
                D_800E6280.unk_F73 = (s8) var_v0_2;
                if (var_v0_2 >= 0x41U) {
                    D_800E6280.unk_F73 = 0x40;
                }
                func_80044750(0x501);
                func_8004284C();
                D_8013D180 = 0;
            } else {
                D_8013D180 = -1;
            }
            break;
        }
    }
    if (D_800E6280.unk_F88 & 0x40) {
        ((OptActor44 *)D_8011ECD0)[116].unk_03 = (u8) (((OptActor44 *)D_8011ECD0)[116].unk_03 & 0xFF7F);
        func_8004284C();
    }
    func_801352FC();
    if ((u8) D_8013D180 != 0) {
        D_8013D180 = (u8) D_8013D180 - 1;
    }
}
