extern u8 D_800E62B8;

void func_80134A18(void) {
    s16 temp_s0;
    s16 temp_v1;
    s16 var_t3;
    s32 *var_s2;
    s32 *var_s2_2;
    s32 *var_s2_3;
    s32 *var_s2_4;
    s32 *var_s2_5;
    s32 *var_s2_6;
    s32 *var_s2_7;
    s32 temp_s0_2;
    s32 temp_s0_3;
    s32 temp_s0_4;
    s32 temp_s0_5;
    s32 temp_s0_6;
    s32 temp_s0_7;
    s32 temp_s0_8;
    s32 var_s1;
    s32 var_s1_2;
    s32 var_s1_3;
    s32 var_s1_4;
    s32 var_s1_5;
    s32 var_s1_6;
    s32 var_s1_7;
    u8 temp_t7;
    u8 temp_t8;
    u8 temp_v0;

    temp_v1 = D_8015F3E8;
    temp_s0 = temp_v1;
    if (temp_v1 < 0x20) {
        if ((u8) D_8012276C < 0xC0U) {
            temp_t7 = D_8012276E + 6;
            D_8012276D = temp_t7;
            D_8012276C = temp_t7;
            temp_t8 = D_8012277E + 1;
            D_8012277D = temp_t8;
            D_8012277C = temp_t8;
            temp_v0 = D_8012278E + 1;
            D_8012278D = temp_v0;
            D_8012278C = temp_v0;
            D_8012276E = temp_t7;
            D_8012277E = temp_t8;
            D_8012278E = temp_v0;
            func_8009AD70(0, (s32) &D_80122760);
            func_8009AD70(1, (s32) &D_80122770);
            func_8009AD70(2, (s32) &D_80122780);
        }
        if (temp_s0 < 0xF) {
            D_800E62B8 += 1;
        }
        goto block_42;
    }
    if (temp_v1 >= 0x20) {
        var_s1 = 0;
        if (temp_v1 < 0x54) {
            var_s2 = &D_8015F3E0;
            do {
                temp_s0_2 = var_s1 + 1;
                func_80147488(temp_s0_2, *var_s2, 1);
                var_s1 = temp_s0_2;
                var_s2 += 4;
            } while (temp_s0_2 != 2);
            goto block_42;
        }
    }
    if (temp_v1 >= 0x54) {
        var_s1_2 = 0;
        if (temp_v1 < 0x68) {
            var_s2_2 = &D_8015F3E0;
            do {
                temp_s0_3 = var_s1_2 + 1;
                func_80147488(temp_s0_3, *var_s2_2, 2);
                var_s1_2 = temp_s0_3;
                var_s2_2 += 4;
            } while (temp_s0_3 != 2);
            goto block_42;
        }
    }
    if (temp_v1 >= 0x68) {
        var_s1_3 = 0;
        if (temp_v1 < 0x72) {
            var_s2_3 = &D_8015F3E0;
            do {
                temp_s0_4 = var_s1_3 + 1;
                func_80147488(temp_s0_4, *var_s2_3, 3);
                var_s1_3 = temp_s0_4;
                var_s2_3 += 4;
            } while (temp_s0_4 != 2);
            goto block_42;
        }
    }
    if (temp_v1 >= 0x72) {
        var_s1_4 = 0;
        if (temp_v1 < 0x7C) {
            var_s2_4 = &D_8015F3E0;
            do {
                temp_s0_5 = var_s1_4 + 1;
                func_80147488(temp_s0_5, *var_s2_4, 4);
                var_s1_4 = temp_s0_5;
                var_s2_4 += 4;
            } while (temp_s0_5 != 2);
            goto block_42;
        }
    }
    if (temp_v1 >= 0x7C) {
        var_s1_5 = 0;
        if (temp_v1 < 0x9A) {
            var_s2_5 = &D_8015F3E0;
            do {
                temp_s0_6 = var_s1_5 + 1;
                func_80147488(temp_s0_6, *var_s2_5, 5);
                var_s1_5 = temp_s0_6;
                var_s2_5 += 4;
            } while (temp_s0_6 != 2);
            goto block_42;
        }
    }
    if (temp_v1 >= 0x9A) {
        var_s1_6 = 0;
        if (temp_v1 < 0xAE) {
            var_s2_6 = &D_8015F3E0;
            do {
                temp_s0_7 = var_s1_6 + 1;
                func_80147488(temp_s0_7, *var_s2_6, 6);
                var_s1_6 = temp_s0_7;
                var_s2_6 += 4;
            } while (temp_s0_7 != 2);
            goto block_42;
        }
    }
    if (temp_v1 >= 0xAE) {
        var_s1_7 = 0;
        if (temp_v1 < 0xEA) {
            var_s2_7 = &D_8015F3E0;
            do {
                temp_s0_8 = var_s1_7 + 1;
                func_80147488(temp_s0_8, *var_s2_7, 7);
                var_s1_7 = temp_s0_8;
                var_s2_7 += 4;
            } while (temp_s0_8 != 2);
block_42:
            var_t3 = temp_v1 + 1;
            goto block_43;
        }
    }
    var_t3 = temp_v1 + 1;
    if (temp_v1 >= 0xF0) {
        return;
    }
block_43:
    D_8015F3E8 = var_t3;
}
