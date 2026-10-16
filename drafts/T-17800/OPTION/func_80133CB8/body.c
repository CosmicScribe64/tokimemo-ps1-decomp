void func_80133CB8(void) {
    s16 sp6C[4];
    s16 sp64[4];
    s16 sp5C[4];
    s16 sp54[4];
    s16 *var_a2;
    s16 *var_a3;
    OptActor44 *var_v0;
    s16 *var_a0;
    s16 *var_v1;
    s16 temp_t5;
    s16 temp_t6_2;
    s16 temp_t7;
    s16 temp_t7_2;
    s16 temp_t9;
    s16 temp_t9_2;
    s16 var_a1;
    s16 var_t0;
    u32 temp_hi;
    u8 *var_v0_2;
    u8 temp_t6;

    if (D_800E6280.unk_110D == 0) {
        ((OptActor44 *)D_8011ECD0)[106].unk_18 = 2;
        ((OptActor44 *)D_8011ECD0)[106].unk_26 = -0xA0;
        ((OptActor44 *)D_8011ECD0)[106].unk_2A = -0x78;
        ((OptActor44 *)D_8011ECD0)[106].unk_03 = 0;
        ((OptActor44 *)D_8011ECD0)[106].unk_1C = 0x7FFF;
        D_800E6280.unk_110D = 1;
        ((OptActor44 *)D_8011ECD0)[106].unk_1A = 0x7FFF;
    } else {
        var_v1 = sp6C;
        var_a0 = sp64;
        var_a1 = -0x38;
        var_a2 = sp5C;
        var_a3 = sp54;
        var_t0 = 0x16;
        temp_t7 = ((s16) ((OptActor44 *)D_8011ECD0)[106].unk_1A / 17) * 0x10;
        var_v0 = &((OptActor44 *)D_8011ECD0)[96];
        ((OptActor44 *)D_8011ECD0)[106].unk_1C = (s16) (((s16) ((OptActor44 *)D_8011ECD0)->unk0 / 17) * 0x10);
        ((OptActor44 *)D_8011ECD0)[106].unk_1A = temp_t7;
        if (temp_t7 < 0x1001) {
            ((OptActor44 *)D_8011ECD0)[106].unk_1C = 0x1000;
            ((OptActor44 *)D_8011ECD0)[106].unk_1A = 0x1000;
            do {
                temp_t6 = var_v0[11].unk_03;
                *var_v1 = -0x20;
                *var_a0 = var_a1;
                temp_t5 = *var_a0;
                temp_t9 = *var_v1;
                var_v0 += 1;
                var_v0[10].unk_18 = var_t0;
                var_t0 += 2;
                var_a1 += 0x18;
                var_v1 += 1;
                var_a0 += 1;
                var_a2 += 1;
                var_a3 += 1;
                *(s16 *)((unsigned char *)var_a2 + (-2)) = 0x50;
                *(s16 *)((unsigned char *)var_a3 + (-2)) = 0x10;
                var_v0[10].unk_03 = (s8) (temp_t6 | 0x80);
                var_v0[10].unk_2A = temp_t5;
                var_v0[10].unk_26 = temp_t9;
            } while ((u32) var_v0 < (u32) (&((OptActor44 *)D_8011ECD0)[99]));
            menu_set(1, 3, sp6C, sp64, sp5C, sp54);
            temp_hi = (u8) D_800E6280.unk_F6C % 3U;
            switch (temp_hi) {
            default:
            case 0:
                D_800E6280.unk_1094 = 0;
                break;
            case 1:
                D_800E6280.unk_1094 = 1;
                break;
            case 2:
                D_800E6280.unk_1094 = 2;
                break;
            }
            func_8006509C();
            var_v0_2 = D_80125C10;
            do {
                (*(s32 *)var_v0_2) = (s16) ((*(s32 *)var_v0_2) + 0xC8);
                temp_t9_2 = *(s16 *)((unsigned char *)var_v0_2 + (12));
                temp_t7_2 = *(s16 *)((unsigned char *)var_v0_2 + (8));
                temp_t6_2 = *(s16 *)((unsigned char *)var_v0_2 + (4)) + 0xC8;
                var_v0_2 += 0x10;
                *(s16 *)((unsigned char *)var_v0_2 + (-8)) = (s16) (temp_t7_2 + 0xC8);
                *(s16 *)((unsigned char *)var_v0_2 + (-4)) = (s16) (temp_t9_2 + 0xC8);
                *(s16 *)((unsigned char *)var_v0_2 + (-12)) = temp_t6_2;
            } while (var_v0_2 != &D_80125C50);
            func_800654D0((s32) (u8) D_800E6280.unk_F6D);
            func_8004284C();
        }
    }
    func_801324E8();
}
