void func_80158C4C(void) {
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v1;
    s32 temp_v1_2;

    switch (D_8015EDD8) {
    case 0:
        if (func_8013E9A8(0xA) == 0) {
            if (D_8015ED98 == 0) {
                func_8014EF3C();
                return;
            }
            D_8015EDD8 = 2;
            return;
        }
        D_8015EDD8 = 2;
        return;
    case 1:
        func_80158B70();
        return;
    case 2:
        func_80157AE0();
        return;
    case 3:
        func_8014F080(0x3C);
        return;
    case 4:
        if (D_8015ED98 == 0) {
            D_8015ED98 |= 0x10000;
            D_8015ED9C = 0;
        }
        ((DisplayActor44 *)D_8011ECD0)[154].unk_38 = 0x51000000;
        ((DisplayActor44 *)D_8011ECD0)[154].unk_01 = 4;
        ((DisplayActor44 *)D_8011ECD0)[125].unk_01 = 4;
        func_8013F15C(0x50C, 1, 0);
        func_8014EF3C();
        return;
    case 5:
        temp_v0 = D_8015EC74 * 4;
        if (temp_v0 < 0x40) {
            func_8013E97C(0x3A, temp_v0 + 0x60, 0x50);
            func_8013E810(0x3A, 4, 1, 1);
        } else {
            func_8013E97C(0x3A, temp_v0 + 0x60, 0x38);
            func_8013E810(0x3A, 5, 1, 1);
        }
        temp_v1 = D_8015EC74 + 1;
        if (((s32) D_8015EC74 % 4) < 2) {
            ((DisplayActor44 *)D_8011ECD0)[154].unk_43 = 0x13;
        } else {
            ((DisplayActor44 *)D_8011ECD0)[154].unk_43 = 0x12;
        }
        D_8015EC74 = temp_v1;
        if (((temp_v1 * 4) + 0x60) >= 0x109) {
            func_8013E97C(0x3A, 0, -8);
            func_8013E7C0(0x3A, 5, 1, 1);
            func_8013F15C(0x50D, 1, 0);
            func_8014EF3C();
            return;
        }
    default:
        return;
    case 6:
        temp_v1_2 = D_8015EC74 + 1;
        if (((s32) D_8015EC74 % 4) < 2) {
            ((DisplayActor44 *)D_8011ECD0)[154].unk_43 = 0x13;
        } else {
            ((DisplayActor44 *)D_8011ECD0)[154].unk_43 = 0x12;
        }
        D_8015EC74 = temp_v1_2;
        temp_v0_2 = (temp_v1_2 * 4) + 0x60;
        if ((temp_v0_2 < 0x200) && (D_8015ED98 == 0x02000000)) {
            D_8015ED98 = 0x100;
        }
        if (temp_v0_2 >= 0x281) {
            if (D_8015ED98 != 0x100) {
                func_8013E7C0(0x30, 5, 1, 1);
            }
            func_8014EF3C();
            return;
        }
        break;
    case 7:
        func_8013E810(0x3A, 0xFF, 1, 0);
        ((DisplayActor44 *)D_8011ECD0)[154].unk_38 = 0x41000000;
        ((DisplayActor44 *)D_8011ECD0)[154].unk_43 = 0x12;
        func_8014EF3C();
        return;
    case 8:
        func_801447E0();
        func_8014D11C((s32) D_8015E828, (s32) D_800E6280.unk_0D4, (s32) "は", D_8015EBB0, (s32) "のダメージを受けた");
        func_8014B738((s32) D_8015E8F0, 1, 0x3C);
        func_8014EF3C();
        return;
    case 9:
        func_8014F178(1);
        return;
    case 10:
        func_8013EA60(0, 0x21, D_8015EBB0, 1);
        if (D_8015ED94 == 0) {
            D_8015ED98 = 0x1000;
            D_8015EDD8 = 0;
            D_8015EDC4 = (D_8015EDC4 & 0xFFFF0000) | 5;
            func_8013E7C0(0x30, 7, 1, 1);
        }
        func_8014EF3C();
        return;
    case 11:
        func_8014F178(0);
        return;
    case 12:
        func_8014F080(0x14);
        return;
    case 13:
        func_8014EF8C();
        break;
    }
}
