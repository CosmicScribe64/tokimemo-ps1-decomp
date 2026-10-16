s16 func_80134110(void) {
    s16 temp_v1;
    s16 var_v0;

    temp_v1 = ((DisplayActor44 *)D_8011ECD0)->unk_26;
    var_v0 = temp_v1;
    if ((temp_v1 >= -0x77) && (temp_v1 < 0x98)) {
        var_v0 = ((DisplayActor44 *)D_8011ECD0)->unk_2A;
        if ((var_v0 >= 0x39) && (var_v0 < 0x68)) {
            D_8014CCB8 = (s32) (temp_v1 + 0x78) / 16;
            D_8014CCBC = (s32) (var_v0 - 0x38) / 16;
            return var_v0;
        }
    }
    D_8014CCB8 = -1;
    D_8014CCBC = -1;
    return var_v0;
}
