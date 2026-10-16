extern u8 D_800E699E;
extern u8 D_800E71DF;
extern u8 D_800E738A;
void func_8014A0E0(void) {
    switch (D_800E738A) {
    case 0:
        func_8004284C();
        break;
    case 1:
        k_reset(0);
        func_8006612C((s32) "　");
        func_80065F34(0);
        dec_bg_reset();
        set_dec_bri(0);
        D_800E699E |= 4;
        func_80044750(0xCF);
        Vblnk_Timer_Init();
        cd_read_sectors(3U, 0x801E8000, 0x49B7);
        func_8004284C();
        break;
    case 2:
        if (Vblnk_Timer() >= 0x15) {
            if (cd_read_sync_status() & 1) {
                func_80044890(1, 0xC478, 0xC44B, 0xCA95, 0xCA4F, 0xCA3E);
                if (D_80122EBC == 0xB) {
                    D_800E71DF = 0;
                    func_800847B8(0U);
                } else {
                    D_800E71DF = (u8) D_80122EBC;
                    func_800847B8((u8) D_80122EBC);
                }
                func_8013257C();
                D_800CA148 = 0x25;
                D_800CA14C = 0;
                D_800CA160 = (s32) D_80155E90;
                D_800CA164 = (s32) D_80155E94;
                D_800CA168 = (s32) D_80155E98;
                func_8004284C();
            }
        }
        break;
    case 3:
        normal_date_speak();
        break;
    case 4:
        if (func_80044E8C() == 1) {
            dec_bg_cd_read(0x42D8, 1);
            func_8004284C();
        }
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            func_80083440(get_g_zyotai_h(D_800E71DF) & 0x7F);
            func_80044750(0x200);
            k_reset(0);
            func_8006612C((s32) "近所の公園");
            D_80125CA0 = 0xC0;
            dec_bg_show_set(1, func_8005751C(1));
            cd_read_sectors(3U, 0x801B0000, 0xAF43);
            func_8004284C();
        }
        break;
    case 6:
        normal_date_bggirl_fadein();
        break;
    case 7:
        if (cd_read_sync_status() & 1) {
            func_80148ED0();
            if (D_80122EBC == 0xB) {
                D_800E71DF = 0;
                func_800847B8(0U);
            } else {
                D_800E71DF = (u8) D_80122EBC;
                func_800847B8((u8) D_80122EBC);
            }
            D_800CA148 = 1;
            D_800CA14C = 0;
            switch (D_80122EBC) {
            case 11:
                D_800CA160 = D_801562A4;
                D_800CA164 = D_801562D8;
                D_800CA168 = D_8015630C;
                break;
            case 4:
                D_800CA160 = D_801562C8;
                D_800CA164 = D_801562FC;
                D_800CA168 = D_80156330;
                break;
            case 5:
                D_800CA160 = D_801562BC;
                D_800CA164 = D_801562F0;
                D_800CA168 = D_80156324;
                break;
            case 6:
                D_800CA160 = D_801562C0;
                D_800CA164 = D_801562F4;
                D_800CA168 = D_80156328;
                break;
            case 7:
                D_800CA160 = D_801562B4;
                D_800CA164 = D_801562E8;
                D_800CA168 = D_8015631C;
                break;
            case 8:
                D_800CA160 = D_801562A0;
                D_800CA164 = D_801562D4;
                D_800CA168 = D_80156308;
                break;
            case 9:
                D_800CA160 = D_801562C4;
                D_800CA164 = D_801562F8;
                D_800CA168 = D_8015632C;
                break;
            case 10:
                D_800CA160 = D_801562D0;
                D_800CA164 = D_80156304;
                D_800CA168 = D_80156338;
                break;
            default:
                func_80046500();
                break;
            }
            func_8004284C();
        }
        break;
    case 8:
        normal_date_speak();
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        normal_date_bggirl_fadeout();
        break;
    case 11:
        func_80085B3C(0x14, 0x19);
        func_80145530();
        break;
    default:
        func_80046500();
        break;
    }
}
