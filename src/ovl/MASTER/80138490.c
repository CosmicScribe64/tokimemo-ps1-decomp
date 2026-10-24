#define MAIN_API_OVERRIDE_Vblnk_Timer_Init /* implicit declaration, as matched (main_api.h: void(void)) */
#include "ovl/MASTER.h"

void func_80138490(void) {
    u32 temp_v0;
    u32 temp_v0_2;

    func_80083808();
    switch (D_800E6280.step) {
    case 0:
        D_800E6280.day_event_index += 1;
        D_800E6280.daily_event_bits |= 1;
        func_80044750(0xCF);
        Vblnk_Timer_Init();
        if ((D_800E6280.month == (D_800E6280.player_birthday & 0xF)) && (D_800E6280.day == ((u32) (D_800E6280.player_birthday << 0x17) >> 0x1B))) {
            if (!(D_800E6280.daily_event_bits & 4)) {
                if (func_8008667C(0) < 2) {
                    D_800E6280.daily_event_bits |= 4;
                    cd_read_sectors(3U, 0x801B0000, 0xAF43);
                    func_80042940(0x1B);
                } else {
                    goto block_7;
                }
            } else {
                goto block_8;
            }
        } else {
block_7:
block_8:
            cd_read_sectors(3U, 0x801E8000, 0x49B7);
            func_8004284C();
        }
        break;
    case 1:
        normal_date_move_place();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80044890(1, 0xC043, 0xC01D, 0xCA95, 0xCA4F, 0xCA3E);
            D_800E6280.current_girl = 0;
            func_800847B8(0U);
            func_801336E8();
            D_800CA148 = 0;
            D_800CA14C = 0;
            D_800CA160 = (s32) D_8013C4A0;
            D_800CA164 = (s32) D_8013C4A4;
            D_800CA168 = (s32) D_8013C4A8;
            func_8004284C();
        }
        break;
    case 3:
        normal_date_speak();
        break;
    case 4:
        normal_date_two_select();
        break;
    case 5:
        if ((*(u32 *)&D_80122CDC) != 0) {
            normal_date_speak();
        } else {
            func_80042940(7);
        }
        break;
    case 6:
        D_800E6280.girls[0].affection -= 1;
        D_800E6280.girls[0].syoushin += 5;
        func_80042808();
        break;
    case 7:
        if (func_80044E8C() == 1) {
            func_80044750(0x200);
            func_80062CD0(0x541D);
            D_800CA148 = 2;
            func_8004284C();
        }
        break;
    case 8:
        normal_date_speak();
        break;
    case 9:
        D_800CA148 = 5;
        func_8004284C();
        break;
    case 10:
        normal_date_speak();
        break;
    case 11:
        normal_date_three_select();
        break;
    case 12:
        func_80138E58();
        D_800CA148 = 6;
        func_8004284C();
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            func_80083474();
        }
        break;
    case 15:
        D_800CA148 = 8;
        func_8004284C();
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        normal_date_girl_in();
        break;
    case 18:
        temp_v0 = func_8008667C(0);
        switch (temp_v0) {
        case 0:
        case 1:
            D_800CA148 = 0xF;
            break;
        case 2:
            D_800CA148 = 0xF;
            break;
        case 3:
            D_800CA148 = 0x10;
            break;
        case 4:
            D_800CA148 = 0x11;
            break;
        }
        func_8004284C();
        break;
    case 19:
        normal_date_speak();
        break;
    case 20:
        temp_v0_2 = func_8008667C(0);
        switch (temp_v0_2) {
        case 0:
        case 1:
            D_800CA148 = 0x17;
            break;
        case 2:
            D_800CA148 = 0x18;
            break;
        case 3:
            D_800CA148 = 0x19;
            break;
        case 4:
            D_800CA148 = 0x1A;
            break;
        }
        func_8004284C();
        break;
    case 21:
        normal_date_speak();
        break;
    case 22:
        switch ((*(u32 *)&D_80122CDC)) {
        case 0:
            D_800CA148 = 0x1B;
            func_80083440(1U);
            D_800E6280.girls[0].tokimeki += 3;
            D_800E6280.girls[0].affection += 2;
            D_800E6280.girls[0].syoushin -= 0xF;
            break;
        case 1:
            D_800CA148 = 0x1D;
            func_80083440(2U);
            D_800E6280.girls[0].tokimeki += 2;
            D_800E6280.girls[0].affection += 1;
            D_800E6280.girls[0].syoushin -= 0xA;
            break;
        case 2:
            D_800CA148 = 0x1F;
            func_80083440(3U);
            D_800E6280.girls[0].affection += 1;
            D_800E6280.girls[0].syoushin += 0xA;
            break;
        }
        func_80063668();
        func_8004284C();
        break;
    case 23:
        normal_date_speak();
        break;
    case 24:
        normal_date_girl_out();
        break;
    case 25:
        normal_date_speak();
        break;
    case 26:
        func_80085B3C(0x15, 6);
        func_80042808();
        break;
    case 27:
        D_800E6280.day_event_index += 1;
        normal_date_move_place();
        break;
    case 28:
        if (cd_read_sync_status() & 1) {
            D_800E6280.current_girl = 0;
            func_800847B8(0U);
            func_801337B8();
            D_800CA148 = 0;
            D_800CA14C = 0;
            D_800CA160 = D_8013C4D8;
            D_800CA164 = D_8013C50C;
            D_800CA168 = D_8013C540;
            func_80062CD0(0x541D);
            func_8004284C();
        }
        break;
    case 29:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            func_80083474();
            func_80044750(0xBF);
            func_80044890(1, 0xC043, 0xC01D, 0xCA95, 0xCA4F, 0xCA3E);
        }
        break;
    case 30:
        if (func_80044E8C() == 1) {
            func_80044750(0x200);
            func_8004284C();
        }
        break;
    case 31:
        normal_date_speak();
        break;
    case 32:
        normal_date_girl_in();
        break;
    case 33:
        normal_date_speak();
        break;
    case 34:
        cd_read_sectors(3U, 0x801E8000, 0x49B7);
        func_8004284C();
        break;
    case 35:
        if (cd_read_sync_status() & 1) {
            func_801336E8();
            D_800CA148 = 0xC;
            D_800CA14C = 0;
            D_800CA160 = (s32) D_8013C4A0;
            D_800CA164 = (s32) D_8013C4A4;
            D_800CA168 = (s32) D_8013C4A8;
            func_8004284C();
        }
        break;
    case 36:
        normal_date_speak();
        break;
    case 37:
        D_800CA148 = 0x21;
        func_8004284C();
        break;
    case 38:
        normal_date_speak();
        break;
    case 39:
        normal_date_three_select();
        break;
    case 40:
        D_800CA148 = 0x17;
        func_8004284C();
        break;
    case 41:
        normal_date_speak();
        break;
    case 42:
        switch ((*(u32 *)&D_80122CDC)) {
        case 0:
            D_800CA148 = 0x1B;
            func_80083440(1U);
            D_800E6280.girls[0].tokimeki += 3;
            D_800E6280.girls[0].affection += 2;
            D_800E6280.girls[0].syoushin -= 0xF;
            break;
        case 1:
            D_800CA148 = 0x1D;
            func_80083440(2U);
            D_800E6280.girls[0].tokimeki += 2;
            D_800E6280.girls[0].affection += 1;
            D_800E6280.girls[0].syoushin -= 0xA;
            break;
        case 2:
            D_800CA148 = 0x1F;
            func_80083440(3U);
            D_800E6280.girls[0].affection += 1;
            D_800E6280.girls[0].syoushin += 0xA;
            break;
        }
        func_80063668();
        func_8004284C();
        break;
    case 43:
        normal_date_speak();
        break;
    case 44:
        normal_date_girl_out();
        break;
    case 45:
        normal_date_speak();
        break;
    case 46:
        func_80085B3C(0x15, 6);
        func_80042808();
        break;
    default:
        func_80046500();
        break;
    }
    Default_Disp();
}

void func_80138E58(void) {
    s32 idx; /* unused: its stack slot sits above tbl (T-3330) */
    PtrTbl3 tbl;

    tbl = D_8013C700;
    strcpy(D_800CA25C, tbl.p[D_80122CDC]);
}

INCLUDE_RODATA("asm/ovl/MASTER/data/MASTER/80138490.rodata", D_8013BFB0);

INCLUDE_RODATA("asm/ovl/MASTER/data/MASTER/80138490.rodata", D_8013BFBC);

INCLUDE_RODATA("asm/ovl/MASTER/data/MASTER/80138490.rodata", D_8013BFC8);
