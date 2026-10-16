void func_80143B58(void) {
    switch (D_8015EDBC) {
    case 0x10:
        func_800462C8(0x20U, 0x80162000, 0xB69B);
        func_800462C8(0x20U, 0x80172000, 0xB6BB);
        func_800462C8(8U, 0x80182000, 0xB47E);
        D_8015EDBC += 1;
        return;
    case 0x14:
        func_80044890(0, 0xBF43, 0xBF26, 0xD310, 0xD2C2, 0xD29D);
        D_8015EDBC += 1;
        return;
    case 0x18:
        func_800462C8(0x12U, 0x8018D000, 0xB438);
        cd_read_sectors(0xCU, 0x801E0000, 0xB44A);
        func_800462C8(0x20U, 0x80162000, 0xB418);
        D_8015EDBC += 1;
        return;
    case 0x20:
        func_800462C8(0x20U, 0x80162000, 0xB647);
        func_800462C8(0x20U, 0x80172000, 0xB5A0);
        D_8015EDBC += 1;
        return;
    case 0x24:
        func_80044890(0, 0xBF61, 0xBF45, 0xD310, 0xD2C2, 0xD29D);
        D_8015EDBC += 1;
        return;
    case 0x28:
        func_800462C8(7U, 0x80196000, 0xB45B);
        cd_read_sectors(5U, 0x801E0000, 0xB462);
        func_800462C8(0x20U, 0x80162000, 0xB535);
        D_8015EDBC += 1;
        return;
    case 0x30:
        func_800462C8(0x20U, 0x80162000, 0xB67B);
        func_800462C8(0x20U, 0x80172000, 0xB5C1);
        D_8015EDBC += 1;
        return;
    case 0x34:
        func_80044890(0, 0xBF77, 0xBF63, 0xD310, 0xD2C2, 0xD29D);
        D_8015EDBC += 1;
        return;
    case 0x38:
        func_800462C8(0xAU, 0x801A7000, 0xB515);
        cd_read_sectors(0x15U, 0x801E0000, 0xB51F);
        func_800462C8(0x20U, 0x80162000, 0xB5FC);
        func_800462C8(0x20U, 0x80172000, 0xB61D);
        D_8015EDBC += 1;
        return;
    case 0x44:
        func_80044890(0, 0xC56D, 0xC54D, 0xD310, 0xD2C2, 0xD29D);
        D_8015EDBC += 1;
        return;
    default:
        if (func_80045288() == 1) {
            D_8015EDBC = 0;
        }
        /* fallthrough */
        return;
    }
}
