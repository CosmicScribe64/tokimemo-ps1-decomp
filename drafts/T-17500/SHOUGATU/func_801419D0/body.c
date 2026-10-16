void func_801419D0(void) {
    s32 idx; /* declared before the table: T-3330 */
    FnTbl40 sp2C;
    s16 temp_v0;

    sp2C = D_80146804;
    idx = D_800E6280.unk_110A;
    sp2C.f[idx](0x80);
    if ((u8) D_800E6280.unk_110A < 0x10U) {
        ((DisplayActor44 *)D_8011ECD0)[96].unk_07 = (u8) D_800B593C;
        if ((((DisplayActor44 *)D_8011ECD0)[96].unk_18 == 0) && (((DisplayActor44 *)D_8011ECD0)[96].unk_08 == 0)) {
            temp_v0 = ((DisplayActor44 *)D_8011ECD0)[96].unk_16 + 1;
            ((DisplayActor44 *)D_8011ECD0)[96].unk_16 = temp_v0;
            if (temp_v0 >= 3) {
                ((DisplayActor44 *)D_8011ECD0)[96].unk_16 = 0;
            }
        }
    }
    func_80141D20();
}
