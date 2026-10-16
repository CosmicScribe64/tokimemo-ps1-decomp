s32 func_801407D0(void) {
    DisplayActor44 *temp_s0;
    GsWord *temp_s2;
    s16 var_s1;
    s32 temp_lo;
    s32 temp_s4;
    s32 temp_t5;
    s32 temp_v1;
    s32 temp_v1_2;
    s32 var_v0;

    if (D_800CA2FC == 1) {
        return 0;
    }
    if ((D_800CA2FC != 2) && (((u8) D_800E6280.unk_03F < 9U) || ((u8) D_800E6280.unk_03F >= 0xCU))) {
        return 0;
    }
    var_s1 = 0;
    do {
        temp_s2 = &D_8015BD40[var_s1];
        temp_s0 = &((DisplayActor44 *)D_8011ECD0)[var_s1];
        var_v0 = *(s32 *)((u8 *)&temp_s0[98] + 36);
        temp_s0[98].unk_03 = (u8) (temp_s0[98].unk_03 | 0x80);
        *(s32 *)((u8 *)&temp_s0[98] + 40) = (s32) (*(s32 *)((u8 *)&temp_s0[98] + 40) + *(s32 *)((u8 *)&temp_s0[98] + 48));
        temp_s0[98].unk_07 = (u8) D_800B593C;
        temp_t5 = *(s32 *)((u8 *)&temp_s0[98] + 44) + ((s32) (temp_s2->w - var_v0) >> 8);
        *(s32 *)((u8 *)&temp_s0[98] + 44) = temp_t5;
        *(s32 *)((u8 *)&temp_s0[98] + 36) = (s32) (var_v0 + temp_t5);
        if (temp_s0[98].unk_2A >= 0x29) {
            temp_s0[98].unk_2A = -0x78;
            temp_v1 = func_800AE0D0() % 2;
            temp_lo = 0x1000 / (s32) (((s16) temp_v1 * 2) + 2);
            temp_s0[98].unk_1A = (s16) temp_lo;
            *(s16 *)((u8 *)&temp_s0[98] + 28) = (s16) temp_lo;
            temp_s0[98].unk_26 = (s16) ((func_800AE0D0() % 300) - 0x96);
            temp_s2->w = *(s32 *)((u8 *)&temp_s0[98] + 36);
            temp_s4 = func_800AE0D0();
            var_v0 = func_800AE0D0();
            temp_v1_2 = (s16) temp_v1 + 1;
            temp_s2->h[1] = (s16) temp_s2->h[1] + ((s32) (((((var_v0 % 2) * 2) - 1) * (temp_s4 % 8)) + 8) / temp_v1_2);
            *(s32 *)((u8 *)&temp_s0[98] + 48) = (s32) ((s32) (((D_800CA2FC != 2) << 0xF) + 0x8000) / temp_v1_2);
        }
        var_s1 += 1;
    } while (var_s1 < 0x14);
    return var_v0;
}
