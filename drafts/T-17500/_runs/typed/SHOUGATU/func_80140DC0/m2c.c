void func_80140DC0(void) {
    s16 temp_v0;

    if (D_800E6280.unk_110A == 0) {
        *(&D_800E6280 + (D_800E6280.unk_F5F * 0x38)) = 0x46;
        func_800847B8(D_800E6280.unk_F5F);
    }
    switch (D_800E6280.unk_F5F) {
    case 0:
        func_80046500();
        break;
    case 1:
        func_80140F54();
        break;
    case 2:
        func_8014102C();
        break;
    case 3:
        func_80141180();
        break;
    case 4:
        func_80141224();
        break;
    case 5:
        func_80141358();
        break;
    case 6:
        func_801413F4();
        break;
    case 7:
        func_801414E0();
        break;
    case 8:
        func_801415C8();
        break;
    case 9:
        func_80141678();
        break;
    case 10:
        func_80046500();
        break;
    }
    *(u8 *)((unsigned char *)D_8011ECD0 + (6535)) = (u8) D_800B593C;
    if ((*(s16 *)((unsigned char *)D_8011ECD0 + (6552)) == 0) && (*(s16 *)((unsigned char *)D_8011ECD0 + (6536)) == 0)) {
        temp_v0 = *(s16 *)((unsigned char *)D_8011ECD0 + (6550)) + 1;
        *(s16 *)((unsigned char *)D_8011ECD0 + (6550)) = temp_v0;
        if (temp_v0 >= 3) {
            *(s16 *)((unsigned char *)D_8011ECD0 + (6550)) = 0;
        }
    }
}
