#include "common.h"
#include "ovl/BUNKASAI.h"

void func_8013B190(void) {
    BunkModePair *state;

    state = (BunkModePair *)&D_80122EB8;
    state->unk_04 = 2;
    D_800E6280.unk_1BC[2].unk_0C.f.b2 = 1;
    switch (state->unk_00) {
    case 2:
        func_80042908(0x3C);
        return;
    case 3:
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            func_80042908(0x3D);
            return;
        case 0x60:
            func_80042908(0x3F);
            return;
        case 0x61:
            func_80042908(0x40);
            return;
        }
        break;
    default:
        func_80046500();
        return;
    }
}

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI/8013B190", func_8013B25C);

void func_8013B688(void) {
    u32 temp_v0;
    u32 temp_v0_2;

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
            func_80062CD0(0x522E);
            dec_bg_show_set(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            func_801431D4();
            addr_init_bustup();
            D_800E6280.unk_F5F = 2;
            func_80083474();
            cd_read_sectors(0xAU, 0x801E8000, 0x4A2C);
        }
        break;
    case 4:
        normal_date_bggirl_fadein();
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(2);
            switch (temp_v0) {
            case 0:
            case 1:
            case 2:
                D_800CA148 = 0x1E;
                break;
            case 3:
                D_800CA148 = 0x20;
                break;
            case 4:
                D_800CA148 = 0x22;
                break;
            }
            func_8013C3C4();
            if ((func_8008667C(2) == 0) && !(D_800E6280.unk_1BC[2].unk_0A & 1)) {
                D_800CA148 = 0x34;
                func_80042940(0x1B);
            }
        }
        break;
    case 6:
        normal_date_speak();
        break;
    case 7:
        if (func_8008667C(2) < 3) {
            func_8004284C();
        } else {
            normal_date_girl_out();
        }
        break;
    case 8:
        normal_date_speak();
        break;
    case 9:
        if (func_8008667C(2) < 3) {
            normal_date_girl_out();
        } else {
            func_8004284C();
        }
        break;
    case 10:
        func_80044750(0x202);
        D_80122EB0 = (s32) D_800E6280.unk_1109;
        D_80122EB4 = D_800E6280.unk_110A + 1;
        func_80042808();
        break;
    case 11:
        cd_read_sectors(0xAU, 0x801E8000, 0x4A2C);
        func_8004284C();
        break;
    case 12:
        if (func_8008667C(2) < 3) {
            normal_date_girl_in();
        } else {
            func_8004284C();
        }
        break;
    case 13:
        if (cd_read_sync_status() & 1) {
            func_8013C3C4();
            temp_v0_2 = func_8008667C(2);
            switch (temp_v0_2) {
            case 0:
            case 1:
                D_800CA148 = (D_80122EA8 * 2) + 0x24;
                break;
            case 2:
                D_800CA148 = (D_80122EA8 * 2) + 0x2A;
                break;
            case 3:
            case 4:
                D_800CA148 = D_80122EA8 + 0x30;
                func_80042940(0x18);
                break;
            }
        }
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        if ((func_8008667C(2) < 2) && (D_80122EA8 != 1)) {
            func_80044750(0x508);
        }
        func_8004284C();
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        if (func_8008667C(2) == 2) {
            D_800CA148 = 0x33;
            func_8004284C();
        } else {
            func_80042940(0x13);
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
        D_800CA148 = 1;
        func_8004284C();
        break;
    case 22:
        normal_date_speak();
        break;
    case 23:
        func_80142BC8();
        break;
    case 24:
        normal_date_speak();
        break;
    case 25:
        normal_date_bg_fadeout();
        break;
    case 26:
        func_80042940(0x15);
        break;
    case 27:
        normal_date_speak();
        break;
    case 28:
        normal_date_girl_out();
        break;
    case 29:
        normal_date_speak();
        break;
    case 30:
        normal_date_girl_in();
        break;
    case 31:
        normal_date_speak();
        break;
    case 32:
        func_80042940(0x13);
        break;
    default:
        func_80046500();
        break;
    }
}

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI/8013B190", func_8013BB94);

void func_8013C0AC(void) {
    u32 temp_v0;
    u32 temp_v0_2;

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
            func_800866A0(0x522E);
        }
        break;
    case 3:
        dec_bg_show_set(1, func_8005751C(1));
        func_801431D4();
        addr_init_bustup();
        D_800E6280.unk_F5F = 2;
        func_80083474();
        cd_read_sectors(0xAU, 0x801E8000, 0x4A2C);
        break;
    case 4:
        normal_date_bggirl_fadein();
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(2);
            switch (temp_v0) {
            case 0:
            case 1:
            case 2:
                D_800CA148 = 0x3E;
                break;
            case 3:
                D_800CA148 = 0x3F;
                break;
            case 4:
                D_800CA148 = 0x40;
                break;
            }
            func_8013C3C4();
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
        func_800866A0(0x522E);
        break;
    case 10:
        func_801431D4();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        D_800E6280.unk_F5F = 2;
        func_80083474();
        cd_read_sectors(0xAU, 0x801E8000, 0x4A2C);
        break;
    case 11:
        normal_date_bggirl_fadein();
        break;
    case 12:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = func_8008667C(2);
            switch (temp_v0_2) {
            case 0:
            case 1:
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
            func_8013C3C4();
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

void func_8013C3C4(void) {
    func_800847B8(2);
    func_80132138();
    D_800CA14C = 0;
    D_800CA160 = D_801604F0;
    D_800CA164 = D_801604F4;
    D_800CA168 = D_801604F8;
    func_8004284C();
}
