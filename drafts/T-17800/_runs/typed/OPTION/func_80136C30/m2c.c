void func_80136C30(void) {
    s16 sp5A;
    s16 sp58;
    s16 sp56;
    s16 sp54;
    s16 sp4E;
    s16 sp4C;
    s16 sp4A;
    s16 sp48;
    s16 sp42;
    s16 sp40;
    s16 sp3E;
    s16 sp3C;
    s16 sp36;
    s16 sp34;
    s16 sp32;
    s16 sp30;
    s16 temp_t4;

    if (D_800E6280.unk_110D == 0) {
        *(s16 *)((unsigned char *)D_8011ECD0 + (7232)) = 6;
        *(s16 *)((unsigned char *)D_8011ECD0 + (7246)) = -0xA0;
        *(s16 *)((unsigned char *)D_8011ECD0 + (7250)) = -0x78;
        *(s8 *)((unsigned char *)D_8011ECD0 + (7211)) = 0;
        *(s16 *)((unsigned char *)D_8011ECD0 + (7236)) = 0x7FFF;
        D_800E6280.unk_110D = 1;
        *(s16 *)((unsigned char *)D_8011ECD0 + (7234)) = 0x7FFF;
    } else {
        temp_t4 = ((s16) *(s16 *)((unsigned char *)D_8011ECD0 + (7234)) / 17) * 0x10;
        *(s16 *)((unsigned char *)D_8011ECD0 + (7236)) = (s16) (((s16) D_8011ECD0->unk0 / 17) * 0x10);
        *(s16 *)((unsigned char *)D_8011ECD0 + (7234)) = temp_t4;
        if (temp_t4 < 0x1001) {
            *(s16 *)((unsigned char *)D_8011ECD0 + (7236)) = 0x1000;
            *(s16 *)((unsigned char *)D_8011ECD0 + (7234)) = 0x1000;
            *(s16 *)((unsigned char *)D_8011ECD0 + (7300)) = 0x34;
            *(s16 *)((unsigned char *)D_8011ECD0 + (7314)) = -0x40;
            *(s16 *)((unsigned char *)D_8011ECD0 + (7318)) = 0x18;
            *(u8 *)((unsigned char *)D_8011ECD0 + (7279)) = (u8) (*(u8 *)((unsigned char *)D_8011ECD0 + (7279)) | 0x80);
            *(s16 *)((unsigned char *)D_8011ECD0 + (7368)) = 0x36;
            *(s16 *)((unsigned char *)D_8011ECD0 + (7382)) = -0x40;
            sp3C = 0x60;
            *(s16 *)((unsigned char *)D_8011ECD0 + (7386)) = 0x28;
            sp30 = 0x10;
            *(u8 *)((unsigned char *)D_8011ECD0 + (7347)) = (u8) (*(u8 *)((unsigned char *)D_8011ECD0 + (7347)) | 0x80);
            *(s16 *)((unsigned char *)D_8011ECD0 + (7436)) = 0x38;
            *(s16 *)((unsigned char *)D_8011ECD0 + (7450)) = -0x40;
            sp3E = 0x60;
            *(s16 *)((unsigned char *)D_8011ECD0 + (7454)) = 0x38;
            sp32 = 0x10;
            *(u8 *)((unsigned char *)D_8011ECD0 + (7415)) = (u8) (*(u8 *)((unsigned char *)D_8011ECD0 + (7415)) | 0x80);
            *(s16 *)((unsigned char *)D_8011ECD0 + (7504)) = 0x3A;
            *(s16 *)((unsigned char *)D_8011ECD0 + (7518)) = -0x40;
            sp40 = 0x60;
            *(s16 *)((unsigned char *)D_8011ECD0 + (7522)) = 0x48;
            sp34 = 0x10;
            *(u8 *)((unsigned char *)D_8011ECD0 + (7483)) = (u8) (*(u8 *)((unsigned char *)D_8011ECD0 + (7483)) | 0x80);
            sp42 = 0x60;
            sp36 = 0x10;
            sp48 = 0x18;
            sp54 = -0x40;
            sp56 = -0x40;
            sp4A = 0x28;
            sp58 = -0x40;
            sp4C = 0x38;
            sp5A = -0x40;
            sp4E = 0x48;
            if (!(D_800E7D4F & 1) && !(D_800E7D50 & 1)) {
                sp3C = 0;
                *(u8 *)((unsigned char *)D_8011ECD0 + (7279)) = 0U;
            }
            if (func_80136F3C() == 0) {
                *(u8 *)((unsigned char *)D_8011ECD0 + (7347)) = 0U;
                sp3E = 0;
                sp40 = 0;
                *(u8 *)((unsigned char *)D_8011ECD0 + (7415)) = 0U;
            }
            if (D_800E7D37 == 0) {
                sp42 = 0;
                *(u8 *)((unsigned char *)D_8011ECD0 + (7483)) = 0U;
            }
            func_8004F870(1, 4, &sp54, &sp48, &sp3C, &sp30);
            D_800E6280.unk_1094 = 0;
            func_8004284C();
        }
    }
    func_801324E8();
}
