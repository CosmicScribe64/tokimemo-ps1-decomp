? func_8004F984(?, s16, s16);                       /* extern */

s32 func_801361FC(void) {
    OptActor44 *temp_a0;
    OptActor44 *var_v0;
    s32 var_v1;

    *(s8 *)((unsigned char *)D_8011ECD0 + (7211)) = 0x80;
    func_8004F984(1, D_8011ECD0->unk_26, D_8011ECD0->unk_2A);
    func_8004FC10(1);
    func_801320C0();
    var_v0 = D_8011ECD0 + 0x1980;
    temp_a0 = &var_v0[D_800E6280.unk_1094];
    var_v1 = 0;
    do {
        if (var_v0 == temp_a0) {
            *(s16 *)((unsigned char *)var_v0 + (772)) = (s16) (var_v1 + 0xB);
        } else {
            *(s16 *)((unsigned char *)var_v0 + (772)) = (s16) (var_v1 + 0xC);
        }
        var_v0 += 0x44;
        var_v1 += 2;
    } while (var_v0 != (D_8011ECD0 + 0x1AD4));
    if (D_800E6280.unk_F88 & 0x20) {
        switch (D_800E6280.unk_1094) {
        case 0:
            D_800E6280.unk_F6F = 1;
            D_800E6280.unk_F74 = 0xFF;
            D_800E6280.unk_1124 |= 8;
            func_8004284C();
            var_v0 = func_80044750(0x501);
            break;
        case 1:
            D_800E6280.unk_F6F = 2;
            D_800E6280.unk_F74 = 0xFF;
            D_800E6280.unk_1124 |= 8;
            func_8004284C();
            var_v0 = func_80044750(0x501);
            break;
        case 2:
            D_800E6280.unk_F6F = 3;
            D_800E6280.unk_F74 = 0xFF;
            D_800E6280.unk_1124 |= 8;
            func_8004284C();
            var_v0 = func_80044750(0x501);
            break;
        case 3:
            D_800E6280.unk_F6F = 4;
            D_800E6280.unk_F74 = 0xFF;
            D_800E6280.unk_1124 |= 8;
            func_8004284C();
            var_v0 = func_80044750(0x501);
            break;
        case 4:
            D_800E6280.unk_F6F = 6;
            D_800E6280.unk_F74 = 0xFF;
            D_800E6280.unk_1124 |= 8;
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
