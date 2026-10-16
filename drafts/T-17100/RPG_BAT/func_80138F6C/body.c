void func_80138F6C(void) {
    s32 sp34;
    s32 temp_v0;
    s32 temp_v1;
    s32 temp_v1_2;
    s32 temp_v1_3;

    switch (D_8015EDD8) {
    case 0:
        switch (D_8015EBAC) {
        case 0:
            func_800AE0F0(D_8015E828, "不良Ａの攻撃");
            break;
        case 1:
            func_800AE0F0(D_8015E828, "不良Ｂの攻撃");
            break;
        case 2:
            func_800AE0F0(D_8015E828, "不良Ｃの攻撃");
            break;
        }
        func_800AE0F0(D_8015E850, "不良パンチ");
        func_800AE0F0(D_8015E878, D_800E6280.unk_0D4);
        func_800AE100(D_8015E878, "は");
        func_800AE100(D_8015E878, girl_short_name_table[D_8015EB9C]);
        func_800AE100(D_8015E878, "をかばった");
        func_8014D11C((s32) &D_8015E8A0, (s32) D_800E6280.unk_0D4, (s32) "は", D_8015EBB0, (s32) "のダメージを受けた");
        func_8014B738((s32) D_8015E8F0, 4, 0x3C);
        func_8013E810(D_8015EBAC + 0x3B, 1, 5, 1);
        func_8013F15C(0x505, 1, 0);
        func_8014EF3C();
    default:
        break;
    case 1:
        func_8014F0F8(D_8015EBAC + 0x3B);
        break;
    case 2:
        func_8014F178(1);
        break;
    case 3:
        func_8013E810(D_8015EBAC + 0x3B, 0, 1, 1);
        func_8014EF3C();
        break;
    case 4:
        temp_v1 = D_8015EDE0 + 1;
        D_8015EDE0 = temp_v1;
        func_8013E97C(0x30, (s32) (temp_v1 * -0x18) / 4, (s32) (temp_v1 * 0x30) / 4);
        if (D_8015EDE0 == 4) {
            func_8014EF3C();
        }
        break;
    case 5:
        func_8013E7C0(0x30, 5, 1, 1);
        ((DisplayActor44 *)D_8011ECD0)[152].unk_01 = 3;
        func_8013E7C0(0x38, 0xC, 5, 1);
        D_8015EC74 = func_8013E9A8(4) + 1;
        sp34 = func_8013E9A8(0x14);
        func_8013E97C(0x38, sp34 - 0x1D, func_8013E9A8(0x14) + 0x2B);
        func_8013F15C(0x506, 1, 0);
        func_8014EF3C();
        break;
    case 6:
        if (func_8013E90C(0x38) != 0) {
            func_8013F15C(0x506, 1, 0);
            sp34 = func_8013E9A8(0x14);
            func_8013E97C(0x38, sp34 - 0x1D, func_8013E9A8(0x14) + 0x2B);
            func_8013E7C0(0x38, 0xC, 5, 1);
            temp_v1_2 = D_8015EC74 - 1;
            D_8015EC74 = temp_v1_2;
            if (temp_v1_2 == 0) {
                func_8014EF3C();
            }
        }
        break;
    case 7:
        if (func_8013E90C(0x38) != 0) {
            func_8013E7C0(0x38, 0xFF, 5, 0);
            func_8014EF3C();
        }
        break;
    case 8:
        func_8014F178(3);
        break;
    case 9:
        func_8013EA60(0, 0x20, D_8015EBB0, 1);
        if (D_8015ED94 == 0) {
            func_8014EF8C();
            D_8015EDC4 = 0x100007;
        } else {
            func_8014EF3C();
        }
        break;
    case 10:
        func_8014F178(0);
        break;
    case 11:
        temp_v1_3 = D_8015EDE0 + 1;
        temp_v0 = 4 - temp_v1_3;
        D_8015EDE0 = temp_v1_3;
        func_8013E97C(0x30, (s32) (temp_v0 * -0x18) / 4, (s32) (temp_v0 * 0x30) / 4);
        if (D_8015EDE0 == 4) {
            func_8014EF3C();
        }
        break;
    case 12:
        func_8014F080(0x14);
        break;
    case 13:
        D_8015ED98 &= ~0x1000;
        func_8014EF8C();
        break;
    }
}
