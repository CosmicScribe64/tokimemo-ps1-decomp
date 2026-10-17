u8 func_8007739C(void) {
    s32 i;
    s32 r;

    func_8004E58C();
    func_80065F34(0);
    if (D_800E6280.unk_110D == 0) {
        for (i = 0; i < 14; i++) {
            if (i != D_800E6280.unk_69C[D_800E6280.unk_71C].unk_01) {
                icon_can_use_set(i, 0);
            } else {
                icon_can_use_set(i, 1);
            }
        }
        D_800E6280.unk_F68.b[0] = (D_800E6280.unk_F68.b[0] & 0xFF0F) | (D_800E6280.unk_69C[D_800E6280.unk_71C].unk_01 << 4);
        D_800E6280.unk_120[0x15 + (D_800E6280.unk_F68.b[0] >> 4) % 9]++;
        if ((D_800E6280.unk_F68.b[0] >> 4) == 4) {
            if (((ScheduleFlags *)&D_800E6280.unk_0F4)->unk_12 < 11) {
                D_800E6280.unk_66C[((ScheduleFlags *)&D_800E6280.unk_0F4)->unk_12].unk_00 += 6;
            }
        }
        r = func_80077330();
        if (r != -1) {
            if ((r == 0) || (r == 1)) {
                dec_bg_show_set(0, r);
                D_800E6280.unk_110D = 2;
            }
        } else {
            D_800E6280.unk_110D = 1;
        }
    } else if (D_800E6280.unk_110D == 1) {
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(0, func_8005751C(0));
            if (D_800B594C == -1) {
                dec_bg_reset();
                func_80077330();
            } else {
                D_800E6280.unk_110D = 2;
            }
        }
    } else if (D_800E6280.unk_110D == 2) {
        func_80076FDC();
        if (((D_800E6280.unk_0F4.b[1] & 0xF) == 3) && !(D_800E6280.unk_0F6.h & 1)) {
            cd_read_sectors(9, 0x801E0000, 0x7D8A);
            D_800E6280.unk_110D = 3;
        } else if (D_800E6280.unk_69C[D_800E6280.unk_71C].unk_01 != 4) {
            cd_read_sectors(0x29, 0x801E0000, D_800B69E0[D_801230D0]);
            D_800E6280.unk_110D = 3;
        } else {
            cd_read_sectors(0x29, 0x801E0000, D_800B6810[D_801230D0]);
            D_800E6280.unk_110D = 3;
        }
    } else if (D_800E6280.unk_110D == 3) {
        if (cd_read_sync_status() & 1) {
            func_8004284C();
        }
    }
}
