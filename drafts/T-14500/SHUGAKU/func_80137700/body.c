void func_80137700(void) {
    switch (D_800E71DF) {
    case 0:
        cd_read_sectors(6U, 0x80198000, 0xAE25);
        break;
    case 1:
        cd_read_sectors(3U, 0x80198000, 0xAE39);
        break;
    case 2:
        cd_read_sectors(4U, 0x80198000, 0xAE2B);
        break;
    case 3:
        cd_read_sectors(5U, 0x80198000, 0xAE2F);
        break;
    case 4:
        cd_read_sectors(7U, 0x80198000, 0xAE4C);
        break;
    case 5:
        cd_read_sectors(5U, 0x80198000, 0xAE3C);
        break;
    case 6:
        cd_read_sectors(6U, 0x80198000, 0xAE41);
        break;
    case 7:
        cd_read_sectors(5U, 0x80198000, 0xAE34);
        break;
    case 8:
        cd_read_sectors(4U, 0x80198000, 0xAE21);
        break;
    case 9:
        cd_read_sectors(5U, 0x80198000, 0xAE47);
        break;
    case 10:
        cd_read_sectors(6U, 0x80198000, 0xAE5A);
        break;
    case 11:
    case 12:
    case 13:
        cd_read_sectors(7U, 0x80198000, 0xAE53);
        break;
    default:
        func_80046500();
        break;
    }
    func_801365F4();
    func_8004284C();
}
