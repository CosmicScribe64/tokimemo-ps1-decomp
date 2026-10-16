void func_8013759C(void) {
    s16 var_s0;
    s16 var_s0_2;
    s32 temp_lo;
    s32 temp_t9;
    s32 temp_v0;
    u8 temp_t6;

    k_reset(0);
    temp_t6 = (((DisplayActor44 *)D_8011ECD0)[31].unk_07 - 1) & 0xFF;
    ((DisplayActor44 *)D_8011ECD0)[31].unk_07 = temp_t6;
    if (temp_t6 < 8U) {
        ((DisplayActor44 *)D_8011ECD0)[31].unk_07 = 0U;
        func_8004284C();
    }
    var_s0 = 4;
    ((DisplayActor44 *)D_8011ECD0)[30].unk_07 = (u8) ((DisplayActor44 *)D_8011ECD0)[31].unk_07;
    do {
        sprite_brightness(var_s0, ((DisplayActor44 *)D_8011ECD0)[31].unk_07);
        var_s0 += 1;
    } while (var_s0 < 0x13);
    var_s0_2 = 0;
    do {
        temp_lo = var_s0_2 * 0x44;
        var_s0_2 += 1;
        *(u8 *)((unsigned char *)((DisplayActor44 *)((u8 *)D_8011ECD0 + temp_lo)) + (6535)) = (u8) ((DisplayActor44 *)D_8011ECD0)[31].unk_07;
    } while (var_s0_2 < 0xE);
    temp_t9 = ((u32) (((DisplayActor44 *)D_8011ECD0)[31].unk_07 * 0xC0) >> 7) & 0xFF;
    temp_v0 = ((u32) (((DisplayActor44 *)D_8011ECD0)[31].unk_07 * 0xFF) >> 7) & 0xFF;
    _sprite_set_box_shade_tarao(-0xA0, -0x78, 0x140, 0xF0, 0xE, (temp_t9 << 0x10) | (temp_t9 << 8) | temp_v0, ((((u32) (((DisplayActor44 *)D_8011ECD0)[31].unk_07 << 6) >> 7) & 0xFF) << 0x10) | ((((u32) (((DisplayActor44 *)D_8011ECD0)[31].unk_07 << 7) >> 7) & 0xFF) << 8) | temp_v0);
    dtd_on(0xE);
}
