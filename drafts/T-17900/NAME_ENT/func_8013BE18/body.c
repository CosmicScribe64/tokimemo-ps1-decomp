void func_8013BE18(void) {
    RECT rect;

    D_800E6280.unk_03A += 4;
    if (D_800E6280.unk_03A >= 0x80) {
        D_800E6280.unk_03A = 0x80;
    }
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004500C(0, 0);
        cd_read_sectors(5, 0x801E0000, 0x9DBA);
        D_800E6280.unk_110D++;
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            cd_read_sectors(0x89, 0x80180000, 0x9CB5);
            D_800E6280.unk_110D++;
        }
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            rect.x = 0x3E0;
            rect.y = 0x100;
            rect.h = 0x80;
            rect.w = 0x20;
            func_8009C884(&rect, (void *)0x801E0200);
            func_80043914(0x801E0000, 0x18, 1, 2, 0);
            D_800E6280.unk_03A = 0x80;
            D_800E6280.unk_110D = 3;
        }
        break;
    case 3:
        func_8004284C();
        break;
    }
}
