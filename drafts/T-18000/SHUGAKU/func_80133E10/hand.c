void func_80133E10(void) {
    u8 value;
    u8 index;
    ShFnTbl44 table;

    table = D_8013C008;
    if (D_800E6280.unk_110A < 8) {
        func_8006B900();
    } else if (D_800E6280.unk_110A >= 0x1D && D_800E6280.unk_110A < 0x23) {
        func_8006B900();
    }
    index = D_800E6280.unk_110A;
    table.f[index](0x80);
    index = D_800E6280.unk_110A;
    if (func_80085368 == table.f[index]) {
        ((DisplayActor44 *)D_8011ECD0)[98].unk_03 |= 0x80;
        ((DisplayActor44 *)D_8011ECD0)[98].unk_07 = ((DisplayActor44 *)D_8011ECD0)[96].unk_07;
    }
    value = ((DisplayActor44 *)D_8011ECD0)[96].unk_07;
    if (index >= 0xD && index < 0x16) {
        ((DisplayActor44 *)D_8011ECD0)[98].unk_07 = value;
    }
    if (func_800853FC == table.f[index]) {
        ((DisplayActor44 *)D_8011ECD0)[98].unk_07 = value;
        if (value < 8) {
            ((DisplayActor44 *)D_8011ECD0)[98].unk_03 &= 0xFF7F;
        }
    }
}
