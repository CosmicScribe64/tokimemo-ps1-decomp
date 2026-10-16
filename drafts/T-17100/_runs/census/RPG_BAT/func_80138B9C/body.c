void func_80138B9C(void) {
    s32 sp34;
    s32 temp_t8;
    s32 temp_v1;

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
        func_8014D11C((s32) D_8015E878, (s32) &D_800E6354, (s32) "は", D_8015EBB0, (s32) "のダメージを受けた");
        func_8014B738((s32) D_8015E8F0, 3, 0x3C);
        func_8013E810(D_8015EBAC + 0x3B, 1, 5, 1);
        func_8013F15C(0x505, 1, 0);
        func_8014EF3C();
        return;
    case 1:
        func_8014F0F8(D_8015EBAC + 0x3B);
        return;
    case 2:
        func_8013E810(D_8015EBAC + 0x3B, 0, 1, 1);
        func_8014EF3C();
        return;
    case 3:
        func_8014F178(1);
        return;
    case 4:
        func_8013E7C0(0x30, 4, 1, 1);
        temp_t8 = D_8015ED98 | 0x1000;
        D_8015ED98 = temp_t8;
        if (temp_t8 & 0x10000) {
            D_8015ED9C = 0;
            D_8015EE50.unk_00 = 4;
            D_8015ED98 = temp_t8 & 0xFFFEFFFF;
        }
        D_80121531 = 3;
        func_8013E7C0(0x38, 0xC, 5, 1);
        D_8015EC74 = func_8013E9A8(4) + 1;
        sp34 = func_8013E9A8(0x14);
        func_8013E97C(0x38, sp34 - 5, func_8013E9A8(0x14) - 5);
        func_8013F15C(0x506, 1, 0);
        func_8014EF3C();
        return;
    case 5:
        if (func_8013E90C(0x38) != 0) {
            func_8013F15C(0x506, 1, 0);
            sp34 = func_8013E9A8(0x14);
            func_8013E97C(0x38, sp34 - 5, func_8013E9A8(0x14) - 5);
            func_8013E7C0(0x38, 0xC, 5, 1);
            temp_v1 = D_8015EC74 - 1;
            D_8015EC74 = temp_v1;
            if (temp_v1 == 0) {
                func_8014EF3C();
                return;
            }
        }
    default:
        return;
    case 6:
        if (func_8013E90C(0x38) != 0) {
            func_8013E7C0(0x38, 0xFF, 5, 0);
            func_8014EF3C();
            return;
        }
        break;
    case 7:
        func_8014F178(2);
        return;
    case 8:
        func_8013EA60(0, 0x21, D_8015EBB0, 1);
        if (D_8015ED94 == 0) {
            func_8014EF8C();
            D_8015EDC4 = 0x100007;
            return;
        }
        func_8014EF3C();
        return;
    case 9:
        func_8014F178(0);
        return;
    case 10:
        func_8014F080(0x14);
        return;
    case 11:
        D_8015ED98 = D_8015ED98 & ~0x1000 & 0xFFFFFF;
        func_8014EF8C();
        break;
    }
}
