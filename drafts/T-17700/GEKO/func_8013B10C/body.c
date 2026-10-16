s32 func_8013B10C(void) {
    s32 idx; /* declared before the table: T-3330 */
    GekoFnTbl42 sp28;
    s32 var_v1;

    sp28 = D_80146630;
    idx = D_800E6280.unk_110A;
    sp28.f[idx](0x80);
    if (D_80122D04 != 0) {
        *(u8 *)((unsigned char *)D_8011ECD0 + (6739)) = (u8) D_800B593C;
        *(u8 *)((unsigned char *)D_8011ECD0 + (6735)) = (u8) (*(u8 *)((unsigned char *)D_8011ECD0 + (6735)) | 0x80);
        if ((u8) D_800E6280.unk_03C != 0) {
            if ((u8) D_8014662C == 0) {
                *(s8 *)((unsigned char *)D_8011ECD0 + (6734)) = 3;
            }
            if ((*(s16 *)((unsigned char *)D_8011ECD0 + (6756)) == 6) && ((u8) D_8014662C == 0)) {
                *(s8 *)((unsigned char *)D_8011ECD0 + (6734)) = 0;
            }
            var_v1 = 0;
            if (func_800460EC() & 4) {
                var_v1 = getCDlevel();
            }
            if (var_v1 != 0) {
                if ((u8) D_8014662C == 0) {
                    *(s8 *)((unsigned char *)D_8011ECD0 + (6734)) = 3;
                    D_8014662C = 1;
                }
            }
        } else if ((u8) D_8014662C == 0) {
            *(s8 *)((unsigned char *)D_8011ECD0 + (6734)) = 3;
            *(s16 *)((unsigned char *)D_8011ECD0 + (6756)) = 1;
            D_8014662C = 1;
        }
    } else {
        *(u8 *)((unsigned char *)D_8011ECD0 + (6735)) = (u8) (*(u8 *)((unsigned char *)D_8011ECD0 + (6735)) & 0xFF7F);
    }
}
