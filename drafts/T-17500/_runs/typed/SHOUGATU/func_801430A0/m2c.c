s32 func_801430A0(void) {
    s32 var_v0;

    M2C_MEMCPY_ALIGNED(&sp2C[0], &D_80146970, 0xC0);
    var_v0 = (&sp2C[0])[D_800E6280.unk_110A](0x3C);
    if ((u8) D_800E6280.unk_110A < 0x11U) {
        *(u8 *)((unsigned char *)D_8011ECD0 + (6535)) = (u8) D_800B593C;
        if ((*(s16 *)((unsigned char *)D_8011ECD0 + (6552)) == 0) && (*(s16 *)((unsigned char *)D_8011ECD0 + (6536)) == 0)) {
            var_v0 = (s32) (s16) (*(s16 *)((unsigned char *)D_8011ECD0 + (6550)) + 1);
            *(s16 *)((unsigned char *)D_8011ECD0 + (6550)) = (s16) var_v0;
            if (var_v0 >= 3) {
                *(s16 *)((unsigned char *)D_8011ECD0 + (6550)) = 0;
                var_v0 = 0;
            }
        }
    }
    if (D_80122D04 != 0) {
        *(u8 *)((unsigned char *)D_8011ECD0 + (6667)) = (u8) (*(u8 *)((unsigned char *)D_8011ECD0 + (6667)) | 0x80);
        *(u8 *)((unsigned char *)D_8011ECD0 + (6671)) = (u8) *(u8 *)((unsigned char *)D_8011ECD0 + (6535));
        if (((u32) D_800E6280.unk_10F8 % 300U) == 0) {
            var_v0 = func_80143228();
            *(s8 *)((unsigned char *)D_8011ECD0 + (6666)) = 3;
        }
    } else {
        *(u8 *)((unsigned char *)D_8011ECD0 + (6667)) = (u8) (D_8011ECD0->unk_00 & 0xFF7F);
    }
    return var_v0;
}
