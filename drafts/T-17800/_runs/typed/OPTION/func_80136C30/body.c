void func_80136C30(void) {
    s16 sp54[6];
    s16 sp48[6];
    s16 sp3C[6];
    s16 sp30[6];
    s16 sp5A;
    s16 sp58;
    s16 sp56;
    s16 sp4E;
    s16 sp4C;
    s16 sp4A;
    s16 sp42;
    s16 sp40;
    s16 sp3E;
    s16 sp36;
    s16 sp34;
    s16 sp32;
    s16 temp_t4;

    if (D_800E6280.unk_110D == 0) {
        ((OptActor44 *)D_8011ECD0)[106].unk_18 = 6;
        ((OptActor44 *)D_8011ECD0)[106].unk_26 = -0xA0;
        ((OptActor44 *)D_8011ECD0)[106].unk_2A = -0x78;
        ((OptActor44 *)D_8011ECD0)[106].unk_03 = 0;
        ((OptActor44 *)D_8011ECD0)[106].unk_1C = 0x7FFF;
        D_800E6280.unk_110D = 1;
        ((OptActor44 *)D_8011ECD0)[106].unk_1A = 0x7FFF;
    } else {
        temp_t4 = ((s16) ((OptActor44 *)D_8011ECD0)[106].unk_1A / 17) * 0x10;
        ((OptActor44 *)D_8011ECD0)[106].unk_1C = (s16) (((s16) ((OptActor44 *)D_8011ECD0)->unk0 / 17) * 0x10);
        ((OptActor44 *)D_8011ECD0)[106].unk_1A = temp_t4;
        if (temp_t4 < 0x1001) {
            ((OptActor44 *)D_8011ECD0)[106].unk_1C = 0x1000;
            ((OptActor44 *)D_8011ECD0)[106].unk_1A = 0x1000;
            ((OptActor44 *)D_8011ECD0)[107].unk_18 = 0x34;
            ((OptActor44 *)D_8011ECD0)[107].unk_26 = -0x40;
            ((OptActor44 *)D_8011ECD0)[107].unk_2A = 0x18;
            ((OptActor44 *)D_8011ECD0)[107].unk_03 = (u8) (((OptActor44 *)D_8011ECD0)[107].unk_03 | 0x80);
            ((OptActor44 *)D_8011ECD0)[108].unk_18 = 0x36;
            ((OptActor44 *)D_8011ECD0)[108].unk_26 = -0x40;
            sp3C[0] = 0x60;
            ((OptActor44 *)D_8011ECD0)[108].unk_2A = 0x28;
            sp30[0] = 0x10;
            ((OptActor44 *)D_8011ECD0)[108].unk_03 = (u8) (((OptActor44 *)D_8011ECD0)[108].unk_03 | 0x80);
            ((OptActor44 *)D_8011ECD0)[109].unk_18 = 0x38;
            ((OptActor44 *)D_8011ECD0)[109].unk_26 = -0x40;
            sp3E = 0x60;
            ((OptActor44 *)D_8011ECD0)[109].unk_2A = 0x38;
            sp32 = 0x10;
            ((OptActor44 *)D_8011ECD0)[109].unk_03 = (u8) (((OptActor44 *)D_8011ECD0)[109].unk_03 | 0x80);
            ((OptActor44 *)D_8011ECD0)[110].unk_18 = 0x3A;
            ((OptActor44 *)D_8011ECD0)[110].unk_26 = -0x40;
            sp40 = 0x60;
            ((OptActor44 *)D_8011ECD0)[110].unk_2A = 0x48;
            sp34 = 0x10;
            ((OptActor44 *)D_8011ECD0)[110].unk_03 = (u8) (((OptActor44 *)D_8011ECD0)[110].unk_03 | 0x80);
            sp42 = 0x60;
            sp36 = 0x10;
            sp48[0] = 0x18;
            sp54[0] = -0x40;
            sp56 = -0x40;
            sp4A = 0x28;
            sp58 = -0x40;
            sp4C = 0x38;
            sp5A = -0x40;
            sp4E = 0x48;
            if (!(D_800E7D4F & 1) && !(D_800E7D50 & 1)) {
                sp3C[0] = 0;
                ((OptActor44 *)D_8011ECD0)[107].unk_03 = 0U;
            }
            if (func_80136F3C() == 0) {
                ((OptActor44 *)D_8011ECD0)[108].unk_03 = 0U;
                sp3E = 0;
                sp40 = 0;
                ((OptActor44 *)D_8011ECD0)[109].unk_03 = 0U;
            }
            if (D_800E7D37 == 0) {
                sp42 = 0;
                ((OptActor44 *)D_8011ECD0)[110].unk_03 = 0U;
            }
            menu_set(1, 4, sp54, sp48, sp3C, sp30);
            D_800E6280.unk_1094 = 0;
            func_8004284C();
        }
    }
    func_801324E8();
}
