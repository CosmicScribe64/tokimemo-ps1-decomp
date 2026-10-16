extern ? D_80085368;

void func_80133E10(void) {
    ? sp2C;
    s32 temp_v1;
    s32 var_t1;
    u8 temp_a0;
    u8 temp_v0;
    u8 var_v0;

    var_v0 = D_800E6280.unk_110A;
    M2C_MEMCPY_ALIGNED(&sp2C, &D_8013C008, 0xAC);
    *(s32 *)((unsigned char *)(&sp2C + 0xA8) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_8013C008.f[0x2A]) + (4));
    if (var_v0 < 8U) {
        func_8006B900();
        var_v0 = D_800E6280.unk_110A;
        goto block_5;
    }
    if (var_v0 >= 0x1DU) {
        var_t1 = var_v0 * 4;
        if (var_v0 < 0x23U) {
            func_8006B900();
            var_v0 = D_800E6280.unk_110A;
            goto block_5;
        }
    } else {
block_5:
        var_t1 = var_v0 * 4;
    }
    *(? (**)(?))((unsigned char *)(sp + var_t1) + (44))(0x80);
    temp_v0 = D_800E6280.unk_110A;
    temp_v1 = *(s32 *)((unsigned char *)(sp + (temp_v0 * 4)) + (44));
    if (&D_80085368 == temp_v1) {
        *(u8 *)((unsigned char *)D_8011ECD0 + (6667)) = (u8) (*(u8 *)((unsigned char *)D_8011ECD0 + (6667)) | 0x80);
        *(u8 *)((unsigned char *)D_8011ECD0 + (6671)) = (u8) *(u8 *)((unsigned char *)D_8011ECD0 + (6535));
    }
    if ((temp_v0 >= 0xDU) && (temp_v0 < 0x16U)) {
        *(u8 *)((unsigned char *)D_8011ECD0 + (6671)) = (u8) *(u8 *)((unsigned char *)D_8011ECD0 + (6535));
    }
    temp_a0 = *(u8 *)((unsigned char *)D_8011ECD0 + (6535)) & 0xFF;
    if (func_800853FC == temp_v1) {
        *(u8 *)((unsigned char *)D_8011ECD0 + (6671)) = temp_a0;
        if (temp_a0 < 8U) {
            *(u8 *)((unsigned char *)D_8011ECD0 + (6667)) = (u8) (*(u8 *)((unsigned char *)D_8011ECD0 + (6667)) & 0xFF7F);
        }
    }
}
