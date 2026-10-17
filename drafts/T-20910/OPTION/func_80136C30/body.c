void func_80136C30(void) {
    s32 i;
    s32 pad[3]; /* FAKE: unused words reproduce the original frame (0x70) and the row offsets (T-4070 pad); real source unknown. T-20910 */
    s16 a[6];
    s16 b[6];
    s16 c[6];
    s16 d[6];

    if (D_800E6280.unk_110D == 0) {
        ((DisplayActor44 *)D_8011ECD0)[106].unk_18 = 6;
        ((DisplayActor44 *)D_8011ECD0)[106].unk_26 = -0xA0;
        ((DisplayActor44 *)D_8011ECD0)[106].unk_2A = -0x78;
        ((DisplayActor44 *)D_8011ECD0)[106].unk_03 = 0;
        D_80120912[1] = 0x7FFF;
        D_80120912[0] = 0x7FFF;
        D_800E6280.unk_110D = 1;
    } else {
        D_80120912[0] = D_80120912[0] / 17 * 16;
        D_80120912[1] = D_80120912[1] / 17 * 16;
        if (D_80120912[0] <= 0x1000) {
            D_80120912[1] = 0x1000;
            D_80120912[0] = 0x1000;
            for (i = 0; i < 4; i++) {
                a[i] = -0x40;
                b[i] = i * 0x10 + 0x18;
                c[i] = 0x60;
                d[i] = 0x10;
                ((DisplayActor44 *)D_80120650)[i + 11].unk_18 = i * 2 + 0x34;
                ((DisplayActor44 *)D_80120650)[i + 11].unk_26 = a[i];
                ((DisplayActor44 *)D_80120650)[i + 11].unk_2A = b[i];
                ((DisplayActor44 *)D_80120650)[i + 11].unk_03 |= 0x80;
            }
            if (!(D_800E7D4F & 1) && !(D_800E7D50 & 1)) {
                c[0] = 0;
                ((DisplayActor44 *)D_80120650)[11].unk_03 = 0;
            }
            if (func_80136F3C() == 0) {
                ((DisplayActor44 *)D_80120650)[12].unk_03 = 0;
                c[1] = 0;
                c[2] = 0;
                ((DisplayActor44 *)D_80120650)[13].unk_03 = 0;
            }
            if (D_800E7D37 == 0) {
                c[3] = 0;
                ((DisplayActor44 *)D_80120650)[14].unk_03 = 0;
            }
            menu_set(1, 4, a, b, c, d);
            D_800E6280.unk_1094 = 0;
            func_8004284C();
        }
    }
    func_801324E8();
}
