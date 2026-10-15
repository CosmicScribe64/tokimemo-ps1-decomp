void func_8013C410(s16 arg0, s32 *arg1, s32 *arg2) {
    DatePlaceWeights sp18;
    s16 var_v0;
    s16 var_v0_2;
    u8 temp_v1;
    u8 temp_v1_2;

    sp18 = D_8015B7A0;
    if (arg0 > 0) {
        var_v0 = 0;
loop_2:
        if (D_800E6392 >= (0x64 - (var_v0 * 0x19))) {
            *arg1 = 5 - var_v0;
        } else {
            var_v0 += 1;
            if (var_v0 < 5) {
                goto loop_2;
            }
        }
        if (D_800E6392 == 0) {
            *arg1 = 0;
        }
        temp_v1 = sp18.b[D_800E71DF * 0x15 + D_800CA2F8];
        if (temp_v1 == 0) {
            *arg2 = 5;
            return;
        }
        if ((temp_v1 == 1) || (temp_v1 == 2)) {
            *arg2 = 4;
            return;
        }
        if (temp_v1 == 3) {
            *arg2 = 3;
            return;
        }
        *arg2 = 2;
        return;
    }
    var_v0_2 = 0;
loop_17:
    if (D_800E6392 >= (0x64 - (var_v0_2 * 0x19))) {
        *arg1 = var_v0_2 + 3;
    } else {
        var_v0_2 += 1;
        if (var_v0_2 < 5) {
            goto loop_17;
        }
    }
    if (D_800E6392 == 0) {
        *arg1 = 8;
    }
    temp_v1_2 = sp18.b[D_800E71DF * 0x15 + D_800CA2F8];
    if (temp_v1_2 == 0) {
        *arg2 = 3;
        return;
    }
    if ((temp_v1_2 == 1) || (temp_v1_2 == 2)) {
        *arg2 = 4;
        return;
    }
    if (temp_v1_2 == 3) {
        *arg2 = 5;
        return;
    }
    *arg2 = 6;
}
