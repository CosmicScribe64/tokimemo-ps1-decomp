void func_80147594(void) {
    s32 sp68[16];
    DisplayActor44 *temp_s1;
    TaiikuSlot1C *var_s0;
    TaiikuSlot1C *var_s2;
    s32 temp_s5;
    s32 temp_v1;
    s32 var_s4;
    u16 *var_s3;
    u16 *var_s3_2;
    u16 *var_s3_3;

    sp68[0] = D_8014A614;
    var_s0 = D_8014A454;
    *(s32 *)((unsigned char *)(&sp68[0]) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_8014A614) + (8));
    *(s32 *)((unsigned char *)(&sp68[0]) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_8014A614) + (4));
    do {
        if ((u16) var_s0[1].unk_14 >= 0x65U) {
            temp_v1 = func_800AE0D0() & 0x1F;
            if (temp_v1 < 0xA) {
                var_s0[1].unk_08 = -0x64;
            } else if (temp_v1 < 0x14) {
                var_s0[1].unk_08 = 0x64;
            } else {
                var_s0[1].unk_08 = 0;
            }
            var_s0[1].unk_14 = 0;
        }
        var_s0 += 1;
    } while (var_s0 != &D_8014A454[3]);
    var_s2 = D_8014A454;
    var_s4 = 0;
    do {
        if ((u16) var_s2[1].unk_18 >= 0xBU) {
            var_s3 = D_8014A4D8;
            temp_s1 = &((DisplayActor44 *)D_8011ECD0)[var_s4];
            temp_s5 = (var_s4 * 0x13) + 0x13;
            do {
                if (*(s16 *)((unsigned char *)temp_s1 + (6620)) == (*var_s3 + temp_s5)) {
                    if (((*(s16 *)((unsigned char *)temp_s1 + (6634)) + 0x4F) < *(s16 *)((unsigned char *)temp_s1 + (6906))) && ((func_800AE0D0() & 0xFF) < (&sp68[0])[var_s4])) {
                        var_s2[1].unk_08 -= 0x22;
                    }
                    if ((*(s16 *)((unsigned char *)temp_s1 + (6906)) < (*(s16 *)((unsigned char *)temp_s1 + (6634)) + 0x40)) && ((func_800AE0D0() & 0xFF) < (&sp68[0])[var_s4])) {
                        var_s2[1].unk_08 += 0x22;
                    }
                }
                var_s3 += 2;
            } while ((u32) var_s3 < (u32) D_8014A4E4);
            var_s3_2 = &D_8014A4E4[2];
            do {
                if (*(s16 *)((unsigned char *)temp_s1 + (6620)) == (*var_s3_2 + temp_s5)) {
                    if (((*(s16 *)((unsigned char *)temp_s1 + (6634)) + 0x2D) < *(s16 *)((unsigned char *)temp_s1 + (6906))) && ((func_800AE0D0() & 0xFF) < (&sp68[0])[var_s4])) {
                        var_s2[1].unk_08 -= 0x22;
                    }
                    if ((*(s16 *)((unsigned char *)temp_s1 + (6906)) < (*(s16 *)((unsigned char *)temp_s1 + (6634)) + 0x28)) && ((func_800AE0D0() & 0xFF) < (&sp68[0])[var_s4])) {
                        var_s2[1].unk_08 += 0x22;
                    }
                }
                var_s3_2 += 2;
            } while ((u32) var_s3_2 < (u32) &D_8014A4F4);
            var_s3_3 = D_8014A4C4;
            do {
                if (*(s16 *)((unsigned char *)temp_s1 + (6620)) == (*var_s3_3 + temp_s5)) {
                    if (((*(s16 *)((unsigned char *)temp_s1 + (6634)) + 0x2D) < *(s16 *)((unsigned char *)temp_s1 + (6906))) && ((func_800AE0D0() & 0xFF) < (&sp68[0])[var_s4])) {
                        var_s2[1].unk_08 -= 0x22;
                    }
                    if ((*(s16 *)((unsigned char *)temp_s1 + (6906)) < (*(s16 *)((unsigned char *)temp_s1 + (6634)) + 0x28)) && ((func_800AE0D0() & 0xFF) < (&sp68[0])[var_s4])) {
                        var_s2[1].unk_08 += 0x22;
                    }
                }
                var_s3_3 += 2;
            } while (var_s3_3 != &D_8014A4D4);
            var_s2[1].unk_18 = 0;
        }
        var_s4 += 1;
        var_s2 += 1;
    } while (var_s4 != 3);
}
