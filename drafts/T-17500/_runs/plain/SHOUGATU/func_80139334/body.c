void func_80139334(void) {
    if (D_80144E0C == 2) {
        D_800E71DF = 7;
    }
    func_80137AB4();
    if (D_80144E0C == 2) {
        D_8011ED5B &= 0xFF7F;
    }
    if (D_80144E0C == 3) {
        D_80122D20 = 1;
        D_8011ED5B &= 0xFF7F;
    } else {
        D_80122D20 = 0;
    }
    if ((D_80144E0C == 2) && (check_end_k(D_80144E0C) != 0) && ((D_800E62BC == 0) || ((D_800E62BC != 0) && (func_80046094() == 9)))) {
        k_reset(1);
        D_80122CE4 = 0;
        D_80122CF0 = 0;
        D_8011ED5B &= 0xFF7F;
        D_800CA2AC = 0;
        D_800E7384 = 0;
    } else {
        if ((u32) D_800E7384++ >= 0x1AU) {
            if ((D_80144E0C == 3) && (check_end_k(D_80144E0C) != 0)) {
                if (D_800E62BC != 0) {
                    if (D_800E62BC != 0) {
                        if (func_80046094() == 9) {
                            goto block_19;
                        }
                    }
                } else {
block_19:
                    D_80122D20 = 1;
                    D_80144E08 += 1;
                    D_80122CE4 = 0;
                    D_80122CF0 = 0;
                    D_8011ED5B &= 0xFF7F;
                    D_800CA2AC = 0;
                    D_80144E0C = 0;
                    func_8004284C();
                }
            }
        }
    }
}
