extern s32 D_8012E694;

s32 func_80145384(void) {
    s32 var_v0;

    M2C_MEMCPY_ALIGNED(&sp28[0], &D_8014901C, 0xBC);
    var_v0 = (&sp28[0])[D_800F647A](0x3C);
    if ((u8) D_800F647A < 0x10U) {
        *(u8 *)((unsigned char *)D_80129F40 + (6535)) = (u8) D_801317EB;
        if ((*(s16 *)((unsigned char *)D_80129F40 + (6552)) == 0) && (*(s16 *)((unsigned char *)D_80129F40 + (6536)) == 0)) {
            var_v0 = (s32) (s16) (*(s16 *)((unsigned char *)D_80129F40 + (6550)) + 1);
            *(s16 *)((unsigned char *)D_80129F40 + (6550)) = (s16) var_v0;
            if (var_v0 >= 3) {
                *(s16 *)((unsigned char *)D_80129F40 + (6550)) = 0;
                var_v0 = 0;
            }
        }
    }
    if (D_8012E694 != 0) {
        *(u8 *)((unsigned char *)D_80129F40 + (6667)) = (u8) (*(u8 *)((unsigned char *)D_80129F40 + (6667)) | 0x80);
        *(u8 *)((unsigned char *)D_80129F40 + (6671)) = (u8) *(u8 *)((unsigned char *)D_80129F40 + (6535));
        if (((u32) D_800F6468 % 300U) == 0) {
            var_v0 = func_80145520();
            *(s8 *)((unsigned char *)D_80129F40 + (6666)) = 3;
        }
    } else {
        *(u8 *)((unsigned char *)D_80129F40 + (6667)) = (u8) (*(u8 *)((unsigned char *)D_80129F40 + (6667)) & 0xFF7F);
    }
    return var_v0;
}
