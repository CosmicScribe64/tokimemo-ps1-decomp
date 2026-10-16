void func_8013DC60(void) {
    u8 temp_v1;

    temp_v1 = D_800E6280.unk_110D;
    if (temp_v1 == 0) {
        switch (D_8015EB98) {
        case 0x10000:
            cd_read_sectors(0x80U, 0x80180000, 0x3A56);
            break;
        case 0x20000:
            cd_read_sectors(0x70U, 0x80180000, 0x3D15);
            break;
        case 0x40000:
            cd_read_sectors(0x70U, 0x80180000, 0x3BA9);
            break;
        case 0x80000000:
            cd_read_sectors(0x99U, 0x80180000, 0x3DF3);
            break;
        case 0x8000000:
            cd_read_sectors(0x95U, 0x80180000, 0x3C5E);
            break;
        case 0x100000:
            cd_read_sectors(0x62U, 0x80180000, 0x396B);
            break;
        }
        D_800E6280.unk_110D = temp_v1 + 1;
    } else if (temp_v1 == 1) {
        if (cd_read_sync_status() & 1) {
            if (D_8015EB98 != 0x100000) {
                D_800E6280.unk_110D = 4;
            } else {
                D_800E6280.unk_110D = D_800E6280.unk0 + 1;
            }
        }
    } else if (temp_v1 == 2) {
        cd_read_sectors(0x2BU, 0x801B1000, 0x3C33);
        D_800E6280.unk_110D += 1;
    } else if (temp_v1 == 3) {
        if (cd_read_sync_status() & 1) {
            D_800E6280.unk_110D += 1;
        }
    } else if (temp_v1 == 4) {
        if (D_8015EBA4 == 1) {
            switch (D_8015EBA0) {
            case 0:
                cd_read_sectors(0x21U, 0x801D8000, 0x3A08);
                break;
            case 1:
                cd_read_sectors(0x21U, 0x801D8000, 0x390C);
                break;
            case 2:
                cd_read_sectors(0x22U, 0x801D8000, 0x3CF3);
                break;
            case 3:
                cd_read_sectors(0x2DU, 0x801D8000, 0x3A29);
                break;
            case 4:
                cd_read_sectors(0x2DU, 0x801D8000, 0x392D);
                break;
            case 5:
                cd_read_sectors(0x22U, 0x801D8000, 0x39E6);
                break;
            case 6:
                cd_read_sectors(0x19U, 0x801D8000, 0x3EB7);
                break;
            case 7:
                cd_read_sectors(0x1AU, 0x801D8000, 0x3D85);
                break;
            case 8:
                cd_read_sectors(0x29U, 0x801D8000, 0x3DCA);
                break;
            case 9:
                cd_read_sectors(0x2BU, 0x801D8000, 0x3D9F);
                break;
            case 10:
                cd_read_sectors(0x19U, 0x801D8000, 0x39CD);
                break;
            case 11:
                cd_read_sectors(0x3BU, 0x801D8000, 0x3AF8);
                break;
            }
            D_800E6280.unk_110D = temp_v1 + 1;
        } else {
            D_800E6280.unk_110D = temp_v1 + 2;
        }
    } else if (temp_v1 == 5) {
        if (cd_read_sync_status() & 1) {
            D_800E6280.unk_110D += 1;
        }
    } else if (temp_v1 == 6) {
        switch (D_8015EB9C) {
        case 0:
            cd_read_sectors(9U, 0x801C7000, 0x3B33);
            break;
        case 1:
            cd_read_sectors(0x11U, 0x801C7000, 0x3B6E);
            break;
        case 2:
            if (D_8015EB98 != 0x80000000) {
                cd_read_sectors(0x22U, 0x801C7000, 0x3AD6);
            } else {
                func_80042808();
            }
            break;
        case 3:
            cd_read_sectors(0x19U, 0x801C7000, 0x3B55);
            break;
        case 4:
            cd_read_sectors(0x11U, 0x801C7000, 0x3C22);
            break;
        case 5:
            cd_read_sectors(0x11U, 0x801C7000, 0x3B98);
            break;
        case 6:
            cd_read_sectors(0x19U, 0x801C7000, 0x3B7F);
            break;
        case 7:
            cd_read_sectors(0x19U, 0x801C7000, 0x3B3C);
            break;
        case 8:
            cd_read_sectors(0x11U, 0x801C7000, 0x395A);
            break;
        case 9:
            cd_read_sectors(9U, 0x801C7000, 0x3C19);
            break;
        case 10:
            cd_read_sectors(0x11U, 0x801C7000, 0x3EE9);
            break;
        case 13:
            cd_read_sectors(0x19U, 0x801C7000, 0x3ED0);
            break;
        case 14:
            func_80042808();
            break;
        }
        D_800E6280.unk_110D = temp_v1 + 1;
    } else if (temp_v1 == 7) {
        if (cd_read_sync_status() & 1) {
            func_80042808();
        }
    } else {
        func_80042808();
    }
}
