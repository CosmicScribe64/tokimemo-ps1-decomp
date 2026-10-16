void func_801419D0(void) {
    s16 temp_v0;

    M2C_MEMCPY_ALIGNED(&sp2C[0], &D_80146804, 0xA0);
    (&sp2C[0])[D_800E6280.unk_110A](0x80);
    if ((u8) D_800E6280.unk_110A < 0x10U) {
        *(u8 *)((unsigned char *)D_8011ECD0 + (6535)) = (u8) D_800B593C;
        if ((*(s16 *)((unsigned char *)D_8011ECD0 + (6552)) == 0) && (*(s16 *)((unsigned char *)D_8011ECD0 + (6536)) == 0)) {
            temp_v0 = *(s16 *)((unsigned char *)D_8011ECD0 + (6550)) + 1;
            *(s16 *)((unsigned char *)D_8011ECD0 + (6550)) = temp_v0;
            if (temp_v0 >= 3) {
                *(s16 *)((unsigned char *)D_8011ECD0 + (6550)) = 0;
            }
        }
    }
    func_80141D20();
}
