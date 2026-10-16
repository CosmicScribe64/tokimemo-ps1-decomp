s32 func_80015A64();                                /* extern */
? func_800458E0();                                  /* extern */
extern s16 D_800F19D0;
extern s16 D_800F19D2;
extern s16 D_800F19D4;
extern s16 D_800F19D6;

void func_800FEDC8(void) {
    s16 temp_v1;
    s16 var_a0;
    s16 var_a0_2;
    s16 var_v0;
    s32 temp_t0;
    s32 temp_v0;
    s32 var_a1;

    if (func_80015A64() & 4) {
        func_800458E0();
    }
    var_a0 = D_800F19D0;
    var_a1 = 0;
    if (var_a0 < D_800F19D2) {
        var_a0 = D_800F19D2;
        var_a1 = 1;
    }
    var_v0 = 0;
    if (var_a0 < D_800F19D4) {
        var_a0 = D_800F19D4;
        var_a1 = 2;
    }
    var_a0_2 = 0;
    if (var_a0 < D_800F19D6) {
        var_a1 = 3;
    }
    do {
        if (var_v0 != var_a1) {
            temp_v1 = (&D_800F19D0)[var_v0];
            if (var_a0_2 < temp_v1) {
                var_a0_2 = temp_v1;
            }
        }
        var_v0 += 1;
    } while (var_v0 < 4);
    temp_t0 = (s32) (D_800F19D6 + D_800F19D0 + D_800F19D2 + D_800F19D4) / 4;
    if ((D_800EAFB8 == 0) || !(D_800B1AE4 & 3)) {
        if (temp_t0 == 0) {
            D_800EAFB8 = 3;
        } else {
            temp_v0 = temp_t0 / 16;
            if (temp_v0 == 0) {
                D_800EAFB8 = 3;
            } else if (temp_v0 == 1) {
                D_800EAFB8 = 2;
            } else if (temp_v0 == 2) {
                D_800EAFB8 = 1;
            } else if (temp_v0 >= 3) {
                D_800EAFB8 = 0;
            }
        }
    }
}
