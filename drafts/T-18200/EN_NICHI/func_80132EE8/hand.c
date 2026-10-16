void func_80132EE8(void) {
    s32 i;
    s32 v;

    D_80139AFC++;
    if (D_80139AF4 >= 80 && D_80139AFC >= 30) {
        for (i = 0; i < 10; i++) {
            v = D_8013984C[i].unk_00;
            if (v & 0x80) {
                D_8013984C[i].unk_00 = v & 0x7F;
                if ((v & 0x7F) == 4) {
                    D_80139B04--;
                } else {
                    D_80139B00--;
                }
            }
        }
        D_800E6280.unk_F7A = ((DisplayActor44 *)D_8011ECD0)[152].unk_26;
        D_800E6280.unk_F7C = ((DisplayActor44 *)D_8011ECD0)[152].unk_2A;
        func_80135A48(((DisplayActor44 *)D_8011ECD0)->unk_26, ((DisplayActor44 *)D_8011ECD0)->unk_2A);
        func_801369F4(0x502);
        D_80139AF8 = 3;
    }
    if (D_80139AFC >= 60) {
        D_800E6280.unk_F7A = ((DisplayActor44 *)D_8011ECD0)[152].unk_26;
        D_800E6280.unk_F7C = ((DisplayActor44 *)D_8011ECD0)[152].unk_2A;
        D_80139AFC = 0;
        D_80139AF8 = 0;
        func_801369F4(0x50D);
        for (i = 0; i < 10; i++) {
            if (D_8013984C[i].unk_00 & 0x80) {
                D_8013984C[i].unk_00 = 0;
            }
        }
    }
}
