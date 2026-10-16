? func_80048514(?, s32, ?);                         /* extern */
s16 func_8007C51C();                                /* extern */

void func_8014472C(void) {
    s16 var_v1;

    if (func_800460EC() & 4) {
        var_v1 = func_8007C51C();
    } else {
        var_v1 = 0;
        if (((u32) D_800E6280.unk_F80 >> 0x10) != 0) {
            var_v1 = D_800E6280.unk_1100 & 0x3F;
        }
    }
    D_80125C80[D_800E6280.unk_10F8 & 3] = var_v1;
    if (func_800460EC() & 4) {
        if (!(*(u8 *)((unsigned char *)D_8011ECD0 + (6666)) & 1) || !(*(u8 *)((unsigned char *)D_8011ECD0 + (6667)) & 0x80)) {
            func_80048F64(0x62);
            *(s8 *)((unsigned char *)D_8011ECD0 + (6665)) = 3;
            *(u8 *)((unsigned char *)D_8011ECD0 + (6666)) = 0x11U;
            *(s32 *)((unsigned char *)D_8011ECD0 + (6720)) = 0x41000000;
            *(u8 *)((unsigned char *)D_8011ECD0 + (6667)) = 0xACU;
            *(s32 *)((unsigned char *)D_8011ECD0 + (6676)) = 0x801B27B0;
            *(s32 *)((unsigned char *)D_8011ECD0 + (6680)) = 0x801B27D8;
            *(s32 *)((unsigned char *)D_8011ECD0 + (6716)) = 0x801B27AC;
            *(s16 *)((unsigned char *)D_8011ECD0 + (6686)) = 1;
            *(s8 *)((unsigned char *)D_8011ECD0 + (6668)) = 8;
            *(s8 *)((unsigned char *)D_8011ECD0 + (6669)) = 0x80;
            *(s16 *)((unsigned char *)D_8011ECD0 + (6702)) = -0xA0;
            *(s16 *)((unsigned char *)D_8011ECD0 + (6706)) = -0x78;
            func_80048514(5, *(s32 *)0x801B27AC, 1);
        }
    } else {
        func_80048F64(0x62);
        *(u8 *)((unsigned char *)D_8011ECD0 + (6667)) = 0U;
        *(u8 *)((unsigned char *)D_8011ECD0 + (6666)) = 0U;
        *(s16 *)((unsigned char *)D_8011ECD0 + (6688)) = 0;
        *(s16 *)((unsigned char *)D_8011ECD0 + (6672)) = 0;
    }
}
