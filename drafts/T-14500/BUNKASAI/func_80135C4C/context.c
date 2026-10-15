#include "common.h"
#include "ovl/BUNKASAI.h"

void func_801354B0(void) {
    BunkSelectWords *work = (BunkSelectWords *)&D_80122EB8;
    s32 selector = work->unk_00;
    work->unk_04 = 1;
    D_800E6280.unk_1BC[1].unk_0C.f.b2 = 1;
    switch (selector) {
    case 0:
        if (D_800E6280.unk_03E == 0x61) {
            func_80042908(0x2C);
            return;
        }
        func_80042908(0x2B);
        return;
    case 1:
        func_80042908(0x2D);
        return;
    default:
        func_80046500();
        return;
    }
}

void func_80135554(void) {
    u32 temp_v0;
    u32 temp_v0_2;

    switch (D_800E6280.unk_110A) {
    case 0:
        dec_bg_cd_read(0x40EA, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_800866A0(0x5D41);
        }
        break;
    case 3:
        dec_bg_show_set(1, func_8005751C(1));
        func_801431D4();
        addr_init_bustup();
        D_800E6280.unk_F5F = 1;
        func_80083474();
        cd_read_sectors(0xBU, 0x801E8000, 0x4AA9);
        break;
    case 4:
        normal_date_bg_fadein();
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(1);
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
            if (D_800E6280.unk_03E == 0x60) {
                D_800CA148 += 8;
            }
            func_80136230();
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
        normal_date_bggirl_fadein();
        break;
    case 12:
        temp_v0_2 = func_8008667C(1);
        switch (temp_v0_2) {
        case 0:
        case 1:
            D_800CA148 = 0x12;
            break;
        case 2:
            D_800CA148 = 0x14;
            break;
        case 3:
            D_800CA148 = 0x16;
            break;
        case 4:
            D_800CA148 = 0x18;
            break;
        }
        if ((u16) D_800E6280.unk_66C[0].unk_00 < (u32) ((D_800E6280.unk_03E * 0x64) - 0x24B8)) {
            D_800CA148 += 1;
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
        if (func_8008667C(1) == 4) {
            if ((u16) D_800E6280.unk_66C[0].unk_00 < (u32) ((D_800E6280.unk_03E * 0x64) - 0x24B8)) {
                D_800CA148 = 0x3D;
            } else {
                D_800CA148 = 0x3E;
            }
            func_8004284C();
        } else {
            func_80042940(0x11);
        }
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        normal_date_bg_fadeout();
        break;
    case 18:
        D_800CA148 = 1;
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

void func_80135914(void) {
    u32 temp_v0;
    u32 temp_v0_2;

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
            func_800866A0(0x5D41);
        }
        break;
    case 3:
        dec_bg_show_set(1, func_8005751C(1));
        func_8014338C();
        addr_init_bustup();
        D_800E6280.unk_F5F = 1;
        func_80083474();
        cd_read_sectors(0xBU, 0x801E8000, 0x4AA9);
        break;
    case 4:
        normal_date_bggirl_fadein();
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(1);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0x1A;
                break;
            case 2:
                D_800CA148 = 0x1C;
                break;
            case 3:
                D_800CA148 = 0x1E;
                break;
            case 4:
                D_800CA148 = 0x20;
                break;
            }
            func_80136230();
        }
        break;
    case 6:
        normal_date_speak();
        break;
    case 7:
        normal_date_bggirl_fadeout();
        break;
    case 8:
        D_80122EAC = 3;
        D_80122EB0 = (s32) D_800E6280.unk_1109;
        D_80122EB4 = D_800E6280.unk_110A + 1;
        main_state_set(0x80);
        break;
    case 9:
        func_800866A0(0x5D41);
        break;
    case 10:
        func_8014338C();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        D_800E6280.unk_F5F = 1;
        func_80083474();
        cd_read_sectors(0xBU, 0x801E8000, 0x4AA9);
        break;
    case 11:
        normal_date_bggirl_fadein();
        break;
    case 12:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = func_8008667C(1);
            switch (temp_v0_2) {
            case 0:
            case 1:
                D_800CA148 = 0x1B;
                break;
            case 2:
                D_800CA148 = 0x1D;
                break;
            case 3:
                D_800CA148 = 0x1F;
                break;
            case 4:
                D_800CA148 = 0x21;
                break;
            }
            func_80136230();
        }
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

