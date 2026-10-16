s32 func_8013394C(void) {
    OptActor44 *temp_a0;
    OptActor44 *temp_a1;
    OptActor44 *var_v0;
    s32 var_v1;

    ((OptActor44 *)D_8011ECD0)[106].unk_03 = 0x80;
    menu_check(1, ((OptActor44 *)D_8011ECD0)->unk_26, ((OptActor44 *)D_8011ECD0)->unk_2A);
    func_8004FC10(1);
    func_801320C0();
    var_v0 = &((OptActor44 *)D_8011ECD0)[96];
    temp_a1 = &((OptActor44 *)D_8011ECD0)[99];
    temp_a0 = &var_v0[D_800E6280.unk_1094];
    var_v1 = 0;
    do {
        if (var_v0 == temp_a0) {
            *(s16 *)((unsigned char *)var_v0 + (772)) = (s16) (var_v1 + 0xD);
        } else {
            *(s16 *)((unsigned char *)var_v0 + (772)) = (s16) (var_v1 + 0xE);
        }
        var_v0 += 1;
        var_v1 += 2;
    } while (var_v0 != temp_a1);
    if (D_800E6280.unk_F88 & 0x20) {
        var_v0 = (OptActor44 *) D_800E6280.unk_1094;
        switch (D_800E6280.unk_1094) {
        case 0:
            k_speed_set(0, temp_a1, D_800E6280.unk_1094);
            D_800E6280.unk_1124 |= 0x80;
            func_8004284C();
            var_v0 = func_80044750(0x501);
            break;
        case 1:
            k_speed_set(1, temp_a1, D_800E6280.unk_1094);
            D_800E6280.unk_1124 |= 0x80;
            func_8004284C();
            var_v0 = func_80044750(0x501);
            break;
        case 2:
            k_speed_set(3, temp_a1, D_800E6280.unk_1094);
            D_800E6280.unk_1124 |= 0x80;
            func_8004284C();
            var_v0 = func_80044750(0x501);
            break;
        }
    }
    if (D_800E6280.unk_F88 & 0x40) {
        var_v0 = func_8004284C();
        D_800E6280.unk_110D = 2;
    }
    return (s32) var_v0;
}
