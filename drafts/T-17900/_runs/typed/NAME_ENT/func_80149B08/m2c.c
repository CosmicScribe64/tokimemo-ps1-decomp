s32 func_8007C51C(?);                               /* extern */

void func_80149B08(void) {
    s32 sp2C;
    s32 var_a0;
    s32 var_a1;

    var_a0 = 0;
    if (func_800460EC() & 4) {
        var_a0 = func_8007C51C(0);
    }
    D_8014D1B4[D_800E6280.unk_10F8 & 3] = var_a0;
    D_8014D1B0 = var_a0;
    var_a1 = (s32) ((s32) (D_8014D1B4[3] + *D_8014D1B4 + D_8014D1B4[1] + D_8014D1B4[2]) / 4) / 10;
    if (var_a1 >= 4) {
        var_a1 = 3;
    }
    sp2C = var_a1;
    func_80048F64(0x61);
    if (func_800460EC() & 4) {
        *(s8 *)((unsigned char *)D_8011ECD0 + (6597)) = 9;
        *(s8 *)((unsigned char *)D_8011ECD0 + (6598)) = 0x40;
        *(s32 *)((unsigned char *)D_8011ECD0 + (6652)) = 0x41000000;
        *(s8 *)((unsigned char *)D_8011ECD0 + (6599)) = 0xA4;
        *(s32 *)((unsigned char *)D_8011ECD0 + (6608)) = 0x80180258;
        *(s32 *)((unsigned char *)D_8011ECD0 + (6612)) = 0x80180298;
        *(s32 *)((unsigned char *)D_8011ECD0 + (6648)) = 0x8018024C;
        *(s16 *)((unsigned char *)D_8011ECD0 + (6620)) = (s16) (var_a1 + 6);
        *(s8 *)((unsigned char *)D_8011ECD0 + (6601)) = 0x80;
        *(s16 *)((unsigned char *)D_8011ECD0 + (6634)) = (s16) *(s16 *)((unsigned char *)D_8011ECD0 + (6566));
        *(s16 *)((unsigned char *)D_8011ECD0 + (6638)) = -0x78;
        *(s8 *)((unsigned char *)D_8011ECD0 + (6665)) = 9;
        *(s8 *)((unsigned char *)D_8011ECD0 + (6666)) = 0x40;
        *(s32 *)((unsigned char *)D_8011ECD0 + (6720)) = 0x41000000;
        *(s8 *)((unsigned char *)D_8011ECD0 + (6667)) = 0xA4;
        *(s32 *)((unsigned char *)D_8011ECD0 + (6676)) = 0x80180258;
        *(s32 *)((unsigned char *)D_8011ECD0 + (6680)) = 0x80180298;
        *(s32 *)((unsigned char *)D_8011ECD0 + (6716)) = 0x8018024C;
        *(s16 *)((unsigned char *)D_8011ECD0 + (6688)) = (s16) (var_a1 + 0xA);
        *(s8 *)((unsigned char *)D_8011ECD0 + (6669)) = 0x80;
        *(s16 *)((unsigned char *)D_8011ECD0 + (6702)) = (s16) *(s16 *)((unsigned char *)D_8011ECD0 + (6566));
        *(s16 *)((unsigned char *)D_8011ECD0 + (6706)) = -0x78;
        *(s8 *)((unsigned char *)D_8011ECD0 + (6802)) = 0;
    }
}
