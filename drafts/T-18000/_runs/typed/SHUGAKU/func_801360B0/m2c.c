void func_801360B0(void) {
    s16 temp_t1;
    s16 temp_t1_2;
    s32 temp_t5;
    s32 temp_t9;
    u32 var_v1;

    if ((*(s16 *)((unsigned char *)D_8011ECD0 + (6754)) == 5) && !(*(u8 *)((unsigned char *)D_8011ECD0 + (6735)) & 0x80)) {
        temp_t1 = D_8013C2E8 + 1;
        D_8013C2E8 = temp_t1;
        if (temp_t1 >= 0x29) {
            func_80044750(0x500);
            D_8013C2E8 = 0;
            *(s16 *)((unsigned char *)D_8011ECD0 + (6754)) = 6;
            *(s16 *)((unsigned char *)D_8011ECD0 + (6756)) = 0;
            *(s16 *)((unsigned char *)D_8011ECD0 + (6740)) = 0;
            *(u8 *)((unsigned char *)D_8011ECD0 + (6735)) = (u8) (*(u8 *)((unsigned char *)D_8011ECD0 + (6735)) | 0x80);
            *(s8 *)((unsigned char *)D_8011ECD0 + (6734)) = 1;
            *(s32 *)((unsigned char *)D_8011ECD0 + (6776)) = 0x64000;
            *(s32 *)((unsigned char *)D_8011ECD0 + (6780)) = 0x12000;
        }
    }
    if ((*(u8 *)((unsigned char *)D_8011ECD0 + (6735)) & 0x80) && (*(s16 *)((unsigned char *)D_8011ECD0 + (6754)) == 6)) {
        temp_t5 = *(s32 *)((unsigned char *)D_8011ECD0 + (6776)) - 0x1800;
        *(s32 *)((unsigned char *)D_8011ECD0 + (6776)) = temp_t5;
        *(s32 *)((unsigned char *)D_8011ECD0 + (6768)) = (s32) (*(s32 *)((unsigned char *)D_8011ECD0 + (6768)) + temp_t5);
        temp_t9 = *(s32 *)((unsigned char *)D_8011ECD0 + (6780)) - 0x600;
        *(s32 *)((unsigned char *)D_8011ECD0 + (6780)) = temp_t9;
        *(s32 *)((unsigned char *)D_8011ECD0 + (6772)) = (s32) (*(s32 *)((unsigned char *)D_8011ECD0 + (6772)) + temp_t9);
        if (*(s16 *)((unsigned char *)D_8011ECD0 + (6770)) < -0xA0) {
            *(u8 *)((unsigned char *)D_8011ECD0 + (6735)) = (u8) (*(u8 *)((unsigned char *)D_8011ECD0 + (6735)) & 0xFF7F);
        }
    }
    if (*(s16 *)((unsigned char *)D_8011ECD0 + (6770)) < -0xA0) {
        D_800B5BD4 = 9;
        if (D_8013C2E8 == 0x64) {
            func_80135B68();
        }
        if (D_8013C2E8 == 0xA) {
            func_80044750(0x502);
        }
        if ((*(s16 *)((unsigned char *)D_8011ECD0 + (6688)) == 1) && (*(s16 *)((unsigned char *)D_8011ECD0 + (6672)) == 0x21)) {
            func_80135BB8();
        }
        temp_t1_2 = D_8013C2E8 + 1;
        D_8013C2E8 = temp_t1_2;
        if (temp_t1_2 >= 0xA6) {
            D_8013C2E8 = 0xA5;
        }
        var_v1 = D_8013C2E8 + 0x80;
        if (var_v1 >= 0x100U) {
            var_v1 = 0xFF;
        }
        if (var_v1 == 0xFF) {
            if (D_800B593C == 0) {
                var_v1 = 0;
            } else {
                var_v1 = (D_800B593C * 2) - 1;
            }
        }
        func_80046AC8(0x25, 0, 0x1FF, (s32) (s16) ((u32) (D_8013C2E8 * D_8013C2E8) >> 1), -0xA0, -0x78, (var_v1 << 0x10) | (var_v1 << 8) | var_v1 | 0x10000000, 0xA);
        func_800494BC(0, 0, 0xA, 0, 0);
    }
}
