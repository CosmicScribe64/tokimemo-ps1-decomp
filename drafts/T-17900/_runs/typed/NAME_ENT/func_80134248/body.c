void func_80134248(s32 arg0) {
    s32 sp34;
    u8 *sp28;
    s32 temp_hi;
    s32 temp_hi_2;
    s32 temp_v1;
    s32 temp_v1_2;
    s32 temp_v1_3;
    u32 temp_a0;
    u32 temp_a0_2;
    u32 temp_a0_3;
    u8 *var_a2;
    u8 *var_v0;
    u8 var_a1;
    u8 var_t3;
    u8 *temp_v0;
    u8 *temp_v0_10;
    u8 *temp_v0_11;
    u8 *temp_v0_12;
    u8 *temp_v0_13;
    u8 *temp_v0_14;
    u8 *temp_v0_2;
    u8 *temp_v0_3;
    u8 *temp_v0_4;
    u8 *temp_v0_5;
    u8 *temp_v0_6;
    u8 *temp_v0_7;
    u8 *temp_v0_8;
    u8 *temp_v0_9;

    var_a1 = D_8014CC70;
    switch (arg0) {
    case 0:
        if ((s32) D_8014CC70 >= 3) {
            var_a1 = 2;
        }
        break;
    case 1:
        var_a1 = D_8014CC70 - 3;
        if ((s32) var_a1 >= 3) {
            var_a1 = 2;
        }
        break;
    case 2:
        var_a1 = D_8014CC70 - 6;
        if ((s32) var_a1 >= 5) {
            var_a1 = 4;
        }
        break;
    }
    switch (arg0) {
    default:
        var_a2 = sp28;
        break;
    case 0:
        var_a2 = D_8014CC74;
        break;
    case 1:
        var_a2 = D_8014CC7C;
        break;
    case 2:
        var_a2 = D_8014CC84;
        break;
    }
    switch (D_8014CCC8) {
    case 0:
        temp_hi = (s32) D_8014CCC0 % 6;
        if (temp_hi == 5) {
            temp_v0 = var_a2 + (var_a1 * 2);
            (*(s32 *)temp_v0) = (u8) *D_800B3644;
            *(u8 *)((unsigned char *)temp_v0 + (1)) = (u8) D_800B3645;
        } else if ((D_8014CCC4 == 2) && ((D_8014CCC0 == 9) || (D_8014CCC0 == 0xA))) {
            temp_v0_2 = var_a2 + (var_a1 * 2);
            (*(s32 *)temp_v0_2) = (u8) *D_800B3644;
            *(u8 *)((unsigned char *)temp_v0_2 + (1)) = (u8) D_800B3645;
        } else if ((D_8014CCC4 == 3) && ((D_8014CCC0 == 3) || (D_8014CCC0 == 4))) {
            temp_v0_3 = var_a2 + (var_a1 * 2);
            (*(s32 *)temp_v0_3) = (u8) *D_800B3644;
            *(u8 *)((unsigned char *)temp_v0_3 + (1)) = (u8) D_800B3645;
        } else {
            if ((D_8014CCC4 == 5) && (D_8014CCC0 >= 0xB)) {
                var_v0 = var_a2 + (var_a1 * 2);
                *var_v0 = *D_800B3644;
                var_t3 = D_800B3645;
            } else {
                var_v0 = var_a2 + (var_a1 * 2);
                *var_v0 = *(u8 *)(*(&(&D_8014CD28)[(s32) D_8014CCC0 / 6] + (D_8014CCC4 * 0xC)) + (temp_hi * 2));
                var_t3 = *(u8 *)((unsigned char *)(*(&(&D_8014CD28)[(s32) D_8014CCC0 / 6] + (D_8014CCC4 * 0xC)) + (((s32) D_8014CCC0 % 6) * 2)) + (1));
            }
block_68:
            *(u8 *)((unsigned char *)var_v0 + (1)) = var_t3;
        }
        break;
    case 1:
        temp_hi_2 = (s32) D_8014CCC0 % 6;
        if (temp_hi_2 == 5) {
            temp_v0_4 = var_a2 + (var_a1 * 2);
            (*(s32 *)temp_v0_4) = (u8) *D_800B3644;
            *(u8 *)((unsigned char *)temp_v0_4 + (1)) = (u8) D_800B3645;
        } else if ((D_8014CCC4 == 2) && ((D_8014CCC0 == 9) || (D_8014CCC0 == 0xA))) {
            temp_v0_5 = var_a2 + (var_a1 * 2);
            (*(s32 *)temp_v0_5) = (u8) *D_800B3644;
            *(u8 *)((unsigned char *)temp_v0_5 + (1)) = (u8) D_800B3645;
        } else if ((D_8014CCC4 == 3) && ((D_8014CCC0 == 3) || (D_8014CCC0 == 4))) {
            temp_v0_6 = var_a2 + (var_a1 * 2);
            (*(s32 *)temp_v0_6) = (u8) *D_800B3644;
            *(u8 *)((unsigned char *)temp_v0_6 + (1)) = (u8) D_800B3645;
        } else if ((D_8014CCC4 == 5) && (D_8014CCC0 >= 0xB)) {
            temp_v0_7 = var_a2 + (var_a1 * 2);
            (*(s32 *)temp_v0_7) = (u8) *D_800B3644;
            *(u8 *)((unsigned char *)temp_v0_7 + (1)) = (u8) D_800B3645;
        } else {
            temp_v0_8 = var_a2 + (var_a1 * 2);
            (*(s32 *)temp_v0_8) = (u8) *(u8 *)(*(&(&D_8014CD6C)[(s32) D_8014CCC0 / 6] + (D_8014CCC4 * 0xC)) + (temp_hi_2 * 2));
            *(u8 *)((unsigned char *)temp_v0_8 + (1)) = (u8) *(u8 *)((unsigned char *)(*(&(&D_8014CD6C)[(s32) D_8014CCC0 / 6] + (D_8014CCC4 * 0xC)) + (((s32) D_8014CCC0 % 6) * 2)) + (1));
        }
        break;
    case 2:
        if (D_8014CCC0 < 0x10) {
            temp_a0 = (D_8014CCC4 * 0x10) + (D_8014CCAC * 0x10) + D_8014CCC0;
            if (temp_a0 < (u16) *(u16 *)((unsigned char *)D_8014CE64 + (76))) {
                temp_v1 = temp_a0 * 2;
                temp_v0_9 = var_a2 + (var_a1 * 2);
                (*(s32 *)temp_v0_9) = (u8) *(u8 *)(*(s32 *)((unsigned char *)D_8014CE68 + (152)) + temp_v1);
                *(u8 *)((unsigned char *)temp_v0_9 + (1)) = (u8) *(u8 *)((unsigned char *)(*(s32 *)((unsigned char *)D_8014CE68 + (152)) + temp_v1) + (1));
            } else {
                temp_v0_10 = var_a2 + (var_a1 * 2);
                (*(s32 *)temp_v0_10) = (u8) *D_800B3644;
                *(u8 *)((unsigned char *)temp_v0_10 + (1)) = (u8) D_800B3645;
            }
        }
        break;
    case 3:
        if (D_8014CCC0 < 0x10) {
            if (D_8014CCB4 == 0x26) {
                temp_a0_2 = (D_8014CCC4 * 0x10) + (D_8014CCAC * 0x10) + D_8014CCC0;
                if (temp_a0_2 < (u16) D_8014CE64[D_8014CCB4]) {
                    temp_v1_2 = temp_a0_2 * 2;
                    temp_v0_11 = var_a2 + (var_a1 * 2);
                    (*(s32 *)temp_v0_11) = (u8) *(D_8014CE68[D_8014CCB4] + temp_v1_2);
                    *(u8 *)((unsigned char *)temp_v0_11 + (1)) = (u8) *(u8 *)((unsigned char *)(D_8014CE68[D_8014CCB4] + temp_v1_2) + (1));
                } else {
                    temp_v0_12 = var_a2 + (var_a1 * 2);
                    (*(s32 *)temp_v0_12) = (u8) *D_800B3644;
                    *(u8 *)((unsigned char *)temp_v0_12 + (1)) = (u8) D_800B3645;
                }
            } else if (D_8014CCC4 == 0) {
                sp34 = (s32) var_a1;
                sp28 = var_a2;
                if (D_8014CCC0 < (func_800AE0E0(D_8014CDB0[D_8014CCB4]) / 2)) {
                    temp_v0_13 = var_a2 + (var_a1 * 2);
                    (*(s32 *)temp_v0_13) = (u8) *(D_8014CDB0[D_8014CCB4] + (D_8014CCC0 * 2));
                    *(u8 *)((unsigned char *)temp_v0_13 + (1)) = (u8) *(u8 *)((unsigned char *)(D_8014CDB0[D_8014CCB4] + (D_8014CCC0 * 2)) + (1));
                } else {
                    var_v0 = var_a2 + (var_a1 * 2);
                    *var_v0 = *D_800B3644;
                    var_t3 = D_800B3645;
                    goto block_68;
                }
            } else {
                temp_a0_3 = ((D_8014CCC4 * 0x10) + (D_8014CCAC * 0x10) + D_8014CCC0) - 0x10;
                if (temp_a0_3 < (u16) D_8014CE64[D_8014CCB4]) {
                    temp_v1_3 = temp_a0_3 * 2;
                    temp_v0_14 = var_a2 + (var_a1 * 2);
                    (*(s32 *)temp_v0_14) = (u8) *(D_8014CE68[D_8014CCB4] + temp_v1_3);
                    *(u8 *)((unsigned char *)temp_v0_14 + (1)) = (u8) *(u8 *)((unsigned char *)(D_8014CE68[D_8014CCB4] + temp_v1_3) + (1));
                } else {
                    var_v0 = var_a2 + (var_a1 * 2);
                    *var_v0 = *D_800B3644;
                    var_t3 = D_800B3645;
                    goto block_68;
                }
            }
        }
        break;
    }
    D_8014CC70 += 1;
}
