void func_80138324(void) {
    s32 sp30;
    DisplayActor44 *temp_v1;
    DisplayActor44 *temp_v1_2;
    s32 temp_v1_3;

    switch (D_8015EDD8) {
    case 0:
        switch (D_8015EBAC) {
        case 0:
            func_800AE0F0(D_8015E828, "不良Ａの攻撃");
            D_8015EC78 = func_8013E9A8(2) + D_8015EBAC + 1;
            break;
        case 1:
            func_800AE0F0(D_8015E828, "不良Ｂの攻撃");
            D_8015EC78 = ((func_8013E9A8(2) * 2) + D_8015EBAC) - 1;
            break;
        case 2:
            func_800AE0F0(D_8015E828, "不良Ｃの攻撃");
            D_8015EC78 = (D_8015EBAC - func_8013E9A8(2)) - 1;
            break;
        }
        func_800AE0F0(D_8015E850, "不良パンチ");
        switch (D_8015EC78) {
        case 0:
            func_8014D11C((s32) D_8015E878, (s32) "不良Ａ", (s32) "は", D_8015EBB0, (s32) "のダメージを受けた");
            break;
        case 1:
            func_8014D11C((s32) D_8015E878, (s32) "不良Ｂ", (s32) "は", D_8015EBB0, (s32) "のダメージを受けた");
            break;
        case 2:
            func_8014D11C((s32) D_8015E878, (s32) "不良Ｃ", (s32) "は", D_8015EBB0, (s32) "のダメージを受けた");
            break;
        }
        func_8014B738((s32) D_8015E8F0, 3, 0x3C);
        func_8013E810(D_8015EBAC + 0x3B, 1, 5, 1);
        func_8013F15C(0x505, 1, 0);
        temp_v1 = &((DisplayActor44 *)D_8011ECD0)[D_8015EBAC];
        temp_v1[155].unk_03 = (u8) (temp_v1[155].unk_03 | 1);
        func_8014EF3C();
        return;
    case 1:
        func_8014F0F8(D_8015EBAC + 0x3B);
        return;
    case 2:
        func_8013E810(D_8015EBAC + 0x3B, 0, 1, 1);
        temp_v1_2 = &((DisplayActor44 *)D_8011ECD0)[D_8015EBAC];
        temp_v1_2[155].unk_03 = (u8) (temp_v1_2[155].unk_03 | 1);
        func_8014EF3C();
        return;
    case 3:
        func_8014F178(1);
        return;
    case 4:
        func_8013EEAC(D_8015EC78);
        func_8013E97C(0x38, D_8015EBFC.unk_2C, D_8015EBFC.unk_30);
        ((DisplayActor44 *)D_8011ECD0)[152].unk_01 = 3;
        func_8013E810(0x38, 0xC, 5, 1);
        D_8015EC74 = func_8013E9A8(4) + 1;
        sp30 = func_8013E9A8(0x14);
        func_8013E97C(0x38, (sp30 + D_8015EBFC.unk_2C) - 0xA, (func_8013E9A8(0x14) + D_8015EBFC.unk_30) - 0xA);
        func_8013F15C(0x506, 1, 0);
        func_8014EF3C();
        return;
    case 5:
        if (func_8013E90C(0x38) != 0) {
            func_8013F15C(0x506, 1, 0);
            sp30 = func_8013E9A8(0x14);
            func_8013E97C(0x38, (sp30 + D_8015EBFC.unk_2C) - 0xA, (func_8013E9A8(0x14) + D_8015EBFC.unk_30) - 0xA);
            func_8013E810(0x38, 0xC, 5, 1);
            temp_v1_3 = D_8015EC74 - 1;
            D_8015EC74 = temp_v1_3;
            if (temp_v1_3 == 0) {
                func_8014EF3C();
                return;
            }
        }
    default:
        return;
    case 6:
        if (func_8013E90C(0x38) != 0) {
            func_8013E810(0x38, 0xFF, 5, 0);
            func_8014EF3C();
            return;
        }
        break;
    case 7:
        func_8014F178(2);
        return;
    case 8:
        func_8013EA60(0, D_8015EC78, D_8015EBB0, 1);
        func_8014EF3C();
        return;
    case 9:
        func_8014F178(0);
        return;
    case 10:
        func_8014F080(0x14);
        return;
    case 11:
        func_8014EF8C();
        break;
    }
}
