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
    ((DisplayActor44 *)D_8011ECD0)[97].unk_07 = (u8) (((DisplayActor44 *)D_8011ECD0)[97].unk_07 + 4);
    D_800E6280.unk_036 = var_v1;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_07 = (u8) (((DisplayActor44 *)D_8011ECD0)[96].unk_07 + 4);
    var_s0 = 4;
    do {
        sprite_brightness(var_s0, ((DisplayActor44 *)D_8011ECD0)[96].unk_07);
        var_s0 += 1;
    } while (var_s0 < 0xA);
    if ((u8) ((DisplayActor44 *)D_8011ECD0)[96].unk_07 >= 0x80U) {
        ((DisplayActor44 *)D_8011ECD0)[96].unk_02 = 5;
        func_8004284C();
    }
}
