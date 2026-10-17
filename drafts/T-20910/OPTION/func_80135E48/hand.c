s32 func_80135E48(void) {
    s16 a[6];
    s16 b[6];
    s16 c[6];
    s16 d[6];
    s32 i;

    if (D_800E6280.unk_110D == 0) {
        ((DisplayActor44 *)D_8011ECD0)[106].unk_18 = 4;
        ((DisplayActor44 *)D_8011ECD0)[106].unk_26 = -0xA0;
        ((DisplayActor44 *)D_8011ECD0)[106].unk_2A = -0x78;
        ((DisplayActor44 *)D_8011ECD0)[106].unk_03 = 0;
        D_80120912[1] = 0x7FFF;
        D_800E6280.unk_110D = 1;
        D_80120912[0] = 0x7FFF;
        return;
    }
    D_80120912[0] = D_80120912[0] / 17 * 16;
    D_80120912[1] = D_80120912[1] / 17 * 16;
    if (D_80120912[0] <= 0x1000) {
        D_80120912[1] = 0x1000;
        D_80120912[0] = 0x1000;
        for (i = 0; i < 5; i++) {
            a[i] = 0x10;
            b[i] = i * 0x18 - 0x18;
            c[i] = 0x30;
            d[i] = 0x10;
            ((DisplayActor44 *)D_80120650)[i + 11].unk_18 = i * 2 + 0xC;
            ((DisplayActor44 *)D_80120650)[i + 11].unk_26 = a[i];
            ((DisplayActor44 *)D_80120650)[i + 11].unk_2A = b[i];
            ((DisplayActor44 *)D_80120650)[i + 11].unk_03 |= 0x80;
        }
        func_8004F870(1, 5, a, b, c, d);
        switch ((u8)D_800E6280.unk_F6F % 10) {
        case 1:
        default:
            ((DisplayActor44 *)D_80120650)[11].unk_18++;
            D_800E6280.unk_1094 = 0;
            break;
        case 2:
            ((DisplayActor44 *)D_80120650)[12].unk_18++;
            D_800E6280.unk_1094 = 1;
            break;
        case 3:
            ((DisplayActor44 *)D_80120650)[13].unk_18++;
            D_800E6280.unk_1094 = 1;
            break;
        case 4:
            ((DisplayActor44 *)D_80120650)[14].unk_18++;
            D_800E6280.unk_1094 = 1;
            break;
        case 6:
            ((DisplayActor44 *)D_80120650)[15].unk_18++;
            D_800E6280.unk_1094 = 1;
            break;
        }
        func_8004284C();
    }
}
