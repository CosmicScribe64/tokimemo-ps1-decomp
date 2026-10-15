void func_80151364(void) {
    u8 year;
    GameState *game = &D_800E6280;
    u32 temp_v0;

    switch (game->unk_110A) {
    case 0:
        dec_bg_cd_read(0x40EA, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, func_8005751C(1));
            func_80062CD0(0x544A);
            func_8004284C();
        }
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            func_801431D4();
            addr_init_bustup();
            game->unk_F5F = 0;
            func_80083474();
            cd_read_sectors(0x18U, 0x801E8000, 0x49D0);
        }
        break;
    case 4:
        normal_date_bg_fadein();
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(0);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 2;
                break;
            case 2:
                D_800CA148 = 4;
                break;
            case 3:
                D_800CA148 = 6;
                break;
            case 4:
                D_800CA148 = 8;
                break;
            }
            if ((year = game->unk_03E) == 0x60) {
                D_800CA148 += 0xC;
            }
            func_80153FA4();
        }
        break;
    case 6:
        normal_date_girl_in();
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        normal_date_speak();
        break;
    case 9:
        normal_date_bggirl_fadeout();
        D_800CA148 = 0;
        break;
    case 10:
        normal_date_speak();
        break;
    case 11:
        func_8007E884();
        break;
    case 12:
        if (func_8008667C(0) == 4) {
            if ((u16) game->unk_66C[0].unk_00 >= (u32) (((year = game->unk_03E) * 0x64) - 0x24B8)) {
                D_800CA148 = 0xC;
            } else {
                D_800CA148 = 0xD;
            }
        } else {
            if ((u16) game->unk_66C[0].unk_00 >= (u32) (((year = game->unk_03E) * 0x64) - 0x24B8)) {
                D_800CA148 = 0xA;
            } else {
                D_800CA148 = 0xB;
            }
        }

        /* FAKE: pointer reassignment splits the year/base live ranges. T-10040. */
        if ((year = (game = &D_800E6280)->unk_03E) == 0x60) {
            D_800CA148 = D_800CA148 + 0xC;
        }
        func_8004284C();
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        normal_date_girl_out();
        break;
    case 15:
        normal_date_bg_fadeout();
        break;
    case 16:
        D_800CA148 = 1;
        func_8004284C();
        break;
    case 17:
        normal_date_speak();
        break;
    case 18:
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}
