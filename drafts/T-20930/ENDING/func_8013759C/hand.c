void func_8013759C(void) {
    s16 i;
    u8 x;

    k_reset(0);
    ((DisplayActor44 *)D_8011ECD0)[31].unk_07 -= 1;
    if (((DisplayActor44 *)D_8011ECD0)[31].unk_07 < 8) {
        ((DisplayActor44 *)D_8011ECD0)[31].unk_07 = 0;
        func_8004284C();
    }
    ((DisplayActor44 *)D_8011ECD0)[30].unk_07 = ((DisplayActor44 *)D_8011ECD0)[31].unk_07;
    for (i = 4; i < 0x13; i++) {
        func_80084C98(i, ((DisplayActor44 *)D_8011ECD0)[31].unk_07);
    }
    for (i = 0; i < 0xE; i++) {
        ((DisplayActor44 *)D_8011ECD0)[i + 96].unk_07 = ((DisplayActor44 *)D_8011ECD0)[31].unk_07;
    }
    x = ((DisplayActor44 *)D_8011ECD0)[31].unk_07;
    _sprite_set_box_shade_tarao(-0xA0, -0x78, 0x140, 0xF0, 0xE,
                                ((u8)((x * 0xC0) >> 7) << 16) | ((u8)((x * 0xC0) >> 7) << 8) | (u8)((x * 0xFF) >> 7),
                                ((u8)((x * 0x40) >> 7) << 16) | ((u8)((x * 0x80) >> 7) << 8) | (u8)((x * 0xFF) >> 7));
    dtd_on(0xE);
}
