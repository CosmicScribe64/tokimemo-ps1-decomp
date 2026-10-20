void func_8013A7EC(void) {
    GameState *g = &D_800E6280;
    s32 var_s2;
    s32 var_s1;
    u8 temp_t9;
    s32 temp_v0;
    s32 var_s0;

    menu_check(0, D_8011ECF6, D_8011ECFA);
    func_8004FC10(0);
    menu_bar_show(0);
    var_s2 = (s32)D_8013E744;
    var_s1 = -0x60;
    do {
        func_8004B338(*(s32 *)var_s2, -0x20, var_s1);
        var_s1 += 0xC;
        var_s2 += 4;
    } while (var_s1 < 0x60);
    func_8004AE28((s32) g->unk_03E, 0x20, -0x60);
    func_8004AE28((s32) g->unk_03F, 0x20, -0x54);
    func_8004AE28((s32) g->unk_040, 0x20, -0x48);
    func_8004AE28((s32) g->unk_041, 0x20, -0x3C);
    func_8004AE28((s32) g->unk_03D, 0x20, -0x30);
    func_8004AE28((s32) g->unk_042, 0x20, -0x24);
    func_8004AE28(g->unk_0F8 & 0xF, 0x20, 0xC);
    func_8004AE28((s32) ((u32) (g->unk_0F8 << 0x17) >> 0x1B), 0x20, 0x18);
    func_8004AE28((s32) ((u32) (g->unk_0F8 << 0x15) >> 0x1E), 0x20, 0x24);
    func_8004AE28((s32) ((u16) g->unk_0F4.h >> 0xC), 0x20, 0x30);
    func_8004AE28(g->unk_1BC[0].unk_10.w & 0xF, 0x20, 0x3C);
    func_8004AE28((s32) ((u32) (g->unk_1BC[0].unk_10.w << 0x17) >> 0x1B), 0x20, 0x48);
    func_8004AE28((s32) ((u32) (g->unk_1BC[0].unk_10.w << 0x15) >> 0x1E), 0x20, 0x54);
    if (g->unk_F80 & 0x90) {
        temp_t9 = (u8) g->unk_1093;
        switch (temp_t9) {
        case 0:
        case 1:
            break;
        case 2:
            if (g->unk_F88 & 0x90) {
                temp_v0 = g->unk_041;
                var_s2 = g->unk_03F;
                var_s0 = 0;
                if (temp_v0 != 6) {
                    var_s0 = temp_v0;
                    if ((s32) temp_v0 < 6) {
                        do {
                            func_80072338();
                            var_s0 += 1;
                        } while (var_s0 != 6 && var_s0 < 6);
                    }
                } else {
                    var_s1 = 7;
                    do {
                        func_80072338();
                        var_s0 += 1;
                    } while (var_s0 != var_s1);
                }
                if (var_s2 != g->unk_03F) {
                    func_80042940(0xFE);
                }
            }
            break;
        case 4:
            g->unk_03D = func_8013A550((s32) g->unk_03D, 3, 0);
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
            g->unk_0F4.b[1] = (func_8013A550((s32) ((u16) g->unk_0F4.h >> 0xC), 0xB, 0) * 0x10) | (g->unk_0F4.b[1] & 0xFF0F);
            break;
        case 13:
            g->unk_1BC[0].unk_10.date.month = func_8013A550(g->unk_1BC[0].unk_10.date.month, 12, 1);
            break;
        case 14:
            g->unk_1BC[0].unk_10.date.day = func_8013A550(g->unk_1BC[0].unk_10.date.day, 31, 1);
            break;
        case 15:
            g->unk_1BC[0].unk_10.date.unk_09 = func_8013A550(g->unk_1BC[0].unk_10.date.unk_09, 3, 0);
            break;
        }
    }
    if (g->unk_F88 & 0x800) {
        func_80041878();
        func_80042940(0);
    }
}