/* bulk_m2c declarations (T-11190) */
extern s8 D_800E71DF;
extern u16 D_800E68F0;
extern u8 D_800E62BE;
extern u8 D_800E7389;
extern u8 D_800E738A;
/* end bulk_m2c declarations */
void func_80135C4C(void) {
    s16 var_v1;
    s16 var_v1_2;
    s16 var_v1_3;
    u32 temp_v0;
    u32 temp_v0_2;
    u32 temp_v0_3;

    switch (D_800E738A) {
    case 0:
        dec_bg_cd_read(0x40F4, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_800866A0(0x5D6E);
        }
        break;
    case 3:
        dec_bg_show_set(1, func_8005751C(1));
        func_8014338C();
        addr_init_bustup();
        D_800E71DF = 1;
        func_80083474();
        cd_read_sectors(0xBU, 0x801E8000, 0x4AA9);
        break;
    case 4:
        normal_date_bggirl_fadein();
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            if (D_800E62BE == 0x60) {
                temp_v0 = func_8008667C(1);
                switch (temp_v0) {
                case 0:
                case 1:
                    D_800CA148 = 0x26;
                    break;
                case 2:
                case 3:
                    D_800CA148 = 0x27;
                    break;
                case 4:
                    D_800CA148 = 0x28;
                    break;
                }
            } else {
                temp_v0_2 = func_8008667C(1);
                switch (temp_v0_2) {
                case 0:
                case 1:
                    var_v1 = 0x22;
block_21:
                    D_800CA148 = var_v1;
                    break;
                case 2:
                    var_v1 = 0x23;
                    goto block_21;
                case 3:
                    var_v1 = 0x24;
                    goto block_21;
                case 4:
                    var_v1 = 0x25;
                    goto block_21;
                }
                if (D_800E62BE == 0x61) {
                    D_800CA148 += 7;
                }
            }
            func_80136230();
        }
        break;
    case 6:
        normal_date_speak();
        break;
    case 7:
        if ((D_800E62BE == 0x61) && (func_8008667C(1) == 4)) {
            D_800CA148 = 0x3E;
            normal_date_girl_out();
        } else {
            normal_date_bggirl_fadeout();
        }
        break;
    case 8:
        if ((D_800E62BE == 0x61) && (func_8008667C(1) == 4)) {
            normal_date_speak();
        } else {
            func_80042940(0xA);
        }
        break;
    case 9:
        normal_date_bg_fadeout();
        break;
    case 10:
        D_80122EB0 = (s32) D_800E7389;
        D_80122EB4 = D_800E738A + 1;
        main_state_set(0x80);
        break;
    case 11:
        dec_bg_reset();
        set_dec_bri(0);
        dec_bg_cd_read(0x40F4, 1);
        func_8004284C();
        break;
    case 12:
        if (cd_read_sync_status() & 1) {
            func_800866A0(0x5D6E);
        }
        break;
    case 13:
        dec_bg_show_set(1, func_8005751C(1));
        func_8014338C();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        D_800E71DF = 1;
        func_80083474();
        cd_read_sectors(0xBU, 0x801E8000, 0x4AA9);
        break;
    case 14:
        normal_date_bggirl_fadein();
        break;
    case 15:
        if (cd_read_sync_status() & 1) {
            temp_v0_3 = func_8008667C(1);
            switch (temp_v0_3) {
            case 0:
            case 1:
                var_v1_2 = 0x2D;
block_48:
                D_800CA148 = var_v1_2;
                break;
            case 2:
                var_v1_2 = 0x2F;
                goto block_48;
            case 3:
                var_v1_2 = 0x31;
                goto block_48;
            case 4:
                var_v1_2 = 0x33;
                goto block_48;
            }
            var_v1_3 = D_800CA148;
            if ((u16) D_800E68F0 < (u32) ((D_800E62BE * 0x64) - 0x24B8)) {
                var_v1_3 += 1;
            }
            D_800CA148 = var_v1_3;
            if (D_800E62BE == 0x61) {
                D_800CA148 = var_v1_3 + 8;
            }
            func_80136230();
        }
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        normal_date_girl_out();
        break;
    case 18:
        if (func_8008667C(1) == 4) {
            if (((u8) D_800E62BE < 0x61U) && ((u16) D_800E68F0 >= (u32) ((D_800E62BE * 0x64) - 0x24B8))) {
                D_800CA148 = 0x3E;
                func_8004284C();
            } else if (D_800E62BE == 0x61) {
                if ((u16) D_800E68F0 >= 0x12CU) {
                    D_800CA148 = 0x3E;
                    func_8004284C();
                } else {
                    D_800CA148 = 0x3F;
                    func_8004284C();
                }
            } else {
                func_80042940(0x14);
            }
        } else {
            func_80042940(0x14);
        }
        break;
    case 19:
        normal_date_speak();
        break;
    case 20:
        normal_date_bg_fadeout();
        break;
    case 21:
        D_800CA148 = 1;
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

void func_80136230(void) {
    func_800847B8(1);
    func_801320D0();
    D_800CA14C = 0;
    D_800CA160 = D_801604F0;
    D_800CA164 = D_801604F4;
    D_800CA168 = D_801604F8;
    func_8004284C();
}
