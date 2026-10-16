void func_8013A7EC(void) {
    GameState *g = &D_800E6280;
    s32 *var_s1;
    s32 var_s0;
    u8 old;
    u8 k;

    menu_check(0, D_8011ECF6, D_8011ECFA);
    func_8004FC10(0);
    menu_bar_show(0);
    var_s1 = D_8013E744;
    var_s0 = -0x60;
    do {
        func_8004B338(*var_s1, -0x20, var_s0);
        var_s0 += 0xC;
        var_s1++;
    } while (var_s0 < 0x60);
    func_8004AE28(g->unk_03E, 0x20, -0x60);
    func_8004AE28(g->unk_03F, 0x20, -0x54);
    func_8004AE28(g->unk_040, 0x20, -0x48);
    func_8004AE28(g->unk_041, 0x20, -0x3C);
    func_8004AE28(g->unk_03D, 0x20, -0x30);
    func_8004AE28(g->unk_042, 0x20, -0x24);
    func_8004AE28(((OptionDateBits *)&g->unk_0F8)->f0, 0x20, 0xC);
    func_8004AE28(((OptionDateBits *)&g->unk_0F8)->f4, 0x20, 0x18);
    func_8004AE28(((OptionDateBits *)&g->unk_0F8)->f9, 0x20, 0x24);
    func_8004AE28(((OptionPlayerBits *)&g->unk_0F4)->f12, 0x20, 0x30);
    func_8004AE28(((OptionDateBits *)&g->unk_1BC[0].unk_10)->f0, 0x20, 0x3C);
    func_8004AE28(((OptionDateBits *)&g->unk_1BC[0].unk_10)->f4, 0x20, 0x48);
    func_8004AE28(((OptionDateBits *)&g->unk_1BC[0].unk_10)->f9, 0x20, 0x54);
    if (g->unk_F80 & 0x90) {
        switch (g->unk_1093) {
        case 2:
            if (g->unk_F88 & 0x90) {
                old = g->unk_03F;
                if (g->unk_041 != 6) {
                    for (k = g->unk_041; k < 6; k++) {
                        func_80072338();
                    }
                } else {
                    for (var_s0 = 0; var_s0 != 7; var_s0++) {
                        func_80072338();
                    }
                }
                if (old != g->unk_03F) {
                    func_80042940(0xFE);
                }
            }
            break;
        case 4:
            g->unk_03D = func_8013A550(g->unk_03D, 3, 0);
            break;
        case 9:
            ((OptionDateBits *)&g->unk_0F8)->f0 = func_8013A550(((OptionDateBits *)&g->unk_0F8)->f0, 12, 1);
            break;
        case 10:
            ((OptionDateBits *)&g->unk_0F8)->f4 = func_8013A550(((OptionDateBits *)&g->unk_0F8)->f4, 31, 1);
            break;
        case 11:
            ((OptionDateBits *)&g->unk_0F8)->f9 = func_8013A550(((OptionDateBits *)&g->unk_0F8)->f9, 3, 0);
            break;
        case 12:
            ((OptionPlayerBits *)&g->unk_0F4)->f12 = func_8013A550(((OptionPlayerBits *)&g->unk_0F4)->f12, 11, 0);
            break;
        case 13:
            ((OptionDateBits *)&g->unk_1BC[0].unk_10)->f0 =
                func_8013A550(((OptionDateBits *)&g->unk_1BC[0].unk_10)->f0, 12, 1);
            break;
        case 14:
            ((OptionDateBits *)&g->unk_1BC[0].unk_10)->f4 =
                func_8013A550(((OptionDateBits *)&g->unk_1BC[0].unk_10)->f4, 31, 1);
            break;
        case 15:
            ((OptionDateBits *)&g->unk_1BC[0].unk_10)->f9 =
                func_8013A550(((OptionDateBits *)&g->unk_1BC[0].unk_10)->f9, 3, 0);
            break;
        }
    }
    if (g->unk_F88 & 0x800) {
        func_80041878();
        func_80042940(0);
    }
}
