void func_80137BB4(void) {
    u32 temp_hi;

    if (D_8013C35C == 1) {
        if (D_8013C360 == 0) {
            D_8011F522 = 7;
            func_801396A4(0, 0xFFU);
            D_80122D0C = 0;
            D_8013CAA0 = 0;
        }
    } else if (D_8013C35C == 0xE) {
        if ((D_8013CAA0 == 0) && ((u8) D_8013C364 == 0x32)) {
            D_8013CAA0 = 1;
            D_8013C364 = 0;
        }
        if (((u8) D_8013C364 == 1) && (D_8011F4DE == 0) && (D_8013CAA0 == 1)) {
            func_801396A4(1, 5U);
            D_80122D0C = 1;
        }
    } else if ((D_8013C35C == 0x11) && (D_8013C360 != 0)) {
        if (D_8013C360 == 1) {
            D_8011F522 = 8;
            func_801396A4(2, 1U);
            D_80122D0C = 1;
        } else if (D_8013C360 == 0x28) {
            func_801396A4(3, 5U);
        } else if (D_8011F4DE == 3) {
            if (D_8011F4E0 == 5) {
                func_801396A4(2, 1U);
            } else {
                goto block_21;
            }
        } else {
block_21:
            if (D_8013C360 == 0x78) {
                func_801396A4(4, 5U);
            } else if ((D_8011F4DE == 4) && (D_8011F4E0 == 7)) {
                func_801396A4(5, 1U);
                D_80122D0C = 0;
            }
        }
    }
    if (D_8013C35C >= 0x12) {
        temp_hi = (u32) D_800E7378 % 240U;
        if (temp_hi == 0) {
            D_8011F4E0 = 0;
            D_8011F4D0 = 0;
            D_8011F4DE = 6;
        }
        if (temp_hi >= 0xCU) {
            D_8011F4E0 = 0;
            D_8011F4CA = 1;
            D_8011F4D0 = 0;
            D_8011F4DE = 5;
        }
    }
    if (D_800E738A == 2) {
        D_8011F4CA = 1;
        D_8011F4DE = 5;
    }
    D_8013C360 += 1;
}
