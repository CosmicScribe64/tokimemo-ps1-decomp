s32 func_80140204(void) {
    DisplayActor44 *temp_s0;
    DisplayActor44 *temp_s0_4;
    DisplayActor44 *temp_s0_5;
    s32 *var_s2;
    s32 temp_s0_2;
    s32 temp_s0_3;
    s32 temp_t3;
    s32 temp_t8;
    s32 temp_t9;
    s32 var_a0;
    s32 var_s0;
    s32 var_s1;
    s32 var_s1_2;
    s32 var_s1_3;

    switch (D_8015EDDC) {
    case 0:
        func_8013E810(0x3B, 0xA, 5, 1);
        func_8013F15C(0x506, 1, 0);
        func_8014EF6C();
    default:
        break;
    case 1:
        func_8014F138(0x3B);
        break;
    case 2:
        ((DisplayActor44 *)D_8011ECD0)[155].unk_02 = 0x40;
        ((DisplayActor44 *)D_8011ECD0)[155].unk_18 = 0x19;
        func_8013E97C(0x39, 0x60, 0x78);
        func_8013E810(0x39, 0xC, 1, 1);
        func_8013E97C(0x38, 0, 0);
        func_8013E7C0(0x38, 9, 5, 1);
        func_8014EF6C();
        break;
    case 3:
        func_8013E97C(0x39, D_8015EDE4 + 0x62, 0x76 - D_8015EDE4);
        D_8015EDE4 += 2;
        if (!(((DisplayActor44 *)D_8011ECD0)[152].unk_02 & 1)) {
            func_8013E7C0(0x38, 0xFF, 1, 0);
        }
        if ((D_8015EDE4 + 0x5A) >= 0x77) {
            func_8013E7C0(0x38, 0xFF, 1, 0);
            func_8013E810(0x39, 0xD, 5, 1);
            ((DisplayActor44 *)D_8011ECD0)[153].unk_01 = 2;
            func_8014EF6C();
        }
        break;
    case 4:
        var_s1 = 0;
        var_s0 = 4;
        do {
            func_8013E97C(var_s0, 0x80, 0x58);
            func_8013E810(var_s0, 0x14, 1, 1);
            var_s1 += 1;
            var_s0 = var_s1 + 4;
        } while (var_s1 != 5);
        func_8014EF6C();
        break;
    case 5:
        temp_t8 = (s32) D_8015EDE4 % 2;
        if (temp_t8 == 0) {
            ((DisplayActor44 *)D_8011ECD0)[101].unk_26 = (s16) ((*(s32 *)D_8011ECD0) - 1);
            ((DisplayActor44 *)D_8011ECD0)[103].unk_26 = (s16) (((DisplayActor44 *)D_8011ECD0)[103].unk_26 - 2);
            ((DisplayActor44 *)D_8011ECD0)[102].unk_2A = (s16) (((DisplayActor44 *)D_8011ECD0)[102].unk_2A + 1);
            ((DisplayActor44 *)D_8011ECD0)[104].unk_2A = (s16) (((DisplayActor44 *)D_8011ECD0)[104].unk_2A + 2);
        }
        if (temp_t8 == 0) {
            ((DisplayActor44 *)D_8011ECD0)[100].unk_26 = (s16) (((DisplayActor44 *)D_8011ECD0)[100].unk_26 + 1);
            ((DisplayActor44 *)D_8011ECD0)[100].unk_2A = (s16) (((DisplayActor44 *)D_8011ECD0)[100].unk_2A - 1);
            temp_s0 = &((DisplayActor44 *)D_8011ECD0)[1];
            temp_s0[101].unk_26 = (s16) (temp_s0[101].unk_26 + 1);
            temp_s0[100].unk_26 = (s16) (temp_s0[100].unk_26 + 1);
            temp_s0[100].unk_2A = (s16) (temp_s0[100].unk_2A - 1);
            temp_s0[102].unk_26 = (s16) (temp_s0[102].unk_26 + 1);
            temp_s0[101].unk_2A = (s16) (temp_s0[101].unk_2A - 1);
            temp_s0[103].unk_2A = (s16) (temp_s0[103].unk_2A - 1);
            temp_s0[102].unk_2A = (s16) (temp_s0[102].unk_2A - 1);
            temp_s0[103].unk_26 = (s16) (temp_s0[103].unk_26 + 1);
        }
        temp_t9 = D_8015EDE4 + 1;
        D_8015EDE4 = temp_t9;
        if (temp_t9 >= 0x21) {
            func_8014EF6C();
        }
        break;
    case 6:
        if (D_8015EDE4 >= 0x1B) {
            func_8013F15C(0x507, 0, 1);
            ((DisplayActor44 *)D_8011ECD0)[100].unk_26 = (s16) (((DisplayActor44 *)D_8011ECD0)[100].unk_26 + 3);
            ((DisplayActor44 *)D_8011ECD0)[100].unk_2A = (s16) (((DisplayActor44 *)D_8011ECD0)[100].unk_2A - 3);
        } else if (!(D_8015EDE4 & 1)) {
            func_801401D0(0);
        }
        if (D_8015EDE4 >= 0xE) {
            func_8013F15C(0x507, 0, 2);
            ((DisplayActor44 *)D_8011ECD0)[101].unk_26 = (s16) (((DisplayActor44 *)D_8011ECD0)[101].unk_26 + 3);
            ((DisplayActor44 *)D_8011ECD0)[101].unk_2A = (s16) (((DisplayActor44 *)D_8011ECD0)[101].unk_2A - 3);
        } else if (!(D_8015EDE4 & 1)) {
            func_801401D0(1);
        }
        if (D_8015EDE4 >= 0xE) {
            ((DisplayActor44 *)D_8011ECD0)[102].unk_26 = (s16) (((DisplayActor44 *)D_8011ECD0)[102].unk_26 + 3);
            ((DisplayActor44 *)D_8011ECD0)[102].unk_2A = (s16) (((DisplayActor44 *)D_8011ECD0)[102].unk_2A - 3);
        } else if (!(D_8015EDE4 & 1)) {
            func_801401D0(2);
        }
        if (D_8015EDE4 > 0) {
            func_8013F15C(0x507, 0, 3);
            ((DisplayActor44 *)D_8011ECD0)[103].unk_26 = (s16) (((DisplayActor44 *)D_8011ECD0)[103].unk_26 + 3);
            ((DisplayActor44 *)D_8011ECD0)[103].unk_2A = (s16) (((DisplayActor44 *)D_8011ECD0)[103].unk_2A - 3);
        }
        if (D_8015EDE4 > 0) {
            ((DisplayActor44 *)D_8011ECD0)[104].unk_26 = (s16) (((DisplayActor44 *)D_8011ECD0)[104].unk_26 + 3);
            ((DisplayActor44 *)D_8011ECD0)[104].unk_2A = (s16) (((DisplayActor44 *)D_8011ECD0)[104].unk_2A - 3);
        }
        temp_t3 = D_8015EDE4 + 1;
        D_8015EDE4 = temp_t3;
        if (temp_t3 >= 0x51) {
            func_8014EF6C();
        }
        break;
    case 7:
        func_8014F0BC(0x1E);
        break;
    case 8:
        D_8015EDE4 += 1;
        D_8015ED14 += 1;
        var_s2 = &D_8015EBAC;
        var_s1_2 = 0;
        do {
            if ((*(s32 *)((unsigned char *)var_s2 + (200)) == 3) && !(D_8015ED98 & 0x100)) {
                func_8013E7C0(0x30, 5, 1, 1);
            }
            if ((D_8015EDE4 < 0x78) && (D_8015ED14 >= 3) && (*(s32 *)((unsigned char *)var_s2 + (200)) == 0) && (func_8013E9A8(5) == 0)) {
                func_8013F15C(0x508, 1, 0);
                D_8015ED14 = 0;
                if (func_8013E9A8(0x64) < 0x1E) {
                    *(s32 *)((unsigned char *)var_s2 + (200)) = 1;
                    temp_s0_2 = var_s1_2 + 4;
                    func_8013E97C(temp_s0_2, func_8013E9A8(0x18) + 0x104, 0);
                    func_8013E810(temp_s0_2, 0x16, 1, 1);
                } else {
                    *(s32 *)((unsigned char *)var_s2 + (200)) = 2;
                    temp_s0_3 = var_s1_2 + 4;
                    func_8013E97C(temp_s0_3, func_8013E9A8(0x40) + 0xF0, 0);
                    func_8013E810(temp_s0_3, 0x16, 1, 1);
                }
            }
            if (*(s32 *)((unsigned char *)var_s2 + (200)) == 1) {
                temp_s0_4 = &((DisplayActor44 *)D_8011ECD0)[var_s1_2];
                temp_s0_4[100].unk_2A = (s16) (temp_s0_4[100].unk_2A + 5);
                if ((func_8013E9A8(4) - 0x22) < temp_s0_4[100].unk_2A) {
                    func_8013E810(var_s1_2 + 4, 0xE, 5, 1);
                    *(s32 *)((unsigned char *)var_s2 + (200)) = 3;
                    func_8013F15C(0x509, 1, 0);
                }
            }
            if (*(s32 *)((unsigned char *)var_s2 + (200)) == 2) {
                temp_s0_5 = &((DisplayActor44 *)D_8011ECD0)[var_s1_2];
                temp_s0_5[100].unk_2A = (s16) (temp_s0_5[100].unk_2A + 5);
                if ((func_8013E9A8(4) + 8) < temp_s0_5[100].unk_2A) {
                    func_8013E810(var_s1_2 + 4, 0xE, 5, 1);
                    *(s32 *)((unsigned char *)var_s2 + (200)) = 3;
                    func_8013F15C(0x509, 1, 0);
                }
            }
            if ((*(s32 *)((unsigned char *)var_s2 + (200)) == 3) && !(D_8011ECD0[var_s1_2].unk_00 & 1)) {
                func_8013E810(var_s1_2 + 4, 0xFF, 1, 0);
                *(s32 *)((unsigned char *)var_s2 + (200)) = 0;
            }
            var_s1_2 += 1;
            var_s2 += 1;
        } while (var_s1_2 != 0xC);
        if (D_8015EDE4 >= 0xDD) {
            func_8013D030();
            func_8014EF6C();
        }
        break;
    case 9:
        func_8013E810(0x3B, 0xB, 5, 1);
        var_s1_3 = 0;
        var_a0 = 4;
        do {
            func_8013E810(var_a0, 0xFF, 1, 0);
            var_s1_3 += 1;
            var_a0 = var_s1_3 + 4;
        } while (var_s1_3 != 0xC);
        func_8014EF6C();
        break;
    case 10:
        func_8014F138(0x3B);
        break;
    case 11:
        func_8014EF3C();
        break;
    }
}
