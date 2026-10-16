void func_80103774(void) {
    EvPairs8 sp68;
    DisplayActor44 *temp_s0;
    s16 var_s2;
    s32 temp_t8;
    u8 *temp_s1;
    u8 *temp_s1_2;
    u8 *temp_s3;
    u8 temp_v0;
    u8 var_v0;

    sp68 = D_80122990;
    var_s2 = 0;
    do {
        temp_s3 = var_s2 + &D_8012274C;
        temp_v0 = *temp_s3;
        temp_s0 = &((DisplayActor44 *)D_800E9620)[var_s2];
        temp_s0[98].unk_07 = (u8) *(u8 *)((unsigned char *)D_800E9620 + (6535));
        if (temp_v0 == 0) {
            temp_s1 = &D_80122774 + var_s2;
            *temp_s1 += 1;
        }
        temp_s1_2 = &D_80122774 + var_s2;
        if ((temp_v0 == 0) && ((u8) *temp_s1_2 >= 0x79U) && ((u32) (0xB4 - *temp_s1_2) < (u32) (func_800789A0() % 60))) {
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
            temp_t8 = *(s32 *)((u8 *)&temp_s0[98] + 48) + 0x4000;
            *(s32 *)((u8 *)&temp_s0[98] + 48) = temp_t8;
            *(s32 *)((u8 *)&temp_s0[98] + 40) = (s32) (*(s32 *)((u8 *)&temp_s0[98] + 40) + temp_t8);
            if ((sp68.v[var_s2][1] - 0x78) < temp_s0[98].unk_2A) {
                temp_s0[98].unk_03 = (u8) (temp_s0[98].unk_03 & 0x7F);
                temp_s0[98].unk_02 = 0;
                temp_s0[98].unk_2A = (s16) (*(&D_80122764 + (var_s2 * 2)) - 0x78);
                *(s32 *)((u8 *)&temp_s0[98] + 40) = 0;
                temp_s0[98].unk_18 = 0;
                temp_s0[98].unk_08 = 0;
                *temp_s3 = 0;
                *(&D_80122774 + var_s2) = 0;
            }
        }
        var_s2 += 1;
    } while (var_s2 < 8);
}
