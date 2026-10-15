void func_8015443C(void *arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 sp4C;
    s32 *var_a0;
    s32 *var_s0;
    s32 temp_s3;
    s32 temp_s7;
    s32 temp_t2;
    s32 temp_t6;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;
    s32 var_a1;
    s32 var_a1_2;
    s32 var_at;
    s32 var_at_2;
    s32 var_at_3;
    s32 var_s1;
    u32 temp_s4;
    u8 temp_v0;

    temp_v0 = *(u8 *)((unsigned char *)D_8015EDB4 + (2722));
    temp_s3 = *(s32 *)((unsigned char *)arg0 + (4));
    temp_s4 = *(u32 *)((unsigned char *)arg0 + (12));
    temp_s7 = arg1 & 0xFFFF;
    var_s1 = 0;
    var_s0 = ((u8 *)arg0) + 0x24;
    if (temp_v0 < 0x10U) {
        var_a1 = (func_800AE0D0() % 150) + 0x64;
        goto block_17;
    }
    if (temp_v0 >= 0x10U) {
        var_at = temp_v0 < 0x20U;
        if (temp_v0 < 0x20U) {
            var_a1 = (func_800AE0D0() % 100) + 0x32;
            goto block_17;
        }
        goto block_6;
    }
    var_at = temp_v0 < 0x20U;
block_6:
    if (var_at == 0) {
        var_at_2 = temp_v0 < 0x30U;
        if (temp_v0 < 0x30U) {
            var_a1 = (func_800AE0D0() % 50) + 0x1E;
            goto block_17;
        }
        goto block_10;
    }
    var_at_2 = temp_v0 < 0x30U;
block_10:
    if (var_at_2 == 0) {
        var_at_3 = temp_v0 < 0x40U;
        if (temp_v0 < 0x40U) {
            var_a1 = (func_800AE0D0() % 30) + 0x14;
            goto block_17;
        }
        goto block_14;
    }
    var_at_3 = temp_v0 < 0x40U;
block_14:
    if (var_at_3 == 0) {
        var_a1_2 = sp4C;
        if (temp_v0 < 0x64U) {
            var_a1 = (func_800AE0D0() % 10) + 5;
block_17:
            sp4C = var_a1;
            goto block_18;
        }
    } else {
block_18:
        var_a1_2 = sp4C;
    }
    var_a0 = var_s0;
loop_20:
    sp4C = var_a1_2;
    if (func_8015431C((s32) var_a0) != 0) {
        temp_v0_2 = (*(s32 *)var_s0);
        if ((temp_v0_2 == 0x24000507) || (temp_v0_2 == 0x34000609)) {
            if ((u8) *(u8 *)((unsigned char *)D_8015EDB4 + (2722)) < 0x50U) {
                if (var_s1 >= var_a1_2) {
                    temp_v0_3 = *(s32 *)((unsigned char *)var_s0 + (8));
                    temp_t2 = (temp_v0_3 & 0xFFFF) + (temp_v0_3 & 0x01E00000) + ((temp_s7 & 0x1F) << 0x10);
                    *(s32 *)((unsigned char *)var_s0 + (4)) = (s32) ((*(s32 *)((unsigned char *)var_s0 + (4)) & 0xFFFF) + ((arg3 << 0x16) + (((arg2 >> 4) & 0x3F) << 0x10)));
                    var_s0 += 8;
                    *var_s0 = temp_t2;
                    var_s1 = 0;
                }
                var_s1 += 1;
            } else {
                var_a1_2 = func_800AE0D0() % 70;
                if (var_a1_2 == 0) {
                    temp_v0_4 = *(s32 *)((unsigned char *)var_s0 + (8));
                    temp_t6 = (temp_v0_4 & 0xFFFF) + (temp_v0_4 & 0x01E00000) + ((temp_s7 & 0x1F) << 0x10);
                    *(s32 *)((unsigned char *)var_s0 + (4)) = (s32) ((*(s32 *)((unsigned char *)var_s0 + (4)) & 0xFFFF) + ((arg3 << 0x16) + (((arg2 >> 4) & 0x3F) << 0x10)));
                    var_s0 += 8;
                    *var_s0 = temp_t6;
                }
            }
        }
        var_s0 += 4;
        if ((temp_s3 == 0) || (temp_s4 >= (u32) var_s0)) {
            var_a0 = var_s0;
            if ((temp_s3 == 0) && (var_a0 = var_s0, (((u32) ((temp_s4 * 4) + var_s0 + 0xC) < (u32) var_s0) != 0))) {

            } else {
                goto loop_20;
            }
        }
    }
}
