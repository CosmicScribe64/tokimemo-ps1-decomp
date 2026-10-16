void func_80133CA8(void) {
    s32 i;
    s16 v;

    for (i = 0; i != 10; i++) {
        if (D_8013984C[i].unk_00 & 0x80) {
            ((DisplayActor44 *)D_8011ECD0)[128 + i].unk_01 = 3;
            D_8013984C[i].unk_10 += 4;
            func_80133F00(i, 1);
        } else {
            D_8013984C[i].unk_10 += 1;
            ((DisplayActor44 *)D_8011ECD0)[128 + i].unk_01 = 7;
            func_80133F00(i, 0);
        }
        if (D_8013984C[i].unk_10 >= 15) {
            D_8013984C[i].unk_10 = 0;
            D_8013984C[i].unk_0C++;
        }
        switch (D_8013984C[i].unk_00 & 0x7F) {
        case 0:
        default:
            ((DisplayActor44 *)D_8011ECD0)[128 + i].unk_03 = 0;
            break;
        case 1:
        case 2:
        case 3:
            if (D_8013984C[i].unk_0C >= 6) {
                D_8013984C[i].unk_0C = 0;
            }
            v = D_80139A34[func_80134008(i)][D_8013984C[i].unk_0C];
            if (v & 0x80) {
                ((DisplayActor44 *)D_8011ECD0)[128 + i].unk_03 = 0x85;
            } else {
                ((DisplayActor44 *)D_8011ECD0)[128 + i].unk_03 = 0x84;
            }
            ((DisplayActor44 *)D_8011ECD0)[128 + i].unk_18 = v & 0x7F;
            ((DisplayActor44 *)D_8011ECD0)[128 + i].unk_43 = D_80139A2C[D_8013984C[i].unk_00 & 0x7F];
            ((DisplayActor44 *)D_8011ECD0)[128 + i].unk_26 = D_8013984C[i].unk_16;
            ((DisplayActor44 *)D_8011ECD0)[128 + i].unk_2A = D_8013984C[i].unk_1A;
            break;
        case 4:
            if (D_8013984C[i].unk_0C >= 4) {
                D_8013984C[i].unk_0C = 0;
            }
            v = D_80139A34[func_80134008(i) + 8][D_8013984C[i].unk_0C];
            if (v & 0x80) {
                ((DisplayActor44 *)D_8011ECD0)[128 + i].unk_03 = 0x85;
            } else {
                ((DisplayActor44 *)D_8011ECD0)[128 + i].unk_03 = 0x84;
            }
            ((DisplayActor44 *)D_8011ECD0)[128 + i].unk_18 = v & 0x7F;
            ((DisplayActor44 *)D_8011ECD0)[128 + i].unk_43 = D_80139A2C[D_8013984C[i].unk_00 & 0x7F];
            ((DisplayActor44 *)D_8011ECD0)[128 + i].unk_26 = D_8013984C[i].unk_16;
            ((DisplayActor44 *)D_8011ECD0)[128 + i].unk_2A = D_8013984C[i].unk_1A;
            break;
        }
    }
}
