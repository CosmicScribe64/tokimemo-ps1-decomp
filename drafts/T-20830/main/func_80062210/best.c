void func_80062210(void) {
    s16 pad[4]; /* FAKE: unused pad grows the frame to the original 0x80; real source unknown. T-17010 */
    s16 x[2];
    s16 y[2];
    s16 w[2];
    s16 h[2];
    s32 i;
    s32 j;

    y[0] = 0x42; x[0] = -0x30; w[0] = 0xA0; h[0] = 0x14;
    y[1] = 0x52; x[1] = -0x30; w[1] = 0xA0; h[1] = 0x14;
    menu_set(0, 2, x, y, w, h);
    D_801230D0 = 0;
    (&D_800E6280.unk_036)[0] = (&D_800E6280.unk_037)[0] = (&D_800E6280.unk_038)[0] = 0xFF;
    func_80041584();
    (&D_800E6280.unk_03A)[0] = 0x80;
    func_8006BC28(0);
    func_8006BD6C(1);
    func_800623E4();
    for (i = 0, j = 0; i < 3; i++) {
        func_80048F64(i + 0x60);
        ((DisplayRecord44 *)D_8011ECD0)[i + 0x60].unk_18 = j;
        j += 2;
        ((DisplayRecord44 *)D_8011ECD0)[i + 0x60].unk_01 = 0xD;
        ((DisplayRecord44 *)D_8011ECD0)[i + 0x60].unk_02 = 0x40;
        ((DisplayRecord44 *)D_8011ECD0)[i + 0x60].unk_38 = 0x01000000;
        ((DisplayRecord44 *)D_8011ECD0)[i + 0x60].unk_03 = 0xA4;
        ((DisplayRecord44 *)D_8011ECD0)[i + 0x60].unk_0C = (u8 *)0x801EAB3C;
        ((DisplayRecord44 *)D_8011ECD0)[i + 0x60].unk_10 = (u8 *)0x801EAB6C;
        ((DisplayRecord44 *)D_8011ECD0)[i + 0x60].unk_34 = (u8 *)0x801EAB20;
        ((DisplayRecord44 *)D_8011ECD0)[i + 0x60].unk_04 = 8;
        ((DisplayRecord44 *)D_8011ECD0)[i + 0x60].unk_05 = 0x80;
        ((DisplayRecord44 *)D_8011ECD0)[i + 0x60].unk_26 = -0xA0;
        ((DisplayRecord44 *)D_8011ECD0)[i + 0x60].unk_2A = -0x78;
    }
    D_801206BE = -0x6E;
    D_80120702 = -0x78;
    D_801206BA = -0xAA;
    D_801206FE = -0xAA;
    func_8004284C();
}
