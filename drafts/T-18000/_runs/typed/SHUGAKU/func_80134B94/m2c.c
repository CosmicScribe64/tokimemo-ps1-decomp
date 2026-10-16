extern ? D_80085368;

void func_80134B94(void) {
    s32 temp_v0;

    M2C_MEMCPY_ALIGNED(&sp30[0], &D_8013C228, 0xA4);
    (&sp30[0])[D_800E6280.unk_110A](0x80);
    temp_v0 = *(s32 *)((unsigned char *)(sp + (D_800E6280.unk_110A * 4)) + (48));
    if (&D_80085368 == temp_v0) {
        *(u8 *)((unsigned char *)D_8011ECD0 + (6667)) = (u8) (*(u8 *)((unsigned char *)D_8011ECD0 + (6667)) | 0x80);
        *(u8 *)((unsigned char *)D_8011ECD0 + (6671)) = (u8) *(u8 *)((unsigned char *)D_8011ECD0 + (6535));
    }
    if (func_800853FC == temp_v0) {
        *(u8 *)((unsigned char *)D_8011ECD0 + (6671)) = (u8) *(u8 *)((unsigned char *)D_8011ECD0 + (6535));
        if ((u8) *(u8 *)((unsigned char *)D_8011ECD0 + (6535)) < 9U) {
            *(u8 *)((unsigned char *)D_8011ECD0 + (6667)) = (u8) (*(u8 *)((unsigned char *)D_8011ECD0 + (6667)) & 0xFF7F);
        }
    }
}
