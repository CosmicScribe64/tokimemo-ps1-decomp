void func_801387B0(void) {
    s32 temp_v1;

    if (D_8013C35C == 1) {
        if (D_8013C360 == 0) {
            ((DisplayActor44 *)D_8011ECD0)[31].unk_16 = 7;
            func_801396A4(0, 0xFFU);
            D_80122D0C = 0;
            D_8013CAA0 = 0;
        }
    } else if (D_8013C35C == 8) {
        if (D_8013C360 == 0xC8) {
            func_801396A4(1, 5U);
            D_80122D0C = 1;
        } else if ((((DisplayActor44 *)D_8011ECD0)[30].unk_16 == 1) && (((DisplayActor44 *)D_8011ECD0)[30].unk_18 == 2) && (((DisplayActor44 *)D_8011ECD0)[30].unk_08 == 1)) {
            func_801396A4(2, 0U);
            D_80122D0C = 0;
        }
    } else {
        temp_v1 = D_800E6280.unk_53C[6] & 1;
        if (D_8013C35C == (0xC - (temp_v1 == 1))) {
            if ((D_8013CAA0 == 0) && ((u8) D_8013C364 == 0xA)) {
                D_8013CAA0 = 1;
                D_8013C364 = 0;
            }
            if (((u8) D_8013C364 == 0x1E) && (((DisplayActor44 *)D_8011ECD0)[30].unk_16 == 2) && (D_8013CAA0 == 1)) {
                func_801396A4(3, 0U);
                ((DisplayActor44 *)D_8011ECD0)[31].unk_16 = 8;
            }
        } else if (D_8013C35C == (0xE - (temp_v1 == 1))) {
            if (D_8013C360 == 0) {
                func_801396A4(0, 0U);
            }
        } else if (D_8013C35C == (0x13 - (temp_v1 == 1))) {
            if (D_8013C360 == 0) {
                ((DisplayActor44 *)D_8011ECD0)[31].unk_16 = 7;
            }
        } else if (D_8013C35C == (0x16 - (temp_v1 == 1))) {
            if (D_8013C360 == 0) {
                ((DisplayActor44 *)D_8011ECD0)[31].unk_16 = 8;
            }
        } else if (D_8013C35C == 0x18) {
            if (D_8013C360 == 1) {
                func_801396A4(4, 5U);
                D_80122D0C = 1;
            }
        } else if (D_8013C35C == 0x1A) {
            if (D_8013C360 == 0) {
                ((DisplayActor44 *)D_8011ECD0)[31].unk_16 = 7;
            }
        } else if ((D_8013C35C == 0x1C) && (D_8013C360 != 0)) {
            if (D_8013C360 == 1) {
                func_801396A4(5, 5U);
            } else if ((((DisplayActor44 *)D_8011ECD0)[30].unk_16 == 5) && (((DisplayActor44 *)D_8011ECD0)[30].unk_18 == 4) && (((DisplayActor44 *)D_8011ECD0)[30].unk_08 == 0xD)) {
                func_801396A4(6, 1U);
            }
        }
    }
    D_8013C360 += 1;
}
