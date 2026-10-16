void func_80137BB4(void) {
    u32 temp_hi;

    if (D_8013C35C == 1) {
        if (D_8013C360 == 0) {
            ((DisplayActor44 *)D_8011ECD0)[31].unk_16 = 7;
            func_801396A4(0, 0xFFU);
            D_80122D0C = 0;
            D_8013CAA0 = 0;
        }
    } else if (D_8013C35C == 0xE) {
        if ((D_8013CAA0 == 0) && ((u8) D_8013C364 == 0x32)) {
            D_8013CAA0 = 1;
            D_8013C364 = 0;
        }
        if (((u8) D_8013C364 == 1) && (((DisplayActor44 *)D_8011ECD0)[30].unk_16 == 0) && (D_8013CAA0 == 1)) {
            func_801396A4(1, 5U);
            D_80122D0C = 1;
        }
    } else if ((D_8013C35C == 0x11) && (D_8013C360 != 0)) {
        if (D_8013C360 == 1) {
            ((DisplayActor44 *)D_8011ECD0)[31].unk_16 = 8;
            func_801396A4(2, 1U);
            D_80122D0C = 1;
        } else if (D_8013C360 == 0x28) {
            func_801396A4(3, 5U);
        } else if (((DisplayActor44 *)D_8011ECD0)[30].unk_16 == 3) {
            if (((DisplayActor44 *)D_8011ECD0)[30].unk_18 == 5) {
                func_801396A4(2, 1U);
            } else {
                goto block_21;
            }
        } else {
block_21:
            if (D_8013C360 == 0x78) {
                func_801396A4(4, 5U);
            } else if ((((DisplayActor44 *)D_8011ECD0)[30].unk_16 == 4) && (((DisplayActor44 *)D_8011ECD0)[30].unk_18 == 7)) {
                func_801396A4(5, 1U);
                D_80122D0C = 0;
            }
        }
    }
    if (D_8013C35C >= 0x12) {
        temp_hi = (u32) D_800E6280.unk_10F8 % 240U;
        if (temp_hi == 0) {
            ((DisplayActor44 *)D_8011ECD0)[30].unk_18 = 0;
            ((DisplayActor44 *)D_8011ECD0)[30].unk_08 = 0;
            ((DisplayActor44 *)D_8011ECD0)[30].unk_16 = 6;
        }
        if (temp_hi >= 0xCU) {
            ((DisplayActor44 *)D_8011ECD0)[30].unk_18 = 0;
            ((DisplayActor44 *)D_8011ECD0)[30].unk_02 = 1;
            ((DisplayActor44 *)D_8011ECD0)[30].unk_08 = 0;
            ((DisplayActor44 *)D_8011ECD0)[30].unk_16 = 5;
        }
    }
    if (D_800E6280.unk_110A == 2) {
        ((DisplayActor44 *)D_8011ECD0)[30].unk_02 = 1;
        ((DisplayActor44 *)D_8011ECD0)[30].unk_16 = 5;
    }
    D_8013C360 += 1;
}
