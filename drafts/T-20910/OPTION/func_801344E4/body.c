void func_801344E4(void) {
    s32 pad[2]; /* FAKE: unused 8-byte local declared first reproduces the original frame, as in the matched siblings (T-4070); real source unknown. T-20910 */
    s16 a[4];
    s16 b[4];
    s16 c[4];
    s16 d[4];
    s32 i;
    s32 j;

    if (D_800E6280.unk_110D == 0) {
        ((DisplayActor44 *)D_8011ECD0)[106].unk_18 = 3;
        ((DisplayActor44 *)D_8011ECD0)[106].unk_26 = -0xA0;
        ((DisplayActor44 *)D_8011ECD0)[106].unk_2A = -0x78;
        ((DisplayActor44 *)D_8011ECD0)[106].unk_03 = 0;
        D_80120912[1] = 0x7FFF;
        D_800E6280.unk_110D = 1;
        D_80120912[0] = 0x7FFF;
    } else {
        D_80120912[0] = D_80120912[0] / 17 * 16;
        D_80120912[1] = D_80120912[1] / 17 * 16;
        if (D_80120912[0] <= 0x1000) {
            D_80120912[1] = 0x1000;
            D_80120912[0] = 0x1000;
            for (i = 0, j = 0x1C; i < 3; i++, j += 2) {
                a[i] = -0x78;
                b[i] = i * 0x28 - 0x20;
                c[i] = 0x6C;
                d[i] = 0x10;
                ((DisplayActor44 *)D_80120650)[i + 11].unk_18 = j;
                ((DisplayActor44 *)D_80120650)[i + 11].unk_03 |= 0x80;
                ((DisplayActor44 *)D_80120650)[i + 11].unk_2A = b[i];
                ((DisplayActor44 *)D_80120650)[i + 11].unk_26 = a[i];
            }
            menu_set(1, 3, a, b, c, d);
            for (i = 0, j = 0x22; i < 2; i++, j += 2) {
                ((DisplayActor44 *)D_8011ECD0)[i + 110].unk_18 = j;
                ((DisplayActor44 *)D_8011ECD0)[i + 110].unk_03 |= 0x80;
                ((DisplayActor44 *)D_8011ECD0)[i + 110].unk_26 = 0;
                ((DisplayActor44 *)D_8011ECD0)[i + 110].unk_2A = i * 0x10 - 0x28;
                if (D_800E6280.unk_F75 == 1) {
                    ((DisplayActor44 *)D_8011ECD0)[14].unk_18++;
                } else {
                    ((DisplayActor44 *)D_8011ECD0)[15].unk_18++;
                }
            }
            for (i = 0, j = 0x26; i < 2; i++, j += 2) {
                ((DisplayActor44 *)D_8011ECD0)[i + 112].unk_18 = j;
                ((DisplayActor44 *)D_8011ECD0)[i + 112].unk_03 |= 0x80;
                ((DisplayActor44 *)D_8011ECD0)[i + 112].unk_26 = 0;
                ((DisplayActor44 *)D_8011ECD0)[i + 112].unk_2A = i * 0x10;
                if ((u8)D_800E6280.unk_F70 == 0) {
                    ((DisplayActor44 *)D_8011ECD0)[16].unk_18++;
                } else {
                    ((DisplayActor44 *)D_8011ECD0)[17].unk_18++;
                }
            }
            for (i = 0, j = 0x2A; i < 2; i++, j += 2) {
                ((DisplayActor44 *)D_8011ECD0)[i + 114].unk_18 = j;
                ((DisplayActor44 *)D_8011ECD0)[i + 114].unk_2A = i * 0x10 + 0x28;
                ((DisplayActor44 *)D_8011ECD0)[i + 114].unk_03 |= 0x80;
                ((DisplayActor44 *)D_8011ECD0)[i + 114].unk_26 = 0;
            }
            func_8004284C();
        }
    }
    func_801324E8();
}
