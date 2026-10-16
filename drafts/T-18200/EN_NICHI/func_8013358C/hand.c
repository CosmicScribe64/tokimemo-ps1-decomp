void func_8013358C(void) {
    s32 i;
    s32 v;

    func_801330D0();
    D_80139AFC++;
    if (D_80139AFC >= 25) {
        D_80139AFC = 0;
        D_80139AF4++;
        if (D_80139AF4 >= 80) {
            D_80139AFC = 0;
            D_80139AF8 = 3;
            return;
        }
    }
    if (!(D_800E6280.unk_F80 & 0x20) && D_80139AFC >= 4) {
        func_80135A48(((DisplayActor44 *)D_8011ECD0)->unk_26, ((DisplayActor44 *)D_8011ECD0)->unk_2A);
        func_801369F4(0x502);
        D_80139AF8 = 0;
        ((DisplayActor44 *)D_8011ECD0)[152].unk_01 = 3;
        D_80139AFC = 0;
        func_801337E0();
        for (i = 0; i < 10; i++) {
            if (D_80139B08[i] == 2) {
                v = D_8013984C[i].unk_00;
                D_80139AF8 = 2;
                if (v == 4) {
                    D_80139AF4 += 0x50;
                    D_80139B04++;
                } else {
                    D_80139AF4 += 0x10;
                    D_80139B00++;
                }
                D_8013984C[i].unk_00 = v | 0x80;
            } else if (D_80139B08[i] == 1) {
                v = D_8013984C[i].unk_00;
                D_80139AF8 = 2;
                if (v == 4) {
                    D_80139B04++;
                } else {
                    D_80139B00++;
                }
                D_8013984C[i].unk_00 = v | 0x80;
            }
        }
    }
}
