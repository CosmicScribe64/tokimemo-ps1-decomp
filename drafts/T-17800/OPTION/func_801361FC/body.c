s32 func_801361FC(void) {
    s32 var_v1;
    u8 *var_v0;

    D_801208FB = 0x80;
    menu_check(1, D_8011ECF6, D_8011ECFA);
    func_8004FC10(1);
    func_801320C0();
    var_v0 = D_80120650;
    var_v1 = 0;
    do {
        if (var_v0 == ((D_800E6280.unk_1094 * 0x44) + D_80120650)) {
            *(s16 *)((unsigned char *)var_v0 + (772)) = (s16) (var_v1 + 0xB);
        } else {
            *(s16 *)((unsigned char *)var_v0 + (772)) = (s16) (var_v1 + 0xC);
        }
        var_v0 += 0x44;
        var_v1 += 2;
    } while (var_v0 != &D_801207A4);
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
