void func_80155E40(void) {
    ? sp2C;
    s16 var_v0;
    s16 var_v0_2;
    s32 temp_lo;
    s32 temp_lo_2;
    s32 var_at;
    void *temp_v1;

    M2C_MEMCPY_ALIGNED(&sp2C, &D_8015FA68, 0xB4);
    if ((((D_800E6280.unk_03E == 0x5F) && (D_800E6280.unk_03F == 7) && (D_800E6280.unk_040 == 0x17)) || ((D_800E6280.unk_03E == 0x60) && (D_800E6280.unk_03F == 7) && (D_800E6280.unk_040 == 0x1C)) || ((D_800E6280.unk_03E == 0x61) && (D_800E6280.unk_03F == 7) && (D_800E6280.unk_040 == 0x1B))) && (D_800B593C == 0x80) && (D_80122D04 == 0) && ((u8) D_800E6280.unk_110A >= 0x21U)) {
        func_8006B900();
    }
    *(? (**)(?))((unsigned char *)(sp + (D_800E6280.unk_110A * 4)) + (44))(0x80);
    if (func_800853FC == *(s32 *)((unsigned char *)(sp + (D_800E6280.unk_110A * 4)) + (44))) {
        var_v0 = 0;
        do {
            temp_lo = var_v0 * 0x44;
            var_v0 += 1;
            *(u8 *)((unsigned char *)(D_8011ECD0 + temp_lo) + (6671)) = (u8) *(u8 *)((unsigned char *)D_8011ECD0 + (6535));
        } while (var_v0 < 8);
        var_at = (u8) D_800E6280.unk_110A < 0x11U;
        if ((u8) *(u8 *)((unsigned char *)D_8011ECD0 + (6535)) < 9U) {
            var_v0_2 = 0;
            do {
                temp_lo_2 = var_v0_2 * 0x44;
                var_v0_2 += 1;
                temp_v1 = D_8011ECD0 + temp_lo_2;
                *(u8 *)((unsigned char *)temp_v1 + (6667)) = (u8) (*(u8 *)((unsigned char *)temp_v1 + (6667)) & 0x7F);
            } while (var_v0_2 < 8);
            goto block_19;
        }
    } else {
block_19:
        var_at = (u8) D_800E6280.unk_110A < 0x11U;
    }
    if (var_at == 0) {
        if ((u8) D_800E6280.unk_110A < 0x14U) {
            func_801560E0();
        }
    }
}
