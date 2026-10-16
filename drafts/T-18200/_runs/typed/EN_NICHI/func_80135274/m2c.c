s32 func_80135274(s32 arg0) {
    s32 sp34;
    s32 sp30;
    s32 sp2C;
    EnObj *sp28;
    EnObj *temp_a1;
    s32 temp_a2;
    s32 temp_a3;
    s32 var_v1;
    u32 temp_v0;

    temp_a1 = &D_8013984C[arg0];
    temp_a2 = D_8011ECD0->unk_26 - temp_a1->unk_16;
    temp_a3 = D_8011ECD0->unk_2A - temp_a1->unk_1A;
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
        if (temp_a1->unk_16 < D_8011ECD0->unk_26) {
            var_v1 = 0;
        }
        if (temp_a1->unk_1A >= D_8011ECD0->unk_2A) {
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
        if (temp_a1->unk_1A < D_8011ECD0->unk_2A) {
            var_v1 = 0;
        }
        if (D_8011ECD0->unk_26 >= temp_a1->unk_16) {
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
        if (D_8011ECD0->unk_26 < temp_a1->unk_16) {
            var_v1 = 0;
        }
        if (D_8011ECD0->unk_2A >= temp_a1->unk_1A) {
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
        if (D_8011ECD0->unk_2A < temp_a1->unk_1A) {
            var_v1 = 0;
        }
        if (temp_a1->unk_16 >= D_8011ECD0->unk_26) {
            var_v1 += 2;
        }
        goto block_43;
    }
    return sp2C;
}
