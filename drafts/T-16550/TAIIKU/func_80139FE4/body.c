void func_80139FE4(void) {
    s32 n;
    s32 i;
    s32 cnt;
    s32 m;
    s32 sp1C[11];

    n = 0;
    for (i = 0; i < 0xB; i++) {
        if (((TaiikuBit1 *)&D_800E6280.unk_1BC[i].unk_0C)->f == 1) {
            sp1C[n] = i;
            n++;
        }
    }
    cnt = 0;
    m = n;
    for (i = 0; i < 0xB; i++) {
        if (((TaiikuBit1 *)&D_800E6280.unk_1BC[i].unk_0C)->f == 1) {
            if (D_800E6280.unk_F80 & 4) {
                ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_26 = ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_26 - 4;
            } else {
                ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_26 = ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_26 - 1;
            }
            if (((DisplayActor44 *)D_8011ECD0)[96 + i].unk_26 < -0xC0) {
                ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_26 = ((DisplayActor44 *)D_8011ECD0)[96 + sp1C[(cnt + n - 1) % m]].unk_26 + 0x78;
                if (((DisplayActor44 *)D_8011ECD0)[96 + i].unk_26 < 0xA0) {
                    ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_26 = 0xA0;
                }
            }
            cnt++;
        }
    }
}
