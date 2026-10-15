u32 func_80052648(s32);                             /* extern */
u32 func_80052C88(s32);                             /* extern */
extern u8 D_800E62BE;
extern s16 D_800E643E;
extern s16 D_800E6442;
extern u8 D_800E644A;

s32 func_8013BD84(void) {
    s16 var_a0;
    s16 var_s1;
    s32 var_at;
    s32 var_s0;
    s32 var_s3;
    u32 temp_t1;
    u32 temp_v0;
    u32 temp_v0_2;

    var_s3 = 0;
    var_s1 = 0;
    if (func_80052C88(0) >= 0x32U) {
        if (func_80052C88(0) >= 0x50U) {
            var_s1 = D_800E643E + D_800E6442;
        } else {
            var_s1 = 0;
        }
    }
    var_s0 = 1;
    if (D_800E62BE == 0x60) {
        temp_v0 = (u8) D_800E644A >> 4;
        if ((temp_v0 == 4) || (temp_v0 == 1)) {
            var_s1 = 0;
        }
    }
    do {
        if ((func_80052648(var_s0) >= 0x32U) && (func_80052C88(var_s0) >= 0x50U) && (((u32) (D_800E6280.unk_1BC[var_s0].unk_0C.w << 0x1E) >> 0x1F) == 1)) {
            var_a0 = D_800E6280.unk_1BC[var_s0].unk_02 + D_800E6280.unk_1BC[var_s0].unk_06;
            if (D_800E62BE == 0x60) {
                temp_t1 = (u8) D_800E6280.unk_1BC[var_s0].unk_0C.b[2] >> 4;
                if ((temp_t1 == 4) || (var_at = var_s1 < var_a0, (temp_t1 == 1))) {
                    var_a0 = 0;
                    goto block_16;
                }
            } else {
block_16:
                var_at = var_s1 < var_a0;
            }
            if (var_at != 0) {
                var_s3 = var_s0;
                var_s1 = var_a0;
            }
        }
        var_s0 += 1;
    } while (var_s0 != 0xB);
    if ((func_80052648(var_s3) >= 0x32U) && (func_80052C88(var_s3) >= 0x50U)) {
        if ((var_s3 == 0) && (D_800E62BE == 0x60) && ((temp_v0_2 = (u8) D_800E644A >> 4, (temp_v0_2 == 4)) || (temp_v0_2 == 1))) {
            /* Duplicate return node #32. Try simplifying control flow for better match */
            return 0;
        }
        if ((var_s3 == 0) && ((u32) ((u8) D_800E644A >> 4) >= 6U)) {
            var_s3 = 0xB;
        }
        D_80122EBC = var_s3;
        return 1;
    }
    return 0;
}
