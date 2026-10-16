s32 func_801430A0(void) {
    FnTbl48 sp2C;
    s32 var_v0;

    sp2C = D_80146970;
    var_v0 = sp2C.f[D_800E6280.unk_110A](0x3C);
    if ((u8) D_800E6280.unk_110A < 0x11U) {
        ((DisplayActor44 *)D_8011ECD0)[96].unk_07 = (u8) D_800B593C;
        if ((((DisplayActor44 *)D_8011ECD0)[96].unk_18 == 0) && (((DisplayActor44 *)D_8011ECD0)[96].unk_08 == 0)) {
            var_v0 = (s32) (s16) (((DisplayActor44 *)D_8011ECD0)[96].unk_16 + 1);
            ((DisplayActor44 *)D_8011ECD0)[96].unk_16 = (s16) var_v0;
            if (var_v0 >= 3) {
                ((DisplayActor44 *)D_8011ECD0)[96].unk_16 = 0;
                var_v0 = 0;
            }
        }
    }
    if (D_80122D04 != 0) {
        ((DisplayActor44 *)D_8011ECD0)[98].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)[98].unk_03 | 0x80);
        ((DisplayActor44 *)D_8011ECD0)[98].unk_07 = (u8) ((DisplayActor44 *)D_8011ECD0)[96].unk_07;
        if (((u32) D_800E6280.unk_10F8 % 300U) == 0) {
            var_v0 = func_80143228();
            ((DisplayActor44 *)D_8011ECD0)[98].unk_02 = 3;
        }
    } else {
        ((DisplayActor44 *)D_8011ECD0)[98].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)->unk_00 & 0xFF7F);
    }
    return var_v0;
}
