void func_801560E0(void) {
    DateShorts8 sp98;
    DateShorts8 sp88;
    DatePairs8 sp68;
    DisplayActor44 *temp_s0;
    s16 var_s2;
    s32 temp_t6;
    u8 *temp_s1;
    u8 *temp_s1_2;
    u8 *temp_s3;
    u8 temp_v0;
    u8 var_v0;

    sp98 = D_8015FB1C;
    sp88 = D_8015FB2C;
    sp68 = D_8015FB3C;
    var_s2 = 0;
    do {
        temp_s3 = var_s2 + &D_8015F9A0;
        temp_v0 = *temp_s3;
        temp_s0 = &((DisplayActor44 *)D_8011ECD0)[var_s2];
        temp_s0[98].unk_07 = (u8) *(u8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6535));
        if (temp_v0 == 0) {
            temp_s1 = &D_8015F9A8 + var_s2;
            *temp_s1 += 1;
        }
        temp_s1_2 = &D_8015F9A8 + var_s2;
        if ((temp_v0 == 0) && ((u8) *temp_s1_2 >= 0x79U) && ((u32) (0xB4 - *temp_s1_2) < (u32) (func_800AE0D0() % 60))) {
            temp_s0[98].unk_02 = 5;
            temp_s0[98].unk_03 = (u8) (temp_s0[98].unk_03 | 0x80);
            *temp_s3 = 1;
        }
        var_v0 = *temp_s3;
        if (var_s2 == 0) {
            if ((var_v0 == 1) && (temp_s0[98].unk_18 == 7) && (temp_s0[98].unk_08 == 0)) {
                *(s32 *)((u8 *)&temp_s0[98] + 48) = 0;
                *temp_s3 = 2;
                goto block_16;
            }
        } else if ((var_v0 == 1) && (temp_s0[98].unk_18 == 3) && (temp_s0[98].unk_08 == 7)) {
            *(s32 *)((u8 *)&temp_s0[98] + 48) = 0;
            *temp_s3 = 2;
block_16:
            var_v0 = 2 & 0xFF;
        }
        if (var_v0 == 2) {
            temp_t6 = *(s32 *)((u8 *)&temp_s0[98] + 48) + 0x4000;
            *(s32 *)((u8 *)&temp_s0[98] + 48) = temp_t6;
            *(s32 *)((u8 *)&temp_s0[98] + 40) = (s32) (*(s32 *)((u8 *)&temp_s0[98] + 40) + temp_t6);
            if ((sp68.v[var_s2][1] - 0x78) < temp_s0[98].unk_2A) {
                temp_s0[98].unk_02 = 0;
                temp_s0[98].unk_03 = (u8) (temp_s0[98].unk_03 & 0x7F);
                temp_s0[98].unk_2A = (s16) (sp88.v[var_s2] - 0x78);
                *(s32 *)((u8 *)&temp_s0[98] + 40) = 0;
                temp_s0[98].unk_18 = 0;
                temp_s0[98].unk_08 = 0;
                *temp_s3 = 0;
                *(&D_8015F9A8 + var_s2) = 0;
            }
        }
        var_s2 += 1;
    } while (var_s2 < 8);
}
