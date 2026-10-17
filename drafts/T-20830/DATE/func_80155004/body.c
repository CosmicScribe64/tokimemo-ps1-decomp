void func_80155004(void) {
    s32 pad[2]; /* FAKE: eight unused bytes above tbl (frame 0x108) */
    FnTbl53 tbl;
    s16 i;

    tbl = D_8015F82C;
    tbl.f[D_800E6280.unk_110A](0x80);
    if (D_800E6280.unk_110A == 0xE) {
        if (D_800E6280.unk_F88 & 0xFF9FFF9F) {
            D_8015F5D0 = 1 - (u8) D_8015F5D0;
        }
        if ((u32) D_800E6280.unk_10F8 % 3U == 0) {
            if ((u8) D_8015F5D0 != 0) {
                D_800B593C = 0x80;
            } else if (D_800B593C == 0x80) {
                D_800B593C = 0x60;
            } else {
                D_800B593C = 0x80;
            }
            ((DisplayRecord44 *)D_8011ECD0)[96].unk_07 = D_800B593C; ((DisplayRecord44 *)D_8011ECD0)[97].unk_07 = D_800B593C; i = 0x3A; do {
                D_801217D0[i].unk_14 = D_801217D0[i].unk_15 = D_801217D0[i].unk_16 = D_800B593C;
                i += 1;
            } while (i < 0x40);
        }
    }
}
