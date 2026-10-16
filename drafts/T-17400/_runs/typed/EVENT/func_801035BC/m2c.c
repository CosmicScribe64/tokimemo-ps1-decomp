void func_801035BC(void) {
    ? sp28;
    s16 var_v0;
    s16 var_v0_2;
    s32 temp_lo;
    s32 temp_lo_2;
    s32 var_at;
    void *temp_v1;

    M2C_MEMCPY_ALIGNED(&sp28, &D_801228E0, 0xA8);
    *(void (**)())((unsigned char *)(&sp28) + (168)) = (void (*)()) D_801228E0.f[0x2A].unk0;
    *(s32 *)((unsigned char *)(&sp28 + 0xA8) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_801228E0.f[0x2A]) + (4));
    func_80078950("%d\n", D_800B1AF6);
    *(? (**)(?))((unsigned char *)(sp + (D_800B1AF6 * 4)) + (40))(0x80);
    if (func_8004C984 == *(s32 *)((unsigned char *)(sp + (D_800B1AF6 * 4)) + (40))) {
        var_v0 = 0;
        do {
            temp_lo = var_v0 * 0x44;
            var_v0 += 1;
            *(u8 *)((unsigned char *)(D_800E9620 + temp_lo) + (6671)) = (u8) *(u8 *)((unsigned char *)D_800E9620 + (6535));
        } while (var_v0 < 8);
        var_at = (u8) D_800B1AF6 < 0x11U;
        if ((u8) *(u8 *)((unsigned char *)D_800E9620 + (6535)) < 9U) {
            var_v0_2 = 0;
            do {
                temp_lo_2 = var_v0_2 * 0x44;
                var_v0_2 += 1;
                temp_v1 = D_800E9620 + temp_lo_2;
                *(u8 *)((unsigned char *)temp_v1 + (6667)) = (u8) (*(u8 *)((unsigned char *)temp_v1 + (6667)) & 0x7F);
            } while (var_v0_2 < 8);
            goto block_6;
        }
    } else {
block_6:
        var_at = (u8) D_800B1AF6 < 0x11U;
    }
    if (var_at == 0) {
        if ((u8) D_800B1AF6 < 0x14U) {
            func_80103774();
        }
    }
}
