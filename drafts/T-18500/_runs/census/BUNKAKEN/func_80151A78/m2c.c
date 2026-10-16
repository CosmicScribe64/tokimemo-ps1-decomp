extern s8 D_800E69A2;
extern s8 D_800E71DF;
extern u8 D_800E69A0;
extern u8 D_800E69A1;
extern u8 D_800E7388;
extern u8 D_800E7389;
extern u8 D_800E738A;
void func_80151A78(void) {
    s32 temp_v0;
    s32 temp_v0_2;

    switch (D_800E738A) {
    case 0:
        dec_bg_cd_read(0x4105, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, func_8005751C(1));
            cd_read_sectors(9U, 0x801E8000, 0x4B3E);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(9);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0x26;
                break;
            case 2:
                D_800CA148 = 0x27;
                break;
            case 3:
                D_800CA148 = 0x28;
                break;
            }
            func_80152618();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_girl_out();
        break;
    case 7:
        D_800E69A0 = D_800E7388;
        D_800E69A1 = D_800E7389;
        D_800E69A2 = D_800E738A + 1;
        D_800E7D4F = (D_800E7D4F | 2) & 0xFFE3;
        func_80042908(0x8D);
        break;
    case 8:
        D_80122E98 = 1;
        D_80122EB0 = (s32) D_800E7389;
        D_80122EB4 = D_800E738A + 1;
        func_80042908(0x8C);
        break;
    case 9:
        func_80044750(0xBF);
        func_80044890(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        func_80085E30(1, 0);
        func_80145844();
        D_80125CA0 = 0xC0;
        message_window_init();
        func_80084E4C();
        func_800847B8(9U);
        D_80122E98 = 0;
        func_8004284C();
        break;
    case 10:
        if (func_80044E8C() == 1) {
            func_80044750(0x200);
            dec_bg_reset();
            set_dec_bri(0);
            dec_bg_cd_read(0x4105, 1);
            func_8004284C();
        }
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 12:
        func_800866A0(0x64D0);
        break;
    case 13:
        addr_init_bustup();
        D_800E71DF = 9;
        func_80083474();
        if (func_8008667C(9) < 2) {
            func_80083440(0U);
        }
        cd_read_sectors(9U, 0x801E8000, 0x4B3E);
        break;
    case 14:
        normal_date_bggirl_fadein();
        break;
    case 15:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = func_8008667C(9);
            switch (temp_v0_2) {
            case 0:
            case 1:
                D_800CA148 = 0x29;
                break;
            case 2:
                D_800CA148 = 0x2A;
                break;
            case 3:
                D_800CA148 = 0x2B;
                break;
            }
            func_80152618();
        }
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        normal_date_bggirl_fadeout();
        break;
    case 18:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 19:
        normal_date_speak();
        break;
    case 20:
        D_800E7D4F |= 1;
        func_80145530();
        break;
    default:
        func_80046500();
        break;
    }
}
