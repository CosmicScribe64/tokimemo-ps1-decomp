/* T-24521: corrected source-only foundation; not matching or proved equivalent. */
s32 func_80137784(s32 arg0) {
    DisplayFixActor44 *temp_v0;
    DisplayFixActor44 *temp_v1;
    DisplayFixActor44 *temp_v1_2;
    DisplayFixActor44 *temp_v1_3;
    DisplayFixActor44 *var_v0;
    DisplayFixActor44 *var_v0_2;
    Fix32 *temp_a0;
    Fix32 *temp_a0_2;
    s32 *temp_a1;
    s32 temp_a2;
    s32 temp_a3;
    s32 temp_t6;
    s32 temp_t7;
    s32 temp_t8;
    u8 var_t2;
    TaiRunnerA *temp_t1;

    /* FAKE: shared player/cpu[3] index, as in matched func_80138680; T-24521. */
    temp_t1 = &(&D_80149208.player)[arg0];
    var_t2 = temp_t1->unk_02;
    temp_v0 = &((DisplayFixActor44 *)D_8011ECD0)[var_t2];
    if ((temp_v0[46].unk_02 != 0) && ((arg0 != 0) || (D_80122E94 == 0xFF))) {
        temp_v0[46].unk_02 = 0U;
        temp_v0[46].unk_03 = 0;
        if (arg0 == 0) {
            var_v0 = ((DisplayFixActor44 *)D_8011ECD0);
            do {
                var_v0 += 1;
                var_v0[103].unk_02 = 0;
                var_v0[103].unk_03 = 0;
            } while (var_v0 != (((DisplayFixActor44 *)D_8011ECD0) + 3));
            k_reset(0);
            var_t2 = temp_t1->unk_02;
        }
        temp_v1 = &((DisplayFixActor44 *)D_8011ECD0)[arg0];
        temp_v1[107].unk_03 = 0xA4U;
        temp_v1[107].unk_02 = 1;
        temp_a2 = temp_v1[96].unk_24.w;
        temp_v1[107].unk_24.h[1] = (s16) (((((s16) var_t2 % 10) << 5) - D_80149298.halves.high) + 0x1E0);
        temp_a3 = temp_v1[107].unk_24.w;
        temp_v1[107].unk_28.h[1] = (s16) ((((s16) var_t2 / 10) * -0x10) - 0x30);
        temp_t7 = (s32) (temp_a2 - temp_a3) >> 7;
        D_8014982C[arg0] = temp_t7;
        D_8014983C[arg0] = (s32) (temp_v1[96].unk_28.w - temp_v1[107].unk_28.w) >> 7;
        temp_a0 = &D_8014984C[arg0];
        temp_a0->w = temp_a3 - temp_a2;
        if (temp_t7 > 0) {
            temp_v1[107].unk_03 = (u8) (temp_v1[107].unk_03 | 1);
        }
        D_8014981C[arg0] = 0;
        temp_t1->unk_03 = 0;
        temp_t1->unk_04 = 0;
        temp_t1->unk_0C = 0;
        temp_t1->unk_0E = 0;
        temp_t1->unk_10 = 0;
        temp_t1->unk_14 = 0;
        temp_t1->unk_18 = 0;
        temp_t1->unk_1C = 0;
        if (temp_a0 == D_8014984C) {
            D_80149208.player.unk_20.halves.high = -0xA0;
            D_8014929C = 0;
        }
        goto block_26;
    }
    temp_v1_2 = &((DisplayFixActor44 *)D_8011ECD0)[D_801491D8];
    if ((temp_v1_2[46].unk_02 != 0) && (D_80122E94 == 1) && (arg0 == 0)) {
        temp_v1_2[46].unk_02 = 0U;
        temp_v1_2[46].unk_03 = 0;
        var_v0_2 = ((DisplayFixActor44 *)D_8011ECD0);
        do {
            var_v0_2 += 1;
            var_v0_2[103].unk_02 = 0;
            var_v0_2[103].unk_03 = 0;
        } while (var_v0_2 != (((DisplayFixActor44 *)D_8011ECD0) + 3));
        k_reset(0);
        ((DisplayFixActor44 *)D_8011ECD0)[107].unk_16 = (s16) ((DisplayFixActor44 *)D_8011ECD0)[115].unk_16;
        ((DisplayFixActor44 *)D_8011ECD0)[107].unk_43 = (u8) ((DisplayFixActor44 *)D_8011ECD0)[115].unk_43;
        ((DisplayFixActor44 *)D_8011ECD0)[107].unk_18 = 0;
        ((DisplayFixActor44 *)D_8011ECD0)[107].unk_08 = 0;
        ((DisplayFixActor44 *)D_8011ECD0)[107].unk_02 = 1;
        ((DisplayFixActor44 *)D_8011ECD0)[107].unk_03 = 0xA4;
        ((DisplayFixActor44 *)D_8011ECD0)[107].unk_24.h[1] = (s16) (((D_801491D8 << 5) - D_80149298.halves.high) + 0x1E0);
        ((DisplayFixActor44 *)D_8011ECD0)[107].unk_28.h[1] = -0x30;
        temp_t8 = (s32) (((DisplayFixActor44 *)D_8011ECD0)[96].unk_24.w - ((DisplayFixActor44 *)D_8011ECD0)[107].unk_24.w) >> 7;
        D_8014983C[0] = (s32) (((DisplayFixActor44 *)D_8011ECD0)[96].unk_28.w - ((DisplayFixActor44 *)D_8011ECD0)[107].unk_28.w) >> 7;
        D_8014984C[0].w = ((DisplayFixActor44 *)D_8011ECD0)[107].unk_24.w - ((DisplayFixActor44 *)D_8011ECD0)[96].unk_24.w;
        D_8014982C[0] = temp_t8;
        if (temp_t8 > 0) {
            ((DisplayFixActor44 *)D_8011ECD0)[107].unk_03 = 0xA5;
        }
        D_8014981C[0] = 0;
        D_80149208.player.unk_03 = 0;
        D_80149208.player.unk_04 = 0;
        D_80149208.player.unk_0C = 0;
        D_80149208.player.unk_0E = 0;
        D_80149208.player.unk_10 = 0;
        D_80149208.player.unk_14 = 0;
        D_80149208.player.unk_18 = 0;
        D_80149208.player.unk_1C = 0;
        D_80149208.player.unk_20.halves.high = -0xA0;
        D_8014929C = 0;
        goto block_26;
    }
    temp_a0_2 = &D_8014984C[arg0];
    temp_a0_2->w += D_8014982C[arg0];
    temp_v1_3 = &((DisplayFixActor44 *)D_8011ECD0)[arg0];
    temp_v1_3[107].unk_24.h[1] = (s16) (temp_a0_2->h[1] + temp_v1_3[96].unk_24.h[1]);
    temp_a1 = &D_8014981C[arg0];
    temp_v1_3[107].unk_28.w = (s32) (temp_v1_3[107].unk_28.w + D_8014983C[arg0]);
    temp_t6 = *temp_a1 + 1;
    *temp_a1 = temp_t6;
    if (temp_t6 >= 0x80) {
        temp_v1_3[96].unk_18 = (s16) (arg0 + 0x90);
        if ((temp_a1 == D_8014981C) && (D_80122E94 == 1)) {
            ((DisplayFixActor44 *)D_8011ECD0)[107].unk_18 = (s16) ((D_80149320[D_801491DC] * 6) + 5);
        } else {
            temp_v1_3[107].unk_18 = (s16) ((D_801491EC * 6) + 5);
        }
        temp_v1_3[107].unk_24.h[1] = (s16) (temp_v1_3[96].unk_24.h[1] - 8);
        temp_v1_3[107].unk_02 = 0x40;
        temp_v1_3[107].unk_03 = 0xA4U;
        temp_v1_3[107].unk_28.w = (s32) temp_v1_3[96].unk_28.w;
        return 1;
    }
block_26:
    return 0;
}
