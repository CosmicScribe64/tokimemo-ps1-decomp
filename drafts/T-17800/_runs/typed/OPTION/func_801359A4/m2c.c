? func_8004E93C(s32, ?);                            /* extern */
? func_8004F984(?, s16, s16);                       /* extern */

void func_801359A4(void) {
    OptActor44 *var_s6;
    s16 var_s7;
    s32 var_s0;
    s32 var_s1;
    s32 var_s2;
    s32 var_s3;
    s32 var_s5;
    u8 *var_s4;

    *(s8 *)((unsigned char *)D_8011ECD0 + (7211)) = 0x80;
    func_8004F984(1, D_8011ECD0->unk_26, D_8011ECD0->unk_2A);
    func_8004FC10(1);
    func_801320C0();
    var_s6 = D_8011ECD0 + 0x1980;
    var_s4 = text_list;
    var_s0 = 0;
    var_s1 = 0xC;
    var_s2 = 0xD;
    var_s3 = 0xE;
    var_s5 = 0;
    do {
        if (var_s0 == D_800E6280.unk_1094) {
            var_s7 = var_s5 + 0x7A;
            func_8004E93C(var_s1, 1);
            func_8004E93C(var_s2, 1);
            func_8004E93C(var_s3, 1);
            *(s8 *)((unsigned char *)var_s4 + (68)) = 0;
        } else {
            var_s7 = var_s5 + 0x7B;
            func_8004E93C(var_s1, 0);
            func_8004E93C(var_s2, 0);
            func_8004E93C(var_s3, 0);
            *(s8 *)((unsigned char *)var_s4 + (68)) = 1;
        }
        *(s16 *)((unsigned char *)var_s6 + (772)) = var_s7;
        var_s0 += 1;
        var_s1 += 3;
        var_s2 += 3;
        var_s3 += 3;
        var_s4 += 8;
        var_s5 += 2;
        var_s6 += 0x44;
    } while (var_s0 != 4);
    func_80049A40(-0x85, 0x3A, 0x100, 0x34, 8, 0x80808, 0x80);
    if (D_800E6280.unk_F88 & 0x20) {
        switch (D_800E6280.unk_1094) {              /* irregular */
        case 0:
            D_800E6280.unk0 = 4;
            func_8004284C();
            func_80044750(0x501);
            break;
        case 1:
            D_800E6280.unk_1092 = 5;
            func_8004284C();
            func_80044750(0x501);
            break;
        case 2:
            D_800E6280.unk_1092 = 7;
            func_8004284C();
            func_80044750(0x501);
            break;
        case 3:
            D_800E6280.unk_1092 = 6;
            func_8004284C();
            func_80044750(0x501);
            break;
        }
    }
    if (D_800E6280.unk_F88 & 0x40) {
        func_8004EAAC();
        func_8004284C();
        D_800E6280.unk_110D = 2;
    }
}
