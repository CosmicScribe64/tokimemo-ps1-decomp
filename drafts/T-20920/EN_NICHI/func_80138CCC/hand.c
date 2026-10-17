void func_80138CCC(void) {
    s32 i;
    s32 r1;
    s32 r2;
    s32 r3;
    EnHalf64 tbl;
    EnHalf13 girls;
    u8 n;
    u8 list[15];

    tbl = D_80139DA0;
    girls = D_80139E20;
    n = 0;
    for (i = 0; i < 11; i++) {
        if (D_800E6280.unk_1BC[i].unk_0C.f.b1 == 1) {
            list[n] = i;
            n++;
        }
    }
    if (D_800E7D10.flags[11] != 0 || D_800E6280.unk_1BC[11].unk_0C.f.f9 != 0) {
        list[n] = 11;
        n++;
    }
    if (D_800E7D10.flags[12] != 0) {
        list[n] = 12;
        n++;
    }
    for (i = 32; i < 48; i++) {
        tbl.value[i] = girls.value[list[func_800AE0D0() % n]];
    }
    for (i = 0; i < 4; i++) {
        ((DisplayFixActor44 *)D_8011ECD0)[105 + i].unk_24.h[1]++;
        if (((DisplayFixActor44 *)D_8011ECD0)[105 + i].unk_24.h[1] >= 0xC1) {
            r1 = func_800AE0D0() & 0x3F;
            ((DisplayFixActor44 *)D_8011ECD0)[105 + i].unk_24.h[1] = ((DisplayFixActor44 *)D_8011ECD0)[105 + (i + 1) % 4].unk_24.h[1] - 0x60;
            ((DisplayFixActor44 *)D_8011ECD0)[105 + i].unk_18 = tbl.value[r1];
            if ((r1 & 1) && r1 < 0x20) {
                ((DisplayFixActor44 *)D_8011ECD0)[105 + i].unk_28.h[1] = -0x38;
            } else {
                ((DisplayFixActor44 *)D_8011ECD0)[105 + i].unk_28.h[1] = -0x48;
            }
            ((DisplayFixActor44 *)D_8011ECD0)[105 + i].unk_38 = 0x01000000;
        }
    }
    for (i = 0; i < 4; i++) {
        ((DisplayFixActor44 *)D_8011ECD0)[109 + i].unk_24.w += 0xFFFF2000;
        if (((DisplayFixActor44 *)D_8011ECD0)[109 + i].unk_24.h[1] < -0xC0) {
            r2 = func_800AE0D0() & 0x3F;
            ((DisplayFixActor44 *)D_8011ECD0)[109 + i].unk_24.h[1] = ((DisplayFixActor44 *)D_8011ECD0)[109 + (i + 3) % 4].unk_24.h[1] + 0x60;
            ((DisplayFixActor44 *)D_8011ECD0)[109 + i].unk_18 = tbl.value[r2];
            if ((r2 & 1) && r2 < 0x20) {
                ((DisplayFixActor44 *)D_8011ECD0)[109 + i].unk_28.h[1] = -0x10;
            } else {
                ((DisplayFixActor44 *)D_8011ECD0)[109 + i].unk_28.h[1] = -0x20;
            }
            ((DisplayFixActor44 *)D_8011ECD0)[109 + i].unk_38 = 0x01000000;
        }
    }
    for (i = 0; i < 4; i++) {
        ((DisplayFixActor44 *)D_8011ECD0)[113 + i].unk_24.w += 0xC000;
        if (((DisplayFixActor44 *)D_8011ECD0)[113 + i].unk_24.h[1] >= 0xC1) {
            r3 = func_800AE0D0() & 0x3F;
            ((DisplayFixActor44 *)D_8011ECD0)[113 + i].unk_24.h[1] = ((DisplayFixActor44 *)D_8011ECD0)[113 + (i + 1) % 4].unk_24.h[1] - 0x60;
            ((DisplayFixActor44 *)D_8011ECD0)[113 + i].unk_18 = tbl.value[r3];
            if ((r3 & 1) && r3 < 0x20) {
                ((DisplayFixActor44 *)D_8011ECD0)[113 + i].unk_28.h[1] = 0x18;
            } else {
                ((DisplayFixActor44 *)D_8011ECD0)[113 + i].unk_28.h[1] = 8;
            }
            ((DisplayFixActor44 *)D_8011ECD0)[113 + i].unk_38 = 0x01000000;
        }
    }
}
