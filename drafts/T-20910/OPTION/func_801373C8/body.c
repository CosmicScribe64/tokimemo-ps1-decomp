s32 func_801373C8(void) {
    s16 a[6];
    s16 b[6];
    s16 c[6];
    s16 d[6];
    s32 i;
    s32 j;

    if (D_800E6280.unk_110D != 0) {
        D_80121726 = D_80121726 / 17 * 16;
        D_80121728 = D_80121728 / 17 * 16;
        if (D_80121726 <= 0x1000) {
            D_80121726 = 0x1000;
            D_80121728 = 0x1000;
            for (i = 0, j = 0x3C; i < 2; i++, j += 2) {
                a[i] = -0x50;
                b[i] = i * 0x10 + 8;
                c[i] = 0xC4;
                d[i] = 0x10;
                ((DisplayActor44 *)D_80120650)[i + 15].unk_18 = j;
                ((DisplayActor44 *)D_80120650)[i + 15].unk_03 |= 0x80;
                ((DisplayActor44 *)D_80120650)[i + 15].unk_2A = b[i];
                ((DisplayActor44 *)D_80120650)[i + 15].unk_26 = a[i];
            }
            ((DisplayActor44 *)D_80120650)[15].unk_03 = 0;
            ((DisplayActor44 *)D_80120650)[16].unk_03 = 0;
            if (D_800B3220 == 0) {
                if (!(D_800E7D4F & 1)) {
                    c[0] = 0;
                }
                if (!(D_800E7D50 & 1)) {
                    c[1] = 0;
                }
            }
            menu_set(1, 2, a, b, c, d);
        }
    }
    if (D_800E6280.unk_110D == 0) {
        ((DisplayActor44 *)D_8011ECD0)[159].unk_18 = 7;
        ((DisplayActor44 *)D_8011ECD0)[159].unk_26 = -0xA0;
        ((DisplayActor44 *)D_8011ECD0)[159].unk_2A = -0x78;
        D_800E6280.unk_110D++;
    } else if (D_800E6280.unk_110D == 1) {
        ((DisplayActor44 *)D_8011ECD0)[107].unk_03 &= ~0x80;
        ((DisplayActor44 *)D_8011ECD0)[108].unk_03 &= ~0x80;
        ((DisplayActor44 *)D_8011ECD0)[109].unk_03 &= ~0x80;
        ((DisplayActor44 *)D_8011ECD0)[110].unk_03 &= ~0x80;
        D_80120912[0] = D_80120912[0] / 9 * 8;
        if (D_80120912[0] < 9) {
            D_800E6280.unk_110D = 2;
        }
    } else {
        func_8004284C();
    }
    ((DisplayActor44 *)D_8011ECD0)[106].unk_03 = 0;
    ((DisplayActor44 *)D_8011ECD0)[159].unk_03 = 0;
    D_80121726 = 0x1000;
    D_80121728 = 0x1000;
    return 0;
}
