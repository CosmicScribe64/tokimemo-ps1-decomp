s32 func_8013E89C(s32 arg0) {
    s32 *var_a0;
    s32 *var_a1;

    switch (D_80150070) {
    case 0:
        func_8013E6AC(D_80150070);
        var_a1 = (s32 *)-1;
        if (func_8013EAC0() != 0) {
            func_8013EB90(arg0, -1);
            var_a1 = D_80150070 + 1;
        }
block_32:
        D_80150070 = var_a1;
    case -1:
        break;
    case 1:
        var_a0 = &D_8015007C;
        if ((D_8014F990 == 1) || (var_a0 = D_8015007C, (var_a0 == (s32 *)1))) {
            if (cd_read_sync_status(var_a0, D_80150070) & 1) {
                dec_bg_reset();
                func_80132FB0(D_80150040, D_80150044, D_8015004C, D_80150050);
                if ((D_8014E0C0 == NULL) && (D_8014F990 == 1)) {
                    if (D_80150064 < 0xFF) {
                        D_80150064 += 1;
                    }
                    if ((D_801500D8 == 0) || (D_801500D8 == 2)) {
                        func_8013EEFC(D_8014E0C0);
                    }
                    D_80150070 = NULL;
                } else {
                    var_a1 = (s32 *)3;
                    if (D_8014E0C0 == (s32 *)1) {
                        var_a1 = D_80150070 + 1;
                    }
                    goto block_32;
                }
            }
        } else if ((D_8014F990 == 0) && (var_a0 == NULL)) {
            if (D_80150064 < 0xFF) {
                D_80150064 += 1;
            }
            if ((D_801500D8 == 0) || (D_801500D8 == 2)) {
                func_8013EEFC(var_a0, D_80150070);
            }
            D_80150070 = NULL;
        }
        break;
    case 2:
        if (cd_read_sync_status(D_80150070) & 1) {
            var_a1 = D_80150070 + 1;
            goto block_32;
        }
        break;
    }
}
