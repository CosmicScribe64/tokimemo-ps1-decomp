void func_80145CC0(void) {
    switch (D_8015EDD8) {
    case 0:
        func_800AE0F0(D_8015E828, "　　　　アストラルファング　　　　");
        func_8013E97C(0x38, 0, 0);
        D_80121531 = 4;
        func_8013E7C0(0x38, 1, 1, 1);
        func_8014EF3C();
        break;
    case 1:
        func_8014F080(0x3C);
        break;
    case 2:
        func_8014B738((s32) D_8015E8F0, 1, 0x3C);
        func_8013E7C0(0x38, 0xFF, 1, 0);
        func_8014EF3C();
        break;
    case 3:
        func_8014F080(0x14);
        break;
    case 4:
        func_8013E7C0(0x38, 2, 5, 1);
        func_8013F15C(0x507, 1, 0);
        func_8014EF3C();
        break;
    case 5:
        D_8015EDE0 += 3;
        func_8013E97C(0x38, (s32) (D_8015EDE0 * 0x90) / 100, 0);
        if (D_8015EDE0 >= 0x65) {
            func_8013F15C(0x506, 1, 0);
            func_8013E97C(0x38, 0x90, 0);
            func_8013E7C0(0x38, 3, 1, 1);
            func_8014EF3C();
        }
        break;
    case 6:
        func_8014F080(0xA);
        break;
    case 7:
        func_8013EA60(0, 0x21, D_8015ED94 - 3, 1);
        func_8013E7C0(0x38, 0xFF, 1, 0);
        func_8013E7C0(0x30, 4, 1, 1);
        D_8015ED98 = 0x1000;
        func_8014EF3C();
        break;
    case 8:
        func_8014F080(0x96);
        break;
    case 9:
        func_800AE0F0(D_8015E828, D_800E6280.unk_0D4);
        func_800AE100(D_8015E828, "「だめだ！　やられてしまう！");
        func_8014B738((s32) D_8015E8F0, 1, 0x78);
        func_8014EF3C();
        break;
    case 10:
        func_8014F178(1);
        break;
    case 11:
        func_800AE0F0(D_8015E828, "？？？「ピキィー！");
        func_8014B738((s32) D_8015E8F0, 1, 0x5A);
        func_8014EF3C();
        break;
    case 12:
        func_8014F080(0xA);
        break;
    case 13:
        func_8013F15C(0x50B, 1, 0);
        func_8014EF3C();
        break;
    case 14:
        func_8014F178(1);
        break;
    case 15:
        func_800AE0F0(D_8015E828, D_800E6280.unk_0D4);
        func_800AE100(D_8015E828, "「なっ、なんだ！");
        func_8014B738((s32) D_8015E8F0, 1, 0x5A);
        func_8014EF3C();
        break;
    case 16:
        func_8014F178(1);
        break;
    case 17:
        D_8015EDE0 += 1;
        if (D_8015EDE0 < 0x3D) {

            if (!(D_8015EDE0 & 1)) {
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
    case 18:
        func_800AE0F0(D_8015E828, "正義のマングース参上！");
        func_800AE0F0(D_8015E850, "　　　　　マングース斬鉄剣　　　　　");
        func_8014B738((s32) D_8015E8F0, 2, 0x5A);
        D_8015EE18 ^= 0x10000000;
        func_8014EF3C();
        break;
    case 19:
        func_8014F178(1);
        break;
    case 20:
        func_8013E7C0(0x28, 1, 5, 1);
        func_8013F15C(0x509, 1, 0);
        func_8014EF3C();
        break;
    case 21:
        if (func_8013E90C(0x28) != 0) {
            func_8013E7C0(0x28, 2, 1, 1);
            func_8013F15C(0x50A, 1, 0);
            func_8014EF3C();
        }
        break;
    case 22:
        D_8015EDE0 += 1;
        func_8013E97C(0x28, (-D_8015EDE0) << 3, 0);
        if ((D_8015EDE0 == 0x13) || (D_8015EDE0 == 0x16) || (D_8015EDE0 == 0x19) || (D_8015EDE0 == 0x1C)) {
            func_8013EA30(0xFF);
        } else {
            func_8013EA30(0x80);
        }
        if (D_8015EDE0 >= 0x26) {
            func_8014EF3C();
        }
        break;
    case 23:
        func_8014F080(0x1E);
        break;
    case 24:
        func_8013E97C(0x28, -0xC0, -0xA0);
        func_8013E7C0(0x28, 3, 1, 1);
        func_8013F15C(0x50A, 1, 0);
        func_8014EF3C();
        break;
    case 25:
        D_8015EDE0 += 1;
        func_8013E97C(0x28, -0xC0, (D_8015EDE0 * 8) - 0xA0);
        if ((D_8015EDE0 == 0xD) || (D_8015EDE0 == 0x10) || (D_8015EDE0 == 0x13) || (D_8015EDE0 == 0x16)) {
            func_8013EA30(0xFF);
        } else {
            func_8013EA30(0x80);
        }
        if (D_8015EDE0 >= 0x1F) {
            func_8014EF3C();
        }
        break;
    case 26:
        func_8014F080(0x3C);
        break;
    case 27:
        func_80146578();
        break;
    case 28:
        func_8014F080(0x1E);
        break;
    case 29:
        func_8013E7C0(0x30, 0, 1, 1);
        if (D_8015EB9C != 0xE) {
            func_8013E7C0(0x1D, 0, 1, 1);
        }
        k_disp_switch(D_8015EDE8, 0);
        func_800AE0F0(D_8015E9E0, "戦闘に勝った！！");
        func_800AE0F0(D_8015EA08, "ありがとう、正義のマングース");
        func_800AE0F0(D_8015EA30, "経験値　：８２０ＥＸＰ");
        func_800AE0F0(D_8015EA58, "お金　　：１７５ゴールド");
        func_80152384();
        func_8014EF3C();
        break;
    case 30:
        func_8014F080(0x3C);
        break;
    case 31:
        func_8004500C(1, 0x202);
        func_8014EF3C();
        break;
    case 32:
        func_8014B738((s32) D_8015EB70, D_8015ED14, 0x5A);
        func_8013E7C0(0x30, 9, 1, 1);
        if (D_8015EB9C != 0xE) {
            func_8013E7C0(0x1D, 4, 1, 1);
        }
        func_8014EF3C();
        break;
    case 33:
        func_8014F178(0);
        break;
    case 34:
        func_80042808();
        break;
    default:
        func_8014EF8C();
        break;
    }
}
