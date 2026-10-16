s32 func_80137EA8(void) {
    s32 sp40[1];
    TkS16x4 sp38;
    s32 *sp34;
    s32 *var_v1_4;
    DisplayActor44 *var_v0_2;
    DisplayActor44 *var_v0_3;
    TaiRunnersA *var_v1;
    TaiRunnersA *var_v1_2;
    s16 *var_v1_3;
    s16 temp_t4;
    s32 var_a0;
    s32 var_a0_2;
    s32 var_v0;
    u8 *var_v0_4;


    sp38 = D_8014985C;
    if (*(u8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (7550)) == 0) {
        var_v0_2 = ((DisplayActor44 *)D_8011ECD0);
        if (D_8014929A >= 0x781) {
            var_v1 = &D_80149208;
            do {
                *(s8 *)((unsigned char *)var_v0_2 + (7550)) = 0x40;
                *(s8 *)((unsigned char *)var_v0_2 + (7551)) = 0xA4;
                if (var_v1->cpu[0].unk_06 == 1) {
                    *(u8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (7619)) = 0U;
                }
                var_v1 += 0x24;
                var_v0_2 += 1;
            } while (var_v1 != &D_80149208.cpu[2]);
        }
    }
    if (((DisplayActor44 *)D_8011ECD0)->unk0 == 0x8D) {
        *(u8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (7619)) = (u8) (*(u8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (7619)) | 0x40);
    }
    var_v1_2 = &D_80149208;
    var_a0 = 0;
    do {
        if ((var_v1_2->cpu[0].unk_06 == 1) && (*(s16 *)((unsigned char *)(&((DisplayActor44 *)D_8011ECD0)[var_a0]) + (6634)) >= (*(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (7654)) + 0x30))) {
            *(u8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (7619)) = 0U;
            *(s8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (7618)) = 0;
        }
        var_a0 += 1;
        var_v1_2 += 0x24;
    } while (var_a0 < 3);
    var_v0_3 = ((DisplayActor44 *)D_8011ECD0);
    var_v1_3 = sp38.v;
    do {
        temp_t4 = *var_v1_3;
        var_v1_3 += 2;
        var_v0_3 += 1;
        *(s16 *)((unsigned char *)var_v0_3 + (7518)) = (s16) ((temp_t4 - (D_8014929A * 2)) - 0xA0);
    } while ((u32) var_v1_3 < (u32) &sp40);
    if ((*(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (7790)) < -0xDC) && (*(u8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (7755)) != 0)) {
        *(u8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (7755)) = 0U;
    }
    if (((DisplayActor44 *)D_8011ECD0)->unk0 == 0x8D) {
        *(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (7654)) = (s16) (*(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (7586)) + 0x20);
    }
    var_v0 = 0;
    if ((((DisplayActor44 *)D_8011ECD0)->unk0 + 0x40) >= ((DisplayActor44 *)D_8011ECD0)->unk0) {
        if ((*(s32 *)D_801491E0) == 0) {
            *(u8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (7619)) = 0xE4U;
            *(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (7640)) = 0x8D;
            *(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (7654)) = (s16) (*(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (7722)) + 0x20);
        }
        var_a0_2 = 0;
        if ((*(s32 *)D_801491E0) != 0) {
            var_v0_4 = D_801491E0;
            do {
                var_a0_2 += 1;
                var_v0_4 += 1;
            } while (var_v0_4[1] != 0);
        }
        D_801491E0[var_a0_2] = 1;
        if ((var_a0_2 + 1) < 4) {
            var_v1_4 = &*(s32 *)((unsigned char *)(&D_801491E0[var_a0_2]) + (1));
            do {
                sp34 = var_v1_4;
                var_v1_4 += 1;
                *(s8 *)((unsigned char *)var_v1_4 + (-1)) = func_8013815C();
            } while (var_v1_4 != &D_801491E4);
        }
        if (D_80122E94 == 1) {
            D_801491E0[1] = 2;
            D_801491E0[2] = 4;
            D_801491E0[3] = 1;
            (*(s32 *)D_801491E0) = 3;
        }
        var_v0 = 1;
    }
    return var_v0;
}
