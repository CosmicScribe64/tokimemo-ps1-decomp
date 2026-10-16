extern s16 D_8012077A;
extern s16 D_8012077C;
void func_8013F82C(void) {
    s32 sp4C;
    s16 temp_t0;
    s16 temp_v1;
    s32 temp_a0;
    s32 temp_s1;
    s32 temp_t9;
    s32 temp_v0;
    s32 temp_v1_2;
    s32 var_s0;
    s32 var_s0_2;
    u8 *var_s2;

    switch (D_8015EDD8) {
    case 0:
        func_800AE0F0(D_8015E828, "ロボ「ガギグ・・・ッ");
        func_8014B738((s32) D_8015E8F0, 1, 0x5A);
        func_8013E54C(4);
        D_8015ED98 = 0x1000;
        func_8013F15C(0x510, 1, 0);
        func_8014EF3C();
        return;
    case 1:
        func_8014F178(1);
        return;
    case 2:
        temp_t9 = D_8015EDE0 + 1;
        D_8015EDE0 = temp_t9;
        D_8015ED14 += 1;
        switch (temp_t9) {
        case 1:
            load_palette(D_8015EEEC, 0x13, 1, 4, 0xA);
            D_8012163F = 0x13;
            break;
        case 10:
            load_palette(D_8015EEEC, 0x13, 1, 4, 9);
            break;
        case 20:
            load_palette(D_8015EEEC, 0x13, 1, 4, 8);
            break;
        case 30:
            load_palette(D_8015EEEC, 0x13, 1, 4, 7);
            break;
        case 40:
            load_palette(D_8015EEEC, 0x13, 1, 4, 6);
            break;
        case 50:
            load_palette(D_8015EEEC, 0x13, 1, 4, 5);
            break;
        }
        var_s2 = D_8011ECD0;
        var_s0 = 0;
        do {
            if ((D_8015ED14 >= 2) && ((&D_8015EC74)[var_s0] == 0) && (func_8013E9A8(5) == 0)) {
                D_8015ED14 = 0;
                temp_s1 = func_8013E9A8(0x40);
                temp_v0 = func_8013E9A8(0x40);
                temp_a0 = var_s0 + 5;
                sp4C = temp_a0;
                func_8013E97C(temp_a0, temp_s1 + 0x28, temp_v0 + 0x40);
                func_8013E810(temp_a0, 0x17, 5, 1);
                if (D_8015EDE4 < 0xB0) {
                    func_8013F15C(0x50D, 2, 1);
                }
            }
            if (!(*(u8 *)((unsigned char *)var_s2 + (6870)) & 1)) {
                func_8013E810(var_s0 + 5, 0xFF, 1, 0);
                *(s32 *)((unsigned char *)(&(&D_8015EBAC)[var_s0]) + (200)) = 0;
            }
            var_s0 += 1;
            var_s2 += 0x44;
        } while (var_s0 < 8);
        if (D_8015EDE0 >= 0x3D) {
            D_8015EDE4 += 2;
            func_8013E97C(4, 0, 0);
            func_8013E7C0(4, 0x18, 1, 1);
            D_80120778 = 5;
            temp_v1 = D_8015EDE4 * 0x10;
            D_8012077A = temp_v1;
            D_8012077C = temp_v1;
            func_80049A40(-0xA0, -0x78, 0x140, 0xA8, 2, ((s32) D_8015EDE4 / 3) * 0x10201, 0x81);
            dtd_on_tpage(0, 0, 2, 1, 0);
        }
        if (D_8015EDE4 == 0xB0) {
            func_8013F15C(0x50C, 1, 0);
        }
        var_s0_2 = 0;
        if (D_8015EDE4 >= 0x151) {
            do {
                func_8013E810(var_s0_2 + 5, 0xFF, 1, 0);
                func_8013E810(0x3B, 0xFF, 1, 0);
                var_s0_2 += 1;
                D_80120778 = 5;
            } while (var_s0_2 != 8);
            k_disp_switch(D_8015EDE8, 0);
            k_disp_switch(D_8015EE40, 0);
            func_8014EF3C();
            return;
        }
    default:
        return;
    case 3:
        D_8015EDE4 += 4;
        func_8013E7C0(4, 0x18, 1, 1);
        D_80120778 = 5;
        temp_v1_2 = 0x150 - D_8015EDE4;
        temp_t0 = temp_v1_2 * 0x10;
        D_8012077A = temp_t0;
        D_8012077C = temp_t0;
        func_80049A40(-0xA0, -0x78, 0x140, 0xA8, 2, (temp_v1_2 / 3) * 0x10201, 0x81);
        dtd_on_tpage(0, 0, 2, 1, 0);
        if (D_8015EDE4 >= 0x149) {
            func_8013E810(4, 0xFF, 1, 0);
            func_8014EF3C();
            return;
        }
        break;
    case 4:
        func_8014F080(0x3C);
        return;
    case 5:
        func_80042808();
        break;
    }
}
