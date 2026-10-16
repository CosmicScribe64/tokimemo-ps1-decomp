void func_80139334(void) {
    if (D_80144E0C == 2) {
        D_800E6280.unk_F5F = 7;
    }
    func_80137AB4();
    if (D_80144E0C == 2) {
        ((DisplayActor44 *)D_8011ECD0)[2].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)[2].unk_03 & 0xFF7F);
    }
    if (D_80144E0C == 3) {
        D_80122D20 = 1;
        ((DisplayActor44 *)D_8011ECD0)[2].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)[2].unk_03 & 0xFF7F);
    } else {
        D_80122D20 = 0;
    }
    if ((D_80144E0C == 2) && (check_end_k(D_80144E0C) != 0) && (((u8) D_800E6280.unk_03C == 0) || (((u8) D_800E6280.unk_03C != 0) && (func_80046094() == 9)))) {
        k_reset(1);
        D_80122CE4 = 0;
        D_80122CF0 = 0;
        ((DisplayActor44 *)D_8011ECD0)[2].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)[2].unk_03 & 0xFF7F);
        D_800CA2AC = 0;
        D_800E6280.unk_1104.w = 0;
    } else {
        D_800E6280.unk_1104.w += 1;
        if ((u32) D_800E6280.unk_1104.w >= 0x1AU) {
            if ((D_80144E0C == 3) && (check_end_k(D_80144E0C) != 0)) {
                if ((u8) D_800E6280.unk_03C != 0) {
                    if ((u8) D_800E6280.unk_03C != 0) {
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
                    ((DisplayActor44 *)D_8011ECD0)[2].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)[2].unk_03 & 0xFF7F);
                    D_800CA2AC = 0;
                    D_80144E0C = 0;
                    func_8004284C();
                }
            }
        }
    }
}
