/* T-24521: corrected source-only foundation; not matching or proved equivalent. */
s32 func_80137784(s32 arg0) {
    DisplayFixActor44 *actor;
    Fix32 *accumulator;
    s32 *counter;
    s32 source_x;
    s32 effect_x;
    s32 value;
    u8 selection;
    TaiRunnerA *runner;

    /* FAKE: shared player/cpu[3] index, as in matched func_80138680; T-24521. */
    runner = &(&D_80149208.player)[arg0];
    selection = runner->unk_02;
    actor = &((DisplayFixActor44 *)D_8011ECD0)[selection];
    if ((actor[46].unk_02 != 0) && ((arg0 != 0) || (D_80122E94 == 0xFF))) {
        actor[46].unk_02 = 0U;
        actor[46].unk_03 = 0;
        if (arg0 == 0) {
            actor = ((DisplayFixActor44 *)D_8011ECD0);
            do {
                actor += 1;
                actor[103].unk_02 = 0;
                actor[103].unk_03 = 0;
            } while (actor != (((DisplayFixActor44 *)D_8011ECD0) + 3));
            k_reset(0);
            selection = runner->unk_02;
        }
        actor = &((DisplayFixActor44 *)D_8011ECD0)[arg0];
        actor[107].unk_03 = 0xA4U;
        actor[107].unk_02 = 1;
        source_x = actor[96].unk_24.w;
        actor[107].unk_24.h[1] = (s16) (((((s16) selection % 10) << 5) - D_80149298.halves.high) + 0x1E0);
        effect_x = actor[107].unk_24.w;
        actor[107].unk_28.h[1] = (s16) ((((s16) selection / 10) * -0x10) - 0x30);
        value = (s32) (source_x - effect_x) >> 7;
        D_8014982C[arg0] = value;
        D_8014983C[arg0] = (s32) (actor[96].unk_28.w - actor[107].unk_28.w) >> 7;
        accumulator = &D_8014984C[arg0];
        accumulator->w = effect_x - source_x;
        if (value > 0) {
            actor[107].unk_03 = (u8) (actor[107].unk_03 | 1);
        }
        D_8014981C[arg0] = 0;
        runner->unk_03 = 0;
        runner->unk_04 = 0;
        runner->unk_0C = 0;
        runner->unk_0E = 0;
        runner->unk_10 = 0;
        runner->unk_14 = 0;
        runner->unk_18 = 0;
        runner->unk_1C = 0;
        if (accumulator == D_8014984C) {
            D_80149208.player.unk_20.halves.high = -0xA0;
            D_8014929C = 0;
        }
        goto block_26;
    }
    actor = &((DisplayFixActor44 *)D_8011ECD0)[D_801491D8];
    if ((actor[46].unk_02 != 0) && (D_80122E94 == 1) && (arg0 == 0)) {
        actor[46].unk_02 = 0U;
        actor[46].unk_03 = 0;
        actor = ((DisplayFixActor44 *)D_8011ECD0);
        do {
            actor += 1;
            actor[103].unk_02 = 0;
            actor[103].unk_03 = 0;
        } while (actor != (((DisplayFixActor44 *)D_8011ECD0) + 3));
        k_reset(0);
        ((DisplayFixActor44 *)D_8011ECD0)[107].unk_16 = (s16) ((DisplayFixActor44 *)D_8011ECD0)[115].unk_16;
        ((DisplayFixActor44 *)D_8011ECD0)[107].unk_43 = (u8) ((DisplayFixActor44 *)D_8011ECD0)[115].unk_43;
        ((DisplayFixActor44 *)D_8011ECD0)[107].unk_18 = 0;
        ((DisplayFixActor44 *)D_8011ECD0)[107].unk_08 = 0;
        ((DisplayFixActor44 *)D_8011ECD0)[107].unk_02 = 1;
        ((DisplayFixActor44 *)D_8011ECD0)[107].unk_03 = 0xA4;
        ((DisplayFixActor44 *)D_8011ECD0)[107].unk_24.h[1] = (s16) (((D_801491D8 << 5) - D_80149298.halves.high) + 0x1E0);
        ((DisplayFixActor44 *)D_8011ECD0)[107].unk_28.h[1] = -0x30;
        value = (s32) (((DisplayFixActor44 *)D_8011ECD0)[96].unk_24.w - ((DisplayFixActor44 *)D_8011ECD0)[107].unk_24.w) >> 7;
        D_8014983C[0] = (s32) (((DisplayFixActor44 *)D_8011ECD0)[96].unk_28.w - ((DisplayFixActor44 *)D_8011ECD0)[107].unk_28.w) >> 7;
        D_8014984C[0].w = ((DisplayFixActor44 *)D_8011ECD0)[107].unk_24.w - ((DisplayFixActor44 *)D_8011ECD0)[96].unk_24.w;
        D_8014982C[0] = value;
        if (value > 0) {
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
    accumulator = &D_8014984C[arg0];
    accumulator->w += D_8014982C[arg0];
    actor = &((DisplayFixActor44 *)D_8011ECD0)[arg0];
    actor[107].unk_24.h[1] = (s16) (accumulator->h[1] + actor[96].unk_24.h[1]);
    counter = &D_8014981C[arg0];
    actor[107].unk_28.w = (s32) (actor[107].unk_28.w + D_8014983C[arg0]);
    value = *counter + 1;
    *counter = value;
    if (value >= 0x80) {
        actor[96].unk_18 = (s16) (arg0 + 0x90);
        if ((counter == D_8014981C) && (D_80122E94 == 1)) {
            ((DisplayFixActor44 *)D_8011ECD0)[107].unk_18 = (s16) ((D_80149320[D_801491DC] * 6) + 5);
        } else {
            actor[107].unk_18 = (s16) ((D_801491EC * 6) + 5);
        }
        actor[107].unk_24.h[1] = (s16) (actor[96].unk_24.h[1] - 8);
        actor[107].unk_02 = 0x40;
        actor[107].unk_03 = 0xA4U;
        actor[107].unk_28.w = (s32) actor[96].unk_28.w;
        return 1;
    }
block_26:
    return 0;
}
