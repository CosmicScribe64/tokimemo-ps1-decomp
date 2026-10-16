? func_8004E93C(s32, ?);                            /* extern */
? func_800AE100(u8 *, ? *);                         /* extern */

void func_80146C30(void) {
    s32 *var_v1;
    s32 temp_t1;
    s32 temp_v1;
    s32 temp_v1_2;
    s32 temp_v1_3;

    switch (D_8015EDD8) {
    case 0:
        func_800AE0F0(D_8015E828, "　　　　　　スタンピート　　　　　　");
        func_8013E97C(0x38, 0, 0);
        func_8013E97C(0x39, 0, 0);
        *(s8 *)((unsigned char *)D_8011ECD0 + (10541)) = 4;
        *(s8 *)((unsigned char *)D_8011ECD0 + (10337)) = 4;
        *(s8 *)((unsigned char *)D_8011ECD0 + (10405)) = 4;
        func_8013E7C0(0x38, 3, 5, 1);
        func_8013E7C0(0x39, 1, 5, 1);
        func_8013F15C(0x508, 1, 2);
        func_8014EF3C();
        break;
    case 1:
        if (!(*(u8 *)((unsigned char *)D_8011ECD0 + (10338)) & 1)) {
            func_8013E7C0(0x38, 0xFF, 1, 0);
        }
        func_8014F080(0x32);
        break;
    case 2:
        func_8014B738((s32) D_8015E8F0, 1, 0x3C);
        func_8013E7C0(0x38, 0xFF, 1, 0);
        func_8013E7C0(0x39, 0xFF, 1, 0);
        func_8013E97C(0x3B, 0, 0);
        func_8013E7C0(0x3B, 2, 1, 1);
        func_8014EF3C();
        break;
    case 3:
        func_8014F178(1);
        break;
    case 4:
        D_8015EDE0 += 6;
        func_8013F15C(0x507, 0, 1);
        func_8013E97C(0x3B, (s32) (D_8015EDE0 * 0x90) / 100, 0);
        if (D_8015EDE0 >= 0x65) {
            func_8013E97C(0x3B, 0x90, 0);
            func_8014EF3C();
        }
        break;
    case 5:
        func_8013E7C0(0x30, 0xA, 1, 1);
        func_8013EA60(0, 0x21, D_8015ED94 - 3, 1);
        D_8015ED98 = 0x1000;
        func_8013F15C(0x505, 1, 0);
        func_8014EF3C();
        break;
    case 6:
        func_8014F080(0x1E);
        break;
    case 7:
        D_8015EDE0 += 6;
        func_8013E7C0(0x3B, 0, 1, 1);
        func_8013E7C0(0x30, 4, 1, 1);
        func_8013E97C(0x3B, 0x90 - ((s32) (D_8015EDE0 * 0x90) / 100), 0);
        if (D_8015EDE0 >= 0x65) {
            func_8013E97C(0x3B, 0, 0);
            func_8014EF3C();
        }
        break;
    case 8:
        func_8014F080(0xA);
        break;
    case 9:
        func_800AE0F0(D_8015E828, D_800E6280.unk_0D4);
        func_800AE100(D_8015E828, "「だめだ！　体が動かない！");
        func_8014B738((s32) D_8015E8F0, 1, 0x78);
        func_8014EF3C();
        break;
    case 10:
        func_8014F178(0);
        break;
    case 11:
        func_801468B0();
        break;
    case 12:
        func_800AE0F0(D_8015E828, D_800E6280.unk_0D4);
        func_800AE100(D_8015E828, "「？！");
        func_8014B738((s32) D_8015E8F0, 1, 0x3C);
        func_8014EF3C();
        break;
    case 13:
        func_8014F178(0);
        break;
    case 14:
        temp_v1 = D_8015EDE0 + 1;
        D_8015EDE0 = temp_v1;
        if (temp_v1 < 0x3D) {
            D_8015EDE0 = temp_v1;
            if (!(temp_v1 & 1)) {
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
            func_8014EF3C();
        }
        break;
    case 15:
        D_8015EE18 ^= 0x10000000;
        func_800AE0F0(D_8015E828, "正義の虚無僧推参！");
        func_8014B738((s32) D_8015E8F0, 1, 0x5A);
        func_8014EF3C();
        break;
    case 16:
        func_8014F178(1);
        break;
    case 17:
        func_8014698C();
        break;
    case 18:
        func_800AE0F0(D_8015E828, "　　　　　現世忘却の呪文　　　　　");
        func_8014B738((s32) D_8015E8F0, 1, 0x5A);
        func_8014EF3C();
        break;
    case 19:
        func_8014F178(1);
        break;
    case 20:
        func_8013E7C0(0x28, 1, 5, 1);
        func_8014EF3C();
        break;
    case 21:
        func_8014F0F8(0x28);
        break;
    case 22:
        *(s8 *)((unsigned char *)D_8011ECD0 + (10541)) = 6;
        func_8013E7C0(0x28, 2, 1, 1);
        D_8015ECEC = func_8013E9A8(0x400);
        D_8015ECF0 = func_8013E9A8(0x400) + 0x400;
        D_8015ECF4 = func_8013E9A8(0x400) + 0x800;
        func_8013E7C0(0x29, func_8013E9A8(2) + 3, 1, 1);
        func_8013E7C0(0x2A, func_8013E9A8(2) + 3, 1, 1);
        func_8013E7C0(0x2B, func_8013E9A8(2) + 3, 1, 1);
        func_8013E97C(0x29, 0x10, (func_800A0070(D_8015ECEC) >> 8) + 0x20);
        func_8013E97C(0x2A, 0x10, (func_800A0070(D_8015ECF0) >> 8) - 0x10);
        func_8013E97C(0x2B, 0x10, (func_800A0070(D_8015ECF4) >> 8) - 0x40);
        func_8013F1C4(0x0C00003A, 0x06000062, 9);
        func_8013F15C(0x509, 1, 3);
        func_8014EF3C();
        break;
    case 23:
        D_8015EDE0 += 1;
        func_8013E97C(0x29, 0x10 - ((s32) D_8015EDE0 / 2), (func_800A0070(D_8015ECEC) >> 8) + 0x20);
        func_8013E97C(0x2A, 0x10 - ((s32) D_8015EDE0 / 2), (func_800A0070(D_8015ECF0) >> 8) - 0x10);
        func_8013E97C(0x2B, 0x10 - ((s32) D_8015EDE0 / 2), (func_800A0070(D_8015ECF4) >> 8) - 0x40);
        var_v1 = &D_8015ECEC;
        do {
            temp_t1 = *var_v1;
            var_v1 += 4;
            *(s32 *)((unsigned char *)var_v1 + (-4)) = (s32) (temp_t1 + 0x28);
        } while (var_v1 != &D_8015ECF8);
        if (D_8015EDE0 >= 0x221) {
            func_8013E7C0(0x28, 0, 1, 1);
            func_8013F15C(0x507, 1, 4);
            func_8014EF3C();
        }
        if ((func_800460EC() == 2) && (D_8015EDE0 >= 0x221)) {
            func_8013E7C0(0x28, 0, 1, 1);
            func_8013F15C(0x507, 1, 4);
            func_8014EF3C();
        }
        break;
    case 24:
        *(s8 *)((unsigned char *)D_8011ECD0 + (10543)) = 0x81;
        temp_v1_2 = D_8015EDE0 + 6;
        D_8015EDE0 = temp_v1_2;
        func_8013E97C(0x3B, -temp_v1_2, 0);
        if (D_8015EDE0 >= 0x89) {
            func_8014EF3C();
        }
        break;
    case 25:
        func_800AE0F0(D_8015E828, "鹿・グレートは去っていった。");
        func_8004E93C(D_8015EDE8, 0);
        func_8014B738((s32) D_8015E8F0, 1, 0x78);
        func_8014EF3C();
        break;
    case 26:
        func_8014F178(1);
        break;
    case 27:
        func_80146AB4();
        break;
    case 28:
        temp_v1_3 = D_8015EDE0 + 1;
        D_8015EDE0 = temp_v1_3;
        if (!(temp_v1_3 & 1)) {
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
    case 29:
        func_8014F080(0x1E);
        break;
    case 30:
        func_800AE0F0(D_8015E9E0, "戦闘に勝った！！");
        func_800AE0F0(D_8015EA08, "ありがとう、正義の虚無僧");
        func_800AE0F0(D_8015EA30, "経験値　：８４１ＥＸＰ");
        func_800AE0F0(D_8015EA58, "お金　　：２０４５ゴールド");
        func_80152384();
        func_8014EF3C();
        break;
    case 31:
        func_8014F080(0x3C);
        break;
    case 32:
        func_8004500C(1, 0x202);
        func_8014EF3C();
        break;
    case 33:
        func_8014B738((s32) D_8015EB70, D_8015ED14, 0x5A);
        func_8013E7C0(0x30, 9, 1, 1);
        if (D_8015EB9C != 0xE) {
            func_8013E7C0(0x1D, 4, 1, 1);
        }
        func_8014EF3C();
        break;
    case 34:
        func_8014F178(0);
        break;
    case 35:
        func_80042808();
        break;
    default:
        func_8014EF8C();
        break;
    }
}
