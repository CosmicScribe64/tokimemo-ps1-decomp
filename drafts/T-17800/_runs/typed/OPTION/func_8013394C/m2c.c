? func_8004EBEC(?, OptActor44 *, s8);               /* extern */
? func_8004F984(?, s16, s16);                       /* extern */

s32 func_8013394C(void) {
    OptActor44 *temp_a0;
    OptActor44 *temp_a1;
    OptActor44 *var_v0;
    s32 var_v1;

    *(s8 *)((unsigned char *)D_8011ECD0 + (7211)) = 0x80;
    func_8004F984(1, D_8011ECD0->unk_26, D_8011ECD0->unk_2A);
    func_8004FC10(1);
    func_801320C0();
    var_v0 = D_8011ECD0 + 0x1980;
    temp_a1 = D_8011ECD0 + 0x1A4C;
    temp_a0 = &var_v0[D_800E6280.unk_1094];
    var_v1 = 0;
    do {
        if (var_v0 == temp_a0) {
            *(s16 *)((unsigned char *)var_v0 + (772)) = (s16) (var_v1 + 0xD);
        } else {
            *(s16 *)((unsigned char *)var_v0 + (772)) = (s16) (var_v1 + 0xE);
        }
        var_v0 += 0x44;
        var_v1 += 2;
    } while (var_v0 != temp_a1);
    if (D_800E6280.unk_F88 & 0x20) {
        var_v0 = (OptActor44 *) D_800E6280.unk_1094;
        switch (D_800E6280.unk_1094) {              /* irregular */
        case 0:
            func_8004EBEC(0, temp_a1, D_800E6280.unk_1094);
            D_800E6280.unk_1124 |= 0x80;
            func_8004284C();
            var_v0 = func_80044750(0x501);
            break;
        case 1:
            func_8004EBEC(1, temp_a1, D_800E6280.unk_1094);
            D_800E6280.unk_1124 |= 0x80;
            func_8004284C();
            var_v0 = func_80044750(0x501);
            break;
        case 2:
            func_8004EBEC(3, temp_a1, D_800E6280.unk_1094);
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
