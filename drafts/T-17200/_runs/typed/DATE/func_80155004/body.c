void func_80155004(void) {
    s32 idx; /* declared before the table: T-3330 */
    FnTbl53 sp2C;
    s16 var_a0;
    s32 temp_lo;
    u8 var_a3;
    Rec24 *temp_v0;

    sp2C = D_8015F82C;
    idx = D_800E6280.unk_110A;
    sp2C.f[idx](0x80);
    if (D_800E6280.unk_110A == 0xE) {
        if (D_800E6280.unk_F88 & 0xFF9FFF9F) {
            D_8015F5D0 = 1 - (u8) D_8015F5D0;
        }
        var_a0 = 0x3A;
        if (((u32) D_800E6280.unk0 % 3U) == 0) {
            if ((u8) D_8015F5D0 != 0) {
                var_a3 = 0x80;
            } else {
                var_a3 = 0x80;
                if (D_800B593C == 0x80) {
                    var_a3 = 0x60;
                }
            }
            *(u8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6535)) = var_a3;
            *(u8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6603)) = var_a3;
            do {
                temp_lo = var_a0 * 0x24;
                var_a0 += 1;
                temp_v0 = &D_801217D0[var_a0];
                temp_v0->unk_16 = var_a3;
                temp_v0->unk_15 = var_a3;
                temp_v0->unk_14 = var_a3;
            } while (var_a0 < 0x40);
            D_800B593C = var_a3;
        }
    }
}
