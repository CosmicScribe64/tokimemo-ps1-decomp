s32 func_80135274(s32 arg0) {
    s32 sp34;
    s32 sp30;
    s32 sp2C;
    void *sp28;
    s32 temp_a2;
    s32 temp_a3;
    s32 var_v1;
    u32 temp_v0;
    u8 *temp_a1;

    temp_a1 = (arg0 * 0x30) + D_8013984C;
    temp_a2 = D_8011ECF6 - *(s16 *)((unsigned char *)temp_a1 + (22));
    temp_a3 = D_8011ECFA - *(s16 *)((unsigned char *)temp_a1 + (26));
    sp30 = temp_a3;
    sp34 = temp_a2;
    sp28 = temp_a1;
    temp_v0 = func_80134008(arg0);
    switch (temp_v0) {
    case 0:
        if (temp_a3 < temp_a2) {
            var_v1 = 0;
        } else {
            var_v1 = 1;
        }
        if (-temp_a2 >= temp_a3) {
            var_v1 += 2;
        }
block_43:
        sp2C = var_v1;
        break;
    case 1:
        var_v1 = 1;
        if (*(s16 *)((unsigned char *)temp_a1 + (22)) < D_8011ECF6) {
            var_v1 = 0;
        }
        if (*(s16 *)((unsigned char *)temp_a1 + (26)) >= D_8011ECFA) {
            var_v1 += 2;
        }
        goto block_43;
    case 2:
        var_v1 = 1;
        if (-temp_a2 < temp_a3) {
            var_v1 = 0;
        }
        if (temp_a2 >= temp_a3) {
            var_v1 += 2;
        }
        goto block_43;
    case 3:
        var_v1 = 1;
        if (*(s16 *)((unsigned char *)temp_a1 + (26)) < D_8011ECFA) {
            var_v1 = 0;
        }
        if (D_8011ECF6 >= *(s16 *)((unsigned char *)temp_a1 + (22))) {
            var_v1 += 2;
        }
        goto block_43;
    case 4:
        if (temp_a2 < temp_a3) {
            var_v1 = 0;
        } else {
            var_v1 = 1;
        }
        if (temp_a3 >= -temp_a2) {
            var_v1 += 2;
        }
        goto block_43;
    case 5:
        var_v1 = 1;
        if (D_8011ECF6 < *(s16 *)((unsigned char *)temp_a1 + (22))) {
            var_v1 = 0;
        }
        if (D_8011ECFA >= *(s16 *)((unsigned char *)temp_a1 + (26))) {
            var_v1 += 2;
        }
        goto block_43;
    case 6:
        var_v1 = 1;
        if (temp_a3 < -temp_a2) {
            var_v1 = 0;
        }
        if (temp_a3 >= temp_a2) {
            var_v1 += 2;
        }
        goto block_43;
    case 7:
        var_v1 = 1;
        if (D_8011ECFA < *(s16 *)((unsigned char *)temp_a1 + (26))) {
            var_v1 = 0;
        }
        if (*(s16 *)((unsigned char *)temp_a1 + (22)) >= D_8011ECF6) {
            var_v1 += 2;
        }
        goto block_43;
    }
    return sp2C;
}
