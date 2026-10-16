s32 func_80137C44(void) {
    DisplayActor44 *temp_a0;
    DisplayActor44 *var_a0;
    TaiRunnersA *var_v1;
    TaiRunnersA *var_v1_2;
    s16 temp_t0;
    s32 temp_a1;
    s32 var_v0;

    var_v1 = &D_80149208;
    var_v0 = 0;
    do {
        if (var_v1->player.unk_00 != 0) {
            temp_a0 = &((DisplayActor44 *)D_8011ECD0)[var_v0];
            *(s16 *)((unsigned char *)temp_a0 + (7314)) = (s16) (*(s16 *)((unsigned char *)temp_a0 + (6566)) - 8);
            if (var_v0 == 0) {
                temp_t0 = D_801492B8[D_80149208.player.unk_04];
                if (D_80122E94 == 1) {
                    *(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (7300)) = (s16) ((((s16) *(s16 *)((unsigned char *)(&((DisplayActor44 *)D_8011ECD0)[D_801491D8]) + (3150)) >> 3) * 6) + temp_t0);
                } else {
                    *(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (7300)) = (s16) ((((s16) *(s16 *)((unsigned char *)(&((DisplayActor44 *)D_8011ECD0)[var_v1->player.unk_02]) + (3150)) >> 3) * 6) + temp_t0);
                }
            } else {
                *(s16 *)((unsigned char *)temp_a0 + (7300)) = (s16) ((((s16) *(s16 *)((unsigned char *)(&((DisplayActor44 *)D_8011ECD0)[var_v1->player.unk_02]) + (3150)) >> 3) * 6) + D_801492B8[var_v1->player.unk_03]);
            }
            if ((var_v0 + 0x90) == *(s16 *)((unsigned char *)temp_a0 + (6552))) {
                if (D_80122E94 == 1) {
                    *(s16 *)((unsigned char *)temp_a0 + (7300)) = (s16) ((D_80149320[D_801491DC] * 6) + 5);
                } else {
                    *(s16 *)((unsigned char *)temp_a0 + (7300)) = (s16) ((((s16) *(s16 *)((unsigned char *)(&((DisplayActor44 *)D_8011ECD0)[var_v1->player.unk_02]) + (3150)) >> 3) * 6) + 5);
                }
            }
        }
        var_v0 += 1;
        var_v1 += 0x24;
    } while (var_v0 < 4);
    var_a0 = ((DisplayActor44 *)D_8011ECD0);
    var_v1_2 = &D_80149208;
    do {
        if ((((var_v1_2->cpu[0].unk_05 == 2) && (*(u8 *)((unsigned char *)var_a0 + (7346)) != 0)) || (var_v1_2->cpu[0].unk_00 == 1)) && (temp_a1 = var_v1_2->cpu[0].unk_20.halves.high - D_8014929A, ((temp_a1 < 0x140) != 0)) && (temp_a1 >= -0x13F)) {
            *(s8 *)((unsigned char *)var_a0 + (7347)) = 0xA4;
        } else {
            *(s8 *)((unsigned char *)var_a0 + (7347)) = 0;
        }
        var_a0 += 1;
        var_v1_2 += 0x24;
    } while (var_a0 != (((DisplayActor44 *)D_8011ECD0) + 0xCC));
    return (s32) D_8014929A;
}
