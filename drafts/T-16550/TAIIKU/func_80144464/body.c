s32 func_80144464(void) {
    s32 sp40[1];
    TkS16x4 sp38;
    s32 *var_s0;
    DisplayActor44 *var_v0_2;
    DisplayActor44 *var_v0_4;
    TaiRunnerB *var_v0_3;
    s16 *var_v1;
    s16 temp_t6;
    s32 var_a0;
    s32 var_a0_2;
    s32 var_v0;
    u8 *var_v0_5;

    var_v0_2 = ((DisplayActor44 *)D_8011ECD0);
    sp38 = D_8014A318;
    do {
        var_v0_2 += 1;
        *(s8 *)((unsigned char *)var_v0_2 + (6802)) = 0x40;
        *(s8 *)((unsigned char *)var_v0_2 + (6803)) = 0xA4;
    } while ((u32) var_v0_2 < (u32) (&((DisplayActor44 *)D_8011ECD0)[3]));
    var_v0_3 = D_8014A13C;
    var_a0 = 0;
    do {
        if ((var_v0_3[1].unk_01 == 1) && ((*(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6906)) + 0x30) < (*(s16 *)((unsigned char *)(&((DisplayActor44 *)D_8011ECD0)[var_a0]) + (6634)) + 0x40))) {
            *(u8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6939)) = 0U;
            *(s8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6938)) = 0;
        }
        var_a0 += 1;
        var_v0_3 += 1;
    } while (var_a0 < 3);
    var_v0_4 = ((DisplayActor44 *)D_8011ECD0);
    var_v1 = sp38.v;
    do {
        temp_t6 = *var_v1;
        var_v1 += 2;
        var_v0_4 += 1;
        *(s16 *)((unsigned char *)var_v0_4 + (6838)) = (s16) ((temp_t6 - D_8014A1CC.h[1]) - 0xA0);
    } while ((u32) var_v1 < (u32) &sp40);
    if (*(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6960)) == 0x32) {
        *(u8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6939)) = (u8) (*(u8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6939)) | 0x40);
        *(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6974)) = (s16) (*(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6906)) + 0x20);
    }
    if ((((DisplayActor44 *)D_8011ECD0)->unk0 < -0xDC) && (*(u8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (7075)) != 0)) {
        *(u8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (7075)) = 0U;
    }
    var_v0 = 0;
    if ((((DisplayActor44 *)D_8011ECD0)->unk0 + 0x40) >= *(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6906))) {
        if (*D_8014A120 == 0) {
            *(u8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6939)) = 0xE4U;
            *(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6960)) = 0x32;
            *(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6974)) = (s16) (*(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6906)) + 0x20);
        }
        var_a0_2 = 0;
        if (*D_8014A120 != 0) {
            var_v0_5 = D_8014A120;
            do {
                var_a0_2 += 1;
                var_v0_5 += 1;
            } while (*(u8 *)((unsigned char *)var_v0_5 + (1)) != 0);
        }
        D_8014A120[var_a0_2] = 1;
        if ((var_a0_2 + 1) < 4) {
            var_s0 = &*(s32 *)((unsigned char *)(&D_8014A120[var_a0_2]) + (1));
            do {
                var_s0 += 1;
                *(s8 *)((unsigned char *)var_s0 + (-1)) = func_801446A0();
            } while (var_s0 != &D_8014A124);
        }
        var_v0 = 1;
    }
    return var_v0;
}
