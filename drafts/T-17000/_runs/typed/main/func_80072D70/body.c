s32 func_80072D70(void) {
    s32 sp3C;
    s32 sp38;
    s32 temp_hi;
    s32 temp_hi_2;
    s32 var_s0;
    s32 var_s0_2;
    s32 var_s0_3;
    s32 var_s0_4;
    s32 var_s0_5;
    s32 var_s1;
    s32 var_s1_2;
    s32 var_v0;

    sp3C = 0;
    var_s1 = ((s16) D_800E6280.unk_118.unk_02 / 8) + 0xCC;
    if (var_s1 >= 0xFF) {
        var_s1 = 0xFE;
    }
    var_v0 = D_800E6280.unk0 & 0xF;
    switch (var_v0) {
    default:
        D_800E6280.unk_0F4.b[1] = D_800E6280.unk0 & 0xFFF0;
block_57:
block_58:
        if (sp3C == 0) {
            var_v0 = func_80042908(0xFF);
        }
        break;
    case 0:
        var_s0 = 0;
        if (((u8) D_800E6280.unk0 >> 5) == 0) {
            if ((D_800E6280.unk_0FC.unk_02 < 0x14) && (var_v0 = rand(), ((var_s1 < (var_v0 & 0xFF)) != 0))) {
                var_s0 = 1;
                sp3C = 1;
            } else if (D_800E6280.unk_0FC.unk_02 == 0) {
                var_s0 = 1;
                sp3C = 1;
            }
            if ((D_800E6280.unk_11C.unk_02 >= 0x51) && ((u32) var_s1 < (u32) (D_800E7D10 & 0xFF))) {
                var_s0 += 2;
                sp3C = 1;
            }
        }
        if (var_s0 == 1) {
            var_v0 = func_80042940(1);
        }
        if (var_s0 == 2) {
            var_v0 = func_80042940(2);
        }
        if (var_s0 == 3) {
            var_v0 = func_80042940(3);
        }
        goto block_58;
    case 1:
        var_s0_2 = 1;
        if (((s16) D_800E6280.unk_000 >= 0x51) && ((u32) var_s1 < (u32) (D_800E7D10 & 0xFF))) {
            var_s0_2 = 3;
            sp3C = 1;
        }
        if ((s16) D_800E6280.unk_000 >= 0x1F) {
            var_s0_2 = 0;
            sp3C = 1;
        }
        if (var_s0_2 == 3) {
            var_v0 = func_80042940(3);
        }
        if (var_s0_2 == 0) {
            var_v0 = func_80042940(5);
        }
        goto block_58;
    case 2:
        var_s0_3 = 2;
        if (((s16) D_800E6280.unk_000 < 0x14) && (var_v0 = rand(), ((var_s1 < (var_v0 & 0xFF)) != 0))) {
            var_s0_3 = 3;
            sp3C = 1;
        } else if (D_800E6280.unk_0FC.unk_02 == 0) {
            var_s0_3 = 3;
            sp3C = 1;
        }
        if (D_800E6280.unk_11C.unk_02 < 0x46) {
            var_s0_3 = 0;
            sp3C = 1;
        }
        if (var_s0_3 == 3) {
            var_v0 = func_80042940(3);
        }
        if (var_s0_3 == 0) {
            var_v0 = func_80042940(6);
        }
        goto block_58;
    case 3:
        var_s0_4 = 3;
        if (((s16) D_800E6280.unk_000 < 0x46) && (D_800E6280.unk_0FC.unk_02 >= 0x1F)) {
            var_s0_4 = 0;
            sp3C = 1;
        }
        if (var_s0_4 == 0) {
            var_v0 = func_80042940(7);
            goto block_57;
        }
        if (D_800E6280.unk_044[0].unk_00[D_800E6280.unk0] == 0) {
            var_v0 = rand();
            temp_hi = var_v0 % 11;
            var_s0_5 = 0;
            sp38 = temp_hi;
            var_s1_2 = temp_hi;
loop_50:
            temp_hi_2 = var_s1_2 % 11;
            if ((((u32) (*(&D_800E6280 + (temp_hi_2 * 0x38)) << 0x1E) >> 0x1F) != 0) && (var_v0 = get_h_tokimeki(temp_hi_2), (((u32) var_v0 < 0x33U) == 0)) && (var_v0 = get_g_zyotai_s(sp38), (var_v0 != 4))) {
                var_v0 = rand();
                var_s0_5 += 1;
                if ((var_v0 & 0xFF) < 0x3F) {
                    D_800E6280.unk_75D = (u8) temp_hi_2;
                    main_state_set(0x62);
                    var_v0 = 1;
                } else {
                    goto block_56;
                }
            } else {
                var_s0_5 += 1;
block_56:
                var_s1_2 += 1;
                if (var_s0_5 == 0xB) {
                    goto block_57;
                }
                goto loop_50;
            }
        } else {
            goto block_58;
        }
        break;
    }
    return var_v0;
}
