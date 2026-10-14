#include "common.h"
#include "ovl/BUNKAKEN.h"

void func_80150D30(void) {
    switch (D_80122EB8) {                           /* switch 1 */
    case 0:                                         /* switch 1 */
        switch (D_800E62BE) {                       /* switch 2; irregular */
        case 0x60:                                  /* switch 2 */
            func_80042908(0x7B);
            return;
        case 0x61:                                  /* switch 2 */
            func_80042908(0x7C);
            return;
        default:                                    /* switch 2 */
            func_80046500();
            return;
        }
        break;
    case 1:                                         /* switch 1 */
        func_80042908(0x7D);
        return;
    case 2:                                         /* switch 1 */
        func_80042908(0x7E);
        return;
    case 3:                                         /* switch 1 */
        switch (D_800E62BE) {                       /* switch 3; irregular */
        case 0x60:                                  /* switch 3 */
            func_80042908(0x7F);
            return;
        case 0x61:                                  /* switch 3 */
            func_80042908(0x7E);
            return;
        }
        break;
    case 4:                                         /* switch 1 */
        switch (D_800E62BE) {                       /* switch 4; irregular */
        case 0x60:                                  /* switch 4 */
            func_80042908(0x80);
            return;
        case 0x61:                                  /* switch 4 */
            func_80042908(0x81);
            return;
        }
        break;
    case 5:                                         /* switch 1 */
        func_80042908(0x82);
        return;
    default:                                        /* switch 1 */
        func_80046500();
        return;
    }
}

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80150D30", func_80150E88);

void func_80151104(void) {
    s32 temp_v0;
    s32 temp_v0_2;

    switch (D_800E6280.unk_110A) {
    case 0:
        dec_bg_cd_read(0x467C, 1);
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
                D_800CA148 = 0xB;
                break;
            case 2:
                D_800CA148 = 0xC;
                break;
            case 3:
                D_800CA148 = 0xD;
                break;
            }
            func_80152618();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_bggirl_fadeout();
        break;
    case 7:
        D_80122EAC = 1;
        D_80122EB0 = (s32) D_800E6280.unk_1109;
        D_80122EB4 = D_800E6280.unk_110A + 1;
        main_state_set(0x80);
        break;
    case 8:
        func_800866A0(0x64D0);
        break;
    case 9:
        func_80145844();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        D_800E6280.unk_F5F = 9;
        func_80083474();
        cd_read_sectors(9U, 0x801E8000, 0x4B3E);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = func_8008667C(9);
            switch (temp_v0_2) {
            case 0:
            case 1:
                D_800CA148 = 0xE;
                break;
            case 2:
                D_800CA148 = 0xF;
                break;
            case 3:
                D_800CA148 = 0x10;
                break;
            }
            func_80152618();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        normal_date_bggirl_fadeout();
        break;
    case 14:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        func_80145530();
        break;
    default:
        func_80046500();
        break;
    }
}

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80150D30", func_801513E8);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80150D30", func_80151738);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80150D30", func_80151A78);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80150D30", func_80151EC0);

void func_801520B4(void) {
    s32 temp_v0;
    s32 temp_v0_2;

    switch (D_800E6280.unk_110A) {
    case 0:
        dec_bg_cd_read(0x410F, 1);
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
                D_800CA148 = 0x30;
                break;
            case 2:
                D_800CA148 = 0x31;
                break;
            case 3:
                D_800CA148 = 0x32;
                break;
            }
            func_80152618();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        D_800CA148 = 1;
        normal_date_bggirl_fadeout();
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        temp_v0_2 = func_8008667C(9);
        switch (temp_v0_2) {
        case 0:
        case 1:
            D_800CA148 = 0x33;
            break;
        case 2:
            D_800CA148 = 0x34;
            break;
        case 3:
            D_800CA148 = 0x35;
            break;
        }
        normal_date_bggirl_fadein();
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        normal_date_bggirl_fadeout();
        break;
    case 11:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        func_80145530();
        break;
    default:
        func_80046500();
        break;
    }
}

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80150D30", func_80152300);

void func_80152618(void) {
    func_800847B8(9);
    func_80132444();
    D_800CA14C = 0;
    D_800CA160 = D_80155E90;
    D_800CA164 = D_80155E94;
    D_800CA168 = D_80155E98;
    func_8004284C();
}
