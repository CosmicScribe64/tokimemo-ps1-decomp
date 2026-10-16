void func_801352B0(void) {
    s32 sp30;
    s32 sp2C;
    s32 temp_v1;

    if ((u8) D_800E6280.unk_03C == 2) {
        D_800E6280.unk_03C = 1;
    }
    temp_v1 = D_800E6280.unk_1104.w;
    sp30 = (s32) D_800E6280.unk_110A;
    if (!(D_800E6280.unk_F88 & 0x600060) || (*(s16 *)((unsigned char *)D_8011ECD0 + (6702)) < -0xBF)) {
        sp2C = temp_v1;
        func_801343B0();
    }
    D_800E6280.unk_1104.w = temp_v1 + 1;
    if (*(s16 *)((unsigned char *)D_8011ECD0 + (6702)) >= -0xBF) {
        if (!(D_800E6280.unk_1104.w & 1) && (func_8004ECB4() != 0) && (((u8) D_800E6280.unk_03C == 0) || (func_80046094() == 9))) {
            *(s16 *)((unsigned char *)D_8011ECD0 + (6702)) = (s16) (*(s16 *)((unsigned char *)D_8011ECD0 + (6702)) - 1);
        }
        *(u8 *)((unsigned char *)D_8011ECD0 + (139)) = (u8) (*(u8 *)((unsigned char *)D_8011ECD0 + (139)) & 0x7F);
    }
    if (sp30 != D_800E6280.unk_1104.w) {
        if (*(s16 *)((unsigned char *)D_8011ECD0 + (6702)) >= -0xBF) {
            D_800E6280.unk_110A = (u8) sp30;
            D_800CA2E4 = 9;
            D_800CA2E8 = 0;
        } else {
            D_800CA2E4 = 9;
            D_800CA2E8 = 1;
        }
    }
}
