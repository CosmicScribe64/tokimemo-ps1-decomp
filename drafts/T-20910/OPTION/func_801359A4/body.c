void func_801359A4(void) {
    GameState *g = &D_800E6280;
    DisplayActor44 *var_s6;
    s16 var_s7;
    s32 var_s0;
    s32 var_s1;
    s32 var_s2;
    s32 var_s3;
    s32 var_s5;
    u8 *var_s4;

    ((DisplayActor44 *)D_8011ECD0)[106].unk_03 = 0x80;
    menu_check(1, ((DisplayActor44 *)D_8011ECD0)->unk_26, ((DisplayActor44 *)D_8011ECD0)->unk_2A);
    func_8004FC10(1);
    func_801320C0();
    var_s6 = &((DisplayActor44 *)D_8011ECD0)[96];
    var_s4 = text_list;
    var_s0 = 0;
    var_s1 = 0xC;
    var_s2 = 0xD;
    var_s3 = 0xE;
    var_s5 = 0;
    do {
        if (var_s0 == g->unk_1094) {
            var_s7 = var_s5 + 0x7A;
            k_disp_switch(var_s1, 1);
            k_disp_switch(var_s2, 1);
            k_disp_switch(var_s3, 1);
            *(s8 *)((unsigned char *)var_s4 + (68)) = 0;
        } else {
            var_s7 = var_s5 + 0x7B;
            k_disp_switch(var_s1, 0);
            k_disp_switch(var_s2, 0);
            k_disp_switch(var_s3, 0);
            *(s8 *)((unsigned char *)var_s4 + (68)) = 1;
        }
        var_s6[11].unk_18 = var_s7;
        var_s0 += 1;
        var_s1 += 3;
        var_s2 += 3;
        var_s3 += 3;
        var_s4 += 8;
        var_s5 += 2;
        var_s6 += 1;
    } while (var_s0 != 4);
    func_80049A40(-0x85, 0x3A, 0x100, 0x34, 8, 0x80808, 0x80);
    if (g->unk_F88 & 0x20) {
        switch (g->unk_1094) {
        case 0:
            g->unk0 = 4;
            func_8004284C();
            func_80044750(0x501);
            break;
        case 1:
            g->unk_1092 = 5;
            func_8004284C();
            func_80044750(0x501);
            break;
        case 2:
            g->unk_1092 = 7;
            func_8004284C();
            func_80044750(0x501);
            break;
        case 3:
            g->unk_1092 = 6;
            func_8004284C();
            func_80044750(0x501);
            break;
        }
    }
    if (g->unk_F88 & 0x40) {
        k_sub_reset();
        func_8004284C();
        g->unk_110D = 2;
    }
}
