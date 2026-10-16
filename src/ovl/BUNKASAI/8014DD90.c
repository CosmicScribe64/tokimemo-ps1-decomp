#include "common.h"
#include "ovl/BUNKASAI.h"

void func_8014DD90(void) {
    BunkaDispatch8 *dispatch = (BunkaDispatch8 *)&D_80122EB8;
    dispatch->unk_04 = 6;
    switch (dispatch->unk_00) {
    case 0:
        if (D_800E6280.unk_03E == 0x61) {
            func_80042908(0x6F);
            return;
        }
        func_80042908(0x6E);
        return;
    case 1:
        func_80042908(0x70);
        return;
    case 2:
        func_80042908(0x71);
        return;
    case 3:
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            func_80042908(0x72);
            return;
        case 0x60:
            func_80042908(0x74);
            return;
        case 0x61:
            func_80042908(0x71);
            return;
        }
        break;
    case 4:
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            func_80042908(0x75);
            return;
        case 0x60:
            func_80042908(0x76);
            return;
        case 0x61:
            func_80042908(0x77);
            return;
        }
        break;
    case 5:
        func_80042908(0x78);
        return;
    default:
        func_80046500();
        return;
    }
}

/* T-12703: existing aggregate and flag views reproduce the scene family. */
void func_8014DF04(void) {
    GameState *g = &D_800E6280;
    u32 temp_v0;
    u32 temp_v0_2;

    switch (g->unk_110A) {
    case 0:
        func_8005742C(0x40EA, 1);
        func_8004284C();
        break;
    case 1:
        func_8007E934();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_801431D4();
            func_80057418(1, func_8005751C(1));
            func_80062CD0(0x622D);
            func_8004284C();
        }
        break;
    case 3:
        func_8007E81C();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            func_8007C740();
            g->unk_F5F = 6;
            func_80083474();
            if (g->unk_03E == 0x5F) {
                D_800CA148 = 0x24;
            } else {
                D_800CA148 = 0x25;
            }
        }
        break;
    case 5:
        func_80085A60();
        break;
    case 6:
        cd_read_sectors(0xFU, 0x801E8000, 0x4B13);
        func_8004284C();
        break;
    case 7:
        func_8007E390();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(6);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 4;
                break;
            case 2:
                D_800CA148 = 5;
                break;
            case 3:
                D_800CA148 = 6;
                break;
            case 4:
                D_800CA148 = 7;
                break;
            }
            if (g->unk_03E == 0x60) {
                D_800CA148 += 4;
            }
            func_801508C4();
        }
        break;
    case 9:
        func_80085A60();
        break;
    case 10:
        func_8004284C();
        temp_v0_2 = func_8008667C(6);
        switch (temp_v0_2) {
        case 0:
        case 1:
            D_800CA148 = 1;
            break;
        case 2:
            D_800CA148 = 2;
            break;
        case 3:
            D_800CA148 = 3;
            break;
        case 4:
            func_80042940(0xC);
            break;
        }
        break;
    case 11:
        func_80085A60();
        break;
    case 12:
        D_800CA148 = 0x78;
        func_8007E5A0();
        break;
    case 13:
        if (func_8008667C(6) == 4) {
            func_80085A60();
        } else {
            func_8007E934();
        }
        break;
    case 14:
        D_800CA148 = 0;
        if (func_8008667C(6) == 4) {
            func_8007E934();
        } else {
            func_8004284C();
        }
        break;
    case 15:
        func_80085A60();
        break;
    case 16:
        if (func_8008667C(6) != 4) {
            if (g->unk_03E == 0x5F) {
                g->unk_1BC[6].unk_0C.f.b12 = 1;
            } else {
                g->unk_1BC[6].unk_0C.f.b13 = 1;
            }
        }
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}

