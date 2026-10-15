extern s32 D_800E6480;

void func_80137E40(void) {
    s32 temp_v0;

    if (D_8013C35C == 1) {
        if (D_8013C360 == 0) {
            D_8011F522 = 4;
            func_801396A4(0, 0xFFU);
            D_80122D0C = 0;
        }
    } else {
        temp_v0 = (((u32) (D_800E6480 << 0x11) >> 0x1F) != 0) * 2;
        if (D_8013C35C == (temp_v0 + 0x13)) {
            if (D_8013C360 == 1) {
                D_8011F522 = 5;
                func_801396A4(1, 1U);
                D_80122D0C = 1;
            }
        } else if (D_8013C35C == (temp_v0 + 0x16)) {
            if (D_8013C360 == 1) {
                func_801396A4(2, 0U);
                D_80122D0C = 0;
            }
        } else if ((D_8013C35C == (temp_v0 + 0x17)) && (D_8013C360 == 1)) {
            func_801396A4(3, 5U);
            D_80122D0C = 1;
        }
    }
    D_8013C360 += 1;
}
