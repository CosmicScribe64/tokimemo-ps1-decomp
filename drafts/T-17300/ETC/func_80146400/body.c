s32 func_80146400(void) {
    s32 *sp2C;
    s32 *temp_v0;
    s32 temp_a0;
    s32 var_v0;
    s32 var_v1;

    if (D_800E62BE != 0x5F) {
        if (D_800E62BE != 0x60) {
            var_v1 = 6;
            if (D_800E62BE != 0x61) {
                var_v1 = 6;
            }
        } else {
            var_v1 = 3;
        }
    } else {
        var_v1 = 0;
    }
    if (D_800E62BF != 2) {
        if ((D_800E62BF != 7) && (D_800E62BF == 0xC)) {
            var_v1 += 1;
        }
    } else {
        var_v1 -= 1;
    }
    temp_v0 = &D_801506F8[var_v1];
    sp2C = temp_v0;
    func_800AE0B0("補正 %d ", *temp_v0);
    temp_a0 = *sp2C;
    var_v0 = 0;
    if ((((func_800AE0D0() & 7) + temp_a0) - 8) > 0) {
        var_v0 = temp_a0;
    }
    return var_v0;
}
