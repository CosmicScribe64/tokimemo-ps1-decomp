s32 func_80133718(void) {
    s16 sp74[6];
    s16 sp68[6];
    s16 sp5C[6];
    s16 sp50[6];
    s16 *var_a3;
    s16 *var_t0;
    OptActor44 *var_v0;
    s16 *var_a0;
    s16 *var_v1;
    s16 temp_t6;
    s16 temp_t7;
    s16 temp_t8;
    s16 var_a2;
    s16 var_t1;
    s32 temp_v0;
    u8 temp_t9;

    if (D_800E6280.unk_110D == 0) {
        ((OptActor44 *)D_8011ECD0)[106].unk_18 = 1;
        ((OptActor44 *)D_8011ECD0)[106].unk_26 = -0xA0;
        ((OptActor44 *)D_8011ECD0)[106].unk_2A = -0x78;
        ((OptActor44 *)D_8011ECD0)[106].unk_03 = 0;
        ((OptActor44 *)D_8011ECD0)[106].unk_1C = 0x7FFF;
        D_800E6280.unk_110D = 1;
        ((OptActor44 *)D_8011ECD0)[106].unk_1A = 0x7FFF;
        return;
    }
    var_v1 = sp74;
    var_a0 = sp68;
    var_a2 = -0x58;
    var_a3 = sp5C;
    var_t0 = sp50;
    var_t1 = 0xE;
    temp_t6 = ((s16) ((OptActor44 *)D_8011ECD0)[106].unk_1A / 17) * 0x10;
    ((OptActor44 *)D_8011ECD0)[106].unk_1C = (s16) (((s16) ((OptActor44 *)D_8011ECD0)->unk0 / 17) * 0x10);
    ((OptActor44 *)D_8011ECD0)[106].unk_1A = temp_t6;
    if (temp_t6 < 0x1001) {
        ((OptActor44 *)D_8011ECD0)[106].unk_1C = 0x1000;
        ((OptActor44 *)D_8011ECD0)[106].unk_1A = 0x1000;
        var_v0 = &((OptActor44 *)D_8011ECD0)[96];
        do {
            temp_t9 = var_v0[11].unk_03;
            *var_v1 = 0x10;
            *var_a0 = var_a2;
            temp_t8 = *var_a0;
            temp_t7 = *var_v1;
            var_v0 += 1;
            var_v0[10].unk_18 = var_t1;
            var_t1 += 2;
            var_a2 += 0x28;
            var_v1 += 1;
            var_a0 += 1;
            var_a3 += 1;
            var_t0 += 1;
            *(s16 *)((unsigned char *)var_a3 + (-2)) = 0x28;
            *(s16 *)((unsigned char *)var_t0 + (-2)) = 0x10;
            var_v0[10].unk_03 = (s8) (temp_t9 | 0x80);
            var_v0[10].unk_2A = temp_t8;
            var_v0[10].unk_26 = temp_t7;
        } while (var_v0 != (&((OptActor44 *)D_8011ECD0)[99]));
        menu_set(1, 3, sp74, sp68, sp5C, sp50);
        temp_v0 = get_k_speed();
        switch (temp_v0) {
        case 0:
            D_800E6280.unk0 = 0;
            break;
        case 1:
            D_800E6280.unk_1094 = 1;
            break;
        case 2:
            D_800E6280.unk_1094 = 2;
            break;
        default:
            k_speed_set(0);
            D_800E6280.unk_1094 = 0;
            break;
        }
        func_8004284C();
    }
}
