? func_8004E93C(s32, ?);                            /* extern */
? func_800AE100(u8 *, ? *);                         /* extern */
extern ? D_800E6354;
extern ? D_8011F550;

void func_80133108(void) {
    s32 temp_s1;
    s32 temp_s1_2;
    s32 temp_s1_3;
    s32 temp_s2;
    s32 temp_s2_2;
    s32 temp_s2_3;
    s32 temp_s2_4;
    s32 temp_t1;
    s32 temp_t1_10;
    s32 temp_t1_2;
    s32 temp_t1_3;
    s32 temp_t1_4;
    s32 temp_t1_5;
    s32 temp_t1_6;
    s32 temp_t1_7;
    s32 temp_t1_8;
    s32 temp_t1_9;
    s32 temp_v1;
    s32 temp_v1_2;
    s32 temp_v1_3;
    s32 var_a0;
    s32 var_a0_2;
    s32 var_a0_3;
    s32 var_a0_4;
    s32 var_a0_5;
    s32 var_a0_6;
    s32 var_at;
    s32 var_s0;
    s32 var_s0_2;
    s32 var_s0_3;
    s32 var_s0_4;
    s32 var_s0_5;
    s32 var_s0_6;
    s32 var_s0_7;
    s32 var_s0_8;
    s32 var_s0_9;
    s32 var_s3;
    s32 var_s3_2;
    s32 var_s3_3;
    u8 *var_v0;

    switch (D_8015EDD8) {
    case 0:
        func_800AE0F0(D_8015E828, "　　　　サーモンブーメラン　　　　");
        func_8013E97C(0x38, 0, 0);
        func_8013E7C0(0x38, 1, 5, 1);
        func_8013F15C(0x50A, 1, 0);
        func_8014EF3C();
        break;
    case 1:
        if (func_8013E90C(0x38) != 0) {
            func_8013E7C0(0x38, 1, 1, 0);
            func_8014B738((s32) D_8015E8F0, 1, 0x3C);
            func_8014EF3C();
        }
        break;
    case 2:
        func_8014F178(1);
        break;
    case 3:
        D_80121531 = 4;
        func_8013E97C(0x38, 0, 0);
        func_8013E7C0(0x38, 2, 1, 1);
        func_8013E810(0x3B, 4, 1, 1);
        func_8014EF3C();
        break;
    case 4:
        temp_t1 = D_8015EDE0 + 2;
        D_8015EDE0 = temp_t1;
        func_8013E97C(0x38, (s32) (temp_t1 * 0xE0) / 100, (s32) (temp_t1 * -0x40) / 100);
        if (D_80121548 == 0) {
            func_8013F15C(0x506, 1, 0);
        }
        if (D_8015EDE0 >= 0x65) {
            func_8013E97C(0x38, 0xE0, 0x40);
            func_8014EF3C();
        }
        if (D_8015EDE0 == 0x46) {
            func_8013F15C(0x50B, 1, 6);
            D_8015ED98 = 0x1000;
            func_8013E7C0(0x30, 0xA, 1, 1);
        }
        break;
    case 5:
        temp_t1_2 = D_8015EDE0 + 3;
        D_8015EDE0 = temp_t1_2;
        func_8013E97C(0x38, 0xE0 - ((s32) (temp_t1_2 * 0xE0) / 100), ((s32) (temp_t1_2 << 6) / 100) - 0x40);
        if (D_8015EDE0 < 0x50) {
            D_8015EDE0 = D_8015EDE0;
            if (D_80121548 == 0) {
                func_8013F15C(0x506, 1, 0);
            }
        }
        if (D_8015EDE0 >= 0x65) {
            func_8013E97C(0x38, 0, 0);
            func_8013E7C0(0x38, 2, 1, 0);
            func_8013E810(0x3B, 0, 1, 1);
            func_8014EF3C();
        }
        if (D_8015EDE0 == 0x33) {
            func_8013E7C0(0x30, 4, 1, 1);
            func_8013EA60(0, 0x21, D_8015ED94 - 3, 1);
        }
        break;
    case 6:
        func_800AE0F0(D_8015E828, &D_800E6354);
        func_800AE100(D_8015E828, "「だめだ！　喰われてしまう！");
        func_800AE0F0(D_8015E850, "くそったれめー！");
        func_8014B738((s32) D_8015E8F0, 2, 0x78);
        func_8014EF3C();
        break;
    case 7:
        func_8014F178(0);
        break;
    case 8:
        temp_t1_3 = D_8015EDE0 + 1;
        D_8015EDE0 = temp_t1_3;
        if (temp_t1_3 < 0x3D) {
            D_8015EDE0 = temp_t1_3;
            if (!(temp_t1_3 & 1)) {
                func_8013E7C0(0x30, 0xFF, 1, 0);
                if (D_8015EB9C != 0xE) {
                    func_8013E7C0(0x1D, 0xFF, 1, 0);
                }
            } else {
                func_8013E7C0(0x30, 0xFF, 1, 1);
                if (D_8015EB9C != 0xE) {
                    func_8013E7C0(0x1D, 0xFF, 1, 1);
                }
            }
        }
        func_8013E97C(0x28, 0, 0);
        if (D_8015EDE0 >= 0x3D) {
            D_8015EDE0 = D_8015EDE0;
            if (!(D_8015EDE0 & 1)) {
                func_8013E7C0(0x28, 0, 1, 1);
            } else {
                func_8013E7C0(0x28, 0, 1, 0);
            }
        }
        if (D_8015EDE0 >= 0x78) {
            D_8015EE18 ^= 0x10000000;
            func_800AE0F0(D_8015E828, "正義のマタギ見参！");
            func_800AE0F0(D_8015E850, "名銃「次郎丸」ショット");
            func_800AE0F0(D_8015E878, "「次郎丸」エネルギー充填");
            func_8014B738((s32) D_8015E8F0, 3, 0x78);
            func_8014EF3C();
        }
        break;
    case 9:
        var_v0 = D_8011ECD0;
        do {
            var_v0 += 0x110;
            *(s32 *)((unsigned char *)var_v0 + (4160)) = (s32) D_8015EEAC;
            *(s32 *)((unsigned char *)var_v0 + (4164)) = (s32) D_8015EEC4;
            *(s32 *)((unsigned char *)var_v0 + (4200)) = (s32) D_8015EE94;
            *(s32 *)((unsigned char *)var_v0 + (4204)) = 0x41000000;
            *(s8 *)((unsigned char *)var_v0 + (4215)) = 0x18;
            *(s32 *)((unsigned char *)var_v0 + (4228)) = (s32) D_8015EEAC;
            *(s32 *)((unsigned char *)var_v0 + (4232)) = (s32) D_8015EEC4;
            *(s32 *)((unsigned char *)var_v0 + (4268)) = (s32) D_8015EE94;
            *(s32 *)((unsigned char *)var_v0 + (4272)) = 0x41000000;
            *(s8 *)((unsigned char *)var_v0 + (4283)) = 0x18;
            *(s32 *)((unsigned char *)var_v0 + (4296)) = (s32) D_8015EEAC;
            *(s32 *)((unsigned char *)var_v0 + (4300)) = (s32) D_8015EEC4;
            *(s32 *)((unsigned char *)var_v0 + (4336)) = (s32) D_8015EE94;
            *(s32 *)((unsigned char *)var_v0 + (4340)) = 0x41000000;
            *(s8 *)((unsigned char *)var_v0 + (4351)) = 0x18;
            *(s32 *)((unsigned char *)var_v0 + (4092)) = (s32) D_8015EEAC;
            *(s32 *)((unsigned char *)var_v0 + (4096)) = (s32) D_8015EEC4;
            *(s32 *)((unsigned char *)var_v0 + (4132)) = (s32) D_8015EE94;
            *(s32 *)((unsigned char *)var_v0 + (4136)) = 0x41000000;
            *(s8 *)((unsigned char *)var_v0 + (4147)) = 0x18;
        } while (var_v0 != &D_8011F550);
        func_8014EF3C();
        break;
    case 10:
        func_8014F178(2);
        break;
    case 11:
        func_8013F15C(0x507, 0, 1);
        var_s0 = 0;
        var_s3 = (s32) (D_8015EDE0 << 0xA) / 60;
        do {
            temp_s1 = var_s0 - 0x20;
            func_8013E7C0(temp_s1, ((s32) (D_8015EDE0 * 6) / 60) + 1, 1, 1);
            temp_s2 = func_800A0070(var_s3);
            temp_v1 = 0x3C - D_8015EDE0;
            func_8013E97C(temp_s1, ((s32) (temp_s2 * temp_v1) >> 0xD) - 0x10, (s32) (func_800A0140(var_s3) * temp_v1) >> 0xD);
            var_s0 += 1;
            var_s3 += 0x200;
        } while (var_s0 != 8);
        temp_t1_4 = D_8015EDE0 + 1;
        D_8015EDE0 = temp_t1_4;
        if (temp_t1_4 == 0x3C) {
            func_8014EF3C();
        }
        break;
    case 12:
        var_s0_2 = 0;
        var_a0 = -0x20;
        do {
            func_8013E7C0(var_a0, 0xFF, 1, 0);
            var_s0_2 += 1;
            var_a0 = var_s0_2 - 0x20;
        } while (var_s0_2 != 8);
        func_8014F080(0xF);
        break;
    case 13:
        func_8013F15C(0x50C, 0, 2);
        var_s0_3 = 0;
        var_s3_2 = (s32) (D_8015EDE0 << 0xA) / 45;
        do {
            temp_s1_2 = var_s0_3 - 0x20;
            func_8013E7C0(temp_s1_2, ((s32) (D_8015EDE0 * 6) / 45) + 1, 1, 1);
            temp_s2_2 = func_800A0070(var_s3_2);
            temp_v1_2 = 0x2D - D_8015EDE0;
            func_8013E97C(temp_s1_2, ((s32) (temp_s2_2 * temp_v1_2 * 0x3C) / 368640) - 0x10, (s32) (func_800A0140(var_s3_2) * temp_v1_2 * 0x3C) / 368640);
            var_s0_3 += 1;
            var_s3_2 += 0x200;
        } while (var_s0_3 != 8);
        temp_t1_5 = D_8015EDE0 + 1;
        D_8015EDE0 = temp_t1_5;
        if (temp_t1_5 == 0x2D) {
            func_8014EF3C();
        }
        break;
    case 14:
        var_s0_4 = 0;
        var_a0_2 = -0x20;
        do {
            func_8013E7C0(var_a0_2, 0xFF, 1, 0);
            var_s0_4 += 1;
            var_a0_2 = var_s0_4 - 0x20;
        } while (var_s0_4 != 8);
        func_8014F080(0xA);
        break;
    case 15:
        func_8013F15C(0x50D, 0, 3);
        var_s0_5 = 0;
        var_s3_3 = (s32) (D_8015EDE0 << 0xA) / 30;
        do {
            temp_s1_3 = var_s0_5 - 0x20;
            func_8013E7C0(temp_s1_3, ((s32) (D_8015EDE0 * 6) / 30) + 1, 1, 1);
            temp_s2_3 = func_800A0070(var_s3_3);
            temp_v1_3 = 0x1E - D_8015EDE0;
            func_8013E97C(temp_s1_3, ((s32) (temp_s2_3 * temp_v1_3 * 2) >> 0xD) - 0x10, (s32) (func_800A0140(var_s3_3) * temp_v1_3 * 2) >> 0xD);
            var_s0_5 += 1;
            var_s3_3 += 0x200;
        } while (var_s0_5 != 8);
        temp_t1_6 = D_8015EDE0 + 1;
        D_8015EDE0 = temp_t1_6;
        if (temp_t1_6 == 0x1E) {
            func_8014EF3C();
        }
        break;
    case 16:
        var_s0_6 = 0;
        var_a0_3 = -0x20;
        do {
            func_8013E7C0(var_a0_3, 0xFF, 1, 0);
            var_s0_6 += 1;
            var_a0_3 = var_s0_6 - 0x20;
        } while (var_s0_6 != 8);
        func_8014F080(5);
        break;
    case 17:
        var_s0_7 = 0;
        var_a0_4 = -0x20;
        do {
            func_8013E7C0(var_a0_4, 0xFF, 1, 0);
            var_s0_7 += 1;
            var_a0_4 = var_s0_7 - 0x20;
        } while (var_s0_7 != 8);
        func_8013E7C0(0x28, 7, 5, 1);
        func_8013F15C(0x508, 1, 4);
        func_8014EF3C();
        break;
    case 18:
        func_8014F080(0x14);
        break;
    case 19:
        temp_t1_7 = D_8015EDE0;
        var_at = temp_t1_7 < 0x11;
        if (!(temp_t1_7 & 3)) {
            temp_s2_4 = func_8013E9A8(0x40);
            func_8013E97C(((s32) D_8015EDE0 / 4) - 0x20, temp_s2_4 - 0xE0, func_8013E9A8(0x60) - 0x40);
            func_8013E7C0(((s32) D_8015EDE0 / 4) - 0x20, 8, 5, 1);
            var_at = D_8015EDE0 < 0x11;
        }
        if ((var_at == 0) && (temp_t1_7 < 0x60)) {
            func_8013F15C(0x509, 0xF, 5);
        }
        temp_t1_8 = temp_t1_7 + 1;
        D_8015EDE0 = temp_t1_8;
        if (temp_t1_8 >= 0x81) {
            func_8014EF3C();
        }
        break;
    case 20:
        temp_t1_9 = D_8015EDE0 + 1;
        D_8015EDE0 = temp_t1_9;
        if (!(temp_t1_9 & 1)) {
            func_8013E97C(0x3B, 0, 0);
            func_8013E7C0(0x3B, 0xFF, 1, 1);
            var_s0_8 = 0;
            var_a0_5 = -0x20;
            do {
                func_8013E7C0(var_a0_5, 0xFF, 0x21, 1);
                var_s0_8 += 1;
                var_a0_5 = var_s0_8 - 0x20;
            } while (var_s0_8 != 0x20);
        } else {
            func_8013E97C(0x3B, 0, 0);
            func_8013E7C0(0x3B, 0xFF, 1, 0);
            var_s0_9 = 0;
            var_a0_6 = -0x20;
            do {
                func_8013E7C0(var_a0_6, 0xFF, 0x21, 0);
                var_s0_9 += 1;
                var_a0_6 = var_s0_9 - 0x20;
            } while (var_s0_9 != 0x20);
        }
        if (D_8015EDE0 >= 0x3D) {
            func_8004E93C(D_8015EDE8, 0);
            func_8014EF3C();
        }
        break;
    case 21:
        func_80132FE0();
        break;
    case 22:
        temp_t1_10 = D_8015EDE0 + 1;
        D_8015EDE0 = temp_t1_10;
        if (!(temp_t1_10 & 1)) {
            func_8013E7C0(0x28, 0, 1, 1);
        } else {
            func_8013E7C0(0x28, 0, 1, 0);
        }
        if (D_8015EDE0 >= 0x5B) {
            D_8015EE18 ^= 0x10000000;
            func_8013E7C0(0x30, 0, 1, 1);
            if (D_8015EB9C != 0xE) {
                func_8013E7C0(0x1D, 0, 1, 1);
            }
            func_8014EF3C();
        }
        break;
    case 23:
        func_8014F080(0x1E);
        break;
    case 24:
        func_800AE0F0(D_8015E9E0, "戦闘に勝った！！");
        func_800AE0F0(D_8015EA08, "ありがとう、マタギのおじさん");
        func_800AE0F0(D_8015EA30, "経験値　：１０２４ＥＸＰ");
        func_800AE0F0(D_8015EA58, "お金　　：５３ゴールド");
        func_80152384();
        func_8014EF3C();
        break;
    case 25:
        func_8014F080(0x3C);
        break;
    case 26:
        func_8004500C(1, 0x202);
        func_8014EF3C();
        break;
    case 27:
        func_8014B738((s32) D_8015EB70, D_8015ED14, 0x5A);
        func_8013E7C0(0x30, 9, 1, 1);
        if (D_8015EB9C != 0xE) {
            func_8013E7C0(0x1D, 4, 1, 1);
        }
        func_8014EF3C();
        break;
    case 28:
        func_8014F178(0);
        break;
    case 29:
        func_80042808();
        break;
    default:
        func_8014EF8C();
        break;
    }
}
