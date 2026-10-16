extern ? D_80085368;

void func_80135480(void) {
    s32 temp_v0;
    u8 temp_v1;

    M2C_MEMCPY_ALIGNED(&sp2C[0], &D_8013C2F0, 0x128);
    (&sp2C[0])[D_800E6280.unk_110A](0x80);
    temp_v1 = D_800E6280.unk_110A;
    temp_v0 = *(s32 *)((unsigned char *)(sp + (temp_v1 * 4)) + (44));
    if (&D_80085368 == temp_v0) {
        *(u8 *)((unsigned char *)D_8011ECD0 + (6667)) = (u8) (*(u8 *)((unsigned char *)D_8011ECD0 + (6667)) | 0x80);
    }
    if (((func_800853FC == temp_v0) || (func_80135A80 == temp_v0)) && ((u8) D_800B593C < 8U)) {
        *(u8 *)((unsigned char *)D_8011ECD0 + (6667)) = (u8) (*(u8 *)((unsigned char *)D_8011ECD0 + (6667)) & 0xFF7F);
        *(u8 *)((unsigned char *)D_8011ECD0 + (6735)) = (u8) (*(u8 *)((unsigned char *)D_8011ECD0 + (6735)) & 0xFF7F);
    }
    if (((temp_v1 >= 0x16U) && (temp_v1 < 0x1CU)) || ((temp_v1 >= 0x20U) && (temp_v1 < 0x25U))) {
        *(u8 *)((unsigned char *)D_8011ECD0 + (6671)) = (u8) *(u8 *)((unsigned char *)D_8011ECD0 + (6535));
    }
    if ((u8) D_8013C2E4 != 0) {
        func_801360B0();
    }
}
