void func_80135480(void) {
    u8 index;
    ShFnTbl74 table;

    table = D_8013C2F0;
    index = D_800E6280.unk_110A;
    table.f[index](0x80);
    index = D_800E6280.unk_110A;
    if (D_80085368 == table.f[index]) {
        ((DisplayActor44 *)D_8011ECD0)[98].unk_03 |= 0x80;
    }
    if ((func_800853FC == table.f[index] || func_80135A80 == table.f[index]) && D_800B593C < 8) {
        ((DisplayActor44 *)D_8011ECD0)[98].unk_03 &= ~0x80;
        ((DisplayActor44 *)D_8011ECD0)[99].unk_03 &= ~0x80;
    }
    if ((index >= 0x16 && index < 0x1C) || (index >= 0x20 && index < 0x25)) {
        ((DisplayActor44 *)D_8011ECD0)[98].unk_07 = ((DisplayActor44 *)D_8011ECD0)[96].unk_07;
    }
    if (D_8013C2E4 != 0) {
        func_801360B0();
    }
}
