extern u8 D_800E62BA;
extern u8 D_800E738D;

void func_8013BE18(void) {
    s16 sp3A;
    s16 sp38;
    s16 sp36;
    s16 sp34;
    u8 temp_t6;

    temp_t6 = (D_800E62BA + 4) & 0xFF;
    D_800E62BA = temp_t6;
    if (temp_t6 >= 0x80U) {
        D_800E62BA = 0x80;
    }
    switch (D_800E738D) {                           /* irregular */
    case 0:
        func_8004500C(0, 0);
        cd_read_sectors(5U, 0x801E0000, 0x9DBA);
        D_800E738D += 1;
        return;
    case 1:
        if (cd_read_sync_status() & 1) {
            cd_read_sectors(0x89U, 0x80180000, 0x9CB5);
            D_800E738D += 1;
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
            func_80043914(0x801E0000, 0x18, 1, 2, 0);
            D_800E738D = 3;
            D_800E62BA = 0x80;
            return;
        }
        break;
    case 3:
        func_8004284C();
        break;
    }
}
