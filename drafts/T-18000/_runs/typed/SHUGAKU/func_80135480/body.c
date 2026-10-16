void func_80135480(void) {
    s32 idx; /* declared before the table: T-3330 */
    ShFnTbl74 sp2C;
    s32 temp_v0;
    u8 temp_v1;

    sp2C = D_8013C2F0;
    idx = D_800E6280.unk_110A;
    sp2C.f[idx](0x80);
    temp_v1 = D_800E6280.unk_110A;
    temp_v0 = sp2C.f[temp_v1];
    if (&D_80085368 == temp_v0) {
        ((DisplayActor44 *)D_8011ECD0)[98].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)[98].unk_03 | 0x80);
    }
    if (((func_800853FC == temp_v0) || (func_80135A80 == temp_v0)) && ((u8) D_800B593C < 8U)) {
        ((DisplayActor44 *)D_8011ECD0)[98].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)[98].unk_03 & 0xFF7F);
        ((DisplayActor44 *)D_8011ECD0)[99].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)[99].unk_03 & 0xFF7F);
    }
    if (((temp_v1 >= 0x16U) && (temp_v1 < 0x1CU)) || ((temp_v1 >= 0x20U) && (temp_v1 < 0x25U))) {
        ((DisplayActor44 *)D_8011ECD0)[98].unk_07 = (u8) ((DisplayActor44 *)D_8011ECD0)[96].unk_07;
    }
    if ((u8) D_8013C2E4 != 0) {
        func_801360B0();
    }
}
