s32 func_8011C164(void) {
    DisplayActor44 *temp_s0;
    GsWord *temp_s2;
    s16 temp_s3;
    s16 var_s1;
    s32 temp_lo;
    s32 temp_s4;
    s32 temp_t4;
    s32 temp_v1;
    s32 temp_v1_2;
    s32 var_v0;

    if (D_8012531C == 1) {
        return 0;
    }
    var_s1 = 0;
    if ((D_8012531C != 2) && (((u8) D_800B0896 < 9U) || ((u8) D_800B0896 >= 0xCU))) {
        return 0;
    }
    do {
        temp_s2 = &D_80125868[var_s1];
        temp_s0 = &((DisplayActor44 *)D_800E9620)[var_s1];
        var_v0 = *(s32 *)((u8 *)&temp_s0[98] + 36);
        temp_s0[98].unk_03 = (u8) (temp_s0[98].unk_03 | 0x80);
        *(s32 *)((u8 *)&temp_s0[98] + 40) = (s32) (*(s32 *)((u8 *)&temp_s0[98] + 40) + *(s32 *)((u8 *)&temp_s0[98] + 48));
        temp_t4 = *(s32 *)((u8 *)&temp_s0[98] + 44) + ((s32) (temp_s2->w - var_v0) >> 8);
        *(s32 *)((u8 *)&temp_s0[98] + 44) = temp_t4;
        *(s32 *)((u8 *)&temp_s0[98] + 36) = (s32) (var_v0 + temp_t4);
        if (temp_s0[98].unk_2A >= 0x29) {
            temp_s0[98].unk_2A = -0x78;
            temp_v1 = func_800789A0() % 2;
            temp_lo = 0x1000 / (s32) (((s16) temp_v1 * 2) + 2);
            temp_s3 = (s16) temp_v1;
            temp_s0[98].unk_07 = 0x80;
            temp_s0[98].unk_1A = (s16) temp_lo;
            *(s16 *)((u8 *)&temp_s0[98] + 28) = (s16) temp_lo;
            temp_s0[98].unk_26 = (s16) ((func_800789A0(temp_lo) % 300) - 0x96);
            temp_s2->w = *(s32 *)((u8 *)&temp_s0[98] + 36);
            temp_s4 = func_800789A0();
            var_v0 = func_800789A0();
            temp_v1_2 = temp_s3 + 1;
            temp_s2->h[1] = (s16) temp_s2->h[1] + ((s32) (((((var_v0 % 2) * 2) - 1) * (temp_s4 % 8)) + 8) / temp_v1_2);
            *(s32 *)((u8 *)&temp_s0[98] + 48) = (s32) (0x8000 / temp_v1_2);
        }
        var_s1 += 1;
    } while (var_s1 < 0x14);
    return var_v0;
}
