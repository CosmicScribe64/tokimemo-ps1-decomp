void func_801376A8(void) {
    DisplayActor44 *temp_a0;
    DisplayActor44 *var_v0;
    s32 temp_t3;
    s32 temp_t9;
    s32 var_v1;

    ((DisplayActor44 *)D_8011ECD0)[159].unk_03 = 0x80;
    ((DisplayActor44 *)D_8011ECD0)[159].unk_1A = 0x1000;
    ((DisplayActor44 *)D_8011ECD0)[159].unk_1C = 0x1000;
    menu_check(1, ((DisplayActor44 *)D_8011ECD0)->unk_26, ((DisplayActor44 *)D_8011ECD0)->unk_2A);
    func_8004FC10(1);
    func_801320C0();
    temp_t9 = D_800E7D4F & 1;
    if (temp_t9 == 1) {
        ((DisplayActor44 *)D_8011ECD0)[111].unk_03 = 0x80;
    }
    temp_t3 = D_800E7D50 & 1;
    if (temp_t3 == 1) {
        ((DisplayActor44 *)D_8011ECD0)[112].unk_03 = 0x80;
    }
    var_v0 = &((DisplayActor44 *)D_8011ECD0)[96];
    temp_a0 = &var_v0[D_800E6280.unk0];
    var_v1 = 0;
    do {
        if (var_v0 == temp_a0) {
            var_v0[15].unk_18 = (s16) (var_v1 + 0x3B);
        } else {
            var_v0[15].unk_18 = (s16) (var_v1 + 0x3C);
        }
        var_v0 += 1;
        var_v1 += 2;
    } while (var_v0 != (&((DisplayActor44 *)D_8011ECD0)[98]));
    if (D_800E6280.unk_F88 & 0x20) {
        switch (D_800E6280.unk0) {
        case 0:
            if (temp_t9 == 1) {
                D_800E7D4F &= 0xFFFD;
                func_80044750(0x501);
                func_80048EB8(0);
                func_8004E58C();
                main_state_set(0xD0);
            }
            break;
        case 1:
            if (temp_t3 == 1) {
                D_800E7D50 = (u8) D_800E7D50 & 0xFFFD;
                func_80044750(0x501);
                func_80048EB8(0);
                func_8004E58C();
                main_state_set(0x90);
            }
            break;
        }
    } else if (D_800E6280.unk_F88 & 0x40) {
        func_8004284C();
    }
}
