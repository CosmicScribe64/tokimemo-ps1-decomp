void func_80140040(void) {
    s16 i;
    s32 any;

    for (i = 0; i < 0xA; i++) {
        if (D_801470AC >= (u32)(i * 0x1E)) {
            *(s16 *)&D_8011ECD0[i * 0x44 + 0x1A7A] = -1;
            *(s16 *)&D_8011ECD0[i * 0x44 + 0x1A7E] = 1;
        }
        *(s32 *)&D_8011ECD0[i * 0x44 + 0x1A70] += *(s32 *)&D_8011ECD0[i * 0x44 + 0x1A78];
        *(s32 *)&D_8011ECD0[i * 0x44 + 0x1A74] += *(s32 *)&D_8011ECD0[i * 0x44 + 0x1A7C];
        if (*(s16 *)&D_8011ECD0[i * 0x44 + 0x1A76] >= 0xA1) {
            D_8011ECD0[i * 0x44 + 0x1A4F] &= ~0x80;
        }
    }
    any = 0;
    for (i = 0; i < 0xA; i++) {
        if (D_8011ECD0[i * 0x44 + 0x1A4F] & 0x80) {
            any = 1;
        }
    }
    if (any == 0) {
        func_800AE0D0();
    }
    D_801470AC += 1;
}
