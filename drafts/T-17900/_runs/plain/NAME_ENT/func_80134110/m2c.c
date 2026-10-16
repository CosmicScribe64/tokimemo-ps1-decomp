s16 func_80134110(void) {
    s16 var_v0;

    var_v0 = D_8011ECF6;
    if ((D_8011ECF6 >= -0x77) && (D_8011ECF6 < 0x98)) {
        var_v0 = D_8011ECFA;
        if ((var_v0 >= 0x39) && (var_v0 < 0x68)) {
            D_8014CCB8 = (s32) (D_8011ECF6 + 0x78) / 16;
            D_8014CCBC = (s32) (var_v0 - 0x38) / 16;
            return var_v0;
        }
    }
    D_8014CCB8 = -1;
    D_8014CCBC = -1;
    return var_v0;
}
