#define MAIN_API_OVERRIDE_func_80083440 /* T-12230: original K&R call; callee narrows the argument. */
#include "common.h"
#include "ovl/BUNKASAI.h"

void func_80083440();

void func_80146030(void) {
    s32 selector;
    BunkSelectWords *work;

    work = (BunkSelectWords *)&D_80122EB8;
    selector = work->unk_00;
    work->unk_04 = 3;
    D_800E6280.unk_1BC[3].unk_0C.f.b2 = 1;
    switch (selector) {
    case 4:
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            func_80042908(0x51);
            return;
        case 0x60:
            func_80042908(0x52);
            return;
        case 0x61:
            func_80042908(0x53);
            return;
        }
        break;
    case 5:
        func_80042908(0x54);
        return;
    default:
        func_80046500();
        return;
    }
}
void func_80146100(void) {
    u32 temp_v0;
    u32 temp_v0_2;

    switch (D_800E6280.unk_110A) {
    case 0:
        dec_bg_cd_read(0x410F, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, func_8005751C(1));
            func_80062CD0(0x58DC);
            func_8004284C();
        }
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            func_801431D4();
            addr_init_bustup();
            D_800E6280.unk_F5F = 3;
            func_80083474();
            cd_read_sectors(0xCU, 0x801E8000, 0x4AFC);
        }
        break;
    case 4:
        normal_date_bggirl_fadein();
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(3);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 2;
                break;
            case 2:
            case 3:
                D_800CA148 = 5;
                break;
            case 4:
                D_800CA148 = 9;
                break;
            }
            func_80146F38();
        }
        break;
    case 6:
        normal_date_speak();
        break;
    case 7:
        if ((func_8008667C(3) == 2) || (func_8008667C(3) == 3)) {
            normal_date_girl_out();
        } else {
            func_8004284C();
        }
        break;
    case 8:
        if ((func_8008667C(3) == 2) || (func_8008667C(3) == 3)) {
            normal_date_speak();
        } else {
            func_8004284C();
        }
        break;
    case 9:
        if ((func_8008667C(3) == 2) || (func_8008667C(3) == 3)) {
            normal_date_bg_fadeout();
        } else {
            normal_date_bggirl_fadeout();
        }
        break;
    case 10:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        normal_date_bggirl_fadein();
        break;
    case 13:
        temp_v0_2 = func_8008667C(3);
        switch (temp_v0_2) {
        case 0:
        case 1:
            D_800CA148 = 3;
            break;
        case 2:
        case 3:
            D_800CA148 = 7;
            break;
        case 4:
            D_800CA148 = 0xA;
            break;
        }
        if ((u16) D_800E6280.unk_66C[4].unk_00 < 0x64U) {
            D_800CA148 += 1;
        }
        func_8004284C();
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        normal_date_girl_out();
        break;
    case 16:
        normal_date_bg_fadeout();
        break;
    case 17:
        D_800CA148 = 1;
        func_8004284C();
        break;
    case 18:
        normal_date_speak();
        break;
    case 19:
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI/80146030", func_801464A8);

void func_801466FC(void) {
    u32 temp_v0;
    u32 temp_v0_2;

    switch (D_800E6280.unk_110A) {
    case 0:
        dec_bg_cd_read(0x410F, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, func_8005751C(1));
            func_80062CD0(0x58DC);
            func_8004284C();
        }
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            func_801431D4();
            addr_init_bustup();
            D_800E6280.unk_F5F = 3;
            func_80083474();
            cd_read_sectors(0xCU, 0x801E8000, 0x4AFC);
        }
        break;
    case 4:
        normal_date_bggirl_fadein();
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(3);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0x10;
                break;
            case 2:
                D_800CA148 = 0x13;
                break;
            case 3:
                D_800CA148 = 0x14;
                break;
            case 4:
                D_800CA148 = 0x15;
                break;
            }
            func_80146F38();
        }
        break;
    case 6:
        normal_date_speak();
        break;
    case 7:
        if (func_8008667C(3) < 2) {
            func_80083440(0U);
            func_8004284C();
        } else {
            func_80042940(0xB);
        }
        break;
    case 8:
        normal_date_speak();
        break;
    case 9:
        func_80083440(get_g_zyotai_h(3));
        func_8004284C();
        break;
    case 10:
        normal_date_speak();
        break;
    case 11:
        normal_date_bggirl_fadeout();
        break;
    case 12:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        normal_date_bggirl_fadein();
        break;
    case 15:
        temp_v0_2 = func_8008667C(3);
        switch (temp_v0_2) {
        case 0:
        case 1:
            D_800CA148 = 0x16;
            break;
        case 2:
            D_800CA148 = 0x18;
            break;
        case 3:
            D_800CA148 = 0x1A;
            break;
        case 4:
            D_800CA148 = 0x1C;
            break;
        }
        if ((u16) D_800E6280.unk_66C[4].unk_00 < 0x12CU) {
            D_800CA148 += 1;
        }
        func_8004284C();
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        normal_date_girl_out();
        break;
    case 18:
        normal_date_bg_fadeout();
        break;
    case 19:
        D_800CA148 = 1;
        func_8004284C();
        break;
    case 20:
        normal_date_speak();
        break;
    case 21:
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}

