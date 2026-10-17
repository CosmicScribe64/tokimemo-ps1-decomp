s32 func_801378DC(void) {
    s16 a[16];
    s16 b[16];
    s16 c[16];
    s16 d[16];
    s32 i;

    if (D_800E6280.unk_110D != 0) {
        D_80121726 = D_80121726 / 17 * 16;
        D_80121728 = D_80121728 / 17 * 16;
        if (D_80121726 <= 0x1000) {
            D_80121726 = 0x1000;
            D_80121728 = 0x1000;
            for (i = 0; i < 14; i++) {
                ((DisplayActor44 *)D_80120650)[i + 15].unk_18 = i * 2 + 0x40;
                c[i] = 0x80;
                d[i] = 0x10;
                ((DisplayActor44 *)D_80120650)[i + 15].unk_03 = 0;
                a[i] = (i / 7) * 128 - 0x78;
                ((DisplayActor44 *)D_80120650)[i + 15].unk_26 = a[i];
                b[i] = (i % 7) * 0x12 - 0x48;
                ((DisplayActor44 *)D_80120650)[i + 15].unk_2A = b[i];
                if (D_800E7D10.flags[i] == 0 && D_800E6280.unk_53C[i] == 0) {
                    c[i] = 0;
                }
            }
            menu_set(1, 14, a, b, c, d);
        }
    }
    if (D_800E6280.unk_110D == 0) {
        ((DisplayActor44 *)D_8011ECD0)[159].unk_18 = 8;
        ((DisplayActor44 *)D_8011ECD0)[159].unk_26 = -0xA0;
        ((DisplayActor44 *)D_8011ECD0)[159].unk_2A = -0x78;
        D_800E6280.unk_110D++;
    } else if (D_800E6280.unk_110D == 1) {
        ((DisplayActor44 *)D_8011ECD0)[107].unk_03 &= ~0x80;
        ((DisplayActor44 *)D_8011ECD0)[108].unk_03 &= ~0x80;
        ((DisplayActor44 *)D_8011ECD0)[109].unk_03 &= ~0x80;
        ((DisplayActor44 *)D_8011ECD0)[110].unk_03 &= ~0x80;
        D_80120912[1] = D_80120912[1] / 9 * 8;
        if (D_80120912[1] < 9) {
            D_800E6280.unk_110D = 2;
        }
    } else {
        k_disp_switch(6, 0);
        set_kanji_string(-0x80, -0x58, 0, (s32)"フリートーク", 0);
        k_sub_disp_start(1);
        func_8004284C();
    }
    ((DisplayActor44 *)D_8011ECD0)[106].unk_03 = 0;
    ((DisplayActor44 *)D_8011ECD0)[159].unk_03 = 0;
    D_80121726 = 0x1000;
    D_80121728 = 0x1000;
    return 0;
}
