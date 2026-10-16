void func_801362F0(void) {
    u8 temp_v1;

    func_800AE120((s32) D_800E6280.unk_10F8);
    temp_v1 = D_80139DF4[(u8) D_80139DF0];
    if (temp_v1 == 1) {
        func_8004284C();
        func_8004284C();
    } else if (temp_v1 == 2) {
        func_8004284C();
        func_8004284C();
    } else {
        D_800E6280.unk_F5F = 0xB;
        D_80139DF8 = (u8) (func_800AE0D0() % 4);
        if ((((u16) D_800E6280.unk_0F4.h >> 0xC) == 0xA) && ((func_800AE0D0() % 2) != 0)) {
            D_80139DF8 = 4;
        }
        switch (D_80139DF8) {                       /* irregular */
        case 0:
            cd_read_sectors(0x35U, 0x801B4400, 0x8FF3);
            break;
        case 1:
            cd_read_sectors(0x35U, 0x801B4400, 0x9028);
            break;
        case 2:
            cd_read_sectors(0x35U, 0x801B4400, 0x905D);
            break;
        case 3:
            cd_read_sectors(0x35U, 0x801B4400, 0x9092);
            break;
        default:
            cd_read_sectors(0x35U, 0x801B4400, 0x90C7);
            break;
        }
    }
    func_8004284C();
}
