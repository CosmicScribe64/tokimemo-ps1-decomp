s32 func_80015A64();                                /* extern */
s32 func_800458E0();                                /* extern */

void func_800FC040(void) {
    s32 var_v1;

    M2C_MEMCPY_ALIGNED(&sp28[0], &D_801212B8, 0xA8);
    (&sp28[0])[D_800B1AF6](0x80);
    if (D_800EECE0 != 0) {
        *(u8 *)((unsigned char *)D_800E9620 + (6735)) = (u8) (*(u8 *)((unsigned char *)D_800E9620 + (6735)) | 0x80);
        *(u8 *)((unsigned char *)D_800E9620 + (6739)) = (u8) D_800F19AB;
        if ((u8) D_801212B4 == 0) {
            *(s8 *)((unsigned char *)D_800E9620 + (6734)) = 3;
        }
        if ((*(s16 *)((unsigned char *)D_800E9620 + (6756)) == 6) && ((u8) D_801212B4 == 0)) {
            *(s8 *)((unsigned char *)D_800E9620 + (6734)) = 0;
        }
        var_v1 = 0;
        if (func_80015A64() & 4) {
            var_v1 = func_800458E0();
        }
        if (var_v1 != 0) {
            if ((u8) D_801212B4 == 0) {
                *(s8 *)((unsigned char *)D_800E9620 + (6734)) = 3;
                D_801212B4 = 1;
            }
        }
    } else {
        *(u8 *)((unsigned char *)D_800E9620 + (6735)) = (u8) (D_800E9620->unk_00 & 0xFF7F);
    }
}
