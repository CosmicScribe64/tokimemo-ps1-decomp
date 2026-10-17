void func_80137BB4(void) {
    u32 remainder;

    if (D_8013C35C == 1) {
        if (D_8013C360 == 0) {
            *(s16 *)&D_8011ECD0[0x852] = 7;
            func_801396A4(0, 0xFF);
            D_80122D0C = 0;
            D_8013CAA0 = 0;
        }
    } else if (D_8013C35C == 0xE) {
        if (D_8013CAA0 == 0 && D_8013C364 == 0x32) {
            D_8013CAA0 = 1;
            D_8013C364 = 0;
        }
        if (D_8013C364 == 1 && D_8011F4DE == 0 && D_8013CAA0 == 1) {
            func_801396A4(1, 5);
            D_80122D0C = 1;
        }
    } else if (D_8013C35C == 0x11 && D_8013C360 != 0) {
        if (D_8013C360 == 1) {
            *(s16 *)&D_8011ECD0[0x852] = 8;
            func_801396A4(2, 1);
            D_80122D0C = 1;
        } else if (D_8013C360 == 0x28) {
            func_801396A4(3, 5);
        } else if (D_8011F4DE == 3 && D_8011F4E0 == 5) {
            func_801396A4(2, 1);
        } else if (D_8013C360 == 0x78) {
            func_801396A4(4, 5);
        } else if (D_8011F4DE == 4 && D_8011F4E0 == 7) {
            func_801396A4(5, 1);
            D_80122D0C = 0;
        }
    }
    if (D_8013C35C >= 0x12) {
        /* FAKE: dead test selects v1 and removes a remainder temporary (T-9410). */
        if (D_800E6280.unk_10F8) {
        }
        remainder = D_800E6280.unk_10F8 % 240;
        if (remainder == 0) {
            D_8011F4E0 = 0;
            D_8011F4D0 = 0;
            D_8011F4DE = 6;
        }
        if (remainder >= 0xCU) {
            D_8011F4E0 = 0;
            D_8011F4CA = 1;
            D_8011F4D0 = 0;
            D_8011F4DE = 5;
        }
    }
    if (D_800E6280.unk_110A == 2) {
        D_8011F4CA = 1;
        D_8011F4DE = 5;
    }
    D_8013C360 += 1;
}
