void func_80132320(void) {
    s32 i;
    s32 val;
    u8 **p;

    if (D_800E6280.unk_110D == 0) {
        if (cd_read_sync_status() & 1) {
            p = D_8014CE68;
            for (i = 0; i < 50; i++) {
                p[i] += 0x801E0000;
            }
            func_80132A38();
            D_801220E1 = 0;
            D_801220E0 = 0;
            D_800E6280.unk_110D += 1;
            D_801220E2 = 0;
            D_800E6280.unk_03A = 0;
        }
    } else {
        val = (D_800B593C + 4) & 0xFF;
        D_801220E1 = val;
        D_801220E0 = val;
        D_800B593C = val;
        D_801220E2 = val;
        D_800E6280.unk_03A = val;
        if ((u32)val >= 0x80) {
            func_80057640(0xF0);
            func_801324AC();
            D_801220D0 = 0x80000000;
            D_801220D4 = -0x18;
            func_8006BD6C(1);
            func_800438F0(0);
            func_80042808();
        }
    }
    func_8004B19C(D_800B593C, 0, 0);
    func_8004B19C(D_801220E0, 0, 0x10);
}