/* T-12703: existing aggregate and flag views reproduce the scene family. */
void func_8014E2AC(void) {
    GameState *g = &D_800E6280;
    s32 temp_v0_2;
    u32 temp_v0;

    switch (g->unk_110A) {
    case 0:
        func_8005742C(0x40F4, 1);
        func_8004284C();
        break;
    case 1:
        func_8007E934();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_8014338C();
            func_80057418(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 3:
        func_8007E81C();
        break;
    case 4:
        D_800CA148 = 0x26;
        func_8004284C();
        break;
    case 5:
        func_80085A60();
        break;
    case 6:
        func_8007E934();
        break;
    case 7:
        D_80122EAC = 0;
        D_80122EB0 = (s32) g->unk_1109;
        D_80122EB4 = g->unk_110A + 1;
        main_state_set(0x80);
        break;
    case 8:
        func_800866A0(0x622D);
        break;
    case 9:
        func_8014338C();
        D_80125CA0 = 0xC0;
        func_8007C740();
        g->unk_F5F = 6;
        func_80083474();
        cd_read_sectors(0xFU, 0x801E8000, 0x4B13);
        break;
    case 10:
        func_8007E884();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(6);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0xC;
                break;
            case 2:
                D_800CA148 = 0xD;
                break;
            case 3:
                D_800CA148 = 0xF;
                break;
            case 4:
                D_800CA148 = 0x10;
                break;
            }
            func_801508C4();
        }
        break;
    case 12:
        func_80085A60();
        break;
    case 13:
        if (func_8008667C(6) == 4) {
            func_8007E5A0();
        } else {
            func_8004284C();
        }
        break;
    case 14:
        temp_v0_2 = func_8008667C(6);
        switch (temp_v0_2) {                        /* switch 3; irregular */
        case 0:
        case 1:
            D_800CA148 = 1;
            break;
        case 3:
            D_800CA148 = 3;
            break;
        }
        func_8004284C();
        break;
    case 15:
        func_80085A60();
        break;
    case 16:
        if (func_8008667C(6) == 4) {
            func_8004284C();
        } else {
            func_8007E5A0();
        }
        break;
    case 17:
        func_8007E934();
        break;
    case 18:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 19:
        func_80085A60();
        break;
    case 20:
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_8014E5EC(void) {
    s32 temp_v0_2;
    u32 temp_v0;

    switch (D_800E6280.unk_110A) {
    case 0:
        dec_bg_cd_read(0x40F4, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_8014338C();
            dec_bg_show_set(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        if (D_800E6280.unk_03E == 0x5F) {
            D_800CA148 = 0x27;
        } else {
            D_800CA148 = 0x28;
        }
        break;
    case 4:
        normal_date_speak();
        break;
    case 5:
        normal_date_bg_fadeout();
        break;
    case 6:
        D_80122EB0 = (s32) D_800E6280.unk_1109;
        D_80122EB4 = D_800E6280.unk_110A + 1;
        main_state_set(0x80);
        break;
    case 7:
        dec_bg_reset();
        set_dec_bri(0);
        dec_bg_cd_read(0x40F4, 1);
        func_8004284C();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            func_800866A0(0x622D);
        }
        break;
    case 9:
        dec_bg_show_set(1, func_8005751C(1));
        func_8014338C();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        D_800E6280.unk_F5F = 6;
        func_80083474();
        cd_read_sectors(0xFU, 0x801E8000, 0x4B13);
        break;
    case 10:
        func_8007E884();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(6);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = (D_800E6280.unk_03E * 3) - 0x10B;
                break;
            case 2:
                D_800CA148 = (D_800E6280.unk_03E * 3) - 0x10A;
                break;
            case 3:
                D_800CA148 = (D_800E6280.unk_03E * 3) - 0x109;
                break;
            case 4:
                D_800CA148 = 0x1B;
                break;
            }
            func_801508C4();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        if (func_8008667C(6) == 4) {
            normal_date_girl_out();
        } else {
            func_8004284C();
        }
        break;
    case 14:
        temp_v0_2 = func_8008667C(6);
        switch (temp_v0_2) {
        case 0:
        case 1:
            D_800CA148 = 1;
            break;
        case 2:
            D_800CA148 = 2;
            break;
        case 3:
            D_800CA148 = 3;
            break;
        }
        func_8004284C();
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        if (func_8008667C(6) == 4) {
            func_8004284C();
        } else {
            normal_date_girl_out();
        }
        break;
    case 17:
        normal_date_bg_fadeout();
        break;
    case 18:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 19:
        normal_date_speak();
        break;
    case 20:
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}

/* T-12703: existing aggregate and flag views reproduce the scene family. */
void func_8014E9D0(void) {
    GameState *g = &D_800E6280;
    u32 temp_v0;
    u32 temp_v0_2;
    u32 temp_v0_3;

    switch (g->unk_110A) {
    case 0:
        if (((u16) g->unk_0F4.h >> 0xC) == 2) {
            func_8005742C(0x40FC, 1);
        } else {
            func_8005742C(0x4105, 1);
        }
        func_8004284C();
        break;
    case 1:
        func_8007E934();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_801431D4();
            func_80057418(1, func_8005751C(1));
            func_80062CD0(0x622D);
            func_8004284C();
        }
        break;
    case 3:
        func_8007E81C();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            func_8007C740();
            g->unk_F5F = 6;
            func_80083474();
            D_800CA148 = 0x29;
        }
        break;
    case 5:
        func_80085A60();
        break;
    case 6:
        cd_read_sectors(0xFU, 0x801E8000, 0x4B13);
        func_8004284C();
        break;
    case 7:
        func_8007E390();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(6);
            switch (temp_v0) {
            case 0:
            case 1:
                if (g->unk_03E == 0x60) {
                    D_800CA148 = 0x21;
                } else {
                    D_800CA148 = 0x1D;
                }
                break;
            case 2:
                D_800CA148 = 0x1E;
                break;
            case 3:
                D_800CA148 = 0x1F;
                break;
            case 4:
                D_800CA148 = 0x20;
                break;
            }
            func_801508C4();
        }
        break;
    case 9:
        func_80085A60();
        break;
    case 10:
        func_8007E99C();
        break;
    case 11:
        D_80122EAC = 0;
        D_80122EB0 = (s32) g->unk_1109;
        D_80122EB4 = g->unk_110A + 1;
        main_state_set(0x80);
        break;
    case 12:
        func_800866A0(0x622D);
        break;
    case 13:
        func_801431D4();
        D_80125CA0 = 0xC0;
        func_8007C740();
        g->unk_F5F = 6;
        func_80083474();
        if (((u16) g->unk_0F4.h >> 0xC) == 3) {
            func_80083440(3U);
        }
        cd_read_sectors(0xFU, 0x801E8000, 0x4B13);
        break;
    case 14:
        if (func_8008667C(6) == 4) {
            func_8007E81C();
        } else {
            func_8007E884();
        }
        break;
    case 15:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = func_8008667C(6);
            switch (temp_v0_2) {
            case 0:
            case 1:
                if (((u16) g->unk_0F4.h >> 0xC) == 3) {
                    D_800CA148 = 0x2B;
                } else {
                    D_800CA148 = (g->unk_03E * 3) - 0xFB;
                }
                break;
            case 2:
                if (((u16) g->unk_0F4.h >> 0xC) == 3) {
                    D_800CA148 = 0x2C;
                } else {
                    D_800CA148 = (g->unk_03E * 3) - 0xFA;
                }
                break;
            case 3:
                if (((u16) g->unk_0F4.h >> 0xC) == 3) {
                    D_800CA148 = 0x2D;
                } else {
                    D_800CA148 = (g->unk_03E * 3) - 0xF9;
                }
                break;
            case 4:
                D_800CA148 = 0x2E;
                break;
            }
            func_801508C4();
        }
        break;
    case 16:
        func_80085A60();
        break;
    case 17:
        func_8004284C();
        temp_v0_3 = func_8008667C(6);
        switch (temp_v0_3) {
        case 0:
        case 1:
            D_800CA148 = 1;
            break;
        case 2:
            D_800CA148 = 2;
            break;
        case 3:
            D_800CA148 = 3;
            break;
        case 4:
            func_80042940(0x13);
            break;
        }
        break;
    case 18:
        func_80085A60();
        break;
    case 19:
        if (func_8008667C(6) == 4) {
            func_8004284C();
        } else {
            func_8007E5A0();
        }
        break;
    case 20:
        func_8007E934();
        break;
    case 21:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 22:
        func_80085A60();
        break;
    case 23:
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_8014EF24(void) {
    u32 temp_v0;
    u32 temp_v0_2;
    u32 temp_v0_3;

    switch (D_800E6280.unk_110A) {
    case 0:
        dec_bg_cd_read(0x4105, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_801431D4();
            dec_bg_show_set(1, func_8005751C(1));
            func_80062CD0(0x622D);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            D_800CA148 = 0x2A;
            addr_init_bustup();
            D_800E6280.unk_F5F = 6;
            func_80083474();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0xFU, 0x801E8000, 0x4B13);
        func_8004284C();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(6);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0x2F;
                break;
            case 2:
                D_800CA148 = 0x30;
                break;
            case 3:
                D_800CA148 = 0x31;
                break;
            case 4:
                D_800CA148 = 0x32;
                break;
            }
            func_801508C4();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        normal_date_girl_out();
        break;
    case 11:
        if (func_8008667C(6) == 4) {
            normal_date_speak();
        } else {
            func_8004284C();
        }
        break;
    case 12:
        func_80044750(0x202);
        D_80122EB0 = (s32) D_800E6280.unk_1109;
        D_80122EB4 = D_800E6280.unk_110A + 1;
        func_80042808();
        break;
    case 13:
        cd_read_sectors(0xFU, 0x801E8000, 0x4B13);
        func_8004284C();
        break;
    case 14:
        if (func_8008667C(6) == 4) {
            func_8004284C();
        } else {
            normal_date_girl_in();
        }
        break;
    case 15:
        if (cd_read_sync_status() & 1) {
            func_801508C4();
            temp_v0_2 = func_8008667C(6);
            switch (temp_v0_2) {
            case 0:
            case 1:
                D_800CA148 = 0x34;
                break;
            case 2:
                D_800CA148 = 0x37;
                break;
            case 3:
                D_800CA148 = 0x3A;
                break;
            case 4:
                D_800CA148 = 0x3D;
                break;
            }
            D_800CA148 += D_80122EA8;
        }
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        func_8004284C();
        temp_v0_3 = func_8008667C(6);
        switch (temp_v0_3) {
        case 0:
        case 1:
            D_800CA148 = 1;
            break;
        case 2:
            D_800CA148 = 2;
            break;
        case 3:
            D_800CA148 = 3;
            break;
        case 4:
            func_80042940(0x14);
            break;
        }
        break;
    case 18:
        normal_date_speak();
        break;
    case 19:
        normal_date_girl_out();
        break;
    case 20:
        normal_date_bg_fadeout();
        break;
    case 21:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 22:
        normal_date_speak();
        break;
    case 23:
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_8014F36C(void) {
    s32 temp_v0_2;
    s32 temp_v0_3;
    u32 temp_v0;
    u32 temp_v0_4;
    u32 temp_v0_5;

    switch (D_800E6280.unk_110A) {
    case 0:
        func_8005742C(0x4105, 1);
        func_8004284C();
        break;
    case 1:
        func_8007E934();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80057418(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 3:
        func_800866A0(0x622D);
        break;
    case 4:
        func_8007C740();
        D_800E6280.unk_F5F = 6;
        func_80083474();
        func_801431D4();
        D_800CA148 = 0x2A;
        break;
    case 5:
        func_8007E81C();
        break;
    case 6:
        func_80085A60();
        break;
    case 7:
        cd_read_sectors(0xFU, 0x801E8000, 0x4B13);
        func_8004284C();
        break;
    case 8:
        func_8007E390();
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(6);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0x40;
                break;
            case 2:
                D_800CA148 = 0x41;
                break;
            case 3:
                D_800CA148 = 0x42;
                break;
            case 4:
                D_800CA148 = 0x43;
                break;
            }
            func_801508C4();
        }
        break;
    case 10:
        func_80085A60();
        break;
    case 11:
        if (func_8008667C(6) == 4) {
            func_8007E5A0();
        } else {
            func_80042940(0xE);
        }
        break;
    case 12:
        func_80085A60();
        break;
    case 13:
        func_80042940(0x1B);
        break;
    case 14:
        temp_v0_2 = func_8008667C(6);
        switch (temp_v0_2) {
        case 0:
        case 1:
            D_800CA148 = 0x45;
            break;
        case 2:
            D_800CA148 = 0x46;
            break;
        case 3:
            D_800CA148 = 0x47;
            break;
        }
        func_8004284C();
        break;
    case 15:
        func_80085A60();
        break;
    case 16:
        func_8007E99C();
        break;
    case 17:
        ((BunkasaiModeFlags *)&D_800E7D4F)->b1 = 1;
        ((BunkasaiModeFlags *)&D_800E7D4F)->mode = 2;
        D_800E6280.unk_720 = D_800E6280.unk_1108;
        D_800E6280.unk_721 = D_800E6280.unk_1109;
        D_800E6280.unk_722 = D_800E6280.unk_110A + 1;
        main_state_set(0xD0);
        break;
    case 18:
        D_80122E98 = 1;
        D_80122EB0 = (s32) D_800E6280.unk_1109;
        D_80122EB4 = D_800E6280.unk_110A + 1;
        func_80042908(0xA3);
        break;
    case 19:
        func_80044750(0xBF);
        func_80044890(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        func_80085E30(1, 0);
        func_801431D4();
        D_80125CA0 = 0xC0;
        func_80064E84();
        func_800649D4();
        func_80084E4C();
        func_800847B8(6U);
        D_80122E98 = 0;
        func_8004284C();
        break;
    case 20:
        if (func_80044E8C() == 1) {
            func_80044750(0x200);
            func_800573AC();
            func_80057390(0);
            func_8005742C(0x4105, 1);
            func_8004284C();
        }
        break;
    case 21:
        if (cd_read_sync_status() & 1) {
            func_80057418(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 22:
        func_800866A0(0x622D);
        break;
    case 23:
        func_8007C740();
        D_800E6280.unk_F5F = 6;
        func_80083474();
        cd_read_sectors(0xFU, 0x801E8000, 0x4B13);
        break;
    case 24:
        func_8007E884();
        break;
    case 25:
        if (cd_read_sync_status() & 1) {
            temp_v0_3 = func_8008667C(6);
            switch (temp_v0_3) {
            case 0:
            case 1:
                D_800CA148 = 0x4B;
                break;
            case 2:
                D_800CA148 = 0x4C;
                break;
            case 3:
                D_800CA148 = 0x4D;
                break;
            }
            func_801508C4();
        }
        break;
    case 26:
        func_80085A60();
        break;
    case 27:
        if (func_8008667C(6) == 4) {
            func_8004284C();
        } else {
            func_8007E5A0();
        }
        break;
    case 28:
        ((BunkasaiModeFlags *)&D_800E7D4F)->b1 = 1;
        ((BunkasaiModeFlags *)&D_800E7D4F)->mode = 0;
        D_800E6280.unk_720 = D_800E6280.unk_1108;
        D_800E6280.unk_721 = D_800E6280.unk_1109;
        D_800E6280.unk_722 = D_800E6280.unk_110A + 1;
        func_80042908(0xA4);
        break;
    case 29:
        D_80122E98 = 1;
        D_80122EB0 = (s32) D_800E6280.unk_1109;
        D_80122EB4 = D_800E6280.unk_110A + 1;
        func_80042908(0xA3);
        break;
    case 30:
        func_80044750(0xBF);
        func_80044890(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        func_80085E30(1, 0);
        func_801431D4();
        D_80125CA0 = 0xC0;
        func_80064E84();
        func_800649D4();
        func_80084E4C();
        func_800847B8(6U);
        D_80122E98 = 0;
        func_8004284C();
        break;
    case 31:
        if (func_80044E8C() == 1) {
            func_80044750(0x200);
            func_800573AC();
            func_80057390(0);
            func_8005742C(0x4105, 1);
            func_8004284C();
        }
        break;
    case 32:
        if (cd_read_sync_status() & 1) {
            func_80057418(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 33:
        func_800866A0(0x622D);
        break;
    case 34:
        func_8007C740();
        D_800E6280.unk_F5F = 6;
        func_80083474();
        cd_read_sectors(0xFU, 0x801E8000, 0x4B13);
        break;
    case 35:
        if (func_8008667C(6) == 4) {
            func_8007E81C();
        } else {
            func_8007E884();
        }
        break;
    case 36:
        if (cd_read_sync_status() & 1) {
            temp_v0_4 = func_8008667C(6);
            switch (temp_v0_4) {
            case 0:
            case 1:
                D_800CA148 = 0x4E;
                break;
            case 2:
                D_800CA148 = 0x50;
                break;
            case 3:
                D_800CA148 = 0x52;
                break;
            case 4:
                D_800CA148 = 0x54;
                break;
            }
            func_801508C4();
            if ((u8) D_800E7D4E < 2U) {
                D_800CA148 += 1;
            }
        }
        break;
    case 37:
        func_80085A60();
        break;
    case 38:
        func_8004284C();
        temp_v0_5 = func_8008667C(6);
        switch (temp_v0_5) {
        case 0:
        case 1:
            D_800CA148 = 1;
            break;
        case 2:
            D_800CA148 = 2;
            break;
        case 3:
            D_800CA148 = 3;
            break;
        case 4:
            func_80042940(0x29);
            break;
        }
        break;
    case 39:
        func_80085A60();
        break;
    case 40:
        func_8007E5A0();
        break;
    case 41:
        func_8007E934();
        break;
    case 42:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 43:
        func_80085A60();
        break;
    case 44:
        ((BunkasaiModeFlags *)&D_800E7D4F)->b0 = 1;
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}

/* T-12703: existing aggregate and flag views reproduce the scene family. */
void func_8014FC70(void) {
    GameState *g = &D_800E6280;
    s32 temp_v0_2;
    u32 temp_v0;

    switch (g->unk_110A) {
    case 0:
        func_8005742C(0x410F, 1);
        func_8004284C();
        break;
    case 1:
        func_8007E934();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_801431D4();
            func_80057418(1, func_8005751C(1));
            func_80062CD0(0x622D);
            func_8004284C();
        }
        break;
    case 3:
        func_8007E81C();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            func_8007C740();
            g->unk_F5F = 6;
            func_80083474();
            D_800CA148 = 0x2B;
        }
        break;
    case 5:
        func_80085A60();
        break;
    case 6:
        cd_read_sectors(0xFU, 0x801E8000, 0x4B13);
        func_8004284C();
        break;
    case 7:
        func_8007E390();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(6);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0x56;
                break;
            case 2:
                D_800CA148 = 0x59;
                break;
            case 3:
                D_800CA148 = 0x5C;
                break;
            case 4:
                D_800CA148 = 0x5F;
                break;
            }
            func_801508C4();
        }
        break;
    case 9:
        func_80085A60();
        break;
    case 10:
        if (func_8008667C(6) == 4) {
            func_80042940(0xE);
        } else {
            if ((u16) g->unk_66C[4].unk_00 < 0x64U) {
                D_800CA148 += 1;
            }
            func_8004284C();
        }
        break;
    case 11:
        func_80085A60();
        break;
    case 12:
        temp_v0_2 = func_8008667C(6);
        switch (temp_v0_2) {                        /* switch 3; irregular */
        case 0:
        case 1:
            D_800CA148 = 1;
            break;
        case 2:
            D_800CA148 = 2;
            break;
        case 3:
            D_800CA148 = 3;
            break;
        }
        func_8004284C();
        break;
    case 13:
        func_80085A60();
        break;
    case 14:
        D_800CA148 = 0x78;
        func_8007E5A0();
        break;
    case 15:
        if (func_8008667C(6) == 4) {
            func_80085A60();
        } else {
            func_8007E934();
        }
        break;
    case 16:
        D_800CA148 = 0;
        if (func_8008667C(6) == 4) {
            func_8007E934();
        } else {
            func_8004284C();
        }
        break;
    case 17:
        func_80085A60();
        break;
    case 18:
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_8014FFE4(void) {
    u32 temp_v0;

    switch (D_800E6280.unk_110A) {
    case 0:
        func_8005742C(0x4118, 1);
        func_8004284C();
        break;
    case 1:
        func_8007E934();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_801431D4();
            func_80057418(1, func_8005751C(1));
            func_80062CD0(0x622D);
            func_8004284C();
        }
        break;
    case 3:
        func_8007E81C();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            func_8007C740();
            D_800E6280.unk_F5F = 6;
            func_80083474();
            D_800CA148 = 0x2C;
        }
        break;
    case 5:
        func_80085A60();
        break;
    case 6:
        cd_read_sectors(0xFU, 0x801E8000, 0x4B13);
        func_8004284C();
        break;
    case 7:
        func_8007E390();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(6);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0x60;
                break;
            case 2:
                D_800CA148 = 0x62;
                break;
            case 3:
                D_800CA148 = 0x64;
                break;
            case 4:
                D_800CA148 = 0x66;
                break;
            }
            func_801508C4();
        }
        break;
    case 9:
        func_80085A60();
        break;
    case 10:
        normal_date_girl_out();
        break;
    case 11:
        func_80085A60();
        break;
    case 12:
        func_8007E934();
        break;
    case 13:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 14:
        func_80085A60();
        break;
    case 15:
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_80150234(void) {
    u32 temp_v0;
    u32 temp_v0_2;

    switch (D_800E6280.unk_110A) {
    case 0:
        func_8005742C(0x410F, 1);
        func_8004284C();
        break;
    case 1:
        func_8007E934();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_801431D4();
            func_80057418(1, func_8005751C(1));
            func_80062CD0(0x622D);
            func_8004284C();
        }
        break;
    case 3:
        func_8007E81C();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            func_8007C740();
            D_800E6280.unk_F5F = 6;
            func_80083474();
            D_800CA148 = 0x2D;
        }
        break;
    case 5:
        func_80085A60();
        break;
    case 6:
        cd_read_sectors(0xFU, 0x801E8000, 0x4B13);
        func_8004284C();
        break;
    case 7:
        func_8007E390();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(6);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0x68;
                break;
            case 2:
                D_800CA148 = 0x69;
                break;
            case 3:
                D_800CA148 = 0x6A;
                break;
            case 4:
                D_800CA148 = 0x6B;
                break;
            }
            func_801508C4();
        }
        break;
    case 9:
        func_80085A60();
        break;
    case 10:
        temp_v0_2 = func_8008667C(6);
        switch (temp_v0_2) {
        case 0:
        case 1:
            D_800CA148 = 1;
            func_8004284C();
            break;
        case 2:
            D_800CA148 = 2;
            func_8004284C();
            break;
        case 3:
            D_800CA148 = 3;
            func_8004284C();
            break;
        case 4:
            func_8007E5A0();
            break;
        }
        break;
    case 11:
        func_80085A60();
        break;
    case 12:
        if (func_8008667C(6) == 4) {
            func_8004284C();
        } else {
            func_8007E5A0();
        }
        break;
    case 13:
        func_8007E934();
        break;
    case 14:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 15:
        func_80085A60();
        break;
    case 16:
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_8015052C(void) {
    u32 temp_v0;
    u32 temp_v0_2;

    switch (D_800E6280.unk_110A) {
    case 0:
        func_8005742C(0x40F4, 1);
        func_8004284C();
        break;
    case 1:
        func_8007E934();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_8014338C();
            func_80057418(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 3:
        func_8007E81C();
        break;
    case 4:
        if (D_800E6280.unk_03E == 0x5F) {
            D_800CA148 = 0x2E;
        } else {
            D_800CA148 = 0x2F;
        }
        func_8004284C();
        break;
    case 5:
        func_80085A60();
        break;
    case 6:
        func_8007E934();
        break;
    case 7:
        D_80122EAC = 2;
        D_80122EB0 = (s32) D_800E6280.unk_1109;
        D_80122EB4 = D_800E6280.unk_110A + 1;
        main_state_set(0x80);
        break;
    case 8:
        func_800866A0(0x622D);
        break;
    case 9:
        func_8014338C();
        D_80125CA0 = 0xC0;
        func_8007C740();
        D_800E6280.unk_F5F = 6;
        func_80083474();
        cd_read_sectors(0xFU, 0x801E8000, 0x4B13);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(6);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = (D_800E6280.unk_03E * 3) - 0xB0;
                break;
            case 2:
                D_800CA148 = (D_800E6280.unk_03E * 3) - 0xAF;
                break;
            case 3:
                D_800CA148 = (D_800E6280.unk_03E * 3) - 0xAE;
                break;
            case 4:
                D_800CA148 = 0x76;
                break;
            }
            func_801508C4();
        }
        break;
    case 12:
        func_80085A60();
        break;
    case 13:
        temp_v0_2 = func_8008667C(6);
        switch (temp_v0_2) {
        case 0:
        case 1:
            D_800CA148 = 1;
            func_8004284C();
            break;
        case 2:
            D_800CA148 = 2;
            func_8004284C();
            break;
        case 3:
            D_800CA148 = 3;
            func_8004284C();
            break;
        case 4:
            normal_date_girl_out();
            break;
        }
        break;
    case 14:
        func_80085A60();
        break;
    case 15:
        if (func_8008667C(6) == 4) {
            func_8004284C();
        } else {
            normal_date_girl_out();
        }
        break;
    case 16:
        func_8007E934();
        break;
    case 17:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 18:
        func_80085A60();
        break;
    case 19:
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_801508C4(void) {
    func_800847B8(6);
    func_801322A4();
    D_800CA14C = 0;
    D_800CA160 = D_801604F0;
    D_800CA164 = D_801604F4;
    D_800CA168 = D_801604F8;
    func_8004284C();
}
