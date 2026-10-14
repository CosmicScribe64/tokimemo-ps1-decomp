#define MAIN_API_OVERRIDE_D_80122EB8 /* matched as u32 (main_api.h: s32) */
#include "common.h"
#include "ovl/BUNKAKEN.h"

extern u32 D_80122EB8;

void func_8013CF30(void) {
    switch (D_80122EB8) {
    case 0:
        if (D_800E6280.unk_03E == 0x60) {
            func_80042908(0x84);
            return;
        }
        func_80042908(0x85);
        return;
    case 1:
        func_80042908(0x86);
        return;
    case 2:
        func_80042908(0x87);
        return;
    case 3:
        if (D_800E6280.unk_03E == 0x60) {
            func_80042908(0x88);
            return;
        }
        func_80042908(0x87);
        return;
    case 4:
        if (D_800E6280.unk_03E == 0x60) {
            func_80042908(0x89);
            return;
        }
        func_80042908(0x8A);
        return;
    case 5:
        func_80042908(0x8B);
        return;
    default:
        func_80046500();
        return;
    }
}

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/8013CF30", func_8013D048);

void func_8013D1F4(void) {
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
            cd_read_sectors(0xDU, 0x801E8000, 0x4B72);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(10);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 5;
                break;
            case 2:
                D_800CA148 = 6;
                break;
            case 3:
                D_800CA148 = 7;
                break;
            }
            func_8013EAC8();
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
        func_800866A0(0x6935);
        break;
    case 9:
        func_80145844();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        D_800E6280.unk_F5F = 10;
        func_80083474();
        cd_read_sectors(0xDU, 0x801E8000, 0x4B72);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = func_8008667C(10);
            switch (temp_v0_2) {
            case 0:
            case 1:
                D_800CA148 = 8;
                break;
            case 2:
                D_800CA148 = 9;
                break;
            case 3:
                D_800CA148 = 0xA;
                break;
            }
            func_8013EAC8();
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

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/8013CF30", func_8013D4D8);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/8013CF30", func_8013D82C);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/8013CF30", func_8013DBE8);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/8013CF30", func_8013E2C0);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/8013CF30", func_8013E578);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/8013CF30", func_8013E7C4);

void func_8013EAC8(void) {
    func_800847B8(10);
    func_801324AC();
    D_800CA14C = 0;
    D_800CA160 = D_80155E90;
    D_800CA164 = D_80155E94;
    D_800CA168 = D_80155E98;
    func_8004284C();
}
