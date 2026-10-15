void func_801395C8(s16 arg0) {
    s32 sp60[1];
    s32 sp48;
    s32 sp44;
    s16 var_s0;
    s16 var_s0_2;
    s16 var_s0_3;
    s16 var_s0_4;
    s32 temp_a3;
    s32 temp_a3_10;
    s32 temp_a3_11;
    s32 temp_a3_12;
    s32 temp_a3_2;
    s32 temp_a3_3;
    s32 temp_a3_4;
    s32 temp_a3_5;
    s32 temp_a3_6;
    s32 temp_a3_7;
    s32 temp_a3_8;
    s32 temp_a3_9;
    s32 temp_hi;
    s32 temp_lo;
    s32 temp_lo_2;
    s32 temp_lo_3;
    s32 temp_lo_4;
    s32 temp_lo_5;
    s32 temp_s0;
    s32 temp_s0_10;
    s32 temp_s0_11;
    s32 temp_s0_12;
    s32 temp_s0_13;
    s32 temp_s0_14;
    s32 temp_s0_2;
    s32 temp_s0_3;
    s32 temp_s0_4;
    s32 temp_s0_5;
    s32 temp_s0_6;
    s32 temp_s0_7;
    s32 temp_s0_8;
    s32 temp_s0_9;
    s32 temp_s1;
    s32 temp_s1_2;
    s32 temp_s1_3;
    s32 temp_s1_4;
    s32 temp_s1_5;
    s32 temp_s1_6;
    s32 temp_s3;
    s32 temp_s3_2;
    s32 temp_s3_3;
    s32 temp_s3_4;
    s32 temp_s3_5;
    s32 temp_s3_6;

    if (D_8013B5C4 != 0) {
        var_s0 = 0;
        if (D_8013B5C4 != 2) {
            var_s0_2 = 0;
            if (D_8013B5C4 != 3) {
                var_s0_3 = 0;
                do {
                    func_800AE0F0(&sp60 + (var_s0_3 * 0x1F), *(s32 *)(D_8013B5E0 + (var_s0_3 * 4)));
                    var_s0_3 += 1;
                } while (var_s0_3 < 0x51);
            } else {
                do {
                    func_800AE0F0(&sp60 + (var_s0_2 * 0x1F), *(s32 *)(D_8013B5DC + (var_s0_2 * 4)));
                    var_s0_2 += 1;
                } while (var_s0_2 < 0x27);
            }
        } else {
            do {
                func_800AE0F0(&sp60 + (var_s0 * 0x1F), *(s32 *)(D_8013B5D4 + (var_s0 * 4)));
                var_s0 += 1;
            } while (var_s0 < 0x1E);
        }
    } else {
        var_s0_4 = 0;
        do {
            func_800AE0F0(&sp60 + (var_s0_4 * 0x1F), *(s32 *)(D_8013B5D8 + (var_s0_4 * 4)));
            var_s0_4 += 1;
        } while (var_s0_4 < 0x33);
    }
    temp_hi = arg0 % 6;
    if (temp_hi == 0) {
        temp_s3 = &sp60 + (arg0 * 0x1F);
        set_kanji_string(-0x74, 0x36, 2, temp_s3, 0);
        temp_lo = func_800AE0E0(temp_s3) * 7;
        temp_a3 = temp_s3 + 0x1F;
        sp48 = temp_a3;
        set_kanji_string((s16) (temp_lo - 0x74), 0x36, 0, temp_a3, 0);
        temp_s0 = func_800AE0E0(temp_s3);
        set_kanji_string((s16) (((func_800AE0E0(sp48) * 7) + (temp_s0 * 7)) - 0x74), 0x36, 0, temp_s3 + 0x3E, 0);
        temp_a3_2 = temp_s3 + 0x5D;
        sp48 = temp_a3_2;
        set_kanji_string(-0x54, 0x4A, 0, temp_a3_2, 0);
        temp_s1 = temp_s3 + 0x7C;
        set_kanji_string((s16) ((func_800AE0E0(sp48) * 7) - 0x54), 0x4A, 0, temp_s1, 0);
        temp_s0_2 = func_800AE0E0(sp48);
        k_disp_start(set_kanji_string((s16) (((func_800AE0E0(temp_s1) * 7) + (temp_s0_2 * 7)) - 0x54), 0x4A, 0, temp_s3 + 0x9B, 0));
        return;
    }
    if (temp_hi == 1) {
        temp_s3_2 = &sp60 + (arg0 * 0x1F);
        temp_a3_3 = temp_s3_2 - 0x1F;
        sp48 = temp_a3_3;
        set_kanji_string(-0x74, 0x36, 2, temp_a3_3, 0);
        set_kanji_string((s16) ((func_800AE0E0(sp48) * 7) - 0x74), 0x36, 2, temp_s3_2, 0);
        temp_s0_3 = func_800AE0E0(sp48);
        set_kanji_string((s16) (((func_800AE0E0(temp_s3_2) * 7) + (temp_s0_3 * 7)) - 0x74), 0x36, 0, temp_s3_2 + 0x1F, 0);
        temp_a3_4 = temp_s3_2 + 0x3E;
        sp44 = temp_a3_4;
        set_kanji_string(-0x54, 0x4A, 0, temp_a3_4, 0);
        temp_lo_2 = func_800AE0E0(sp44) * 7;
        temp_a3_5 = temp_s3_2 + 0x5D;
        sp48 = temp_a3_5;
        set_kanji_string((s16) (temp_lo_2 - 0x54), 0x4A, 0, temp_a3_5, 0);
        temp_s0_4 = func_800AE0E0(sp44);
        k_disp_start(set_kanji_string((s16) (((func_800AE0E0(sp48) * 7) + (temp_s0_4 * 7)) - 0x54), 0x4A, 0, temp_s3_2 + 0x7C, 0));
        return;
    }
    if (temp_hi == 2) {
        temp_s3_3 = &sp60 + (arg0 * 0x1F);
        temp_s0_5 = temp_s3_3 - 0x3E;
        set_kanji_string(-0x74, 0x36, 2, temp_s0_5, 0);
        temp_s1_2 = temp_s3_3 - 0x1F;
        set_kanji_string((s16) ((func_800AE0E0(temp_s0_5) * 7) - 0x74), 0x36, 2, temp_s1_2, 0);
        temp_s0_6 = func_800AE0E0(temp_s0_5);
        set_kanji_string((s16) (((func_800AE0E0(temp_s1_2) * 7) + (temp_s0_6 * 7)) - 0x74), 0x36, 2, temp_s3_3, 0);
        temp_a3_6 = temp_s3_3 + 0x1F;
        sp48 = temp_a3_6;
        set_kanji_string(-0x54, 0x4A, 0, temp_a3_6, 0);
        temp_lo_3 = func_800AE0E0(sp48) * 7;
        temp_a3_7 = temp_s3_3 + 0x3E;
        sp44 = temp_a3_7;
        set_kanji_string((s16) (temp_lo_3 - 0x54), 0x4A, 0, temp_a3_7, 0);
        temp_s0_7 = func_800AE0E0(sp48);
        k_disp_start(set_kanji_string((s16) (((func_800AE0E0(sp44) * 7) + (temp_s0_7 * 7)) - 0x54), 0x4A, 0, temp_s3_3 + 0x5D, 0));
        return;
    }
    if (temp_hi == 3) {
        temp_s3_4 = &sp60 + (arg0 * 0x1F);
        temp_a3_8 = temp_s3_4 + 0x5D;
        sp48 = temp_a3_8;
        set_kanji_string(-0x74, 0x36, 0, temp_a3_8, 0);
        temp_s1_3 = temp_s3_4 + 0x7C;
        set_kanji_string((s16) ((func_800AE0E0(sp48) * 7) - 0x74), 0x36, 0, temp_s1_3, 0);
        temp_s0_8 = func_800AE0E0(sp48);
        set_kanji_string((s16) (((func_800AE0E0(temp_s1_3) * 7) + (temp_s0_8 * 7)) - 0x74), 0x36, 0, temp_s3_4 + 0x9B, 0);
        set_kanji_string(-0x54, 0x4A, 2, temp_s3_4, 0);
        temp_lo_4 = func_800AE0E0(temp_s3_4) * 7;
        temp_a3_9 = temp_s3_4 + 0x1F;
        sp48 = temp_a3_9;
        set_kanji_string((s16) (temp_lo_4 - 0x54), 0x4A, 0, temp_a3_9, 0);
        temp_s0_9 = func_800AE0E0(temp_s3_4);
        k_disp_start(set_kanji_string((s16) (((func_800AE0E0(sp48) * 7) + (temp_s0_9 * 7)) - 0x54), 0x4A, 0, temp_s3_4 + 0x3E, 0));
        return;
    }
    if (temp_hi == 4) {
        temp_s3_5 = &sp60 + (arg0 * 0x1F);
        temp_a3_10 = temp_s3_5 + 0x3E;
        sp44 = temp_a3_10;
        set_kanji_string(-0x74, 0x36, 0, temp_a3_10, 0);
        temp_lo_5 = func_800AE0E0(sp44) * 7;
        temp_a3_11 = temp_s3_5 + 0x5D;
        sp48 = temp_a3_11;
        set_kanji_string((s16) (temp_lo_5 - 0x74), 0x36, 0, temp_a3_11, 0);
        temp_s0_10 = func_800AE0E0(sp44);
        set_kanji_string((s16) (((func_800AE0E0(sp48) * 7) + (temp_s0_10 * 7)) - 0x74), 0x36, 0, temp_s3_5 + 0x7C, 0);
        temp_s1_4 = temp_s3_5 - 0x1F;
        set_kanji_string(-0x54, 0x4A, 2, temp_s1_4, 0);
        set_kanji_string((s16) ((func_800AE0E0(temp_s1_4) * 7) - 0x54), 0x4A, 2, temp_s3_5, 0);
        temp_s0_11 = func_800AE0E0(temp_s1_4);
        k_disp_start(set_kanji_string((s16) (((func_800AE0E0(temp_s3_5) * 7) + (temp_s0_11 * 7)) - 0x54), 0x4A, 0, temp_s3_5 + 0x1F, 0));
        return;
    }
    temp_s3_6 = &sp60 + (arg0 * 0x1F);
    temp_a3_12 = temp_s3_6 + 0x1F;
    sp48 = temp_a3_12;
    set_kanji_string(-0x74, 0x36, 0, temp_a3_12, 0);
    temp_s1_5 = temp_s3_6 + 0x3E;
    set_kanji_string((s16) ((func_800AE0E0(sp48) * 7) - 0x74), 0x36, 0, temp_s1_5, 0);
    temp_s0_12 = func_800AE0E0(sp48);
    set_kanji_string((s16) (((func_800AE0E0(temp_s1_5) * 7) + (temp_s0_12 * 7)) - 0x74), 0x36, 0, temp_s3_6 + 0x5D, 0);
    temp_s0_13 = temp_s3_6 - 0x3E;
    set_kanji_string(-0x54, 0x4A, 2, temp_s0_13, 0);
    temp_s1_6 = temp_s3_6 - 0x1F;
    set_kanji_string((s16) ((func_800AE0E0(temp_s0_13) * 7) - 0x54), 0x4A, 2, temp_s1_6, 0);
    temp_s0_14 = func_800AE0E0(temp_s0_13);
    k_disp_start(set_kanji_string((s16) (((func_800AE0E0(temp_s1_6) * 7) + (temp_s0_14 * 7)) - 0x54), 0x4A, 2, temp_s3_6, 0));
}
