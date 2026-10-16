void func_8013BE18(void) {
    s16 sp3A;
    s16 sp38;
    s16 sp36;
    s16 sp34;
    u8 temp_t6;
    u8 temp_v0;

    temp_t6 = (D_800E6280.unk_03A + 4) & 0xFF;
    D_800E6280.unk_03A = temp_t6;
    if (temp_t6 >= 0x80U) {
        D_800E6280.unk_03A = 0x80;
    }
    temp_v0 = D_800E6280.unk_110D;
    switch (temp_v0) {
    case 0:
        func_8004500C(0, 0);
        cd_read_sectors(5U, 0x801E0000, 0x9DBA);
        D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
        return;
    case 1:
        if (cd_read_sync_status() & 1) {
            cd_read_sectors(0x89U, 0x80180000, 0x9CB5);
            D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
            return;
        }
        return;
    case 2:
        if (cd_read_sync_status() & 1) {
            sp34 = 0x3E0;
            sp36 = 0x100;
            sp3A = 0x80;
            sp38 = 0x20;
            func_8009C884((RECT *) &sp34, (void *)0x801E0200);
            load_palette(0x801E0000, 0x18, 1, 2, 0);
            D_800E6280.unk_110D = 3;
            D_800E6280.unk_03A = 0x80;
            return;
        }
        break;
    case 3:
        func_8004284C();
        break;
    }
}
