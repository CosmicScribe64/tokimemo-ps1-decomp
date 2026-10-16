extern s32 D_800E7378;
extern s32 D_8015F4A4;
extern s32 D_8015F4A8;

s32 func_80138964(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 var_a0;
    s32 var_at;

    if (arg3 == 0) {
        D_8015F4A4 = arg0;
        D_8015F4A8 = 0;
        return arg0;
    }
    var_a0 = D_8015F4A8;
    if ((var_a0 < D_8015F4A4) && !(D_800E7378 & 1)) {
        if (var_a0 < ((s32) D_8015F4A4 % 10)) {
            var_a0 += 1;
            goto block_22;
        }
        if (var_a0 < ((s32) D_8015F4A4 % 100)) {
            var_a0 += 0xA;
            goto block_22;
        }
        if (var_a0 < ((s32) D_8015F4A4 % 1000)) {
            var_a0 += 0x64;
            goto block_22;
        }
        if (var_a0 < ((s32) D_8015F4A4 % 10000)) {
            var_a0 += 0x3E8;
            goto block_22;
        }
        if (var_a0 < ((s32) D_8015F4A4 % 100000)) {
            var_a0 += 0x2710;
            goto block_22;
        }
        if (var_a0 < ((s32) D_8015F4A4 % 1000000)) {
            var_a0 += 0x186A0;
            goto block_22;
        }
        var_at = D_8015F4A4 < var_a0;
        if (var_a0 < ((s32) D_8015F4A4 % 10000000)) {
            var_a0 += 0xF4240;
            goto block_22;
        }
    } else {
block_22:
        var_at = D_8015F4A4 < var_a0;
    }
    if (var_at != 0) {
        var_a0 = D_8015F4A4;
    }
    D_8015F4A8 = var_a0;
    func_801430D0(var_a0, arg1, arg2, 0);
    return D_8015F4A8;
}
