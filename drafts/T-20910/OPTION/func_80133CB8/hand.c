void func_80133CB8(void) {
    s32 pad[2]; /* FAKE: unused 8-byte local declared first reproduces the original frame, as in the matched siblings (T-4070); real source unknown. T-20910 */
    s16 a[4];
    s16 b[4];
    s16 c[4];
    s16 d[4];
    s32 i;
    s32 j;

    if (D_800E6280.unk_110D == 0) {
        ((DisplayActor44 *)D_8011ECD0)[106].unk_18 = 2;
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
            for (i = 0, j = 0x16; i < 3; i++, j += 2) {
                a[i] = -0x20;
                b[i] = i * 0x18 - 0x38;
                c[i] = 0x50;
                d[i] = 0x10;
                ((DisplayActor44 *)D_80120650)[i + 11].unk_18 = j;
                ((DisplayActor44 *)D_80120650)[i + 11].unk_03 |= 0x80;
                ((DisplayActor44 *)D_80120650)[i + 11].unk_2A = b[i];
                ((DisplayActor44 *)D_80120650)[i + 11].unk_26 = a[i];
            }
            func_8004F870(1, 3, a, b, c, d);
            switch ((u8)D_800E6280.unk_F6C % 3) {
            case 0:
            default:
                D_800E6280.unk_1094 = 0;
                break;
            case 1:
                D_800E6280.unk_1094 = 1;
                break;
            case 2:
                D_800E6280.unk_1094 = 2;
                break;
            }
            func_8006509C();
            /* FAKE: loop body on the for line, as in the matched func_80134210 (T-7020). T-20910 */
            for (i = 0; i < 16; i++) *(s16 *)&D_80125C10[i * 4] += 200;
            func_800654D0(D_800E6280.unk_F6D);
            func_8004284C();
        }
    }
    func_801324E8();
}
