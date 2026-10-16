void func_8013759C(void) {
    s16 i;
    u8 x;

    k_reset(0);
    D_8011F513 -= 1;
    if (D_8011F513 < 8) {
        D_8011F513 = 0;
        func_8004284C();
    }
    D_8011F4CF = D_8011F513;
    for (i = 4; i < 0x13; i++) {
        func_80084C98(i, D_8011F513);
    }
    for (i = 0; i < 0xE; i++) {
        D_8011ECD0[i * 0x44 + 0x1987] = D_8011F513;
    }
    x = D_8011F513;
    _sprite_set_box_shade_tarao(-0xA0, -0x78, 0x140, 0xF0, 0xE,
                                ((u8)((x * 0xC0) >> 7) << 16) | ((u8)((x * 0xC0) >> 7) << 8) | (u8)((x * 0xFF) >> 7),
                                ((u8)((x * 0x40) >> 7) << 16) | ((u8)((x * 0x80) >> 7) << 8) | (u8)((x * 0xFF) >> 7));
    dtd_on(0xE);
}
