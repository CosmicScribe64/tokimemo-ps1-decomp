extern u8 D_800E69DD;

void func_8013E7B0(void) {
    switch (D_800E69DD) {
    case 0:
        date_fill_sections_shiori();
        cd_read_sectors(0x17U, 0x80198000, 0xAD29);
        break;
    case 1:
        date_fill_sections_mio();
        cd_read_sectors(0x18U, 0x80198000, 0xAD82);
        break;
    case 2:
        date_fill_sections_yuina();
        cd_read_sectors(0x15U, 0x80198000, 0xAD40);
        break;
    case 3:
        date_fill_sections_ayako();
        cd_read_sectors(0x16U, 0x80198000, 0xAD55);
        break;
    case 4:
        date_fill_sections_saki();
        cd_read_sectors(0x16U, 0x80198000, 0xADDB);
        break;
    case 5:
        date_fill_sections_yukari();
        cd_read_sectors(0x17U, 0x80198000, 0xAD9A);
        break;
    case 6:
        date_fill_sections_nozomi();
        cd_read_sectors(0x15U, 0x80198000, 0xADB1);
        break;
    case 7:
        date_fill_sections_mira();
        cd_read_sectors(0x17U, 0x80198000, 0xAD6B);
        break;
    case 8:
        date_fill_sections_yuko();
        cd_read_sectors(0x14U, 0x80198000, 0xAD15);
        break;
    case 9:
        date_fill_sections_megumi();
        cd_read_sectors(0x15U, 0x80198000, 0xADC6);
        break;
    default:
        date_fill_sections_yumi();
        cd_read_sectors(0x16U, 0x80198000, 0xADF1);
        break;
    }
    func_8004284C();
}
