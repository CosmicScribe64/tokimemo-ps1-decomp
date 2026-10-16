void func_80134B94(void) {
    s32 idx; /* declared before the table: T-3330 */
    ShFnTbl41 sp30;
    s32 temp_v0;

    sp30 = D_8013C228;
    idx = D_800E6280.unk_110A;
    sp30.f[idx](0x80);
    temp_v0 = sp30.f[D_800E6280.unk_110A];
    if (&D_80085368 == temp_v0) {
        ((DisplayActor44 *)D_8011ECD0)[98].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)[98].unk_03 | 0x80);
        ((DisplayActor44 *)D_8011ECD0)[98].unk_07 = (u8) ((DisplayActor44 *)D_8011ECD0)[96].unk_07;
    }
    if (func_800853FC == temp_v0) {
        ((DisplayActor44 *)D_8011ECD0)[98].unk_07 = (u8) ((DisplayActor44 *)D_8011ECD0)[96].unk_07;
        if ((u8) ((DisplayActor44 *)D_8011ECD0)[96].unk_07 < 9U) {
            ((DisplayActor44 *)D_8011ECD0)[98].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)[98].unk_03 & 0xFF7F);
        }
    }
}
