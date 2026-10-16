void func_80134B18(void) {
    s16 var_s0;
    u8 temp_t6;
    u8 var_v1;

    temp_t6 = (D_800E6280.unk_036 + 8) & 0xFF;
    var_v1 = temp_t6;
    if ((temp_t6 == 0) || (temp_t6 == 7)) {
        var_v1 = 0xFF;
    }
    D_800E6280.unk_038 = var_v1;
    D_800E6280.unk_037 = var_v1;
    *(u8 *)((unsigned char *)D_8011ECD0 + (6603)) = (u8) (*(u8 *)((unsigned char *)D_8011ECD0 + (6603)) + 4);
    D_800E6280.unk_036 = var_v1;
    *(u8 *)((unsigned char *)D_8011ECD0 + (6535)) = (u8) (*(u8 *)((unsigned char *)D_8011ECD0 + (6535)) + 4);
    var_s0 = 4;
    do {
        func_80084C98(var_s0, *(u8 *)((unsigned char *)D_8011ECD0 + (6535)));
        var_s0 += 1;
    } while (var_s0 < 0xA);
    if ((u8) *(u8 *)((unsigned char *)D_8011ECD0 + (6535)) >= 0x80U) {
        *(s8 *)((unsigned char *)D_8011ECD0 + (6530)) = 5;
        func_8004284C();
    }
}
