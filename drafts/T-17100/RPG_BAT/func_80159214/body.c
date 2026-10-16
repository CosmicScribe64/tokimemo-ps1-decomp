void func_80159214(void) {
    s16 temp_v0;
    s32 temp_t1;
    s32 temp_t1_2;
    s32 temp_t1_3;
    s32 var_t0;
    s32 var_v1;

    switch (D_8015EDD8) {
    case 0:
        func_80157CCC();
    default:
        break;
    case 1:
        func_8013E7C0(0x3B, 9, 5, 1);
        func_8013F15C(0x50A, 1, 0);
        func_8014EF3C();
        break;
    case 2:
        temp_t1 = D_8015EDE0;
        if (temp_t1 == 0x6E) {
            func_8013E97C(0x3A, 0, 0);
            ((DisplayActor44 *)D_8011ECD0)[154].unk_1A = 0x100;
            *(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (10500)) = 0x100;
            ((DisplayActor44 *)D_8011ECD0)[154].unk_01 = 4;
            func_8013E7C0(0x3A, 0xB, 1, 1);
            func_8013F15C(0x50B, 1, 0);
            func_8014EF3C();
        }
        D_8015EDE0 = temp_t1 + 1;
        break;
    case 3:
        temp_v0 = ((s32) (D_8015EDE0 << 0xD) / 256) + 0x100;
        ((DisplayActor44 *)D_8011ECD0)[154].unk_1A = temp_v0;
        *(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (10500)) = temp_v0;
        temp_t1_2 = D_8015EDE0 + 3;
        if (temp_t1_2 < 0x100) {
            D_8015EDE0 = temp_t1_2;
            var_t0 = (temp_t1_2 / 2) * 0x10201;
        } else {
            D_8015EDE0 = temp_t1_2;
            var_t0 = 0x7FFE7F;
        }
        func_80049A40(-0xA0, -0x78, 0x140, 0xA8, 2, var_t0, 0x81);
        dtd_on_tpage(0, 0, 2, 1, 0);
        if (D_8015EDE0 >= 0x1F5) {
            ((DisplayActor44 *)D_8011ECD0)[154].unk_1A = 0x1000;
            *(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (10500)) = 0x1000;
            ((DisplayActor44 *)D_8011ECD0)[154].unk_01 = 5;
            func_8013E7C0(0x3A, 0xFF, 1, 0);
            func_8013E7C0(0x3B, 8, 1, 1);
            var_v1 = D_8015ED98;
            if (var_v1 == 0x02000000) {
                var_v1 = 0x100;
            }
            D_8015ED98 = var_v1;
            if (var_v1 != 0x100) {
                func_8013E7C0(0x30, 5, 1, 1);
            }
            func_8014EF3C();
        }
        break;
    case 4:
        func_80049A40(-0xA0, -0x78, 0x140, 0xA8, 2, (0x7F - D_8015EDE0) * 0x10201, 0x81);
        dtd_on_tpage(0, 0, 2, 1, 0);
        temp_t1_3 = D_8015EDE0 + 7;
        D_8015EDE0 = temp_t1_3;
        if (temp_t1_3 >= 0x80) {
            func_8014EF3C();
        }
        break;
    case 5:
        func_801447E0();
        func_8014D11C((s32) D_8015E828, (s32) D_800E6280.unk_0D4, (s32) "は", D_8015EBB0, (s32) "のダメージを受けた");
        func_8014B738((s32) D_8015E8F0, 1, 0x3C);
        func_8014EF3C();
        break;
    case 6:
        func_8014F178(1);
        break;
    case 7:
        func_8013EA60(0, 0x21, D_8015EBB0, 1);
        if (D_8015ED94 == 0) {
            D_8015EDD8 = 0;
            D_8015EDC4 = (D_8015EDC4 & 0xFFFF0000) | 5;
            D_8015ED98 = 0x1000;
            func_8013E7C0(0x30, 7, 1, 1);
        }
        func_8014EF3C();
        break;
    case 8:
        func_8014F178(0);
        break;
    case 9:
        func_8014F080(0x14);
        break;
    case 10:
        func_8014EF8C();
        break;
    }
}