/* T-12702: reload the selector after calls that may update GameState. */
void func_80146A78(void) {
    u32 temp_v0;
    u32 temp_v0_2;
    u8 temp_v0_3;

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
            func_80062CD0(0x58AF);
            dec_bg_show_set(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            func_8014338C();
            addr_init_bustup();
            D_800E6280.unk_F5F = 3;
            func_80083474();
            cd_read_sectors(0xCU, 0x801E8000, 0x4AFC);
        }
        break;
    case 4:
        normal_date_bggirl_fadein();
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(3);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0x1E;
                break;
            case 2:
                D_800CA148 = 0x1F;
                break;
            case 3:
                D_800CA148 = 0x20;
                break;
            case 4:
                D_800CA148 = 0x21;
                break;
            }
            D_800CA148 = (D_800CA148 + (D_800E6280.unk_03E * 4)) - 0x17C;
            func_80146F38();
        }
        break;
    case 6:
        normal_date_speak();
        break;
    case 7:
        normal_date_girl_out();
        break;
    case 8:
        normal_date_bggirl_fadeout();
        break;
    case 9:
        D_80122EAC = 6;
        D_80122EB0 = (s32) D_800E6280.unk_1109;
        D_80122EB4 = D_800E6280.unk_110A + 1;
        main_state_set(0x80);
        break;
    case 10:
        func_800866A0(0x58AF);
        break;
    case 11:
        func_8014338C();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        D_800E6280.unk_F5F = 3;
        func_80083474();
        cd_read_sectors(0xCU, 0x801E8000, 0x4AFC);
        break;
    case 12:
        normal_date_bggirl_fadein();
        break;
    case 13:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = func_8008667C(3);
            switch (temp_v0_2) {
            case 0:
            case 1:
                D_800CA148 = 0x2A;
                break;
            case 2:
                D_800CA148 = 0x2C;
                break;
            case 3:
                D_800CA148 = 0x2E;
                break;
            case 4:
                D_800CA148 = 0x30;
                break;
            }
            temp_v0_3 = D_800E6280.unk_03E;
            D_800CA148 = (D_800CA148 + (temp_v0_3 * 0xA)) - 0x3B6;
            if ((u16) D_800E6280.unk_66C[5].unk_00 < (u32) ((temp_v0_3 * 0x64) - 0x24B8)) {
                if (func_8008667C(3) == 4) {
                    D_800CA148 += 2;
                } else {
                    D_800CA148 += 1;
                }
            }
            if ((D_800E6280.unk_03E == 0x61) && (func_8008667C(3) >= 2)) {
                D_800CA148 -= 2;
            }
            func_80146F38();
        }
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        if (func_8008667C(3) == 4) {
            normal_date_girl_out();
        } else {
            func_80042940(0x11);
        }
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        if (func_8008667C(3) == 4) {
            func_8004284C();
        } else {
            normal_date_girl_out();
        }
        break;
    case 18:
        normal_date_bg_fadeout();
        break;
    case 19:
        D_800CA148 = 1;
        func_8004284C();
        break;
    case 20:
        normal_date_speak();
        break;
    case 21:
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_80146F38(void) {
    func_800847B8(3);
    func_80132208();
    D_800CA14C = 0;
    D_800CA160 = D_801604F0;
    D_800CA164 = D_801604F4;
    D_800CA168 = D_801604F8;
    func_8004284C();
}
