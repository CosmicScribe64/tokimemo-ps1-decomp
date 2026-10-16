void func_80155E40(void) {
    s32 i; /* the two counters are declared before tbl: their slots sit above it (T-3330) */
    s32 j;
    FnTbl45 tbl;

    tbl = D_8015FA68;
    if (((D_800E6280.unk_03E == 0x5F && D_800E6280.unk_03F == 7 && D_800E6280.unk_040 == 0x17) ||
         (D_800E6280.unk_03E == 0x60 && D_800E6280.unk_03F == 7 && D_800E6280.unk_040 == 0x1C) ||
         (D_800E6280.unk_03E == 0x61 && D_800E6280.unk_03F == 7 && D_800E6280.unk_040 == 0x1B)) &&
        D_800B593C == 0x80 && D_80122D04 == 0 && D_800E6280.unk_110A >= 0x21) {
        func_8006B900();
    }
    tbl.f[D_800E6280.unk_110A](0x80);
    if (func_800853FC == tbl.f[D_800E6280.unk_110A]) {
        for (i = 0; i < 8; i++) {
            ((DisplayActor44 *)D_8011ECD0)[98 + i].unk_07 = ((DisplayActor44 *)D_8011ECD0)[96].unk_07;
        }
        if (((DisplayActor44 *)D_8011ECD0)[96].unk_07 < 9) {
            for (j = 0; j < 8; j++) {
                ((DisplayActor44 *)D_8011ECD0)[98 + j].unk_03 &= 0x7F;
            }
        }
    }
    if (D_800E6280.unk_110A >= 0x11 && D_800E6280.unk_110A < 0x14) {
        func_801560E0();
    }
}
