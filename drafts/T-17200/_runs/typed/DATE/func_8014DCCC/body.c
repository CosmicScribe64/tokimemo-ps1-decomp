void func_8014DCCC(void) {
    FnTbl39 sp30;
    s32 (*temp_v0)(s32);
    s32 var_at;
    u8 temp_v1;

    sp30 = D_8015E3C4;
    temp_v0 = sp30.f[D_800E6280.unk_110A];
    if (&D_8007EC9C == temp_v0) {
        *(u8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6531)) = (u8) (*(u8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6531)) | 0x80);
    }
    if (&D_8007D878 == temp_v0) {
        *(u8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6531)) = (u8) (*(u8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6531)) & 0xFF7F);
    }
    temp_v0(0x80);
    if (D_80122D04 != 0) {
        func_8014E010();
    }
    temp_v1 = D_800E6280.unk_110A;
    var_at = temp_v1 < 0x19U;
    if (temp_v1 == 0xE) {
        func_8014E108();
        var_at = (u8) D_800E6280.unk_110A < 0x19U;
    }
    if (var_at == 0) {
        if (temp_v1 < 0x24U) {
            func_80140D38();
        }
    }
}
