void func_80155004(void) {
    s16 i; /* the three locals are declared before tbl: their slots sit above it (T-3330) */
    u8 c;
    s32 idx;
    FnTbl53 tbl;

    tbl = D_8015F82C;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
    if (D_800E6280.unk_110A == 0xE) {
        if (D_800E6280.unk_F88 & 0xFF9FFF9F) {
            D_8015F5D0 = 1 - (u8) D_8015F5D0;
        }
        if (D_800E6280.unk_10F8 % 3 == 0) {
            if ((u8) D_8015F5D0 != 0) {
                c = 0x80;
            } else if (D_800B593C == 0x80) {
                c = 0x60;
            } else {
                c = 0x80;
            }
            ((DisplayActor44 *)D_8011ECD0)[96].unk_07 = c;
            ((DisplayActor44 *)D_8011ECD0)[97].unk_07 = c;
            for (i = 0x3A; i < 0x40; i++) {
                D_801217D0[i].unk_16 = c;
                D_801217D0[i].unk_15 = c;
                D_801217D0[i].unk_14 = c;
            }
            D_800B593C = c;
        }
    }
}
